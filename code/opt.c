#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
extern "C"
{
  #include "opt.h"
}

struct options opt;

extern char g_OutPutInfo[100];
extern char g_YUVFile[100];

extern char is_IP;
void options_init()
{
  opt.verbose = 0;
  opt.width = 176;
  opt.height = 144;
}

void options_deal(int argc, char *argv[])
{
  int my_opt;
  while ((my_opt = getopt(argc, argv, "r:is:")) != -1) 
  {
    switch(my_opt) 
    {
    case 'r':
      memcpy(g_OutPutInfo, optarg,strlen(optarg));
      printf("Output Information File:%s\n", g_OutPutInfo);
      break;
    case 'i':
      printf("Just use I frame\n");
      is_IP = 0;
      break;
    case 's':
      memcpy(g_YUVFile, optarg,strlen(optarg));
      printf("Input Sequence File:%s\n", g_YUVFile);
      break;
    default:
      break;
    }
  }
}
