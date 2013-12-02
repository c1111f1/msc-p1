#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C"
{
  #include "screen.h"
  #include "opt.h"
  #include "video.h"
  #include <stdint.h>
  #include "x264.h"
  #include "x264_encoder.h"
}

unsigned int frame_num = 0;

extern struct options opt;
extern struct video video;

void screen_mainloop()
{
  int i;
  for (i = 0; i <= video.buffer.req.count; i++) {
    frame_num ++ ;
    printf("Current Frame Number :%d\n", frame_num);
    if (i == video.buffer.req.count) {
      i = 0;
    }
    buffer_dequeue(i);
    encode_one_frame((uint8_t*)video.buffer.buf[i].start);
    buffer_enqueue(i);
  }
}
