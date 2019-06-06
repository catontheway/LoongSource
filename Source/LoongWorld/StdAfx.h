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

#define _WIN32_WINNT 0x0403

//----------------------------------------------------------------------
// Cool3D
//----------------------------------------------------------------------
#include "..\Cool3D\Cool3DEngine.h"
using namespace Cool3D;

#ifdef _DEBUG
#pragma comment(lib,"..\\vsout\\Cool3D\\debug\\Cool3D.lib")
#else
#pragma comment(lib,"..\\vsout\\Cool3D\\release\\Cool3D.lib")
#endif

//---------------------------------------------------------------------
// vEngine
//---------------------------------------------------------------------
#include "..\vEngine\vEngine.h"
using namespace vEngine;

#ifdef _DEBUG
#pragma comment(lib,"..\\vsout\\vEngine\\debug\\vEngine.lib")
#else
#define X_STRING_RUN_TIME "Release"
#pragma comment(lib,"..\\vsout\\vEngine\\release\\vEngine.lib")
#endif

//#ifdef ASSERT
//#undef ASSERT
//#define ASSERT(f)		((void)0)
//#endif



//-----------------------------------------------------------------------
// ServerDefine
//-----------------------------------------------------------------------
#ifdef _DEBUG
#pragma comment(lib,"..\\vsout\\ServerDefine\\debug\\ServerDefine.lib")
#else
#pragma comment(lib,"..\\vsout\\ServerDefine\\release\\ServerDefine.lib")
#endif


//-----------------------------------------------------------------------
// WorldBase
//-----------------------------------------------------------------------
#include "..\WorldBase\WorldBaseDefine.h"
using namespace WorldBase;

#ifdef _DEBUG
#pragma comment(lib,"..\\vsout\\WorldBase\\debug\\WorldBase.lib")
#else
#pragma comment(lib,"..\\vsout\\WorldBase\\release\\WorldBase.lib")
#endif


#ifdef _DEBUG
#  pragma comment (lib, "..\\ca\\debug\\ClientLib.lib" )
#else
#  pragma comment (lib, "..\\ca\\release\\ClientLib.lib" )
#endif

//-----------------------------------------------------------------------
// Other
//-----------------------------------------------------------------------
#include "../WorldDefine/time.h"

//------------------------------------------------------------------------------
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"winmm.lib")

//-------------------------------------------------------------------------------
class World;
class WorldSession;
class DBSession;
class PlayerSession;
class LoginSession;
class Map;
class MapMgr;
class MapCreator;
class Role;
class PlayerNetCmdMgr;
class CSocialMgr;
class CGroupMgr;
class CTradeYB;
class CActivityMgr;

