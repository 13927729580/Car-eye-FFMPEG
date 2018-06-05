/*
 * Car eye ��������ƽ̨: www.car-eye.cn
 * Car eye ��Դ��ַ: https://github.com/Car-eye-team
 * CarEyeEncoderAPI.h
 *
 * Author: Wgj
 * Date: 2018-05-16 22:40
 * Copyright 2018
 *
 * Car eye����FFMPEG������Ƶ����ӿ�����
 */
#ifndef __CarEyeEncoderAPI_H_
#define __CarEyeEncoderAPI_H_

#include "CarEyeTypes.h"

// ԭʼ���ṹ����
typedef struct
{
	// ��Ƶ��������ʽ
	CarEye_AVType InVideoType;
	// �����������Ƶ����ʽ�����������������ΪCAREYE_CODEC_NONE
	CarEye_CodecType OutVideoType;
	// �����������Ƶ����ʽ�����������������ΪCAREYE_CODEC_NONE
	CarEye_CodecType OutAudioType;
	// ��Ƶ֡��(FPS)���Ƽ�ֵ��25
	unsigned char	FramesPerSecond;
	// ��Ƶ�������
	unsigned short	Width;
	// ��Ƶ�ĸ߶�����
	unsigned short  Height;
	// һ��ͼƬ�е�ͼƬ�������Ƽ�ֵ��10
	int				GopSize;
	// ��B֮֡���B֡������������Ƽ�ֵ��1
	int				MaxBFrames;
	// ��Ƶ���ʣ�Խ����ƵԽ�������Ӧ���ҲԽ�� �磺4000000
	unsigned int	VideoBitrate;

	// ��Ƶ������ �磺44100
	unsigned int	SampleRate;
	// ��Ƶ������ �磺64000��Խ������Խ�������Ӧ���ҲԽ��
	unsigned int	AudioBitrate;
}CarEye_OriginalStream;

#ifdef __cplusplus
extern "C"
{
#endif

	/*
	* Comments: ����һ������������
	* Param aInfo: Ҫ�����ý����Ϣ
	* @Return CarEye_Encoder_Handle �ɹ����ر��������󣬷��򷵻�NULL
	*/
	CE_API CarEye_Encoder_Handle CE_APICALL CarEye_EncoderCreate(CarEye_OriginalStream aInfo);

	/*
	* Comments: �ͷű�������Դ
	* Param aEncoder: Ҫ�ͷŵı�����
	* @Return None
	*/
	CE_API void CE_APICALL CarEye_EncoderRelease(CarEye_Encoder_Handle aEncoder);

	/*
	* Comments: ������YUV��Ƶ����Ϊ���úõĸ�ʽ�������
	* Param aEncoder: ���뵽����Ч������
	* Param aFilter: �������ˮӡ�������Ѵ�����ˮӡ����������
	* Param aYuv: Ҫ�����YUV����
	* Param aBytes: [���]��������Ƶ��
	* @Return int < 0����ʧ�ܣ�> 0Ϊ����������ֽڸ��� ==0��ʾ������Ч
	*/
	CE_API int CE_APICALL CarEye_EncoderYUV(CarEye_Encoder_Handle aEncoder,
										CarEye_YUVFrame *aYuv, int pts, unsigned char *aBytes);

	/*
	* Comments: ��ȡPCM����ʱ���ܵ�����ֽ���
	* Param aEncoder: ���뵽����Ч������
	* @Return PCM���뻺��������ֽ���
	*/
	CE_API int CE_APICALL CarEye_GetPcmMaxSize(CarEye_Encoder_Handle aEncoder);

	/*
	* Comments: �������PCM��Ƶ����Ϊָ�����ݸ�ʽ���
	* Param aEncoder: ���뵽����Ч������
	* Param aPcm: Ҫ�����PCM����
	* Param aSize: Ҫ������Ƶ���ֽ���
	* Param aBytes: [���] ��������Ƶ��
	* Param aPts: ��ǰ����֡�����
	* @Return int < 0����ʧ�ܣ�> 0Ϊ�����PCM���ֽڸ��� ==0��ʾ������Ч
	*/
	CE_API int CE_APICALL CarEye_EncoderPCM(CarEye_Encoder_Handle aEncoder,
		unsigned char *aPcm, int aSize, int aPts,
		unsigned char *aBytes);

#ifdef __cplusplus
}
#endif


#endif // __CarEyeEncoderAPI_H_