//-----------------------------------------------------------------------------
//!\file stdafx.h
//!\author Lyp
//!
//!\date 2008-03-27
//! last 2008-03-27
//!
//!\brief ��׼ϵͳ�����ļ��İ����ļ�
//!
//!	Copyright (c) 2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
// �����궨��
//-----------------------------------------------------------------------------
#define _WIN32_WINNT 0x0400
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_DEPRECATE

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

//-----------------------------------------------------------------------------
// ϵͳͷ�ļ�
//-----------------------------------------------------------------------------
#include <windows.h>
#include <ctype.h>
#include <time.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <process.h>	// for _beginthreadex
#include <mmsystem.h>
#include <tchar.h>
#include <assert.h>
#include <eh.h>			// for "_set_se_translator"
#include <math.h>
#include <new.h>
#include <dinput.h>
#include <ddraw.h>
#include <dsound.h>
#include <dmusici.h>

//-----------------------------------------------------------------------------
// STL
//-----------------------------------------------------------------------------
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <memory>
#include <queue>

//-----------------------------------------------------------------------------
// ���õײ�����
//-----------------------------------------------------------------------------
#include "..\Cool3D\Cool3D.h"
#include "..\Cool3D\Cool3DEngine.h"
using namespace Cool3D;

#include "..\vEngine\vEngine.h"
using namespace vEngine;

#include "..\WorldDefine\time.h"
#include "..\WorldBase\WorldBaseDefine.h"
using namespace WorldBase;


//-----------------------------------------------------------------------------
// �����̳���ͷ�ļ�
//-----------------------------------------------------------------------------
#include "GameFrame.h"
#include "GameFrameMgr.h"
#include "NetSession.h"
#include "GameScript.h"
#include "GameScriptMgr.h"
#include "GameInputMap.h"

#include "MsgBox.h"
#include "StringTable.h"


//-----------------------------------------------------------------------------
// ȫ�ֱ���
//-----------------------------------------------------------------------------
extern LPCTSTR g_pAppName;
extern LPCTSTR g_pVersion;
extern LPCTSTR g_pBuildType;
extern LPCTSTR g_pBuildTime;
extern StringTable g_StrTable;

//-----------------------------------------------------------------------------
// �ֶ����� lib ��
//-----------------------------------------------------------------------------
#ifdef _DEBUG
#pragma comment(lib,"..\\vsout\\vengine\\debug\\vengine.lib")
#pragma comment(lib,"..\\vsout\\cool3d\\debug\\cool3d.lib")
#pragma comment(lib,"..\\vsout\\worldbase\\debug\\worldbase.lib")
#else
#pragma comment(lib,"..\\vsout\\vengine\\release\\vengine.lib")
#pragma comment(lib,"..\\vsout\\cool3d\\release\\cool3d.lib")
#pragma comment(lib,"..\\vsout\\worldbase\\release\\worldbase.lib")
#endif

#pragma comment(lib, "Imm32.lib")
#pragma comment(lib, "WinMM.lib")




//-----------------------------------------------------------------------------
// ����˵��
//-----------------------------------------------------------------------------
class GUIRender;
class NetSession;
class AudioSys;