// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//
#pragma once

//#define _Cool3D_LEAK_DECTOR_
#ifdef _Cool3D_LEAK_DECTOR_
#include "vld.h"
#endif
#pragma warning(disable:4996)

#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
#include <windows.h>

// TODO: �ڴ˴����ó���Ҫ��ĸ���ͷ�ļ�
#include <stdio.h>
#include <stdlib.h>


#include <D3D9.h>
#include <d3dx9math.h>
#pragma comment(lib,"d3dx9.lib")

//--STL
#include <string>
#include <vector>
#include <memory>
#include <list>
#include <algorithm>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <locale>
using namespace std;

#include ".\Util\UnicodeUtil.h"

//#include "MemDebug/MemoryManager.h"