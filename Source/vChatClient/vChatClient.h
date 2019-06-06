#pragma once

//-----------------------------------------------------------------------------
//!\file vChatClient.h
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

//-----------------------------------------------------------------------------
// test
//-----------------------------------------------------------------------------
class ChatClient
{
public:
	BOOL Init(HINSTANCE hInst);
	VOID Mainloop();
	VOID Destroy();

	ChatClient():m_Trunk(this){}

private:
	TSFPTrunk<ChatClient>			m_Trunk;
	TObjRef<GDIRender>				m_pRender;
	TObjRef<Util>					m_pUtil;
	TObjRef<WindowWrap>				m_pWindow;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<Thread>					m_pThread;
	TObjRef<Console>				m_pConsole;
	TObjRef<SystemTray>				m_pTray;
	TObjRef<XClient>				m_pClient;
	IGUIRenderNative				m_render;

	BOOL VOLATILE 					m_bTerminateThread;
	CHAR							m_szDestIP[256];
	INT								m_nPort;
	DWORD							m_dwSelfIP;
	HICON							m_hIconQuest;
	HICON							m_hIconInfo;

	VOID Maximise();
	VOID Minimise();
	VOID InitClient();

	// δ֪����̨�������
	DWORD Sender(LPCTSTR szText);
	// �����߳�
	DWORD ThreadXClient(INT);
	// ���ݴ�����
	VOID Fileter(tagMsg* pMsg, DWORD dwSize);
	
};

