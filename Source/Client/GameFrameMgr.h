//-----------------------------------------------------------------------------
//!\file GameFrameMgr.h
//!\author Lyp
//!
//!\date 2008-02-17
//! last 2008-04-15
//!
//!\brief game frame mgr
//!
//!	Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

class GameFrame;
struct tagGameEvent
{
	DWORD		dwNameCrc;
	tstring		strEventName;
	GameFrame*	pSender;	// ���ʹ��¼���Frame

	tagGameEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
	{
		strEventName = szEventName;
		dwNameCrc = TObjRef<Util>()->Crc32(szEventName);
		pSender = pSenderFrame;
	}
};


typedef DWORD (WINAPI* FRAMEEVENTPROC)(tagGameEvent* pEvent);
//-----------------------------------------------------------------------------
//!\brief game frame mgr
//!
//!
//-----------------------------------------------------------------------------
class GameFrameMgr
{
public:
	GameFrameMgr();
	~GameFrameMgr();

	BOOL Init();
	BOOL Destroy();
	VOID Update();
	VOID Render3D();

	VOID SendEvent(tagGameEvent* pEvent);

	GameFrame* CreateFrame(LPCTSTR szFatherName, LPCTSTR szName, LPCTSTR szClass, DWORD dwParam);
	GameFrame* GetFrame(LPCTSTR szName) { return m_mapChild.Peek(m_pUtil->Crc32(szName)); }
	TMap<DWORD, GameFrame*>& GetMap() { return m_mapChild; }

	VOID DestroyFrame(GameFrame* pFrame);	// ����ɾ��
	BOOL AddToDestroyList(GameFrame* pWnd);	// ��һ��ɾ��


	// ע��frame����
	template<typename T>VOID Register(TWrap<T> wrap, LPCTSTR szName);

	// ע��Frame�¼�������
	BOOL RegisterEventHandle(LPCTSTR szEvent, FRAMEEVENTPROC fp);
	BOOL UnRegisterEventHandler(LPCTSTR szEvent, FRAMEEVENTPROC fp);

private:
	TObjRef<Util>				m_pUtil;
	TAutoFactory<GameFrame>		m_Factory;

	std::list<tstring>			m_listWndType;	// ���пؼ�����
	GameFrame*					m_pRoot;		// ��frame
	TMap<DWORD, GameFrame*>		m_mapChild;		// ����frame

	TList<GameFrame*>			m_listDestroy;	// ɾ���б�

	struct tagEventHandle	// �¼�����ṹ����
	{
		tstring					strName;
		DWORD					dwCount;	// ��¼�������������ٴδ��¼�
		TList<FRAMEEVENTPROC>	listFP;
	};
	TMap<DWORD, tagEventHandle*>	m_mapHandle;	// �¼�������

};




//-----------------------------------------------------------------------------
// ע��frame
//-----------------------------------------------------------------------------
template<typename T>VOID GameFrameMgr::Register(TWrap<T> wrap, LPCTSTR szName)
{ 
	if( m_Factory.Register(wrap, m_pUtil->Crc32(szName)) )
		m_listWndType.push_back(tstring(szName));
}