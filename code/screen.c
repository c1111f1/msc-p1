#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/time.h>    
#include <unistd.h>

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
char g_YUVFile[100] = "test_seq.yuv";
char g_OutPutInfo[100] = "output.txt";
FILE *YUV_FP;
FILE *OUTPUT_FP;
unsigned int frame_num = 0, NAL_num = 0, Frame_Size = 0;
void RTP_Test();
extern int RTP_send(char * sdat, int ndat);
extern struct options opt;
extern struct video video;
#ifdef _ARM
extern float * get_uart();
#endif
float Energy_M, Energy_W;

long getCurrentTime()  
{  
   struct timeval tv;  
   gettimeofday(&tv,NULL);  
   return tv.tv_sec * 1000 + tv.tv_usec / 1000;  
}  

#ifdef _RealTime
void screen_mainloop()
{
  int i;
  float *p;
  //HIDE_CURSOR();
  p = get_uart();

  for (i = 0; i <= video.buffer.req.count; i++) 
  {
    frame_num ++ ;

    if (i == video.buffer.req.count) 
    {
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

#else

unsigned long interval = 0, time_temp = 0;

void screen_mainloop()
{
  uint8_t *YUVframe = (uint8_t *) malloc(sizeof(uint8_t) * opt.width * opt.height * 1.5);
  YUV_FP = fopen(g_YUVFile, "rb");
  int i;
  float *p;
  //HIDE_CURSOR();

  time_temp = (unsigned long)getCurrentTime();
  while (fread(YUVframe, 1 ,opt.width * opt.height * 1.5, YUV_FP) == opt.width * opt.height * 1.5 
          && frame_num<300) 
  {
    printf("BEGIN:%lu\t", (unsigned long)getCurrentTime());
    frame_num ++ ;
    
#ifdef _ARM
    p = get_uart(); //Clean the energy buffer
#endif
    encode_one_frame(YUVframe);

#ifdef _ARM
    p = get_uart();
    Energy_W = *(p + 1);
#endif

    //system("clear");
    printf("Frame No.:%d\t", frame_num);
    printf("NAL No.:%d \t Size:%d\n", NAL_num, Frame_Size);
#ifdef _ARM
    //printf("ARM:%7.2f\tWIFI:%7.2f\n", Energy_M, Energy_W);
    fprintf(OUTPUT_FP, "%d\t%d\t%d\t",frame_num ,NAL_num , Frame_Size);
    fprintf(OUTPUT_FP, "%7.2f\t%7.2f\n",Energy_M, Energy_W);
#else
    fprintf(OUTPUT_FP, "%d\t%d\t%d\n",frame_num ,NAL_num , Frame_Size);
#endif
    //Wait for 25fps
    interval =  (unsigned long)getCurrentTime() - time_temp;
    //printf("%lu\n",interval);
    if (interval <= 40)
    {
      usleep(40000 - interval * 1000);
    }
    time_temp = (unsigned long)getCurrentTime();

  }
  fclose(OUTPUT_FP);
  fclose(YUV_FP);
}

#endif


void RTP_Test()
{
  float *p;
  char p_test[200000];
  unsigned long t1, t2;
  float speed;
  fprintf(OUTPUT_FP,"%d\t",0);

  RTP_send((char *)p_test, 5000); //Senddata by RTP
  RTP_send((char *)p_test, 5000); //Senddata by RTP
  RTP_send((char *)p_test, 5000); //Senddata by RTP
  RTP_send((char *)p_test, 5000); //Senddata by RTP
  RTP_send((char *)p_test, 5000); //Senddata by RTP
  RTP_send((char *)p_test, 5000); //Senddata by RTP
  RTP_send((char *)p_test, 5000); //Senddata by RTP
  RTP_send((char *)p_test, 5000); //Senddata by RTP
  RTP_send((char *)p_test, 5000); //Senddata by RTP
  RTP_send((char *)p_test, 5000); //Senddata by RTP

  
  RTP_send((char *)p_test, 5000); //Senddata by RTP
  RTP_send((char *)p_test, 5000); //Senddata by RTP
  RTP_send((char *)p_test, 5000); //Senddata by RTP
  RTP_send((char *)p_test, 5000); //Senddata by RTP
  RTP_send((char *)p_test, 5000); //Senddata by RTP
  RTP_send((char *)p_test, 5000); //Senddata by RTP
  RTP_send((char *)p_test, 5000); //Senddata by RTP
  RTP_send((char *)p_test, 5000); //Senddata by RTP
  RTP_send((char *)p_test, 5000); //Senddata by RTP
  RTP_send((char *)p_test, 5000); //Senddata by RTP


  printf("RTP1:%lu\n", (unsigned long)getCurrentTime());
  t1=(unsigned long)getCurrentTime();
  RTP_send((char *)p_test, 10000); //Senddata by RTP
  RTP_send((char *)p_test, 10000); //Senddata by RTP
  RTP_send((char *)p_test, 10000); //Senddata by RTP
  RTP_send((char *)p_test, 10000); //Senddata by RTP
  RTP_send((char *)p_test, 10000); //Senddata by RTP
  RTP_send((char *)p_test, 10000); //Senddata by RTP
  RTP_send((char *)p_test, 10000); //Senddata by RTP
  RTP_send((char *)p_test, 10000); //Senddata by RTP
  RTP_send((char *)p_test, 10000); //Senddata by RTP
  RTP_send((char *)p_test, 10000); //Senddata by RTP
  t2 = (unsigned long)getCurrentTime();
  speed = 100.0 / (t2 - t1) * 8000.0;
  printf("RTP2:%lu\n", t2);
  printf("Speed:%f kbps\n", speed);
  fprintf(OUTPUT_FP,"%f\t",speed);

  printf("RTP1:%lu\n", (unsigned long)getCurrentTime());
  t1=(unsigned long)getCurrentTime();
  RTP_send((char *)p_test, 20000); //Senddata by RTP
  RTP_send((char *)p_test, 20000); //Senddata by RTP
  RTP_send((char *)p_test, 20000); //Senddata by RTP
  RTP_send((char *)p_test, 20000); //Senddata by RTP
  RTP_send((char *)p_test, 20000); //Senddata by RTP
  RTP_send((char *)p_test, 20000); //Senddata by RTP
  RTP_send((char *)p_test, 20000); //Senddata by RTP
  RTP_send((char *)p_test, 20000); //Senddata by RTP
  RTP_send((char *)p_test, 20000); //Senddata by RTP
  RTP_send((char *)p_test, 20000); //Senddata by RTP
  t2 = (unsigned long)getCurrentTime();
  speed = 200.0 / (t2 - t1) * 8000.0;
  printf("RTP2:%lu\n", t2);
  printf("Speed:%f kbps\n", speed);
  fprintf(OUTPUT_FP,"%f\t",speed);

  printf("RTP1:%lu\n", (unsigned long)getCurrentTime());
  p = get_uart();
  t1=(unsigned long)getCurrentTime();
  RTP_send((char *)p_test, 20000); //Senddata by RTP
  RTP_send((char *)p_test, 20000); //Senddata by RTP
  RTP_send((char *)p_test, 20000); //Senddata by RTP
  RTP_send((char *)p_test, 20000); //Senddata by RTP
  RTP_send((char *)p_test, 20000); //Senddata by RTP
  RTP_send((char *)p_test, 20000); //Senddata by RTP
  RTP_send((char *)p_test, 20000); //Senddata by RTP
  RTP_send((char *)p_test, 20000); //Senddata by RTP
  RTP_send((char *)p_test, 20000); //Senddata by RTP
  RTP_send((char *)p_test, 20000); //Senddata by RTP
  RTP_send((char *)p_test, 20000); //Senddata by RTP
  RTP_send((char *)p_test, 20000); //Senddata by RTP
  RTP_send((char *)p_test, 20000); //Senddata by RTP
  RTP_send((char *)p_test, 20000); //Senddata by RTP
  RTP_send((char *)p_test, 20000); //Senddata by RTP
  RTP_send((char *)p_test, 20000); //Senddata by RTP
  RTP_send((char *)p_test, 20000); //Senddata by RTP
  RTP_send((char *)p_test, 20000); //Senddata by RTP
  RTP_send((char *)p_test, 20000); //Senddata by RTP
  RTP_send((char *)p_test, 20000); //Senddata by RTP
  t2 = (unsigned long)getCurrentTime();
  speed = 400.0 / (t2 - t1) * 8000.0;
  printf("RTP2:%lu\n", t2);
  printf("Speed:%f kbps\n", speed);
  fprintf(OUTPUT_FP,"%f\t",speed);
  p = get_uart();
  fprintf(OUTPUT_FP,"%f\n",*(p + 1));
  //getchar();
}