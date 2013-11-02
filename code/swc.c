/*
Master project of XJTLU
Fei Cheng
2013/10/31
*/

#include <stdlib.h>
#include "opt.h"
#include "video.h"
#include "screen.h"
#include <stdio.h>
#include "x264.h"

#include "x264_encoder.h"
extern int frame_num;

int
main(int argc, char *argv[])
{
  options_init();
  options_deal(argc, argv);
  video_init();
  screen_init();
  X264_init();
  printf("Begin\n");
  screen_mainloop();
  printf("%d Exit\n",frame_num);
  X264_end();
  screen_quit();
  video_quit();
  exit(EXIT_SUCCESS);
}