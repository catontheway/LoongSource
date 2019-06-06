// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once

//-----------------------------------------------------------------------------
// �������ݿ�����(�������ͷ�ļ����Ѱ���vEngine)
//-----------------------------------------------------------------------------
#include "../Beton/Beton.h"
using namespace vEngine;

#ifdef _DEBUG
#define X_STRING_RUN_TIME "Debug"
#pragma comment(lib, "../vsout/Beton/debug/Beton.lib")
#pragma comment(lib, "../vsout/ServerDefine/debug/ServerDefine.lib")
#else
#define X_STRING_RUN_TIME "Release"
#pragma comment(lib, "../vsout/Beton/release/Beton.lib")
#pragma comment(lib, "../vsout/ServerDefine/release/ServerDefine.lib")
#endif

//------------------------------------------------------------------------------

#pragma comment(lib,"libmysql.lib")
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"winmm.lib")


#include "ResGuard.h"
#include "../WorldDefine/base_define.h"
#include "../WorldDefine/msg_common_errorcode.h"
#include "../ServerDefine/base_define.h"
#include "../WorldDefine/time.h"
//-----------------------------------------------------------------