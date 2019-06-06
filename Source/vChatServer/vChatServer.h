//-----------------------------------------------------------------------------
//!\file vChatServer.h
//!\author Lyp
//!
//!\date 2004-08-11
//! last 2007-10-11
//!
//!\brief vengine���Գ���
//-----------------------------------------------------------------------------
#pragma once

struct tagMsg
{
	DWORD dwSize;
	DWORD dwState;	// 0:�Ի� 1:���� 2:���� 3:����
	DWORD dwCode;
	TCHAR szName[32];
	TCHAR szText[1];

	tagMsg() { ZeroMemory(this, sizeof(*this)); dwSize = sizeof(tagMsg); }
};


struct tagClient
{
	DWORD	dwIP;
	DWORD	dwHandle;
	tstring strName;
};

//-----------------------------------------------------------------------------
// test
//-----------------------------------------------------------------------------
class ChatServer
{
public:
	BOOL Init(HINSTANCE hInst);
	VOID Mainloop();
	VOID Destroy();

	ChatServer():m_Trunk(this){}

private:
	TSFPTrunk<ChatServer>		m_Trunk;
	TObjRef<GDIRender>			m_pRender;
	TObjRef<Util>				m_pUtil;
	TObjRef<WindowWrap>			m_pWindow;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Console>			m_pConsole;
	TObjRef<SystemTray>			m_pTray;
	TObjRef<XServer>			m_pServer;
	TObjRef<XClient>			m_pClient;
	IGUIRenderNative			m_render;
	
	HICON						m_hIconInfo;
	CRITICAL_SECTION			m_LockPlayerData;
	DWORD						m_dwIDHolder;
	INT							m_nPort;
	TMap<DWORD, tagClient*>		m_mapClient;
	std::map<DWORD, tstring>	m_mapIP;

	VOID Maximise();
	VOID Minimise();
	VOID InitSet();

	// �ͻ��˵�½���������ص�����
	DWORD LoginCallBack(LPVOID, LPVOID);
	DWORD LogoutCallBack(LPVOID);

	// �������пͻ��˷�������Ϣ
	VOID RecvAll(BOOL bHeartBeat);
	// �����пͻ��˷�����Ϣ
	VOID SendToAll(tagMsg* pMsg, DWORD dwSize);
};

