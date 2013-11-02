#ifndef _H264ENCODER_H
#define _H264ENCODER_H

typedef unsigned char uint8_t;
typedef unsigned int uint;

typedef struct {
	x264_param_t *param;
	x264_t *handle;
	x264_picture_t *picture; //说明一个视频序列中每帧
	x264_nal_t *nal;
} Encoder;


void encode_init(Encoder *encoder, int img_width, int img_height);

int encode_frame(Encoder *encoder, int type, uint8_t *frame, uint8_t *h264stream);

void encode_end(Encoder *encoder);

void init_file();

void close_file();

void X264_init();

void X264_end();

void encode_one_frame(uint8_t *YUVframe);

#endif