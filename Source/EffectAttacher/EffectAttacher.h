// EffectAttacher.h : EffectAttacher Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������


// CEffectAttacherApp:
// �йش����ʵ�֣������ EffectAttacher.cpp
//

class CEffectAttacherApp : public CWinApp
{
public:
	CEffectAttacherApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CEffectAttacherApp theApp;
