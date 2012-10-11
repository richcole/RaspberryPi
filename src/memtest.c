#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

double diff(struct timeval *after, struct timeval *before) {
    double result = 0;
    result += after->tv_sec - before->tv_sec;
    result += (after->tv_usec - after->tv_usec) / 1000000.0;
    return result;
}

int main(int argc, char **argv) {
    struct timeval before, after;
    int len1 = 1024 * 1024 * 2;
    int len2 = 1000;
    int *buf = malloc(len1 * sizeof(int));
    int sum = 0;
    int i = 0;
    int j = 0;

    if (buf == 0) {
      fprintf(stderr, "Unable to allocate memory");
      exit(-1);
    }
    
    gettimeofday(&before, 0);
    for(j=0;j<len2;j++) {
      for(i=0;i<len1;++i) {
        sum += buf[i];
      }
    }
    gettimeofday(&after, 0);
    fprintf(stdout, "%lf MB/s\n", (len1 * len2 * sizeof(int)) / (1024.0 * 1024.0) / diff(&after, &before)); 
    return sum;
}
