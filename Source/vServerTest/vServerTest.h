//-----------------------------------------------------------------------------
//!\file vServerTest.h
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
	DWORD dwIndex;
	DWORD dwTimeServerSend;
	DWORD dwTimeServerRecv;
	DWORD dwTimeClientSend;
	DWORD dwTimeClientRecv;
	CHAR  szMess[1];
};

struct tagClient
{
	DWORD dwID;
	DWORD dwHandle;
};

//-----------------------------------------------------------------------------
// test
//-----------------------------------------------------------------------------
class CServerTest
{
public:
	BOOL Init(HINSTANCE hInst);
	VOID Mainloop();
	VOID Destroy();

	CServerTest():m_Trunk(this){}

private:
	TSFPTrunk<CServerTest>		m_Trunk;
	TObjRef<GDIRender>			m_pRender;
	TObjRef<Util>				m_pUtil;
	TObjRef<WindowWrap>			m_pWindow;
	TObjRef<GUISystem>			m_pGUI;
	IGUIRenderNative			m_render;

	TObjRef<Console>			m_pConsole;
	CRITICAL_SECTION			m_LockPlayerData;
	//TObjRef<CompletionSession>	m_pServer;
	TObjRef<XServer>			m_pServer;

	DWORD						m_dwIDHolder;
	TMap<DWORD, tagClient*>		m_mapClient;

	VOLATILE DWORD				m_dwMinDelay;	// ������Сֵ
	VOLATILE DWORD				m_dwMaxDelay;	// �������ֵ
	VOLATILE DWORD				m_dwAveDelay;	// ����ƽ��ֵ
	VOLATILE DWORD				m_dwPackageNum;	// �����հ���
	VOLATILE DWORD				m_dwBigDelayNum;// ����ƽ��ֵ��2������



	// �ͻ��˵�½���������ص�����
	DWORD LoginCallBack(LPVOID, LPVOID);
	DWORD LogoutCallBack(LPVOID);

	// �����пͻ��˷�����Ϣ
	VOID SendToAll();
	// �������пͻ��˷�������Ϣ
	VOID RecvAll();
};

