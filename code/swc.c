/*
Master project of XJTLU
Fei Cheng
2013/10/31
*/

#include <stdlib.h>
#include <stdio.h>
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

int
main(int argc, char *argv[])
{
  RTP_init();
  options_init();
  options_deal(argc, argv);
  video_init();
  X264_init();
  printf("Begin\n");
  screen_mainloop();
  printf("%d Exit\n",frame_num);
  X264_end();
  video_quit();
  RTP_end();
  exit(EXIT_SUCCESS);
}
