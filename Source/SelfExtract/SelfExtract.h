// SelfExtract.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CSelfExtractApp:
// �йش����ʵ�֣������ SelfExtract.cpp
//

class CSelfExtractApp : public CWinApp
{
public:
	CSelfExtractApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSelfExtractApp theApp;
