/*
 * Car eye ��������ƽ̨: www.car-eye.cn
 * Car eye ��Դ��ַ: https://github.com/Car-eye-team
 * CarEyeOSDAPI.cpp
 *
 * Author: Wgj
 * Date: 2018-05-18 21:47
 * Copyright 2018
 *
 * Car eye��Ƶˮӡ��ӽӿ�ʵ��
 */
#include "CarEyeOSDAPI.h"
#include "FFVideoFilter.h"

 // ˮӡ�������ṹ�嶨��
typedef struct
{
	// ��Ƶ����֡�ݴ�
	AVFrame *VFrame;
	// ��Ƶ��Ļ����
	FFVideoFilter *VideoFilter;
}CarEyeFilter;

  /*
 * Comments: ��ˮӡ��Դ
 * Param aParam: ˮӡ����
 * @Return int �Ƿ�ɹ���0�ɹ�������ʧ��
 */
CE_API CarEye_OSD_Handle CE_APICALL CarEye_OSD_Create(CarEye_OSDParam *aParam)
{
	CarEyeFilter *filter = new CarEyeFilter;
	if (filter == NULL)
	{
		return NULL;
	}

	filter->VFrame = av_frame_alloc();
	if (filter->VFrame == NULL)
	{
		printf("Alloc video frame faile!\n");
		CarEye_OSD_Release(filter);
		return NULL;
	}

	filter->VFrame->format = (AVPixelFormat)aParam->YUVType;
	filter->VFrame->width = aParam->Width;
	filter->VFrame->height = aParam->Height;

	if (av_image_alloc(filter->VFrame->data, filter->VFrame->linesize,
		filter->VFrame->width, filter->VFrame->height,
		(AVPixelFormat)filter->VFrame->format, 16) < 0)
	{
		printf("Could not allocate raw picture buffer!\n");
		CarEye_OSD_Release(filter);
		return NULL;
	}

	filter->VideoFilter = new FFVideoFilter();
	if (filter->VideoFilter == NULL)
	{
		CarEye_OSD_Release(filter);
		return NULL;
	}
	if (filter->VideoFilter->InitFilters(aParam) != 0)
	{
		CarEye_OSD_Release(filter);
		return NULL;
	}

	return filter;
}

/*
* Comments: ������YUV֡��Ϣ���ˮӡ
* Param : aFrame: [����/���] Ҫ���ˮӡ��YUV֡����
* @Return void
*/
CE_API int CE_APICALL CarEye_OSD_Encode(CarEye_OSD_Handle aFilter, CarEye_YUVFrame *aFrame, char* txtoverlay)
{
	CarEyeFilter *filter = (CarEyeFilter *)aFilter;

	if (filter == NULL)
	{
		return -1;
	}

	memcpy(filter->VFrame->data[0], aFrame->Y, aFrame->YSize);
	memcpy(filter->VFrame->data[1], aFrame->U, aFrame->USize);
	memcpy(filter->VFrame->data[2], aFrame->V, aFrame->VSize);
	filter->VFrame->pts++;
	if (filter->VideoFilter->BlendFilters(filter->VFrame, txtoverlay) < 0)
	{
		printf("Filter video error.\n");
		return -1;
	}
	memcpy(aFrame->Y, filter->VFrame->data[0], aFrame->YSize);
	memcpy(aFrame->U, filter->VFrame->data[1], aFrame->USize);
	memcpy(aFrame->V, filter->VFrame->data[2], aFrame->VSize);
	return 0;
}


/*
* Comments: �ͷ�ˮӡ��Դ
* Param aFilter: ˮӡ����������
* @Return int �رճɹ���� 0�ɹ�
*/
CE_API void CE_APICALL CarEye_OSD_Release(CarEye_OSD_Handle aFilter)
{
	CarEyeFilter *filter = (CarEyeFilter *)aFilter;

	if (filter == NULL)
	{
		return;
	}

	if (filter->VFrame != NULL)
	{
		av_frame_free(&filter->VFrame);
		filter->VFrame = NULL;
	}
	if (filter->VideoFilter != NULL)
	{
		delete filter->VideoFilter;
		filter->VideoFilter = NULL;
	}

	delete filter;
}
