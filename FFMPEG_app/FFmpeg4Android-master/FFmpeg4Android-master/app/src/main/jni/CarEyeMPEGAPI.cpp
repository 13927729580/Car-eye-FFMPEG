/*
 * Car eye ��������ƽ̨: www.car-eye.cn
 * Car eye ��Դ��ַ: https://github.com/Car-eye-team
 * CarEyeMPEGAPI.cpp
 *
 * Author: Wgj
 * Date: 2018-05-18 23:25
 * Copyright 2018
 *
 * Car eye MPEG������ӿ�ʵ��
 */

#include "CarEyeTypes.h"

extern "C"
{
#include "libavformat/avformat.h"
#include "libavfilter/avfilter.h"
};
/*
* Comments: ʹ�ñ���֮ǰ�������һ�α�����
* Param : None
* @Return void
*/
CE_API void CE_APICALL CarEye_MPEG_Init(void)
{
	// ע�������
	av_register_all();
	// ע��OSD��
	avfilter_register_all();
}
