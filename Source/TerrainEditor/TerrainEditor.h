// TerrainEditor.h : TerrainEditor DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CTerrainEditorApp
// �йش���ʵ�ֵ���Ϣ������� TerrainEditor.cpp
//

class CTerrainEditorApp : public CWinApp
{
public:
	CTerrainEditorApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
