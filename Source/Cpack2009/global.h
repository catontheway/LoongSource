#pragma	   once

//----------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <strstream>
#include <string>
using namespace std;
//----------------------------------------------------------------------
//����ĳЩ����

#pragma warning(disable:4244) //�������� : �ӡ�SOCKET��ת������DWORD�������ܶ�ʧ����
#pragma warning(disable:4201)
#pragma warning(disable:4100)
#pragma warning(disable:4005)
#pragma warning(disable:4312) //����ת�����ӡ�DWORD��ת���������...
#pragma warning(disable:4311) //������ת����...
#pragma warning(disable:4355) //��this�� : ���ڻ���Ա��ʼֵ�趨���б�
#pragma warning(disable:4533) //��goto������ĳЩ��ʼ������

#if defined(_WIN64)
typedef __int64 INT3264, *PINT3264;
typedef unsigned __int64 UINT3264, *PUINT3264;
typedef __int64 LONG3264, *PLONG3264;
typedef unsigned __int64 ULONG3264, *PULONG3264, DWORD3264, *PDWORD3264;
#define __int3264   __int64
#else
typedef int INT3264, *PINT3264;
typedef unsigned int UINT3264, *PUINT3264;
typedef long LONG3264, *PLONG3264;
typedef unsigned long ULONG3264, *PULONG3264, DWORD3264, *PDWORD3264;
#define __int3264   __int32
#endif

typedef signed char 			SBYTE;
typedef signed short			SWORD;
typedef signed long 			SDWORD;
typedef unsigned char			*POINTER;
typedef unsigned short int		UINT2;
typedef unsigned long int		UINT4;

//�ͷ��ڴ�����
#define FREEARR(p)		{ {delete [] (p);(p) = NULL;} }
#define FREE(p)			{ {delete (p); (p) = NULL;} }

//---------------------------------------------------------------------

typedef UINT (WINAPI* THREADPROC)(LPVOID);


