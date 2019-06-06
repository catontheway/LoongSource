#pragma once
#include "..\WorldDefine\selectrole.h"

enum EGUI_Mouse_Event
{
	EMouse_LeftDown = EGUIE_User + 110,		//������һֱ������ת��ť(ԭ�¼���û�����һֱ���µ��¼�)
};

class Player;
class NetSession;
/** \class CreateRoleFrame
\brief ������ɫ���
*/
class CreateRoleFrame :
	public GameFrame
{
public:
	enum ECRState
	{
		ECRState_Cartoon = 0,				//Ƭͷ����
		ECRState_Inputing,					//�ȴ��������������ѡ����������
		ECRState_WaitRepose,				//�ȴ���������Ӧ
		ECRState_PlayAction,				//����������ݺ󲥷�һ�ζ���
		ECRState_Close,						//�ر�
		ECRState_End,						
	};
public:
	CreateRoleFrame(void);
	~CreateRoleFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual VOID Render3D();

	EPreChapStatus GetChapterStatus() { return m_eStatus; }

	//��ʼ������ɫ
	void BeginCreateRole();

	//����������ɫ
	void EndCreateRole();

	//��ý�ɫ���������
	ECRState GetCurState()const { return m_curState; }

	//���շ�������Ϣ(��������)
	DWORD NetRecvCreateRole(tagNS_CreateRole* pMsg, DWORD pPrama);

private:

	//״̬ת������
	void SwitchToState(ECRState nextState);

	DWORD EventHandler(tagGUIEvent* pEvent);

	//�������������������û�з���������Ϣ��ȷ����ť�ſ���
	BOOL CheckInputedName(); 

	//������ý�ɫ����
	void RandAvatarAtt(tagAvatarAtt& att, bool bRandAll=false);

	//������������
	void UpdateAttUI(bool bSave);

	void Clear();

	//������������ת��ť
	void CheckMouseLeftDown(float deltaTime);

	//��ɫ��ת
	void RoleRotate(float deltaTime);

	//�Ƿ�ʰȡ�˽�ɫ
	bool IsPickRole(int mouseX, int mouseY);

	void CanRolerAnimation(float time);

	void RefreshCamera(bool bFar);

	VOID FilterString(tstring& str);

private:

	TSFPTrunk<CreateRoleFrame>		m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<Util>					m_pUtil;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetSession>				m_pSession;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<VarContainer>			m_pVarContainer;
	TObjRef<InputDX8>				m_pInput;

	GUIWnd*							m_pWnd;
	GUIWnd*							m_pWndCaption;
	GUIEditBox*						m_pName;			//��������ؼ�
	GUIStatic*						m_pStcErrHint;		//������ʾstatic
	GUIStatic*						m_pStcSex;			//
	GUIFrameMutex*					m_pSexMutex;		//�Ա𻥳�ؼ�
	GUIPushButton*					m_pMalePBtn;		//����pushbutton
	GUIPushButton*					m_pFmalePBtn;		//Ů��pushbutton
	GUIPushButton*					m_pStatusPBtn[EPCS_End];	//�������pushbutton

	GUIButton*						m_pLBtnHType;		//������ť
	GUIButton*						m_pRBtnHType;		//�����Ұ�ť
	GUIStatic*						m_pStcHType;		//���ͺ���ʾstatic

	GUIButton*						m_pLBtnHColor;		//��ɫ��ť
	GUIButton*						m_pRBtnHColor;		//��ɫ�Ұ�ť
	GUIStatic*						m_pStcHColor;		//��ɫ����ʾstatic

	GUIButton*						m_pLBtnFace;		//�沿��ť
	GUIButton*						m_pRBtnFace;		//�沿�Ұ�ť
	GUIStatic*						m_pStcFace;			//�沿����ʾstatic

	GUIButton*						m_pLBtnFDetail;		//�沿ϸ����ť
	GUIButton*						m_pRBtnFDetail;		//�沿ϸ���Ұ�ť
	GUIStatic*						m_pStcFDetail;		//�沿ϸ�ں���ʾstatic

	GUIButton*						m_pLBtnDress;		//��ʽ��ť
	GUIButton*						m_pRBtnDress;		//��ʽ�Ұ�ť
	GUIStatic*						m_pStcFDress;		//��ʽ����ʾstatic

	GUIButton*						m_pBtnOK;			//ȷ��
	GUIButton*						m_pBtnRandom;		//���
	GUIButton*						m_pBtnCancel;		//����

	GUIWnd*							m_pWnd_Control;		//
	GUIButton*						m_pLBtnRotate;		//����ת��ť
	GUIButton*						m_pRBtnRotate;		//����ת��ť
	GUIPushButton*					m_pPBtn_Push;
	GUIPushButton*					m_pPBtn_Pull;

	ECRState						m_curState;			//��ǰ״̬
	float							m_stateTime;		//��ǰ״̬������ʱ��
	bool							m_bRotate;			//�����Ƿ���ת
	bool							m_bLRotate;			//��������ת
	bool							m_bRRotate;			//��������ת
	EPreChapStatus					m_eStatus;			//�������
	bool							m_bSentCreate;		//�Ƿ��Ѿ����͹�������ɫ��Ϣ,��ֹ�ظ�����
	tagAvatarAtt					m_avatarAtt;		//��ɫ����
	tagAvatarEquip					m_avatarEquip;      //��ɫװ��������ʱװ��
	float							m_fRoleYaw;			//��ɫ��ת�ĺ�ƫֵ(��Y��)
	Player*							m_pPlayer;			//��ɫ	
	INT								m_nRoleNameLength;	//������������󳤶�
	bool							m_bPickRole;		//�Ƿ�ʰȡ�˽�ɫ
	BOOL							m_bAnimation;
	float							m_fRolerTimer;
	float							m_fAnimDelay;
};
