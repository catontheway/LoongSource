#pragma once


#include "PersonalityActData.h"

struct tagNS_DuetMotionInvite;
struct tagNS_DuetMotionOnInvite;
struct tagRoleGetNameEvent;
struct tagNS_RoleStyleAction;
struct tagMouseMoveFinishEvent;

/** ���Բ��Ÿ��Զ����¼�
*/
struct tagTryToPlayPersonalityActEvent : public tagGameEvent
{
	const tagPersonAct* pData;
	tagTryToPlayPersonalityActEvent(LPCTSTR szEventName, GameFrame* pSenderFrame,const tagPersonAct* pAct) 
		: tagGameEvent(szEventName,pSenderFrame),pData(pAct){}
};


/** \class StyleActionFrame
	\brief ���Զ���
*/
class StyleActionFrame : public GameFrame
{
public:
	StyleActionFrame();
	~StyleActionFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();
private:
	BOOL EventHandler(tagGUIEvent* pEvent);
	VOID SafeDestroyUI();
	VOID ShowInviteMsgBox( const TCHAR* szInviteName );
	void MoveToTarget(const Vector3& vTargetPos);
	/** \��Ϸ�¼���Ӧ
	*/
	DWORD OnOpenClose_StyleActionEvent(tagGameEvent* );
	DWORD OnTryToPlayPersonalityActEvent(tagTryToPlayPersonalityActEvent* pEvent);
	DWORD OnStyleActionInviteMsgBox( tagMsgBoxEvent* pEvent );
	DWORD OnRoleGetNameEvent(tagRoleGetNameEvent* pEvent);
	DWORD OnMouseMoveFinishEvent( tagMouseMoveFinishEvent* pEvent );
	/** \������Ϣ��Ӧ
	*/
	DWORD OnNS_DuetMotionInvite( tagNS_DuetMotionInvite* pCmd, DWORD );
	DWORD OnNS_DuetMotionOnInvite( tagNS_DuetMotionOnInvite* pCmd, DWORD );
	DWORD OnNS_RoleStyleAction(tagNS_RoleStyleAction* pNetCmd, DWORD);        // ��������ҽ��������Ĳ���
private:
	TSFPTrunk<StyleActionFrame>			m_Trunk;
	TObjRef<GUISystem>					m_pGUI;
	TObjRef<NetCmdMgr>					m_pCmdMgr;
	TObjRef<NetSession>					m_pSession;
	GUIWnd*                             m_pWnd;
	GUIButton*							m_pBn_Close;
	DWORD								m_dwActionTargetID;			// ��Ҫ����Ŀ��ID�Ͷ����ṹ��
	tagPersonAct                        m_sPersonActData;
	bool                                m_bWaitRemotePesponses;		// ���ڵȴ��Է���Ӧ
	bool                                m_bWaitLocalPesponses;		// ���ڵȴ��Լ���Ӧ
	DWORD								m_dwInviteMsgBoxShowTime;
	DWORD								m_dwWaitRemoteResponseTime; // ��ʼ�ȴ���������Ӧ��ʱ���
	bool								m_bWaitForInviteRoleName;
	bool                                m_bMovingTo;                // ��������Ŀ��
	bool                                m_bReadyToStart;			// �Ѿ�����Ŀ��λ��
};