// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
#pragma once

//-----------------------------------------------------------------------------
// �������ݿ�����(�������ͷ�ļ����Ѱ���vEngine)
//-----------------------------------------------------------------------------
#pragma warning(disable:4355)	// Level 3: "this": ���ڻ���Ա��ʼֵ�趨���б�
#pragma warning(disable:4251)	// Level 3: need to have dll-interface
#pragma warning(disable:4996)
#pragma warning(disable:4311)
#pragma warning(disable:4267)
#pragma warning(disable:4244)
#pragma warning(disable:4245)
#pragma warning(disable:4100)
#pragma warning(disable:4201)
#pragma warning(disable:4127)
#pragma warning(disable:4312)

#define WIN32_LEAN_AND_MEAN

#define _WIN32_WINNT 0x0500

//---------------------------------------------------------------------
// vEngine
//---------------------------------------------------------------------
#include "../vEngine/vEngine.h"
using namespace vEngine;

#ifdef _DEBUG
#pragma comment(lib,"../vsout/vEngine/debug/vEngine.lib")
#else
#define X_STRING_RUN_TIME "Release"
#pragma comment(lib,"..\\vsout\\vEngine\\release\\vEngine.lib")
#endif

//----------------------------------------------------------------------
// WorldDefine
//----------------------------------------------------------------------

#ifdef _DEBUG
#pragma comment(lib, "../vsout/WorldDefine/Debug/WorldDefine.lib")
#else
#pragma comment(lib, "../vsout/WorldDefine/Release/WorldDefine.lib")
#endif


//----------------------------------------------------------------------
// Beton
//----------------------------------------------------------------------
#include "../Beton/Beton.h"
using namespace Beton;

#ifdef _DEBUG
#pragma comment(lib, "../vsout/Beton/Debug/Beton.lib")
#else
#pragma comment(lib, "../vsout/Beton/release/Beton.lib")
#endif

//---------------------------------------------------------------------
// Other
//---------------------------------------------------------------------
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"winmm.lib")

#define SERVICE_CONFIG_INI		"config"

#include "../WorldDefine/time.h"