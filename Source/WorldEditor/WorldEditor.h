// WorldEditorMgr.h : WorldEditor Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������


// CWorldEditorApp:
// �йش����ʵ�֣������ WorldEditor.cpp
//

class CWorldEditorApp : public CWinApp
{
public:
	CWorldEditorApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int Run();

// ʵ��

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);

public:
	static	bool m_bMyActive;
};

extern CWorldEditorApp theApp;
