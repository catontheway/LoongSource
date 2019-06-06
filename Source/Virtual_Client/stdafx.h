#pragma once

//-----------------------------------------------------------------------------
// ���õײ�����
//-----------------------------------------------------------------------------


#pragma warning(disable:4311)
#pragma warning(disable:4312)
#pragma warning(disable:4267)

#define _WIN32_WINNT 0x0403
#define _USE_D3D7_
//����vEngineͷ�ļ�
#include "..\vEngine\vEngine.h"
using namespace vEngine;

#ifdef _DEBUG
#define X_STRING_RUN_TIME "Debug"
#pragma comment(lib,"..\\vsout\\vEngine\\debug\\vEngine.lib")
#else
#define X_STRING_RUN_TIME "Release"
#pragma comment(lib,"..\\vsout\\vEngine\\release\\vEngine.lib")
#endif

////----------------------------------------------------------------------
//// Cool3D
////----------------------------------------------------------------------
//#include "..\Cool3D\Cool3D.h"
//#include "..\Cool3D\Cool3DEngine.h"
//using namespace Cool3D;
//
//#ifdef _DEBUG
//#pragma comment(lib,"..\\vsout\\Cool3D\\debug\\Cool3D.lib")
//#else
//#pragma comment(lib,"..\\vsout\\Cool3D\\release\\Cool3D.lib")
//#endif

#include "..\WorldDefine\base_define.h"

#include "..\WorldDefine\time.h"

struct Vector3 
{
	float x;
	float y;
	float z;
};
struct tagWorldInfo
{
	
};
struct tagLoginInfo 
{
	INT			nPort;				// LoginServer IP
	INT			nInLogin;			// ���ڵ�½���������
	INT			nInSelect;			// ��ѡ�˵�����
	INT			nInWorld;			// ��Ϸ�е�����
	TList<tstring>* listWorldName;	// LoongWorld�����б�
	tstring		strLoginIP;			// LoginServer IP

	//tstring		strWorldName;		// LoongWorld����

	tagLoginInfo()
	{
		nPort = 0;
		nInLogin = nInSelect = nInWorld = 0;
		strLoginIP.clear();
		//strWorldName.clear();
		listWorldName = NULL;
	}

	~tagLoginInfo()
	{
		SAFE_DEL(listWorldName);
	}
};

//����STL
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <list>
using namespace std;
