//-----------------------------------------------------------------------------
// File: stdafx
// Desc: ����Ԥ����ͷ�ļ�
// Auth: Lyp
// Date: 2003/11/30
//
//
// include file for standard system include files, or project specific 
// include files that are used frequently, but are changed infrequently
//-----------------------------------------------------------------------------
#pragma once

#define _CRT_SECURE_NO_DEPRECATE

// ����һЩ����
#pragma warning(disable:4355)// Level 3: "this": ���ڻ���Ա��ʼֵ�趨���б�
#pragma warning(disable:4251)// Level 3: need to have dll-interface
#pragma warning(disable:4996)


//-----------------------------------------------------------------------------
// ȫ�ֻ�������
//-----------------------------------------------------------------------------
#include "globe\globe_define.h"

//-----------------------------------------------------------------------------
// ����ϵͳ����
//-----------------------------------------------------------------------------
#define WIN32_LEAN_AND_MEAN

#define _USE_IJL_		// �Ƿ�ʹ��Ijl
//#define _MEM_TRACE_		// �Ƿ�ʹ��mem_trace
//#define _USE_FMOD_	// �Ƿ�ʹ��Fmod

#ifdef VENGINE_LIB
	#define  _USE_D3D7_
	#if _WIN32_WINNT < 0x0500
	#undef _WIN32_WINNT
	#define _WIN32_WINNT 0x0500
	#endif 
#else
	#ifdef VENGINE_EXPORTS
		#define  _USE_D3D7_
		#if _WIN32_WINNT < 0x0500
			#undef _WIN32_WINNT
			#define _WIN32_WINNT 0x0500
		#endif 
	#endif 
#endif 

#include <windows.h>
#include <ctype.h>
#include <time.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <wininet.h>
#include <shlwapi.h>
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
#include <ShellAPI.h>

#ifdef _USE_D3D7_
	#include <d3d.h>
#endif

#include "debug\dbghelp.h"

#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <memory>
#include <queue>
#include <algorithm>

//-----------------------------------------------------------------------------
// ���涨��
//-----------------------------------------------------------------------------
extern HINSTANCE g_hInstance; 

// �������Ͷ���
#include "globe\globe_typedefine.h"	
#include "globe\globe_table.h"
#include "network\net_define.h"
#include "globe\interface_helper.h"

// ��׼���ݽṹ
#include "memory\mem_pool.h"
#include "memory\safe_mem_pool.h"
#include "frame\rect.h"
#include "frame\msg_queue.h"
#include "frame\simple_map.h"
#include "frame\simple_list.h"
#include "frame\sfp_trunk.h"
#include "frame\safe_list.h"
#include "frame\safe_map.h"
#include "frame\binder.h"
#include "xml\tinyXml.h"

// �ӿڹ���
#include "globe\interfacemgr.h"

// ȫ�ֺ���
#include "globe\globe_function.h"

// ���ò���
#include "util\util.h"
#include "debug\debug.h"
#include "debug\log.h"
#include "variable\var_mgr.h"
#include "gui\gui_define.h"
#include "network\server_define.h"
#include "network\x_define.h"

// �ڴ�й©���
#ifdef _MEM_TRACE_
#include "memory\mem_map.h"
#endif
