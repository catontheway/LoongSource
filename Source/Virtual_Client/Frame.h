#pragma once

class Player;
struct tagNS_JoinGame;

// Frame�������
class FrameBase
{
	friend class FrameMgr;

public:
	FrameBase();
	virtual ~FrameBase() = 0;

	virtual VOID Init() = 0;
	virtual VOID Update() = 0;
	virtual VOID Destroy() = 0;

protected:
	DWORD				m_dwStateTime;
	Player*				m_pPlayer;
	BYTE				m_eFrameType;
};

// LoginFrame
class LoginFrame : public FrameBase
{
public:
	LoginFrame(Player* pPlayer);
	~LoginFrame();

	VOID Init();
	VOID Update();
	VOID Destroy();

private:
	TSFPTrunk<LoginFrame>		m_Trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<VarContainer>		m_pVarContainer;
	TObjRef<NetCmdMgr>			m_pCmdMgr;

	TObjRef<XClient>			m_pWorldSession;	//��world������
	TObjRef<XClient>			m_pLoginSession;	//��login������

	enum ELoginState
	{
		ELS_Abandon,
		ELS_WaitRetry,
		ELS_Connect,
		ELS_WaitResponse,
		ELS_WaitProofResult,
		ELS_Queue,
		ELS_ConnectZoneServer,		// �������������
		ELS_SentJoinGame,			// ����JoinGame��Ϣ
		ELS_JoinGame,				// ���յ�JoinGame��Ϣ��
	};

	ELoginState					m_eState;
	DWORD						m_dwZoneIP;			//ѡ���Zone������IP
	DWORD						m_dwZonePort;		//ѡ���Zone������port
	DWORD						m_dwAccountID;		//joingame�õ�
	DWORD						m_dwVerifyCode;		//joingame�õ�
	DWORD						m_dwSentJoinGame;

	VOID Connect();
	VOID WaitResponse();
	VOID WaitProofResult();	
	VOID WaitQueue();
	VOID TryIsConnectZoneSer();

	// ����JoinGame��Ϣ
	void NetRecvJoinGame(tagNS_JoinGame* pMsg, LPVOID pPrama); 

	// ��ʾ�û���\���������Ϣ
	VOID ShowUserPwdInfo(DWORD nInfo);

	// �Ͽ���zone������������
	VOID CutOffZone();

	// �Ͽ���login������������
	VOID CutOffLogin();

	VOID SetState(ELoginState eState);
};

// SelectRoleFrame
struct tagNS_EnumRole;
struct tagNS_SelectRole;
struct tagNS_CreateRole;
struct tagNS_SynchronismTime;

class SelectRoleFrame : public FrameBase
{
public:
	SelectRoleFrame(Player* pPlayer);
	~SelectRoleFrame();

	VOID Init();
	VOID Update();
	VOID Destroy();

private:
	enum ESRState
	{
		ESRS_EnumRole,					//��zone������������ҵĽ�ɫ��Ϣ
		ESRS_SentEnumRole,				//�ѷ��ͽ�ɫ��Ϣ����
		ESRS_SentCreateRole,			//���ʹ�����ɫ��Ϣ
		ESRS_SentSelectRole,			//����ѡ���ɫ������Ϸ����
		ESRS_RecvEnumRole,				//���մ���Ľ�ɫ�б�
		ESRS_RecvSelectRole,			//���ս�ɫ������Ϸ����
		ESRS_SelectedRole,				//ѡ���˽�ɫ
		ESRS_Back,						//���ص���¼
	};

	BOOL EnterGame(DWORD dwLocalRoleID);
	VOID SendEnterGameMsg();
	BOOL ReturnGame();
	BOOL CreateRole();

	VOID SetCurState(ESRState state) { m_eCurState = state; m_dwStateTime = timeGetTime();}

	// ������ҵĽ�ɫ��Ϣ
	void NetRecvEnumRole(tagNS_EnumRole* pMsg, LPVOID pPrama);

	// ���մ�����ɫ��Ϣ
	void NetRecvCreateRole(tagNS_CreateRole* pMsg, LPVOID pPrama);

	// ������ҵ�ѡ���ɫ��Ϣ
	void NetRecvSelectRole(tagNS_SelectRole* pMsg, LPVOID pPrama);

	// �ͻ���ʱ��ͬ����Ϣ
	void NetRecvSynchronismTime(tagNS_SynchronismTime* pMsg, LPVOID pPrama);

private:
	TSFPTrunk<SelectRoleFrame>	m_Trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<NetCmdMgr>			m_pCmdMgr;

	TObjRef<XClient>			m_pWorldSession;	//��world������

	DWORD						m_nSelectRoleID;
	ESRState					m_eCurState;	
	DWORD						m_dwStateTime;
};

// LoongWorldFrame

struct tagNS_GotoNewMap;
struct tagNS_MoveFailed;
struct tagNS_RoleChat;

class WorldFrame : public FrameBase
{
public:
	enum EWorldState
	{
		EWState_GotoNewMap,			//���շ����͵�gotomap��Ϣ
		EWState_Playing,			//���Կ�ʼ����
	};

public:
	WorldFrame(Player* pPlayer);
	~WorldFrame();

	VOID Init();
	VOID Update();
	VOID Destroy();

private:
	TSFPTrunk<WorldFrame>		m_Trunk;
	TObjRef<VarContainer>		m_pVarContainer;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	EWorldState					m_curState;
	TObjRef<Util>				m_pUtil;

	TObjRef<XClient>			m_pWorldSession;	//��world������

	DWORD						m_dwNextChatTime;
	DWORD						m_dwNextMoveTime;

	BYTE						m_byChatChannel;

	DWORD						m_dwLogoutTime;
	DWORD						m_dwOnlineTime;

	void	RegisterALLRecvCmd();
	void	UnRegisterALLRecvCmd();

	void	SwitchToState(EWorldState state);

	// ���շ�������Ϣ
	void	NetRecvGotoMap(tagNS_GotoNewMap* pMsg, LPVOID pPrama);
	void	NetRecvMoveFailed(tagNS_MoveFailed* pMsg, LPVOID pPrama);
	void	NetRecvRoleChat(tagNS_RoleChat* pMsg, LPVOID pPrama);
	void	NetRecvDoNothing(tagNetCmd* pMsg, LPVOID pPrama);

	// �������������Ϣ
	void	SendChatMsg();
	void	SendMoveMsg();

	// �Ͽ������������
	void	Disconnection();

	// ����
	BOOL	ReturnGame();
};
