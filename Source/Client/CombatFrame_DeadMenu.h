#pragma once
#include "gameframe.h"
#include "..\WorldDefine\msg_combat.h"

/** \class CombatFrame_DeadMenu
	\brief �����˵�
*/
class CombatFrame_DeadMenu :
	public GameFrame
{
public:
	CombatFrame_DeadMenu(void);
	virtual ~CombatFrame_DeadMenu(void);

	//--GameFrame
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual VOID Update();
	virtual BOOL ReloadUI();

	DWORD NetRecvNS_RoleRevive(tagNS_RoleRevive* pMsg, DWORD dwParam);
	DWORD NetRecvNS_RoleReviveNotify(tagNS_RoleReviveNotify* pMsg, DWORD dwParam);

private:
	BOOL EventHandler(tagGUIEvent* pEvent);

private:
	TSFPTrunk<CombatFrame_DeadMenu>			m_Trunk;
	TObjRef<GUISystem>						m_pGUI;
	TObjRef<GameFrameMgr>					m_pFrameMgr;
	TObjRef<NetSession>					m_pSession;
	TObjRef<NetCmdMgr>						m_pCmdMgr;

	GUIWnd*						m_pWnd;				//������
	GUIButton*					m_pBtnLocalRise;	//ԭ�ظ���
	GUIButton*					m_pBtnAcceptRise;	//���ܸ���
	GUIButton*					m_pBtnBackRise;		//�سǸ���
	GUIStatic*					m_pStcHint;			//��ʾ

};
