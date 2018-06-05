/*
 * Car eye ��������ƽ̨: www.car-eye.cn
 * Car eye ��Դ��ַ: https://github.com/Car-eye-team
 * CarEyeOSDAPI.h
 *
 * Author: Wgj
 * Date: 2018-05-18 21:48
 * Copyright 2018
 *
 * Car eye��Ƶˮӡ��ӽӿ�����
 */

#ifndef __CarEyeOSDAPI_H_
#define __CarEyeOSDAPI_H_

#include "CarEyeTypes.h"
#ifdef __cplusplus
extern "C"
{
#endif
 /*
 * Comments: ��ˮӡ��Դ
 * Param aParam: ˮӡ����
 * @Return int �Ƿ�ɹ���0�ɹ�������ʧ��
 */
CE_API CarEye_OSD_Handle CE_APICALL CarEye_OSD_Create(CarEye_OSDParam *aParam);

/*
* Comments: ������YUV֡��Ϣ���ˮӡ
* Param : aFrame: [����/���] Ҫ���ˮӡ��YUV֡����
* @Return void
*/
CE_API int CE_APICALL CarEye_OSD_Encode(CarEye_OSD_Handle aFilter, CarEye_YUVFrame *aFrame, char* txtoverlay);

/*
* Comments: �ͷ�ˮӡ��Դ
* Param aFilter: ˮӡ����������
* @Return int �رճɹ���� 0�ɹ�
*/
CE_API void CE_APICALL CarEye_OSD_Release(CarEye_OSD_Handle aFilter);
#ifdef __cplusplus
}
#endif
#endif // __CarEyeFilterAPI_H_
