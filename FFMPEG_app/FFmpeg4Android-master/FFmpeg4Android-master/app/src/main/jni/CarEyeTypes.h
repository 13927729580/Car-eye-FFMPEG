/*
 * Car eye ��������ƽ̨: www.car-eye.cn
 * Car eye ��Դ��ַ: https://github.com/Car-eye-team
 * CarEyeTypes.h
 *
 * Author: Wgj
 * Date: 2018-05-16 21:36
 * Copyright 2018
 *
 * ����ʹ�õ����������Ͷ�������
 */
#ifndef __CarEyeTypes_H_
#define __CarEyeTypes_H_


#ifdef _WIN32
#define CE_API  __declspec(dllexport)
#define CE_APICALL  __stdcall
#else
#define CE_API
#define CE_APICALL 
#endif




#ifdef __cplusplus
  #define __STDC_CONSTANT_MACROS
  #ifdef _STDINT_H
   #undef _STDINT_H
  #endif
  # include <stdint.h>
 #endif


#ifdef __ANDROID__       //android�ı��������Զ�ʶ�����Ϊ�档

#include <android/log.h>

#define __STDC_CONSTANT_MACROS

#define CarEyeLog(...) __android_log_print(ANDROID_LOG_DEBUG, "Car-eye-ffmpeg", __VA_ARGS__)


 


#endif


 // ����������������
#define CarEye_Encoder_Handle void*
 // ����������������
#define CarEye_Decoder_Handle void*
 // ˮӡ����������������
#define CarEye_OSD_Handle void*

 // �����Ƶ֡��С 1 second of 48khz 32bit audio
#define MAX_AUDIO_FRAME_SIZE 192000

 // ý��������Ͷ��� ��FFMPEG��һһ��Ӧ��H265�����������ⶨ����Ҫת��
typedef enum
{
	// �����б���
	CAREYE_CODEC_NONE = 0,
	// H264����
	CAREYE_CODEC_H264 = 0x1C,
	// H265����
	CAREYE_CODEC_H265 = 0xAE,
	// MJPEG����
	CAREYE_CODEC_MJPEG = 0x08,
	// MPEG4����
	CAREYE_CODEC_MPEG4 = 0x0D,
	// AAC����
	CAREYE_CODEC_AAC = 0x15002,
	// G711 Ulaw���� ��ӦFFMPEG�е�AV_CODEC_ID_PCM_MULAW����
	CAREYE_CODEC_G711U = 0x10006,
	// G711 Alaw���� ��ӦFFMPEG�е�AV_CODEC_ID_PCM_ALAW����
	CAREYE_CODEC_G711A = 0x10007,
	// G726���� ��ӦFFMPEG�е�AV_CODEC_ID_ADPCM_G726����
	CAREYE_CODEC_G726 = 0x1100B,
}CarEye_CodecType;

// YUV��Ƶ����ʽ���壬��FFMPEG��һһ��Ӧ
typedef enum
{
	CAREYE_FMT_YUV420P = 0,
	CAREYE_FMT_YUV422P = 4,
	CAREYE_FMT_YUV444P = 5,
	CAREYE_FMT_YUV410P = 6,
	CAREYE_FMT_YUV411P = 7,
}CarEye_AVType;

// YUVý�����ṹ����
typedef struct
{
	// Y�������ݴ洢��
	unsigned char *Y;
	// Y���������ֽ���
	int YSize;
	// U�������ݴ洢��
	unsigned char *U;
	// U���������ֽ���
	int USize;
	// V�������ݴ洢��
	unsigned char *V;
	// V���������ֽ���
	int VSize;
}CarEye_YUVFrame;

#define MAX_STRING_LENGTH 1024
#define MAX_FILE_NAME 64
// ˮӡ�������
typedef struct
{
	// ��Ƶ���
	int Width;
	// ��Ƶ�߶�
	int Height;
	// ��Ƶ֡�ʣ�FPS��
	int FramesPerSecond;
	// ���ˮӡ����Ƶ��ʽ
	CarEye_AVType YUVType;
	// ˮӡ��ʼX������
	int X;
	// ˮӡ��ʼY������
	int Y;
	// ˮӡ�����С
	int FontSize;
	// 16���Ƶ�RGB��ɫֵ������ɫ��0x00FF00
	unsigned int FontColor;
	// ˮӡ͸���� 0~1
	float Transparency;
	// ˮӡ����
	char SubTitle[MAX_STRING_LENGTH];
	// �������ƣ������ļ��ŵ����ͬĿ¼�£��硰arial.ttf��
	// Windows��ϵͳĿ¼ʹ�ø�ʽ��"C\\\\:/Windows/Fonts/msyh.ttc"
	char FontName[MAX_FILE_NAME];
}CarEye_OSDParam;

/*
* Comments: ʹ�ñ���֮ǰ�������һ�α�����
* Param : None
* @Return void
*/
#ifdef __cplusplus
extern "C"
{
#endif
CE_API void CE_APICALL CarEye_MPEG_Init(void);

#ifdef __cplusplus
}
#endif

#endif // __CarEyeTypes_H_