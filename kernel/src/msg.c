#include "msg.h"
#include "malloc.h"
#include "uart.h"

void msg_init(struct msg_t *msg, uint32 type, uint32 size) {
  msg->size = size - sizeof(struct msg_t);
  msg->type = type;
  print_buf("msg_init:");
  print_buf("  msg=");
  print_ptr(msg);
  print_buf("  msg->type=");
  print_hex(msg->type);
  print_buf("  msg->size=");
  print_hex(msg->size);
  print_buf("\n");
}

void msg_move(struct msg_t *src, struct msg_t *dst) {
  uint32 len = src->size;
  print_buf("msg_move:");
  print_buf("  src=");
  print_ptr(src);
  print_buf("  src->size=");
  print_hex(src->size);
  print_buf("  src->type=");
  print_hex(src->type);
  print_buf("  dst=");
  print_ptr(dst);
  print_buf("  dst->size=");
  print_hex(dst->size);
  print_buf("  dst->type=");
  print_hex(dst->type);
  print_buf("\n");
  if ( dst->size < src->size ) {
    len = dst->size;
  }
  memcpy(src->data, dst->data, len);
  dst->type = src->type;
}
