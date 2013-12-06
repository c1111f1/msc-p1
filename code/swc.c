/*
Master project of XJTLU
Fei Cheng
2013/10/31
*/

#include <stdlib.h>
#include <stdio.h>
#include <core.h>
#include "Serial.h"
extern "C"
{
  #include "opt.h"
  #include "video.h"
  #include "screen.h"
  #include <stdint.h>
  #include "x264.h"
  #include "x264_encoder.h"
}
extern int frame_num;
extern int RTP_init();
extern int RTP_send(unsigned char * sdat, int ndat);
extern int RTP_end();

void uart_init();
float * get_uart();

int
main(int argc, char *argv[])
{
  uart_init();
  get_uart();
  RTP_init();
  options_init();
  options_deal(argc, argv);
  video_init();
  X264_init();
  //printf("Begin\n");
  screen_mainloop();
  //printf("%d Exit\n",frame_num);
  X264_end();
  video_quit();
  RTP_end();
  exit(EXIT_SUCCESS);
}


void uart_init() { 
  init(); 
  int rate = 115200;
  Serial.begin(rate); 
  while (!Serial) {
  }
} 

int thisByte = 33; 

float * get_uart()
{
    char dat[100];
    int dat_i = 0;
    float x[3];
    float *p = x;
    Serial.print('1');
    while (Serial.available() == 0)
    {
      if (serialEventRun) serialEventRun(); //Important
    };
    do
    {
      if (Serial.available() > 0)
      {
        thisByte = Serial.read();
        dat[dat_i++] = thisByte;
        //printf("%c", thisByte);
      }
      if (serialEventRun) serialEventRun();
    } while (thisByte != 10);
    sscanf(dat, "%f\t%f\t%f", p, p + 1, p + 2);
  return p;
}