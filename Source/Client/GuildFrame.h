#pragma once
#include "gameframe.h"
#include "GuildMembers.h"

struct tagRoleGetNameEvent;
struct tagGetNameByNameID;
struct tagGuildPosAppointEvent;
struct tagGuildInfoReflashEvent;
struct tagGuildNameGetEvent;
struct tagGuildBase;
struct tagGuildMemberClient;
struct tagUseGuildSkillBookEvent;



class IconStatic;
struct tagGuildSkill;
class Item;

#define GUILD_SKILL_ITEM_NUM_PERPAGE 12

class GuildFrame :
	public GameFrame
{
public:
	typedef vector<GUIButton*> GUIButtonList;
	
	struct tagMemberNameReq
	{
		INT		nIndex;
		DWORD	dwColor;

		tagMemberNameReq(INT nIdx, DWORD dwClr){nIndex = nIdx; dwColor = dwClr;}
	};

	typedef map<DWORD, tagMemberNameReq> NameReqList;  
	
	// ���ɽ���״̬
	enum EGuildFrameState
	{
		EGFS_MemberList, 
		EGFS_GuildInfo,
		EGFS_GuildSkill
	};

public:
	GuildFrame(void);
	~GuildFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();

	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

	EGuildFrameState GetFrameState() const { return m_eGuildFrameState; }
	VOID ShowWnd(bool bShow){ m_bShow = bShow; m_pWnd->SetInvisible(!bShow);}
	bool IsShow() const { return m_bShow; }
private:

	VOID EventHandler(tagGUIEvent* pEvent);
	
	// ˢ�°���
	VOID ReflashGuild();
	// ˢ���б�
	VOID ReflashList();
	// ˢ���б�IMPL
	VOID ReflashListImpl();
	// ˢ�¹��ܰ�ť
	VOID ReflashButton();
	// ˢ�¼���
	void RefreshSkills();
	

	// ��ȡ����ʱ����ʾ
	tstring GetOfflineTime(bool bIsOnline, DWORD dwOfflineTime = GT_INVALID);
	
	// ����������Ϣ
	VOID SendGuildAppoint(DWORD dwDstRoleID, INT nPos);
	VOID SendGuildLeave();
	VOID SendGuildKick(DWORD dwDstRoleID);
	VOID SendGuildTurnover(DWORD dwDstRoleID);
	VOID SendGuildDemiss();
	VOID SendGuildTenetChange(tstring strTenetInfo);
	
	// ������Ϸ�¼�
	DWORD OnEventGetRoleName(tagRoleGetNameEvent* pGameEvent);
	DWORD OnEventGetNameByNameID(tagGetNameByNameID* pGameEvent);
	DWORD OnEventGetGuildName(tagGuildNameGetEvent* pGameEvent);
	DWORD OnEventGetGuildTenet(tagGameEvent* pGameEvent);
	DWORD OnEventGuildPosAppoint(tagGuildPosAppointEvent* pGameEvent);
	DWORD OnEventGuildInfoReflash(tagGuildInfoReflashEvent* pGameEvent);
	DWORD OnEventGuildListReflash(tagGameEvent* pGameEvent);
	DWORD OnEventGuildInfoReq(tagGuildInfoReflashEvent* pGameEvent);
	DWORD OnEventMsgBoxInputTenet(tagMsgInputBoxEvent* pGameEvent);
	DWORD OnEventMsgBoxLeaveCheck(tagMsgBoxEvent* pGameEvent);
	DWORD OnEventMsgBoxTurnoverCheck(tagMsgBoxEvent* pGameEvent);
	DWORD OnEventMsgBoxKickCheck(tagMsgBoxEvent* pGameEvent);
	DWORD OnEventMsgBoxDemissCheck(tagMsgBoxEvent* pGameEvent);
	DWORD OnEventRefreshGuildSkills(tagGameEvent *pEvent);
	DWORD OnEventShowGuildFrame(tagGameEvent *pEvent);
	DWORD OnEventHideGuildFrame(tagGameEvent *pEvent);
	DWORD OnEventRefreshGuild(tagGameEvent *pEvent);
	DWORD OnEventUsGuildSkillBook(tagUseGuildSkillBookEvent *pGameEvent);
	DWORD OnEventRefreshBook(tagGameEvent *pGameEvent);
	DWORD OnEventCommerceWarningMsgBox(tagMsgBoxEvent *pEvent);
	// ����ť�����¼�
	VOID OnClickedLookupInfo();
	VOID OnClickedLeaveGuild();
	VOID OnClickedTurnoverLeader();
	VOID OnClickedAppointMember();
	VOID OnClickedModifyTenet();
	VOID OnClickedKickMember();
	VOID OnClickedDemissPostion();

	VOID DestroyMsgBox();
	void SetSkillDefaultUI();
	void OnSkillButtonClick(int nButton);		// ѡ��ĳ������button
	void OnClickPreOrNextButton(bool bOrder);	// trueΪ���ˣ�falseΪǰ��

	// ��ȡ��ǰ�еĳ�Ա
	const tagGuildMemberClient* GetMember(INT nIndex);
	
private:
	TSFPTrunk<GuildFrame>		m_Trunk;
	TObjRef<NetSession>			m_pSession;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<GameFrameMgr>		m_pFrameMgr;

	NameReqList					m_mapTemp;
	vector<INT>					m_vecIndex;			// ��Ա��������
	GuildMembers::EListColType	m_eSortType;		// ����ʽ


	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndMembers;
	GUIButton*					m_pBtnClose;
	GUIListBox*					m_pLstMembers;		// ��Ա�б�
	GUIStatic*					m_pStcName;			// ��������
	GUIStatic*					m_pStcLevel;		// �ȼ�
	IconStatic*					m_pStcSymbol;		// ���ɱ�־
	GUIStatic*					m_pStcFounder;		// ��ʼ��
	GUIStatic*					m_pStcLeader;		// ����
	GUIStatic*					m_pStcMemberNum;	// ��Ա����	
	GUIStatic*					m_pStcReputation;	// ����
	

	GUIStatic*					m_pStcFund;			// �ʽ�
	GUIStatic*					m_pStcMaterial;		// �ʲ�
	GUIStatic*					m_pStcPeace;		// ������
	GUIStatic*					m_pStcDailyCost;	// ÿ��ά����
	GUIEditBoxEx*				m_pEdtTenet;		// ������ּ
	GUIPushButton*				m_pPbnMembers;		// ��Ա�б�
	GUIPushButton*				m_pPbnSkills;		// ���ɼ���
	GUIPushButton*				m_pPbnInfomation;	// ������Ϣ
	
	GUIButtonList				m_pBtnListIndex;	// ��Ա�б��ͷ
	GUIButton*					m_pBtnLookupInfo;	// �鿴��Ϣ
	GUIButton*					m_pBtnLeaveGuild;	// �뿪����
	GUIButton*					m_pBtnTurnLeader;	// �ƽ�����
	GUIButton*					m_pBtnAppointMember;// ����ְλ
	GUIButton*					m_pBtnModifyTenet;	// �޸���ּ
	GUIButton*					m_pBtnKickMember;	// �����Ա
	GUIButton*					m_pBtnDemissPostion;// ��ȥְ��
	GUIPushButton*				m_pPbnOffline;		// ��ʾ����

	GUIPatch*					m_pGuildMemberPatch;

	const tagGuildBase*			m_pGuild;			// ������Ϣ

	DWORD						m_dwDstRoleID;		// ������Ŀ��ID

	GUIWnd *m_pWndGuild;			// ������
	IconStatic *m_pBtnCurSkillIcon;	// ��ǰ�о�����ͼ��
	GUIStatic *m_pStcCurSkillName;	// ��ǰ�о���������
	GUIStatic *m_pStcCurSkillLevel; // ��ǰ�о����ܵȼ�
	GUIStatic *m_pStcFundCost;		// ���İ����ʽ�
	GUIStatic *m_pStcMaterialCost;	// ���İ����ʲ�
	GUIProgress * m_pProResearchProgress; //��ǰ�����о�����

	IconStatic *m_pBtnBookIcon;		// ����伮
	GUIStatic *m_pProgressIncrement; // ��������
	GUIButton *m_pLearnBook;		// ���е伮

	GUIButton *m_pBtnBack;
	GUIButton *m_pBtnNext;

	GUIStatic *m_pStcLearnGoldCost;			// ѧϰ�������
	GUIStatic *m_pStcLearnSilverCost;		// ѧϰ��������
	GUIStatic *m_pStcLearnContributionNeed; // ѧϰ������ɹ���
	GUIStatic *m_pStcLearnGuildFundCost;	// ѧϰ��������ʽ�
	GUIStatic *m_pStcLearnGuildMaterialCost;// ѧϰ��������ʲ�

	GUIButton *m_pBtnLearnSkill;			// ѧϰ����
	GUIButton *m_pBtnCancelResearch;		// ȡ���о�
	GUIButton *m_pBtnStartResearch;			// ��ʼ�о�

	GUIPatch *m_pSkillPatch;
	IconStatic *m_pIstState[5];				// ����״̬
	
	IconStatic *m_pSkillItemBack[GUILD_SKILL_ITEM_NUM_PERPAGE];
	IconStatic *m_pSkillItem[GUILD_SKILL_ITEM_NUM_PERPAGE];
	GUIStatic *m_pSkillName[GUILD_SKILL_ITEM_NUM_PERPAGE];
	GUIStatic *m_pSkillLevel[GUILD_SKILL_ITEM_NUM_PERPAGE];
	int m_nCurPageNum;	// ��ǰ��ҳ��
	DWORD m_dwCurChooseSkill; // ��ǰѡ�а��ɼ���ID;
	std::vector<tagGuildSkill*> m_vecCurPageItem;
	
	EGuildFrameState m_eGuildFrameState;
	bool m_bShow;
	Item *m_pSkillBook;			// ����伮��ָ��

	DWORD m_dwTempRoleID; // ��ʱ�����ɫID
	INT m_nTempPOS;     // ��ʱ����POSID
};
