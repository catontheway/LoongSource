// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// DummyClient.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

// TODO: �� STDAFX.H ��
//�����κ�����ĸ���ͷ�ļ����������ڴ��ļ�������

#ifdef _DEBUG
	#pragma comment(lib,"..\\VSOut\\Cool3D\\Debug\\Cool3D.lib")
	#pragma comment(lib,"..\\VSOut\\WorldBase\\Debug\\WorldBase.lib")
#else
	#pragma comment(lib,"..\\VSOut\\Cool3D\\Release\\Cool3D.lib")
	#pragma comment(lib,"..\\VSOut\\WorldBase\\Release\\WorldBase.lib")
#endif


HWND g_hWnd;