//-----------------------------------------------------------------------------
//!\file GameInputMap.h
//!\author Lyp
//!
//!\date 2008-10-09
//! last 2008-10-09
//!
//!\brief game input map
//!
//!	Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once


//-----------------------------------------------------------------------------
//!\brief ��������ע��
//!
//-----------------------------------------------------------------------------
class GameInputMap : public InputMap
{
public:
	virtual BOOL LoadGUI(LPCTSTR szWindowName, tagPoint& ptPos);
	virtual VOID UnloadGUI();
	virtual VOID Update();

	BOOL LoadFromFile();
	BOOL SaveToFile();

	VOID SetAllEnable(BOOL bEnable=TRUE) { m_bEnable = bEnable; }

protected:
	TObjRef<GameFrameMgr>		m_pFrameMgr;

	GUIButton*					m_pWndCancle;
	GUIButton*					m_pWndClose;
	GUIButton*					m_pWndDefault;
	GUIPushButton*				m_pWndPage1;
	GUIPushButton*				m_pWndPage2;
	GUIPushButton*				m_pWndPage3;

	GUIStatic*					m_pText;
	BOOL						m_bEnable;
	INT							m_nCurrentTextRow;			//��ǰlistbox��ʾ���ǵڼ���


	BOOL GUIEventHandler(tagGUIEvent* pEvent);
	virtual VOID ShowKeys();

	VOID UpdateSetup();	// ����ʱ�ĸ���

	BOOL Save();
	VOID Recover();
	VOID RecoverDefault();

	VOID CheckRepeatSet(tagKeyMapInfo* pInfoFind);	// ����Ƿ����ظ����õİ���������У������
};