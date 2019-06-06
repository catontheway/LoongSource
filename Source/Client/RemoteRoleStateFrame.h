#pragma once
#include "..\WorldDefine\container_define.h"
class PlayerViewWindow;
class ItemButton;
class Equipment;
struct tagNS_GetRemoteRoleEquipInfo;
class RemoteRoleStateFrame : public GameFrame
{
public:
	RemoteRoleStateFrame(void);
	~RemoteRoleStateFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Render3D();

	void SetDisplayRemoteRoleID(DWORD dwRoleID) { m_dwRemoteRoleID = dwRoleID; }
	DWORD GetDisplayRemoteRoleID()const { return m_dwRemoteRoleID; }

	//�������������������װ��ʱ��
	void SetRequireTime(DWORD dwTime) { m_dwTime = dwTime; }
	DWORD GetRequireTime()const { return m_dwTime; }

	//���µ�ǰҪ�鿴�Ľ�ɫavatar
	void UpdateEquipAvatar();

	//���µ�ǰҪ�鿴�Ľ�ɫװ��
	void UpdateAllEquip();

private:
	DWORD EventHandler(tagGUIEvent* pEvent);

	//��Ϸ�¼�
	DWORD OnDeleRemoteRoleEquipInfo(tagGameEvent* pGameEvent);

	//������Ϣ
	DWORD OnNS_GetRemoteRoleEquipInfo(tagNS_GetRemoteRoleEquipInfo* pMsg, DWORD dwParam);

	//��ȡװ����λ
	EEquipPos GetEquipPos(INT nIndex);

	//��ȡװ��
	Equipment* GetCurRemoteRoleEquip( EEquipPos pos=EEP_Null );

	//���µ���װ��
	void UpdateEquipment( EEquipPos ePos );

private:
	TSFPTrunk<RemoteRoleStateFrame>		m_Trunk;
	TObjRef<GUISystem>					m_pGUI;
	TObjRef<Util>						m_pUtil;
	TObjRef<NetCmdMgr>					m_pCmdMgr;
	TObjRef<NetSession>					m_pSession;

	GUIWnd*								m_pWnd_Father;
	GUIWnd*								m_pWnd_State;				
	GUIStatic*							m_pStc_PlayerName;			
	GUIStatic*							m_pStc_PlayerGuild;
	GUIStatic*							m_pStc_PlayerLevel;
	GUIStatic*							m_pStc_PlayerClass;
	GUIFrameMutex*						m_pFm_SuitOpt;
	GUIStatic*							m_pStc_SuitOpt;
	GUIPushButton*						m_pPbn_DisplayEquip;
	GUIPushButton*						m_pPbn_DisplayFashion;
	GUIWnd*								m_pWnd_AttEx;
	GUIWnd*								m_pWnd_AttIn;
	GUIWnd*								m_pWnd_Resist;

	GUIStatic*							m_pStc_EquipBack[EEP_MaxEquip];
	ItemButton*							m_pBtn_Equip[EEP_MaxEquip];
	PlayerViewWindow*					m_pWnd_EquipView;

	BOOL								m_bIsFashionDisplay;
	DWORD								m_dwRemoteRoleID;
	DWORD								m_dwTime;

	map<INT16, Equipment>				m_mapRemoteRoleEquip;
};