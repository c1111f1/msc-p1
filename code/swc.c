#include <stdlib.h>
#include "opt.h"
#include "video.h"
#include "screen.h"
#include <stdio.h>

extern int frame_num;

int
main(int argc, char *argv[])
{
  options_init();
  options_deal(argc, argv);
  video_init();
  screen_init();
  printf("Begin\n");
  screen_mainloop();
  printf("%d Exit\n",frame_num);

  screen_quit();
  video_quit();
  exit(EXIT_SUCCESS);
}