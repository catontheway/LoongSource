//-----------------------------------------------------------------------------
//!\file GameFrame.h
//!\author Lyp
//!
//!\date 2008-02-17
//! last 2008-04-15
//!
//!\brief game frame
//!
//!	Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once


class GameFrameMgr;
struct tagGameEvent;
//-----------------------------------------------------------------------------
//!\brief game frame
//!
//!
//-----------------------------------------------------------------------------
class GameFrame
{
	friend class GameFrameMgr;

public:
	GameFrame();
	virtual ~GameFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual VOID Update();
	virtual VOID Render3D();

	virtual BOOL ReloadUI();

	virtual VOID AddChild(GameFrame* pChild) {m_listChild.PushBack(pChild);}	// ����Ӵ���
	virtual VOID DelChild(GameFrame* pChild) {m_listChild.Erase(pChild);} // ɾ���Ӵ���

	virtual BOOL EscCancel() { return FALSE; }

	GameFrame* GetFather() { return m_pFather; }
 	const tstring& GetName() { return m_strName; }			// �õ�����

protected:
	TObjRef<Util>		m_pUtil;
	TObjRef<Console>	m_pConsole;
	GameFrameMgr*		m_pMgr;

	DWORD				m_dwID;					// name CRC32 id
	DWORD				m_dwClassID;			// classID
	tstring				m_strName;				// frame name
	GameFrame*			m_pFather;				// Ψһ��frame
	TList<GameFrame*>	m_listChild;			// ����һ����frame


};


