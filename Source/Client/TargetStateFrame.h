#pragma once

/** \˽����Ϸ�¼�
*/

class BuffUIList;
class RoleHeadPicIcon;
class Role;
class Player;
class NPC;
struct tagRolePickEvent;
struct tagRoleGetNameEvent;
struct tagUpdataAvatarEvent;
struct tagRoleBuffChangeEvent;
struct tagUpdataAvatarDisplaySetEvent;
struct tagApplyJoinTeamEvent;

/** \class TargetStateFrame
    \brief ��ǰĿ�����
*/
class TargetStateFrame : public GameFrame
{
public:
	enum { BUFFUI_NUM = 20 };

	/** \Ŀ��˵�ѡ��
	*/
	enum EMenuItem
	{
		EMI_PrivateChat,       // ˽    ��
		EMI_Friend,            // ��    ��
		EMI_Trade,             // ��    ��
		EMI_Follow,            // ��    ��
		EMI_TeamInvite,        // �������
		EMI_FactionInvite,     // ��������
		EMI_PlayerInfo,        // �����Ϣ
		EMI_AddToBlackList,    // �Ӻ�����
        EMI_PetTrade,          // ���޽���
		EMT_MountInvite,	   // �������
		EMI_TeamApply,         // �������
		EMI_PlayerInfo2,	   // �������
		EMI_Num,
	};

	TargetStateFrame();
	~TargetStateFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual VOID Render3D();
	virtual BOOL EscCancel();
private:
	BOOL GUIEventHandler(tagGUIEvent* pEvent);
	VOID SafeDestroyUI();

	VOID OnOpen( const DWORD dwRoleID );
	VOID OnClose();

	VOID UpdataRoleData( Role* pRole );           // ���½�ɫ������ʾ
	VOID UpdataPlayerData( Player* pPlayer );     // ����Player���е�������ʾ
	VOID UpdataNPCData( NPC* pNpc );              // ����NPC���е�������ʾ
	VOID UpdataMenuItem();
	VOID ForceResizeMenu( GUIListBox* pListBox, const INT nNumRow );
	VOID SetMenuFromStrTable( GUIListBox* pListBox, const TCHAR* szBaseKey, const INT nLbIndex, const INT nStrIndex );
	VOID SetMenuFromStrTable( GUIListBox* pListBox, const TCHAR* szBaseKey, const INT nNumRow, const INT nStartRow, const INT nEndRow, const BOOL bAutoResize );
	VOID RebuildBuffs( Role* pRole );
    VOID SetPlayerClassName(DWORD dwRoleID);
    DWORD OnChangePlayerClass(tagGameEvent* pEvent);
	/** \��Ϸ�¼���Ӧ����
	*/
	DWORD OnRolePickEvent(tagRolePickEvent* pEvent);
	DWORD OnRoleGetNameEvent(tagRoleGetNameEvent* pEvent);
	DWORD OnUpdataAvatarEvent(tagUpdataAvatarEvent* pEvent);
	DWORD OnRoleBuffChangeEvent(tagRoleBuffChangeEvent* pEvent);
	DWORD OnUpdataAvatarDisplaySetEvent(tagUpdataAvatarDisplaySetEvent* pEvent);

private:
	TSFPTrunk<TargetStateFrame>	    m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	
	GUIWnd*                         m_pWnd;                    // ������
	RoleHeadPicIcon*				m_pHead;				   // ͷ��ͼ��
	GUIStatic*                      m_pSt_PVP;				   // PVP״̬��ǩ
	GUIStatic*                      m_pSt_Name;                // ���Ʊ�ǩ
	GUIStatic*                      m_pSt_Level;               // �ȼ���ǩ
	GUIProgress*                    m_pPro_Hp;                 // HP������
	GUIProgress*                    m_pPro_Mp;                 // MP������
	GUIStatic*                      m_pSt_AppendInfo;          // ������Ϣ��ǩ
	GUIPushButton*                  m_pBn_Menu;                // �˵���ť
	
	GUIWnd*							m_pWnd_Menu;               // Ŀ��˵�����
	GUIListBox*                     m_pLb_Menu;                // Ŀ��˵��б��

	BuffUIList*                     m_pBuffUIList;             // BUFF�ؼ��б�
	DWORD                           m_dwCurShowRoleID;         // ��ǰ��ʾ��ɫ��ID
	DWORD                           m_dwSelectEffectID;        // ѡ����ЧID
	INT								m_nCurMenuIndex[EMI_Num];  // ��ǰĿ��˵�ѡ���ӦListBox������
	INT								m_nMenuPanelHeightExt;

	INT								m_nLastLevel;              // ��¼�ϴ���ʾ�����ݣ�����ÿ�ζ�ˢ�½���
	INT								m_nLastMaxHp;
	INT								m_nLastHp;
	INT								m_nLastMaxMp;
	INT								m_nLastMp;
	bool							m_bLastNameGray;
};

