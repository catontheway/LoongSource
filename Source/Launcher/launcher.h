// launcher.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// ClauncherApp:
// �йش����ʵ�֣������ launcher.cpp
//

class ClauncherApp : public CWinApp
{
public:
	ClauncherApp();
	virtual ~ClauncherApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern ClauncherApp theApp;