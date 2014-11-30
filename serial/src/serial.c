#include <stdio.h>
#include <termios.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>

struct channel;
struct context;
struct processor;

typedef void (data_handler_t)(
  struct context *context,
  struct processor *processor,
  char *buf,
  int   buf_len
);

struct processor {
  data_handler_t   *processor;
  struct processor *next;
};

struct copy_processor {
  struct processor processor;
  struct channel *dest_channel;
};

struct reply_processor {
  struct processor processor;
  struct channel *reply_channel;
  char   buf[4096];
  int    head;
  char   const* search_string;
  char   const* reply_string;
  int    state;
};

struct pipe_processor {
  struct processor processor;
  struct channel *reply_channel;
  char   buf[4096];
  int    head;
  char   const* search_string;
  char   const* reply_string;
  int    state;
  int    input_fds[2];
  int    output_fds[2];
  int    pid;
  char   const * const * argv;
  struct channel *c_input;
  struct channel *c_output;
  char   const *cmd;
};

struct channel {
  int               fd;
  struct termios    tios;
  int               is_input_channel;
  int               is_closed;
  char              input_buffer[4096];
  struct processor* processor;
};

struct context {
  struct channel channels[20];
  int num_channels;

  struct channel* serial;
  struct channel* stdin;
  struct channel* stdout;

  fd_set read_fds;
  fd_set write_fds;
  int    max_fd;
};

#define check(expr) \
  ( ( (expr) < 0 ) ? \
(fprintf(stderr, "Line %d: error executing %s: %s\n",__LINE__, #expr, strerror(errno)), 0) \
    : 1 )

// ------------------------------------------------------------------------
// Functions

int close_on_error(struct channel *c, int result) {
  if ( result < 0 ) {
    c->is_closed = 1;
  }
  return result;
}

struct channel *allocate_channel(struct context* context) {
  struct channel *channel = context->channels + context->num_channels;
  context->num_channels += 1;
  return channel;
}

struct channel *open_console_channel(
  struct context* context, 
  int fd,
  int is_input_channel
) {
  struct channel *channel = allocate_channel(context);

  channel->fd = fd;
  channel->is_input_channel = is_input_channel;

  if (channel->fd > context->max_fd ) {
    context->max_fd = channel->fd;
  }

  return channel;
};

void channel_add_processor(struct channel *channel, struct processor *processor)
{
  processor->next = channel->processor;
  channel->processor = processor;
};

void copy_processor_handler(
  struct context *context,
  struct processor *processor,
  char *buf,
  int   buf_len
)
{
  struct copy_processor *copy_processor = (struct copy_processor *)processor;
  struct channel *c = copy_processor->dest_channel;
  if ( ! c->is_closed ) {
    close_on_error(c, write(c->fd, buf, buf_len));
  }
};

void reply_processor_handler(
  struct context *context,
  struct processor *processor,
  char *buf,
  int   buf_len
)
{
  struct reply_processor *p = 
    (struct reply_processor *) processor;
  struct channel *ch = p->reply_channel;

  if ( p->state > 0 ) {
    if ( p->head + buf_len >= (signed int) sizeof(p->buf)-1 ) {
      fprintf(stderr, "Overflow %d\n", buf_len);
    }
    memmove(p->buf + p->head, buf, buf_len);
    p->head += buf_len;
    p->buf[p->head] = 0;
    char *c = strstr(p->buf, p->search_string);
    if ( c != 0 ) {
      if ( ! ch->is_closed ) {
        close_on_error(
          ch, 
          write(ch->fd, p->reply_string, strlen(p->reply_string)
	));
      }
      p->head = 0;
      p->buf[p->head] = 0;
      p->state -= 1;
    }
    else {
      while( (c = strstr(p->buf, "\n")) != 0) {
        p->head -= (c - p->buf) + 1;
        memmove(p->buf, c+1, p->head);
        p->buf[p->head] = 0;
      };
    }
  }
};

struct processor* new_copy_processor(struct channel *dest_channel) {
  struct copy_processor *p = 
    (struct copy_processor *) calloc(1, sizeof(struct copy_processor));
  p->processor.processor = &copy_processor_handler;
  p->dest_channel = dest_channel;
  return (struct processor *)p;
};

void pipe_processor_handler(
  struct context *context,
  struct processor *processor,
  char *buf,
  int   buf_len
)
{
  struct pipe_processor *p = 
    (struct pipe_processor *) processor;

  if ( p->state == 0 ) {
    if ( p->head + buf_len >= (signed int) sizeof(p->buf)-1 ) {
      fprintf(stderr, "Overflow %d \n", buf_len);
      return;
    }
    memmove(p->buf + p->head, buf, buf_len);
    p->head += buf_len;
    p->buf[p->head] = 0;
    char *c = strstr(p->buf, p->search_string);
    if ( c != 0 ) {
      fprintf(stderr, "Matched upload string\n");
      check(pipe(p->input_fds));
      fcntl(p->input_fds[0], F_SETFD, O_NONBLOCK);
      fcntl(p->input_fds[1], F_SETFD, O_NONBLOCK);
      check(pipe(p->output_fds));
      fcntl(p->output_fds[0], F_SETFD, O_NONBLOCK);
      fcntl(p->output_fds[1], F_SETFD, O_NONBLOCK);

      p->pid = fork();
      if ( p->pid == 0 ) {
        check(close(0));
        check(close(1));
        check(dup2(p->input_fds[0], 0));
        check(dup2(p->output_fds[1], 1));
        check(close(p->input_fds[1]));
        check(close(p->input_fds[0]));
        check(close(p->output_fds[0]));
        check(close(p->output_fds[1]));
        check(execvp(p->cmd, (char* const*)(p->argv)));
        fprintf(stderr, "execvp failed\n");
        exit(-1);
      }
      else if ( p->pid == -1 ) {
        perror("failed to fork");
      }
      else {
        fprintf(stderr, "pipe input %d\n", p->input_fds[1]);
        fprintf(stderr, "pipe output %d\n", p->output_fds[0]);
        check(close(p->input_fds[0]));
        check(close(p->output_fds[1]));
        p->c_input = open_console_channel(context, p->input_fds[1], 0);
        p->c_output = open_console_channel(context, p->output_fds[0], 1);
        channel_add_processor(p->c_output, new_copy_processor(p->reply_channel));
        channel_add_processor(p->c_output, new_copy_processor(context->stdout));
        channel_add_processor(p->reply_channel, new_copy_processor(p->c_input));
        p->state = 1;
        p->head = 0;
        p->buf[p->head] = 0;
      }
    }
    else {
      while( (c = strstr(p->buf, "\n")) != 0) {
        p->head -= (c - p->buf) + 1;
        memmove(p->buf, c+1, p->head);
        p->buf[p->head] = 0;
      };
    }
  }
  else if ( p->pid != 0 ) {
    int status;
    if ( waitpid(p->pid, &status, WNOHANG) == p->pid ) {
      fprintf(stderr, "Process terminated %d", status);
      p->pid = 0;
      p->state = 0;
    }
  }
};

struct processor* new_reply_processor(struct channel *reply_channel, char const* search_string, char const* reply_string, int num_actions) {
  struct reply_processor *p = 
    (struct reply_processor *) calloc(1, sizeof(struct reply_processor));
  p->processor.processor = &reply_processor_handler;
  p->reply_channel = reply_channel;
  p->search_string = search_string;
  p->reply_string = reply_string;
  p->state = num_actions;
  return (struct processor *)p;
};

struct processor* new_pipe_processor(struct channel *reply_channel, char const* search_string, char const *cmd, char const * const * argv) {
  struct pipe_processor *p = 
    (struct pipe_processor *) calloc(1, sizeof(struct pipe_processor));
  p->processor.processor = &pipe_processor_handler;
  p->reply_channel = reply_channel;
  p->search_string = search_string;
  p->cmd = cmd;
  p->argv = argv;
  return (struct processor *)p;
};

struct channel* open_serial_channel(struct context* context, char const* dev_name) 
{
  struct channel *channel = allocate_channel(context);

  channel->fd = open(dev_name, O_RDWR | O_NOCTTY);

  if ( channel->fd == 0 ) {
    fprintf(stderr, "   ERROR: %s\n", strerror(errno));
  };
  channel->is_input_channel = 1;

  channel->tios.c_iflag = IGNPAR;
  channel->tios.c_oflag = 0;
  channel->tios.c_cflag = CS8 | CLOCAL | CREAD;
  channel->tios.c_lflag = 0; 
  channel->tios.c_cc[VMIN]  = 0;
  channel->tios.c_cc[VTIME] = 0;

  cfmakeraw(&channel->tios);
  cfsetspeed(&channel->tios, B115200);
  tcsetattr(channel->fd, TCSANOW, &channel->tios);

  if (channel->fd > context->max_fd ) {
    context->max_fd = channel->fd;
  }

  return channel;
};

void add_read_channels(
  struct context *context
)
{
  int i;
  FD_ZERO(&context->read_fds);
  for(i = 0; i<context->num_channels; ++i) {
    struct channel *channel = &context->channels[i];
    
    if ( channel->is_input_channel ) {
      if ( ! channel->is_closed ) {
	FD_SET(channel->fd,  &context->read_fds);
      }
    }
  }
};

void handle_input_channels(
  struct context *context
)
{
  int i;
  for(i = 0; i<context->num_channels; ++i) {
    struct channel *channel = &context->channels[i];
    
    if ( channel->is_input_channel && 
         FD_ISSET(channel->fd, &context->read_fds)
    ) {
      int num_read = close_on_error(channel, 
        read(
          channel->fd, channel->input_buffer, 
          sizeof(channel->input_buffer)-1
      ));
      if ( num_read > 0 ) {
        struct processor *processor = channel->processor;
        channel->input_buffer[num_read] = 0;
        while( processor ) {
          (*processor->processor)(context, processor, channel->input_buffer, num_read);
          processor = processor->next;
        }
      }
    };
  };
};

void signal_handler_IO (int status) {
  fprintf(stderr, "IO Signal %d\n", status);
};

void signal_handler_PIPE (int status) {
  fprintf(stderr, "PIPE Signal %d\n", status);
};

void select_loop(struct context *context) {
  while(1) {
    FD_ZERO(&context->read_fds);
    FD_ZERO(&context->write_fds);
    add_read_channels(context);
    select(context->max_fd + 1, &context->read_fds, &context->write_fds, 0, 0);
    handle_input_channels(context);
  };
}

void init_signal_handlers() {
  struct sigaction io_signal_action;
  memset(&io_signal_action, 0, sizeof(io_signal_action));

  io_signal_action.sa_handler = signal_handler_IO;
  sigaction(SIGIO, &io_signal_action, 0);

  io_signal_action.sa_handler = signal_handler_PIPE;
  sigaction(SIGPIPE, &io_signal_action, 0);
};

struct context* new_context() {
  struct context* context = (struct context *)malloc(sizeof(struct context));
  memset(context, 0, sizeof(struct context));
  return context;
}

void terminal(int argc, char **argv) {
  init_signal_handlers();
  struct context* context = new_context();

  if ( argc < 3 ) {
    fprintf(stderr, "terminal <device>\n");
    exit(-1);
  }

  char const* device_name = argv[2];

  context->serial = open_serial_channel (context, device_name);
  context->stdin  = open_console_channel(context, STDIN_FILENO, 1);
  context->stdout = open_console_channel(context, STDOUT_FILENO, 0);

  channel_add_processor(context->serial, new_copy_processor(context->stdout));
  channel_add_processor(context->stdin,  new_copy_processor(context->serial));
  
  select_loop(context);
}

void bootload_kernel(int argc, char **argv) {

  if ( argc < 4 ) {
    fprintf(stderr, "bootload <device> <kernel>\n");
    exit(-1);
  }

  char const* binary_filename = argv[2];
  char const* device_name = argv[3];

  init_signal_handlers();
  struct context* context = new_context();

  context->serial = open_serial_channel (context, device_name);
  context->stdin  = open_console_channel(context, STDIN_FILENO, 1);
  context->stdout = open_console_channel(context, STDOUT_FILENO, 0);

  channel_add_processor(context->serial, new_copy_processor(context->stdout));
  channel_add_processor(context->stdin,  new_copy_processor(context->serial));

  /*
  channel_add_processor(
    context->serial, 
    new_reply_processor(context->serial, "Hit any key to stop autoboot:", " ", 1)
  );
  channel_add_processor(
    context->serial, 
    new_reply_processor(context->serial, "U-Boot# ", "loady\n", 1) 
  );
  */

  char const* cmd = "/usr/bin/sx";
  char const* const args[] = { cmd, "--xmodem", binary_filename, 0 };
  channel_add_processor(context->serial, 
    new_pipe_processor(context->serial, 
      "00008000", 
      cmd, args
  ));

  /*
  channel_add_processor(
    context->serial, 
    new_reply_processor(context->serial, "## Total Size ", "go 0x82000000\n", 1) 
  );
  */

  select_loop(context);
};

int main(int argc, char **argv) {

  if ( argc < 2 ) {
    fprintf(stderr, "Usage: %s COMMAND\n", argv[0]);
    exit(1);
  }

  if ( strcmp(argv[1], "bootload") == 0 ) {
    bootload_kernel(argc, argv);
  }
  if ( strcmp(argv[1], "terminal") == 0 ) {
    terminal(argc, argv);
  }
  else {
    fprintf(stderr, "Unknown command %s\n", argv[1]);
    exit(-1);
  }

  return 0;
}
 
