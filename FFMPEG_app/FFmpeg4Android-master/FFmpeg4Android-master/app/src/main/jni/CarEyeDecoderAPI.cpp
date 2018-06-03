/*
 * Car eye ��������ƽ̨: www.car-eye.cn
 * Car eye ��Դ��ַ: https://github.com/Car-eye-team
 * CarEyeDecoderAPI.cpp
 *
 * Author: Wgj
 * Date: 2018-05-16 22:52
 * Copyright 2018
 *
 * Car eye����FFMPEG������Ƶ����ӿ�ʵ��
 */

#include <stdio.h>
#include "CarEyeDecoderAPI.h"
#include "CarEyeOSDAPI.h"

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswresample/swresample.h"
};
#include "public.h"


/*
* Comments: ����Ƶ֡���ˮӡ���Ƕ���ӿں���
* Param : aFrame: [����/���] Ҫ���ˮӡ����Ƶ֡
* @Return void
*/
int CarEye_OSD_Add(CarEye_OSD_Handle aFilter, AVFrame *aFrame);


// �������ṹ�嶨��
typedef struct
{
	// ��Ƶ������
	AVCodecContext *VDecoder;
	// ��Ƶ������
	AVCodecContext *ADecoder;
	// ��������Ƶ֡ ����Ƶ֡����ֱ��壬��ֹ���̷ֱ߳��������Ƶ��ɶ�д��ͻ
	AVFrame *VFrame;
	// ��������Ƶ֡
	AVFrame *AFrame;
	// ��Ƶ�����ش�С
	int PixelSize;
}CarEyeDecoder;

/*
* Comments: ���ý�������ý������н��벢�������������
* Param aDecoder: ��Ч�Ľ�����
* Param aPacket: Ҫ�����ý�����ݰ�
* Param aFrame: [���] ����������
* @Return int С��0ʧ�ܣ�����0�ɹ�
*/
static int Decode(AVCodecContext *aDecoder, AVPacket *aPacket, AVFrame *aFrame)
{
	int ret;

	ret = avcodec_send_packet(aDecoder, aPacket);
	if (ret < 0)
	{
		printf("Error sending a packet for decoding\n");
		return ret;
	}

	return avcodec_receive_frame(aDecoder, aFrame);
}

/*
* Comments: ����һ������������
* Param aInfo: Ҫ�����ý����Ϣ
* @Return CarEye_Decoder_Handle �ɹ����ؽ��������󣬷��򷵻�NULL
*/
CE_API CarEye_Decoder_Handle CE_APICALL CarEye_DecoderCreate(CarEye_FrameInfo aInfo)
{
	if (aInfo.ACodec == CAREYE_CODEC_NONE
		&& aInfo.VCodec == CAREYE_CODEC_NONE)
	{
		// ���ٰ���һ���������
		return NULL;
	}

	CarEyeDecoder *decoder = new CarEyeDecoder;
	if (decoder == NULL)
	{
		return NULL;
	}

	memset(decoder, 0x00, sizeof(CarEyeDecoder));

	// ý�������
	AVCodec *pCodec;
	if (aInfo.VCodec != CAREYE_CODEC_NONE)
	{
		// ������Ƶ������
		pCodec = avcodec_find_decoder((AVCodecID)aInfo.VCodec);
		if (pCodec == NULL)
		{
			printf("Could not find video decoder.\n");
			CarEye_DecoderRelease(decoder);
			return NULL;
		}
		// ���������������
		decoder->VDecoder = avcodec_alloc_context3(pCodec);
		if (decoder->VDecoder == NULL)
		{
			printf("Could not alloc video decoder.\n");
			CarEye_DecoderRelease(decoder);
			return NULL;
		}
		decoder->VDecoder->time_base.num = 1;
		// ֡��
		decoder->VDecoder->time_base.den = aInfo.FramesPerSecond;
		// ÿ��һ����Ƶ֡
		decoder->VDecoder->frame_number = 1;
		// ý������Ϊ��Ƶ
		decoder->VDecoder->codec_type = AVMEDIA_TYPE_VIDEO;
		decoder->VDecoder->bit_rate = aInfo.VideoBitrate;
		// ��Ƶ�ֱ���
		decoder->VDecoder->width = aInfo.Width;
		decoder->VDecoder->height = aInfo.Height;
		decoder->VDecoder->pix_fmt = AV_PIX_FMT_YUV420P;
		decoder->PixelSize = decoder->VDecoder->width * decoder->VDecoder->height;
		if (avcodec_open2(decoder->VDecoder, pCodec, NULL) < 0)
		{
			printf("Could not open video decoder.\n");
			CarEye_DecoderRelease(decoder);
			return NULL;
		}

		decoder->VFrame = av_frame_alloc();
		if (decoder->VFrame == NULL)
		{
			printf("Alloc video frame faile!\n");
			CarEye_DecoderRelease(decoder);
			return NULL;
		}
	}
	if (aInfo.ACodec != CAREYE_CODEC_NONE)
	{
		// ������Ƶ������
		pCodec = avcodec_find_decoder((AVCodecID)aInfo.ACodec);
		if (pCodec == NULL)
		{
			printf("Could not find audio decoder.\n");
			CarEye_DecoderRelease(decoder);
			return NULL;
		}
		// ���������������
		decoder->ADecoder = avcodec_alloc_context3(pCodec);
		if (decoder->ADecoder == NULL)
		{
			printf("Could not alloc audio decoder.\n");
			CarEye_DecoderRelease(decoder);
			return NULL;
		}

		// ������ֵ
		decoder->ADecoder->codec_type = AVMEDIA_TYPE_AUDIO;
		decoder->ADecoder->sample_rate = aInfo.SampleRate;
		decoder->ADecoder->channels = aInfo.Channels;
		decoder->ADecoder->bit_rate = aInfo.AudioBitrate;
		decoder->ADecoder->channel_layout = AV_CH_LAYOUT_STEREO;
		if (avcodec_open2(decoder->ADecoder, pCodec, NULL) < 0)
		{
			printf("Could not open audio decoder.\n");
			CarEye_DecoderRelease(decoder);
			return NULL;
		}

		decoder->AFrame = av_frame_alloc();
		if (decoder->AFrame == NULL)
		{
			printf("Alloc audio frame fail!\n");
			CarEye_DecoderRelease(decoder);
			return NULL;
		}
	}

	return decoder;
}

/*
* Comments: �ͷŽ�������Դ
* Param aDecoder: Ҫ�ͷŵĽ�����
* @Return None
*/
CE_API void CE_APICALL CarEye_DecoderRelease(CarEye_Decoder_Handle aDecoder)
{
	CarEyeDecoder *decoder = (CarEyeDecoder *)aDecoder;

	if (decoder == NULL)
	{
		return;
	}
	if (decoder->VDecoder != NULL)
	{
		avcodec_close(decoder->VDecoder);
		decoder->VDecoder = NULL;
	}
	if (decoder->ADecoder != NULL)
	{
		avcodec_close(decoder->ADecoder);
		decoder->ADecoder = NULL;
	}
	if (decoder->VFrame != NULL)
	{
		av_frame_free(&decoder->VFrame);
		decoder->VFrame = NULL;
	}
	if (decoder->AFrame != NULL)
	{
		av_frame_free(&decoder->AFrame);
		decoder->AFrame = NULL;
	}

	delete decoder;
	decoder = NULL;
}

/*
* Comments: ��ȡYUV������������ֽڴ�С
* Param aDecoder: ������
* @Return int �����������С < 0ʧ��
*/
CE_API int CE_APICALL CarEye_GetYUVSize(CarEye_Decoder_Handle aDecoder)
{
	CarEyeDecoder *decoder = (CarEyeDecoder *)aDecoder;
	if (decoder == NULL || decoder->VDecoder == NULL)
	{
		return -1;
	}

	int y_size = decoder->PixelSize;

	return y_size + y_size / 2;
}

/*
* Comments: ��������Ƶ����ΪYUV420��ʽ�������
* Param aDecoder: ���뵽����Ч������
* Param aFilter: �������ˮӡ�������Ѵ�����ˮӡ����������
* Param aBytes: Ҫ���н������Ƶ��
* Param aSize: Ҫ������Ƶ���ֽ���
* Param aYuv: [���] ����ɹ��������YUV420����
* @Return int < 0����ʧ�ܣ�> 0Ϊ�����YUV420���ֽڸ��� ==0��ʾ������Ч
*/
CE_API int CE_APICALL CarEye_DecoderYUV420(CarEye_Decoder_Handle aDecoder, 
	unsigned char *aBytes, int aSize,
	unsigned char *aYuv)
{
	CarEyeDecoder *decoder = (CarEyeDecoder *)aDecoder;
	if (decoder == NULL || decoder->VDecoder == NULL)
	{
		return 0;
	}
	if (aBytes == NULL || aSize < 1 || aYuv == NULL)
	{
		return 0;
	}

	int ret;
	int y_size;
	int out_size = 0;
	AVPacket packet = { 0 };

	packet.data = aBytes;
	packet.size = aSize;
	ret = Decode(decoder->VDecoder, &packet, decoder->VFrame);
	if (ret < 0)
	{
		printf("Decode video error.\n");
		av_packet_unref(&packet);
		return ret;
	}


	y_size = decoder->VDecoder->width * decoder->VDecoder->height;

	// ��ֵYֵ
	memcpy(aYuv, decoder->VFrame->data[0], y_size);
	out_size += y_size;
	memcpy(aYuv + out_size, decoder->VFrame->data[1], y_size / 4);
	out_size += (y_size / 4);
	memcpy(aYuv + out_size, decoder->VFrame->data[2], y_size / 4);
	out_size += (y_size / 4);

	av_packet_unref(&packet);

	return out_size;
}

/*
* Comments: ��������Ƶ����ΪPCM��ʽ�������
* Param aDecoder: ���뵽����Ч������
* Param aBytes: Ҫ���н������Ƶ��
* Param aSize: Ҫ������Ƶ���ֽ���
* Param aYuv: [���] ����ɹ��������PCM����
* @Return int < 0����ʧ�ܣ�> 0Ϊ�����PCM���ֽڸ��� ==0��ʾ������Ч
*/
CE_API int CE_APICALL CarEye_DecoderPCM(CarEye_Decoder_Handle aDecoder,
	unsigned char *aBytes, int aSize,
	unsigned char *aPcm)
{
	CarEyeDecoder *decoder = (CarEyeDecoder *)aDecoder;
	if (decoder == NULL || decoder->ADecoder == NULL)
	{
		return 0;
	}
	if (aBytes == NULL || aSize < 1 || aPcm == NULL)
	{
		return 0;
	}

	int ret;
	int out_size = 0;
	AVPacket packet = { 0 };

	packet.data = aBytes;
	packet.size = aSize;

	ret = Decode(decoder->ADecoder, &packet, decoder->AFrame);
	if (ret < 0)
	{
		printf("Decode audio error.\n");
		av_packet_unref(&packet);
		return ret;
	}

	int data_size = av_get_bytes_per_sample(decoder->ADecoder->sample_fmt);

	for (int i = 0; i < decoder->AFrame->nb_samples; i++)
	{
		for (int ch = 0; ch < decoder->ADecoder->channels; ch++)
		{
			memcpy(aPcm, decoder->AFrame->data[ch] + data_size * i, data_size);
			aPcm += data_size;
			out_size += data_size;
		}
	}

	av_packet_unref(&packet);

	return out_size;
}

