#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 清除屏幕 
#define CLEAR() printf("\033[2J") 
// 上移光标 
#define MOVEUP(x) printf("\033[%dA", (x)) 
// 下移光标 
#define MOVEDOWN(x) printf("\033[%dB", (x)) 
// 左移光标 
#define MOVELEFT(y) printf("\033[%dD", (y)) 
// 右移光标 
#define MOVERIGHT(y) printf("\033[%dC",(y)) 
// 定位光标 
#define MOVETO(x,y) printf("\033[%d;%dH", (x), (y)) 
// 光标复位 
#define RESET_CURSOR() printf("\033[H") 
// 隐藏光标 
#define HIDE_CURSOR() printf("\033[?25l") 
// 显示光标 
#define SHOW_CURSOR() printf("\033[?25h") 
//反显
#define HIGHT_LIGHT() printf("\033[7m")
#define UN_HIGHT_LIGHT() printf("\033[27m")

extern "C"
{
  #include "screen.h"
  #include "opt.h"
  #include "platform.h"
  #include "video.h"
  #include <stdint.h>
  #include "x264.h"
  #include "x264_encoder.h"
}

unsigned int frame_num = 0, NAL_num = 0, Frame_Size = 0;

extern struct options opt;
extern struct video video;
#ifdef _ARM
extern float * get_uart();
#endif
void screen_mainloop()

{
  int i;
  float *p;
  HIDE_CURSOR();
  for (i = 0; i <= video.buffer.req.count; i++) {
    frame_num ++ ;

    if (i == video.buffer.req.count) {
      i = 0;
    }
    buffer_dequeue(i);
    encode_one_frame((uint8_t*)video.buffer.buf[i].start);
#ifdef _ARM
    p = get_uart();
#endif
    system("clear");
    printf("- Encode Frame Number:%d\n", frame_num);
    printf("- NAL Number:%d \t Size:%d\n", NAL_num, Frame_Size);
#ifdef _ARM
    printf("- ARM:%lf\tWIFI:%f\tWebCam:%f \n", *p, *(p + 1), *(p + 2));
#endif
    buffer_enqueue(i);
  }
}
