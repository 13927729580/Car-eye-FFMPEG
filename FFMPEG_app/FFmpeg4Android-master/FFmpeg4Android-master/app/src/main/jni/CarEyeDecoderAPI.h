/*
 * Car eye ��������ƽ̨: www.car-eye.cn
 * Car eye ��Դ��ַ: https://github.com/Car-eye-team
 * CarEyeDecoderAPI.h
 *
 * Author: Wgj
 * Date: 2018-05-16 22:54
 * Copyright 2018
 *
 * Car eye����FFMPEG������Ƶ����ӿ�����
 */
#ifndef __CarEyeDecoderAPI_H_
#define __CarEyeDecoderAPI_H_

#include "CarEyeTypes.h"

// ý��֡��Ϣ����
typedef struct
{
	// ��Ƶ�����ʽ
	CarEye_CodecType VCodec;
	// ��Ƶ�����ʽ������Ƶ����ΪCAREYE_CODEC_NONE
	CarEye_CodecType ACodec;
	// ��Ƶ֡��(FPS)
	unsigned char	FramesPerSecond;
	// ��Ƶ�������
	unsigned short	Width;
	// ��Ƶ�ĸ߶�����
	unsigned short  Height;
	// ��Ƶ���ʣ�Խ����ƵԽ�������Ӧ���ҲԽ�� �磺4000000
	unsigned int	VideoBitrate;

	// ��Ƶ������
	unsigned int	SampleRate;
	// ��Ƶ������
	unsigned int	Channels;
	// ��Ƶ�������� 16λ 8λ�ȣ����ڲ��̶�Ϊ16λ
	unsigned int	BitsPerSample;
	// ��Ƶ������ �磺64000��Խ������Խ�������Ӧ���ҲԽ��
	unsigned int	AudioBitrate;
}CarEye_FrameInfo;

#ifdef __cplusplus
extern "C"
{
#endif

	/*
	* Comments: ����һ������������
	* Param aInfo: Ҫ�����ý����Ϣ
	* @Return CarEye_Decoder_Handle �ɹ����ؽ��������󣬷��򷵻�NULL
	*/
	CE_API CarEye_Decoder_Handle CE_APICALL CarEye_DecoderCreate(CarEye_FrameInfo aInfo);

	/*
	* Comments: �ͷŽ�������Դ
	* Param aDecoder: Ҫ�ͷŵĽ�����
	* @Return None
	*/
	CE_API void CE_APICALL CarEye_DecoderRelease(CarEye_Decoder_Handle aDecoder);

	/*
	* Comments: ��ȡYUV������������ֽڴ�С
	* Param aDecoder: ������
	* @Return int �����������С < 0ʧ��
	*/
	CE_API int CE_APICALL CarEye_GetYUVSize(CarEye_Decoder_Handle aDecoder);

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
		unsigned char *aYuv);

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
		unsigned char *aPcm);

#ifdef __cplusplus
}
#endif


#endif // __CarEyeDecoderAPI_H_