// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//
#pragma once

#define _WIN32_WINNT 0x0403

//---------------------------------------------------------------------
// vEngine
//---------------------------------------------------------------------
#include "..\vEngine\vEngine.h"
using namespace vEngine;

//---------------------------------------------------------------------
// ��ؿ�
//---------------------------------------------------------------------

#ifdef _DEBUG
#pragma comment(lib,"..\\vsout\\vEngine\\debug\\vEngine.lib")
#else
#define X_STRING_RUN_TIME "Release"
#pragma comment(lib,"..\\vsout\\vEngine\\release\\vEngine.lib")
#endif



#include "../ServerDefine/msg_rt_s.h"

//�����ļ�·��
#define RTSERVICE_CONFIG_INI		"\\RTService.ini"

// name of the executable
#define SZAPPNAME				"RtService.exe"
// internal name of the service
#define SZSERVICENAME			"Local_RtService"
// displayed name of the service
#define SZSERVICEDISPLAYNAME	"Loong Server Local Tool Service"
// service description
#define SZSERVICEDESCRIPTION	"Loong Server Local Tool Service"
// list of service dependencies - "dep1\0dep2\0\0"
#define SZDEPENDENCIES			""
