//-----------------------------------------------------------------------------
// File: globe_type define
// Desc: base type define
// Auth: Lyp
// Date: 2003/12/11
// Last: 2004/03/14
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
// �������Ͷ���
//-----------------------------------------------------------------------------
#ifdef _UNICODE
	typedef std::wstring		tstring;
	typedef std::wstringstream	tstringstream;
	typedef std::wstreambuf		tstreambuf;
#else
	typedef std::string			tstring;
	typedef std::stringstream	tstringstream;
	typedef std::streambuf		tstreambuf;
#endif


namespace vEngine{

// ����
typedef unsigned short DIK;

// �����̺߳�������
typedef UINT (WINAPI* THREADPROC)(LPVOID);

// ͨ�õ��ļ�ϵͳ�ص�ָ��
typedef UINT(*FP_OPENCALLBACK)  (CONST CHAR* szFileName);
typedef VOID(*FP_CLOSECALLBACK) (UINT Handle);
typedef INT (*FP_READCALLBACK)  (LPVOID, INT Size, UINT Handle);
typedef INT (*FP_SEEKCALLBACK)  (UINT Handle, INT Pos, signed char Mode);
typedef INT (*FP_TELLCALLBACK)  (UINT Handle);

// �����������
class Audio;
class Debug;
class Console;
class DiscIOMgr;
class FastCode;
class FilterMgr;
class INIObj;
class ObjMgr;
class ImageObj;
class Log;
class MsgQueueTS;
class IRender;
class SysInfo;
class MemPool;
class TSMemMgr;
class Util;
class VarContainer;
class VirtualFileSys;
class VarMgr;
class Input;
class StreamTransport;
class XServer;
class Package;
class ImageCodec;
class LuaManager;
class LuaScript;
class InputMap;
class D3DRender;
class InputDX8; 
class GUITree;
}	// namespace vEngine{


struct lua_State;

// ���(����̨ʹ��)
//typedef std::vector<std::string> COMMANDS;


// To declare an interlocked function for use as an intrinsic,
// First, the function must be declared with the leading underscore.
// Second, the new function must appear in a #pragma intrinsic statement.
// For convenience, the intrinsic versions of the functions may be
// #define'd to appear in the source code without the leading underscore.
// Declare all of the supported interlocked intrinsic functions:
// compile with: /Oi /MT /D"_X86_"

#ifndef VOLATILE
#define VOLATILE volatile
#endif

/*extern "C"
{
	LONG  __cdecl _InterlockedIncrement(LONG VOLATILE *Addend);
	LONG  __cdecl _InterlockedDecrement(LONG VOLATILE *Addend);
	LONG  __cdecl _InterlockedCompareExchange(LPLONG VOLATILE Dest, LONG Exchange, LONG Comp);
	LONG  __cdecl _InterlockedExchange(LPLONG VOLATILE Target, LONG Value);
	LONG  __cdecl _InterlockedExchangeAdd(LPLONG VOLATILE Addend, LONG Value);
}

#pragma intrinsic (_InterlockedCompareExchange)
#define InterlockedCompareExchange _InterlockedCompareExchange

#pragma intrinsic (_InterlockedExchange)
#define InterlockedExchange _InterlockedExchange 

#pragma intrinsic (_InterlockedExchangeAdd)
#define InterlockedExchangeAdd _InterlockedExchangeAdd

#pragma intrinsic (_InterlockedIncrement)
#define InterlockedIncrement _InterlockedIncrement

#pragma intrinsic (_InterlockedDecrement)
#define InterlockedDecrement _InterlockedDecrement*/