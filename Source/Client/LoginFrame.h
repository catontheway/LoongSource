//-----------------------------------------------------------------------------
//!\file LoginFrame.h
//!\author Lyp
//!
//!\date 2008-02-17
//! last 2008-04-15
//!
//!\brief login frame
//!
//!	Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once
#include "..\Cool3D\Animation\AnimationCtrl.h"
#include "..\WorldDefine\selectrole.h"

//-----------------------------------------------------------------------------
//!\brief login frame
//!
//!
//-----------------------------------------------------------------------------
class NetSession;
class Client;
class LoginFrame : public GameFrame
{
public:
	LoginFrame();
	~LoginFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Render3D();
	virtual VOID Update();
	
	VOID SetPlayState(int nState) { m_nPlayState = nState; }

	VOID Connect();

	void SetEncrypt( const char* szEncry , const char* szUserName ); 
	void SetUserNameUI( const char* szUserName );

private:
	TSFPTrunk<LoginFrame>		m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<XClient>			m_pStream;
	TObjRef<VarContainer>		m_pVarContainer;
	TObjRef<NetSession>			m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<Client>				m_pClient;
	
	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndKey;
	GUIStatic*					m_pWndCaption;
	GUIStatic*					m_pWndPswHint;
	GUIEditBox*					m_pEditAccount;
	GUIEditBox*					m_pEditPassword;
	GUIButton*					m_pButtonLogin;
	GUIButton*					m_pButtonQuit;
	GUIStatic*					m_pStcHint;				//������ʾ
	GUIPushButton*				m_pPBtn;
	GUIPushButton*				m_pPBtnSaveAccount;
	GUIPushButton*				m_pPBtn_Tab;			//��Сд�л�
	GUIStatic*					m_pStcSavePass;
	GUIStatic*					m_pStcUser;
	GUIEditBox*					m_pLastActive;

	GUIWnd*						m_pWndWorldList;		//������ѡ��
	GUIStatic*					m_pStcWLHint;
	GUIButton*					m_pBtnOKWorld;
	GUIButton*					m_pBtnCancelWorld;
	GUIListBox*					m_pWorldList;

	GUIWnd*						m_pWndQueue;
	GUIStatic*					m_pStcTime;				//�Ŷ�ʣ��ʱ��
	GUIStatic*					m_pStcPosition;			//�Ŷӵ�λ��
	GUIButton*					m_pBtnCancelQueue;		//ȡ���Ŷ�

	GUIWnd*						m_pWndSecurity;			//�ܱ�
	GUIStatic*					m_pStcSecuHint;			//�ܱ���ʾ
	GUIEditBox*					m_pStcSecuNum;			//��ʾ������

	GUIWnd*						m_pWndSeInputBack;		//�ܱ����뱳��
	GUIButton*					m_pBtnSecuInput[10];	//
	GUIStatic*					m_pStcInputHint;		//
	GUIButton*					m_pBtnSecuOK;			//
	GUIButton*					m_pBtnSecuCancel;		

	bool						m_bLoginType;			// �Ƿ��������е�½��ʽ
	

	enum EMBPhase
	{
		EMBP_NULL	= -1,					// ��ʼ
		EMBP_First	= 0,					// �ܱ���һ�׶�
		EMBP_Second	= 2,					// �ܱ��ڶ��׶�
		EMBP_Third	= 4,					// �ܱ������׶�
		EMBP_End	= 6,					// ����
	};

	enum ELoginState
	{
		ELS_WaitInput,
		ELS_Connect,
		ELS_WaitProofResult,
		ELS_MiBao,
		ELS_Queue,
		ELS_ConnectZoneServer,		// �������������
		ELS_SentJoinGame,			// ����JoinGame��Ϣ
		ELS_JoinGame,				// ���յ�JoinGame��Ϣ��
		ELS_Launcher,				// �汾����,�������³���
	};

	enum
	{
		Loading	= 0,				//0������״̬��
		Preface,					//1�����ſ���������
		Loop,						//2�������м䶯����
		Over,						//3�����Ž���������
		Enter,						//4����������ѡ�����
	};

	enum EXLHint
	{
		EXLHint_User,				//�û���������ʾ
		EXLHint_UserError,			//�û������������ʾ
		EXLHint_SendMsg,			//������Ϣ��ʾ
		EXLHint_ConnectFail,		//��ʱ
		EXLHint_Account_No_Match,	
		EXLHint_Account_In_Use,
		EXLHint_Account_Freezed,
		EXLHint_Wrong_Build_Number,
		EXLHint_Disabled,
		EXLHint_NoFindLauncher,
		EXLHint_Wrong_Type,
		EXLHint_Server_Full,
		EXLHint_Server_Suspended,
		EXLHint_Server_Maintenance,
		EXLHint_Failed,
		EXLHint_Short_Time,
		EXLHint_No_Select,
		EXLHint_GameWorldName_Wrong,
		EXLHint_SystemError,
		EXLHint_ProofResult_Forbid_MemberCenter,
		EXLHint_ProofResult_Forbid_GMTool,
		EXLHint_ProofResult_Forbid_CellPhone,
		EXLHint_ProofResult_Forbid_MiBao,
	};

	ELoginState					m_eState;
	DWORD						m_dwStateTime;
	DWORD						m_dwZoneIP;			//ѡ���Zone������IP
	DWORD						m_dwZonePort;		//ѡ���Zone������port
	FLOAT						m_fPingTime;		//pingʱ��
	NullSceneGraph*				m_pSG;				//����
	CameraEuler					m_Camera;			//�����
	EntityNode*					m_EntityNode;		//����
	VertexAnimationCtrl*		m_pAniCtrl;			//�ؼ�֡����
	float						m_pTime;			//ÿ�ζ����Ŀ�ʼʱ��
	ResRenderable*				m_pResRenderable;	//��Դ
	int							m_nPlayState;		//�������ŵ�״̬
	BOOL						m_bEnter;			//�Ƿ��������ѡ�����
	DWORD						m_dwAccountID;		//joingame�õ�
	DWORD						m_dwVerifyCode;		//joingame�õ�
	DWORD						m_dwSentJoinGame;
	BOOL						m_bSelectedWorld;
	BOOL						m_bGuard;			//�Ƿ�������˺�
	BOOL						m_bSave;

	INT							m_nPolicy;			//��֤����
	EMBPhase					m_eMB;				//�����ܱ��׶�
	BOOL						m_bMiBao;			//�Ƿ�����ܱ�����
	tstring						m_strRand;			//��¼����ַ���
	string						m_strMiBao;			//��¼�ܱ���ʾ
	tstring						m_strInputMiBao;	//��¼������ܱ�
	BOOL						m_bTab;				//��Сд

	char						m_szEncrypt[100];		//Ѹ�׽ӿڷ��صļ���
	char						m_szUserName[20];		//Ѹ�׽ӿڷ��ص��ʺ�
	EXLHint						m_eHint;			//Ѹ�׽ӿ���ʾ
	HINSTANCE					m_hDll;

	char						m_szPass[64];	// ̨������

	VOID SetState(ELoginState eState);

	DWORD EventHandler(tagGUIEvent* pEvent);
	DWORD EventHandlerForSecurity(tagGUIEvent* pEvent);
	
	VOID WaitProofResult();	
	VOID WaitQueue();
	VOID TryIsConnectZoneSer();

	void JudegPolicy();
	void SendLoginMsg();
	
	// ���ض���
	VOID LoadMap();		

	// ��ʾ�û���\���������Ϣ
	VOID ShowUserPwdInfo(DWORD nInfo);

	// ����UI
	VOID SetUIEnable(BOOL bVale);

	// ����������̰�ťλ��
	VOID SetKeyButtonPos();	

	// ����JoinGame��Ϣ
	DWORD NetRecvJoinGame(tagNS_JoinGame* pMsg, DWORD pPrama); 

	// �Ͽ���zone������������
	VOID CutOffZone();

	// �Ͽ���login������������
	VOID CutOffLogin();

	// ��ע����ȡ��ǰ�û���������
	VOID LoadLastLoginInfo();
	// ��¼��ǰ�û��������뵽ע���
	VOID SaveLastLoginInfo();

	// �����û����������еĻس����е��������ַ�
	VOID FilterString(tstring& str);

	// ����û��ܱ���ť����
	void RandSetKeyMB();

	// �ж���������׶�
	void MBPhaseHint();
	void ClearMBPhase(){ m_eMB = EMBP_NULL; } 
	void SetMBWndShow(BOOL bVal);

	LPCTSTR GetXunLeiHint(EXLHint eType);

	INT GetWindowMode(DWORD dwIndex);

	void ShellExecuteXunLei();

	void LetterTab();

	bool GetCmdLineUserAndPassword();

	void HideLoginFrame( bool bHide );

	void LoginGame();
};

