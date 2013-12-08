/*
MSc Project at XJTLU
Fei Cheng
2013/10/28
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>



extern "C"
{
#include "x264.h"
#include "opt.h"
#include "platform.h"
#include "x264_encoder.h"
}

extern float Energy_M, Energy_W;

#ifdef _ARM
extern float * get_uart();
float *p;
#endif


extern struct options opt;

extern unsigned int frame_num, NAL_num, Frame_Size;

extern int RTP_send(char * sdat, int ndat);

extern long getCurrentTime();

char is_IP = 1;

//File names of input and output file
char g_X264File[100] = "data/output.264";
//File pointer
FILE *H264_FP;
//FILE *YUV_FP;
//X264 Encoder
Encoder g_X264Encoder;

uint g_ImgWidth;
uint g_ImgHeight;

uint8_t *g_H264_Buf;
uint8_t *YUVframe = NULL;
void init_file() 
{
	H264_FP = fopen(g_X264File, "w");
//YUV_FP = fopen(g_YUVFile, "rb");
}

void close_file() 
{
	fclose(H264_FP);
//fclose(YUV_FP);
}

void encode_init(Encoder *encoder, int img_width, int img_height)
{
//Set default x264 parameters
	encoder->param = (x264_param_t *) malloc(sizeof(x264_param_t));
	encoder->picture = (x264_picture_t *) malloc(sizeof(x264_picture_t));
	x264_param_default(encoder->param);

encoder->param->i_width = img_width; //set frame width
encoder->param->i_height = img_height; //set frame height
encoder->param->rc.i_lookahead = 0; //表示i帧向前缓冲区
encoder->param->i_fps_num = 25; //帧率分子
encoder->param->i_fps_den = 1; //帧率分母
encoder->param->rc.i_lookahead = 0;
encoder->param->i_sync_lookahead = 0;
encoder->param->i_bframe = 0;
encoder->param->b_sliced_threads = 1;
encoder->param->b_vfr_input = 0;
encoder->param->rc.b_mb_tree = 0;

x264_param_apply_profile(encoder->param, x264_profile_names[0]);

encoder->handle = x264_encoder_open(encoder->param);

if (encoder->handle == 0) 
{
	return;
}
/* Create a new pic */

//encoder->picture->param->i_width = img_width;
//encoder->picture->param->i_height = img_height;

x264_picture_alloc(encoder->picture, X264_CSP_I420, 
	encoder->param->i_width,encoder->param->i_height);

encoder->picture->img.i_csp = X264_CSP_I420;
encoder->picture->img.i_plane = 3;

g_H264_Buf = (uint8_t *) malloc(
sizeof(uint8_t) * g_ImgWidth * g_ImgHeight * 3); // 设置缓冲区

}

void encode_one_frame(uint8_t *YUVframe)
{
	encode_frame(&g_X264Encoder,-1, YUVframe, g_H264_Buf);
}

int encode_frame(Encoder *encoder, int type, uint8_t *frame, uint8_t *h264stream)
{
	x264_picture_t pic_out;
	int num_Nal = -1;
	int result = 0;
	uint8_t *p_out = h264stream;
	uint i = 0;

	uint8_t *Y = encoder->picture->img.plane[0];
	uint8_t *U = encoder->picture->img.plane[1];
	uint8_t *V = encoder->picture->img.plane[2];

#ifdef _RealTime

	int YUV422_length = encoder->param->i_width * encoder->param->i_height * 2;

	int Y_index = 0, U_index = 0, V_index = 0, H_index = 0;

	for (i = 0; i < YUV422_length ; i = i + 4)
	{
		H_index = i / (encoder->param->i_width * 2);

		*(Y + Y_index++) = *(frame + i + 0);
		*(Y + Y_index++) = *(frame + i + 2);
		if(H_index % 2)
		{
			*(U + U_index++) = *(frame + i +1);
		}
		else
		{
			*(V + V_index++) = *(frame + i +3);
		}
	}
#else
	int YUV420_length = encoder->param->i_width * encoder->param->i_height * 1.5;
	memcpy(Y, frame, YUV420_length);
#endif
//fwrite(encoder->picture->img.plane[0],1,640*480*1.5,H264_FP);
//encoder->picture->i_pts ++;

	switch (type) {
		case 0:
		encoder->picture->i_type = X264_TYPE_P;
		break;
		case 1:
		encoder->picture->i_type = X264_TYPE_IDR;
		break;
		case 2:
		encoder->picture->i_type = X264_TYPE_I;
		break;
		default:
		encoder->picture->i_type = X264_TYPE_AUTO;
		break;
	}
//encoder->picture->i_qpplus1  = 0;
//Encode one frame
	if (is_IP == 0)
	{
		encoder->picture->i_type = X264_TYPE_I;
	}

	if (x264_encoder_encode(encoder->handle, &(encoder->nal), &num_Nal, 
		encoder->picture, &pic_out) < 0) 
	{
		return -1;
	}
//Encoding Finish
//Mark the Energy



	NAL_num = num_Nal;

printf("ENCOD:%lu\t", (unsigned long)getCurrentTime());

#ifdef _ARM
	
	p = get_uart();
	Energy_M = *p;
#endif
//Copy to 
	for (i = 0; i < num_Nal; i++) 
	{
		memcpy(p_out, encoder->nal[i].p_payload, encoder->nal[i].i_payload);
		RTP_send((char *)p_out, encoder->nal[i].i_payload); //Senddata by RTP
		p_out += encoder->nal[i].i_payload;
		result += encoder->nal[i].i_payload;
	}
printf("TRANS:%lu\t", (unsigned long)getCurrentTime());
	if (result > 0)
	{
		//fwrite(h264stream,result,1,H264_FP);
		//RTP_send((char *)h264stream,result); //Senddata by RTP
	}
	Frame_Size = result;
	//printf("  Size of Output:%d\n", result);
	return 0;
}

void encoder_end(Encoder *encoder) 
{
	if (encoder->picture) {
		x264_picture_clean(encoder->picture);
		free(encoder->picture);
		encoder->picture = 0;
	}
	if (encoder->param) {
		free(encoder->param);
		encoder->param = 0;
	}
	if (encoder->handle) {
		x264_encoder_close(encoder->handle);
	}
	free(g_H264_Buf);
}


void X264_init()
{
	g_ImgWidth = opt.width;
	g_ImgHeight = opt.height;
	YUVframe = (uint8_t *) malloc(sizeof(uint8_t) * g_ImgWidth * g_ImgHeight * 2);
	encode_init(&g_X264Encoder, g_ImgWidth, g_ImgHeight);
	init_file();
}

void X264_end()
{
	encoder_end(&g_X264Encoder);
	close_file();
	free(YUVframe);	
}
/*
int main()
{
int i = 0;
uint8_t *YUVframe = (uint8_t *) malloc(sizeof(uint8_t) * g_ImgWidth * g_ImgHeight * 2);
encode_init(&g_X264Encoder, g_ImgWidth, g_ImgHeight);
init_file();

//fread(YUVframe, 1 ,g_ImgWidth * g_ImgHeight * 2, YUV_FP);
//encode_frame(&g_X264Encoder,1, YUVframe, g_H264_Buf);

for (i = 0; i < 20; ++i)
{
fread(YUVframe, 1 ,g_ImgWidth * g_ImgHeight * 2, YUV_FP);
encode_frame(&g_X264Encoder,-1, YUVframe, g_H264_Buf);
}
encoder_end(&g_X264Encoder);
close_file();
free(YUVframe);
return 0;
}
*/
