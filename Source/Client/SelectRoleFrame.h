//-----------------------------------------------------------------------------
//!\file SelectRoleFrame.h
//!\author Lyp
//!
//!\date 2008-02-17
//! last 2008-04-15
//!
//!\brief select role frame
//!
//!	Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once
#include "..\WorldDefine\selectrole.h"

class NetSession;
struct tagMsgBoxEvent;
//-----------------------------------------------------------------------------
//!\brief select role frame
//!
//!
//-----------------------------------------------------------------------------
class SelectRoleFrame : public GameFrame
{
public:
	enum ESafeCodeState
	{
		ESCS_NULL			= 0,		//	û�а�ȫ��
		ESCS_HasSafeCode	= 1,		//	��ȫ��������
		ESCS_Reseting		= 2,		//	��ȫ����������
		ESCS_Reseted		= 3,		//  ��ȫ������ʱ���ѵ�
	};
public:
	SelectRoleFrame();
	~SelectRoleFrame();

	enum ESRState
	{
		ESRS_Loading,					//������Դ
		ESRS_EnumRole,					//��zone������������ҵĽ�ɫ��Ϣ
		ESRS_Inputing,					//�ȴ��������
		ESRS_Deling,					//ɾ����ɫ��
		ESRS_SentSelectRole,			//����ѡ���ɫ������Ϸ����
		ESRS_RecvSelectRole,			//���ս�ɫ������Ϸ����
		ESRS_SelectedRole,				//ѡ���˽�ɫ
		ESRS_Disconnect,				//��������Ͽ�����
		ESRS_Back,						//���ص���¼
	};
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Render3D();
	virtual VOID Update();
	VOID LoadMap();
	
	VOID SetCurState(ESRState state) { m_eCurState = state; m_dwStateTime = timeGetTime();}
	
	// ���������
	void SetCamera();

private:
	DWORD EventHandler(tagGUIEvent* pEvent);

	BOOL EnterGame(DWORD dwLocalRoleID);
	VOID SendEnterGameMsg();
	BOOL ReturnGame();
	BOOL CreateRole();

	VOID SendDeleRoleMsg();

	BOOL DeleteRole();

	//��������PButton״̬
	void ReSetPButtonState(int nSele);

	//--UI����
	VOID UpdateUI();
	VOID UpdateSafeCodeUI();
	VOID SetDeleteRoleUI(BOOL bVal);

	// ���շ�����ɾ����ɫ��Ϣ
	DWORD NetRecvDeleteRole(tagNS_DeleteRole* pMsg, DWORD pPrama);

	// ������ҵĽ�ɫ��Ϣ
	DWORD NetRecvEnumRole(tagNS_EnumRole* pMsg, DWORD pPrama);

	// ������ҵ�ѡ���ɫ��Ϣ
	DWORD NetRecvSelectRole(tagNS_SelectRole* pMsg, DWORD pPrama);

	//--��ȫ��
	void JudeSafeCodeState();
	VOID SendSafeCodeMsg();
	DWORD NetRecvNS_SafeCode(tagNS_SafeCode* pMsg, DWORD pParam);
	DWORD NetRecvNS_ResetSafeCode(tagNS_ResetSafeCode* pMsg, DWORD pParam);
	DWORD NetRecvNS_CancelSafeCodeReset(tagNS_CancelSafeCodeReset* pMsg, DWORD pParam);
	DWORD OnResetSafeCodeEvent(tagMsgBoxEvent* pEvent);
	DWORD OnCancelSafeCodeEvent(tagMsgBoxEvent* pEvent);
	DWORD OnSafeCodeIntroductionEvent(tagMsgBoxEvent* pEvent);
	DWORD OnAcceptDeleteRole( tagMsgBoxEvent* pEvent );
	void DisplayResetSafeCodeTime();

	//��鰲ȫ���Ƿ���ϱ�׼
	int IsValidSafeCode(const TCHAR* szSafeCode);

	void CreateSafeCodeTime();

	//
	DWORD OnDisconnectServer(tagMsgBoxEvent* pGameEvent);

private:
	TSFPTrunk<SelectRoleFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<InputDX8>			m_pInput;
	
	GUIWnd*						m_pWndSelct;
	GUIWnd*						m_pWndMain;
	GUIWnd*						m_pWndDel;

	GUIStatic*					m_pStc_Sele_Name;
	GUIStatic*					m_pStc_Sele_Info1;
	GUIStatic*					m_pStc_Sele_Info2;
	GUIStatic*					m_pWndHintInfo;
	GUIButton*					m_pButtonCreate;
	GUIButton*					m_pButtonDelete;
	GUIButton*					m_pButtonEenter;
	GUIButton*					m_pButtonReturn;
	GUIButton*					m_pBtnSafeCode;
	GUIButton*					m_pBtnDelOk;
	GUIButton*					m_pBtnDelCancel;
	GUIEditBox*					m_pEditSafeCode;
	GUIStatic*					m_pStcDelHint;

	GUIWnd*						m_pWndSD;
	GUIStatic*					m_pStcSDHint1;
	GUIStatic*					m_pStcSDHint2;
	GUIEditBox*					m_pEdtSafeCode1;
	GUIEditBox*					m_pEdtSafeCode2;
	GUIButton*					m_pBtnSDOK;
	GUIButton*					m_pBtnSDCancel;
	GUIStatic*					m_pStcSDHint;

	int							m_nCurSelRole;
	int							m_nLastSelRole;
	DWORD						m_nSelectRoleID;
	ESRState					m_eCurState;	
	DWORD						m_dwStateTime;
	DWORD						m_dwMouseDown;
	DWORD						m_dwErrorTime;		//������ʾ��ʼ��ʱ��
	ESafeCodeState				m_eSafeCodeState;
};
