// Cool3DView.h : Cool3DView Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������


// CCool3DViewApp:
// �йش����ʵ�֣������ Cool3DView.cpp
//

class CCool3DViewApp : public CWinApp
{
public:
	CCool3DViewApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CCool3DViewApp theApp;
