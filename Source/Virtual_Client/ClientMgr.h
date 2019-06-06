/////////////////////////////////////////////////////////////////////////////
// Name:       client_mgr.h
// Purpose:     ��������client
// Author:      
// Modified by:
// Created:     2008-12-09
// Copyright:   (c) 2004 TENGWU Entertainment All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef CLIENT_MGR_H_
#define CLIENT_MGR_H_

#define MAX_CLIENT 500
class Player;

class ClientMgr
{
public:
	//���캯��
	ClientMgr();
	//��������
	~ClientMgr();

	//��ʼ������
	BOOL Init(HINSTANCE hInst);
	//���º���
	VOID Update();
	//���ٺ���
	VOID Destroy();

	// ѹ�����ƹ����߳�
	VOID ThreadUpdate();

	// ������Ϣ����
	DWORD CreateClient(LPCTSTR szAccount, LPCTSTR szNum, LPCTSTR szWayPoint);
	DWORD LoadClient(LPCTSTR szXML, LPCTSTR szNum);
	DWORD TerminateClient();
	DWORD ChatControl(LPCTSTR szActive, LPCTSTR szChannel = _T("5"));
	DWORD MoveControl(LPCTSTR szActive);

	// ��Դ����/��ȡ
	VOID InitWayPoint(Player* pPlayer, vector<vector<Vector3>*>& vecWayPoints);
	BOOL GetRandomLoginServer(tagLoginInfo* &pLoginInfo) { return m_listLoginInfo.RandPeek(pLoginInfo); }
	BOOL GetRandomChatContent(tstring& strChat) { return m_listChat.RandPeek(strChat); }

// ���������
private:
	TSFPTrunk<ClientMgr>		m_Trunk;
	TObjRef<Thread>				m_pThread;
	TObjRef<Util>				m_pUtil;
	TObjRef<GDIRender>			m_pRender;
	TObjRef<WindowWrap>			m_pWindow;
	TObjRef<GUISystem>			m_pGUI;
	IGUIRenderNative			m_render;

// ��Դ�Ϳ��Ʋ���
private:
	TObjRef<VarContainer>		m_pVar;
	TObjRef<VarContainer>		m_pConfigVar;
	TObjRef<Console>			m_pConsole;
	vector<vector<Vector3>*>	m_vecWayPoints;
	TList<tagLoginInfo*>		m_listLoginInfo;	//Login��������Ϣ����
	TList<tstring>				m_listChat;			//��������

// �����˹�����
private:
	VOLATILE BOOL				m_bTerminate;		// �߳�ֹͣ��־
	INT32						m_nPressLimit;
	TList<Player*>				m_listAllPlayer;
	TList<Player*>				m_listWaiting;
	DWORD						m_dwTotalRunMinute;		// �����˻ʱ��
};

extern ClientMgr* g_pClientMgr;

#endif /* CLIENT_MGR_H_ */





