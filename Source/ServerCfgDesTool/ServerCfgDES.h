// ServerCfgDES.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������



// CServerCfgDESApp:
// �йش����ʵ�֣������ ServerCfgDES.cpp
//

class CServerCfgDESApp : public CWinApp
{
public:
	CServerCfgDESApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CServerCfgDESApp theApp;
