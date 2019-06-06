#pragma once

#include "..\WorldDefine\guild_define.h"

struct tagNS_GetGuildUpgradeInfo;
struct tagNS_UpdateFacilitiesInfo;
struct tagNS_GuildProcFailed;
struct tagUpdateLPItemEvent;
struct tagItemPosChangeExEvent;
class ActiveStatic;

/** \class GuildUpgradeFrame
	\brief ������ʩ��������
*/
class GuildUpgradeFrame : public GameFrame
{
public:
	GuildUpgradeFrame();
	~GuildUpgradeFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

private:
	VOID SafeDestroyUI();
	BOOL GUIEventHandler(tagGUIEvent *pEvent);
	VOID UpdataFacilitiesInfoUI( const EFacilitiesType eType );
	VOID SetItemNum( const EFacilitiesType eType, const int nItemIndex, const int nItemNum );
	VOID HandInItems();
	VOID CheckLevelFull( const EFacilitiesType eType );
	/** \������Ϣ��Ӧ
	*/
	DWORD OnNS_GetGuildUpgradeInfo( tagNS_GetGuildUpgradeInfo* pCmd, DWORD );
	DWORD OnNS_UpdateFacilitiesInfo( tagNS_UpdateFacilitiesInfo* pCmd, DWORD );
	DWORD OnNS_GuildProcFailed( tagNS_GuildProcFailed* pCmd, DWORD );
	/** \��Ϸ�¼�����
	*/
	DWORD OnUpdateItemEvent( tagUpdateLPItemEvent* pEvent );
	DWORD OnClose_GuildUpgrade( tagGameEvent* pEvent );
	DWORD OnItemEvent_Move2GuildUpgrade( tagItemPosChangeExEvent* pEvent );
private:
	TSFPTrunk<GuildUpgradeFrame>	m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<NetSession>				m_pSession;

	tagGuildFacilitiesInfo	        m_sFacilitiesInfo[MAX_GUILD_FACILITIES_TYPE];  // ��ʩ������Ϣ
	GUIWnd*							m_pWnd;
	GUIButton*						m_pBn_Close;
	GUIPushButton*                  m_pBn_Tab[MAX_GUILD_FACILITIES_TYPE];          // ��ʩ��ǩ
	GUIStatic*						m_pSt_LevelCaption;							   // �ȼ�
	GUIStatic*						m_pSt_Level;
	GUIProgress*					m_pPro_Progress;							   // ����
	ActiveStatic*                   m_pSt_Item_Icon[MAX_UPGRADE_NEED_ITEM_TYPE];   // ��Ʒͼ��
	GUIStatic*						m_pSt_Item_Name[MAX_UPGRADE_NEED_ITEM_TYPE];
	GUIStatic*						m_pSt_Item_Num[MAX_UPGRADE_NEED_ITEM_TYPE];
	GUIStatic*						m_pSt_Fund;                                    // �����Ǯ
	GUIStatic*						m_pSt_Material;                                // �����ʲ�
	GUIStatic*						m_pSt_Tips;									   // ˵��
	GUIButton*                      m_pBn_HandInItems;                             // �Ͻ���Ʒ
	EFacilitiesType					m_eCurSelType;								   // ��ǰѡ������ʩ����

	// �Ž��������Ʒ����
	int                             m_nPutInedItemNum[MAX_GUILD_FACILITIES_TYPE][MAX_UPGRADE_NEED_ITEM_TYPE];
	bool                            m_bWaitingForHandInRet;                        // �Ƿ����ڵȴ��Ͻɷ��ؽ��
	int                             m_bLevelFull[MAX_GUILD_FACILITIES_TYPE];	   // �Ƿ��Ѿ��ﵽ�ȼ�����
};



