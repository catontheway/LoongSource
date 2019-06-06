#pragma once

class GuildMembers;
struct tagGuildBase;
struct tagGuildMemberClient;
struct tagGuildMember;
struct tagGuildPower;
struct tagGuildKick;
struct tagGuildAppoint;
struct tagRoleGetNameEvent;
struct tagInviteJoinGuildEvent;
struct tagNS_GuildCreateBroad;
struct tagNS_GuildDismissBroad;
struct tagNS_GuildJoinReq;
struct tagNS_GuildJoinReqRes;
struct tagNS_GuildJoinBroad;
struct tagNS_GuildProcFailed;
struct tagNS_GuildLeaveBroad;
struct tagNS_GuildKickBroad;
struct tagNS_GuildTurnoverBroad;
struct tagNS_GuildDemissBroad;
struct tagNS_GuildAppointBroad;
struct tagNS_GuildTenetChangeBroad;
struct tagNS_GetRoleInitState_Guild;
struct tagNS_GetGuildName;
struct tagNS_GetGuildTenet;
struct tagNS_GuildGetAllMembers;
struct tagNS_GuildGetMemberEx;
struct tagNS_GuildRefMember;
struct tagNS_SpreadGuildAffair;
struct tagNS_SyncGuildInfo;
struct tagNS_GetGuildSkillInfo;
struct tagGuildSkillInfo;
struct tagNS_SetResearchSkill;
struct tagNS_LearnGuildSkill;
struct tagGuildAffairInfo;
struct tagNS_GuildContribution;
struct tagNS_GuildStateSet;
struct tagNS_GuildStateUnset;
struct tagNS_GuildSkillUpgrade;
struct tagNS_GuildSkillLevelUp;
struct tagNS_AcceptCommerce;
struct tagNS_GetTaelInfo;
struct tagNS_CompleteCommerce;
struct tagNS_AbandonCommerce;
struct tagNS_GuildUpgrade;
struct tagNS_GetCofCInfo;
struct tagNS_GetCommodityInfo;
struct tagCommerceInfo;
struct tagGoodInfo;
struct tagCOCInfo;
struct tagNS_GetCommerceRank;
struct tagNS_SwitchCommendation;
struct tagNS_GetGuildWare;
struct tagNS_GuildWarePri;
struct tagNS_GuildExploit;
struct tagNS_GetSomeGuildName;

#include "..\WorldDefine\guild_define.h"
/** \class	GuildMgr
	\brief	���ɹ�����
*/
class GuildMgr
{
public:
	GuildMgr(void);
	~GuildMgr(void);

	enum EReqReason
	{
		ERQ_NoSelfRight,			// �Լ�ûȨ������
		ERQ_AlreadyInSelfGuild,		// �Ѿ��ڱ�����
		ERQ_AlreadyInOtherGuild,	// �Ѿ�����������
		ERQ_OtherReason				// ����������Ϣ
	};

	static GuildMgr* Inst();
	
	VOID Init();
	VOID Destroy();
	
	void Update();
	//
	VOID SortMembers(INT nCol);

	const tagGuildMemberClient* GetMember(INT nIndex) const;
	const tagGuildMemberClient* GetMemberByID(DWORD dwRoleID) const;
	INT GetMemberIndex(DWORD dwRoleID) const;
	
	
	void SendGetSomeGuildName(int nFlag, const std::vector<DWORD> &vecIDList);
	tstring GetGuildName(DWORD dwGuildID);

	// ������Ϣ��������񷢳������
	bool IsGuildNameExist(DWORD dwGuildID);

	tstring GetGuildTenet() const {return m_strTenet;}
	tstring GetMemberSex(BYTE bySex) const;
	tstring GetMemberPos(INT nPos) const;
	INT32   GetGuildPurchase()const { return m_pGuild->nGroupPurchase; }

	const GuildMembers* GetMembers() const;
	const tagGuildBase*	GetGuildInfo() const;
	const tagGuildMember* GetLpInfo() const;

	bool IsCanJionReq(DWORD dwRoleID, DWORD *pReaseon = NULL) const;

	const tagGuildPower* GetPowerList(INT nPos) const;
	const tagGuildKick*	GetKickList(INT nPos) const;
	const tagGuildAppoint* GetAppointList(INT nPos) const;

	const std::vector<tagGuildAffairInfo*>& GetAffairInfo() const { return m_vecAffairInfo; }
	const std::vector<DWORD>& GetSkill() const { return m_vecSkill; }
	DWORD GetCurrentResearchSkillID() const { return m_dwCurResearchSkillID; }
	INT32 GetCurrentResearchSkillProgress() const { return m_nCurResearchProgress; }
	INT32 GetCurrentOwnCommerceSilver() const { return m_nOwnCommerceSilver; }
	INT GetRunForBusinessLevel() const { return m_nRunForBusinessLevel; }
	DWORD GetCurrentTalkCOCNPCID() const { return m_dwCurTalkCOCNPCID; }
	DWORD GetCurrentCOCID() const { return m_dwCurCOCID; }
	
	const std::map<DWORD, tagCommerceGoodInfo>& GetCurrentCOCGoods() const { return m_mapCOCGoods; }
	const std::map<DWORD, tagCommerceGoodInfo>& GetCurrentPlayerGoods() const { return m_mapPlayerGoods; }

	VOID SendGuildCreate(LPCTSTR szGuildName, DWORD dwNPCID);
	VOID SendGuildDismiss(DWORD dwNPCID);
	VOID SendGuildJoinReq(DWORD dwRoleID);
	VOID SendGuildAllMembersGet();
	VOID SendSpreadGuildAffair(DWORD dwID);
	VOID SendGetGuildSkillInfo();
	VOID SendResearchSkill(DWORD dwID);
	VOID SendLearnGuildSkill(DWORD dwID);
	// ���������Ϣͬ��
	VOID SendSyncGuildInfo(const tstring &strFrameName);
	VOID SendGuildSkillUpgrade(INT64 n64Serial);
	// �����ȡ����
	VOID SendAcceptCommerce();
	// ��ȡ���̳�ʼ��Ϣ
	VOID SendGetTaelInfo();
	// �Ͻ�����
	VOID SendCompleteCommerce();
	// ��������
	VOID SendQuitCommerce();
	// ��ȡ�̻��̻���Ϣ������̻���Ϣ
	VOID SendGetCofCInfo(DWORD dwNPCID);
	// �ر��̻�
	VOID SendCloseCOC(DWORD dwNPCID);
	// ����Ʒ
	VOID SendBuyCOCGoods(DWORD dwNPCID, DWORD dwGoodID, BYTE byBuyNum);
	// ����Ʒ
	VOID SendSellCOCGoods(DWORD dwNPCID, DWORD dwGoodID, BYTE bySellNum);
	// ��������������Ϣ
	VOID SendGetCommerceRank();
	// �������̼ν�״̬
	VOID SendSwitchCommendation(BOOL b);
	

	// ��ʾ������ش�����
	VOID ShowGuildErrorMsg(DWORD dwErrorCode);
	// ����Ƶ����ʾ������Ϣ
	void ShowGuildMsg( const TCHAR* szFormat,... );

	// ���Ұ��ɼ�����Ϣ;
	const tagGuildSkillInfo* FindSkillInfo(DWORD dwSkillID);
	// ������ҵȼ�����������Ϣ
	const tagCommerceInfo *FindCommerceInfo(INT nPlayerLevel);
	// ����ID������Ʒ��Ϣ
	const tagGoodInfo* FindGoodInfo(DWORD dwGoodID);
	// ����ID�����̻���Ϣ
	const tagCOCInfo* FindCOCInfo(DWORD dwCOCID);
	
	//�жϰ����Ƿ���ĳ��״̬
	bool IsInSpecState(EGuildSpecState eState) const;
	// �жϸó����Ƿ񱻰����ӵ��
	bool IsBeOwned(BYTE byCity);
	// ȡ�õ�ǰ�򿪵��̻�ռ�����ID
	BYTE GetCurrentCOCHoldCity() const { return m_byCurHoldCity; }

    // �ֿ����

    //! ��ȡ��ǰ�ֿ�
    void BeginNC_GetGuildWare(DWORD dwNPCID);
    DWORD OnNS_GetGuildWare(tagNS_GetGuildWare* pMsg, DWORD);
    //! ���òֿ�Ȩ��
    void BeginNC_GuildWarePri(DWORD npcID, DWORD roleID, BOOL canUse);
    //! �ֿ����ñ��
    DWORD OnNS_GuildWarePri(tagNS_GuildWarePri* pMsg, DWORD);

    //! �����Ƿ���Ҫ��ʼ���ֿ�
    void SetifNeedInitWarehouse(bool bVal)
    {
        m_bifNeedInitWarehouse  =   bVal;
        m_dwWarehouseLastUpTime =   0;
    }

    // ��������Ƿ��в����ֿ�Ȩ��
    bool isWareOpretable();

private:
	VOID InitPosPower();
	
	// ��ʼ����������
	void InitGuildAffairs();
	// ��ʼ�����ɼ��ܱ�
	void InitGuildSkills();
	// ��ʼ�����̱�
	void InitGuildCommerce();
	// ��ʼ����Ʒ��Ϣ��
	void InitGoodsInfo();
	// ��ʼ���̻���Ϣ��
	void InitCOCInfo();

	VOID AddLocalPlayer();
	VOID ShowJoinReqMsgBox();

	DWORD OnEventGetRoleName(tagRoleGetNameEvent* pGameEvent);
	DWORD OnEventGuildCreate(tagGameEvent* pGameEvent);
	DWORD OnEventGuildDismiss(tagGameEvent* pGameEvent);
	DWORD OnEventGuildOpen(tagGameEvent* pGameEvent);
	DWORD OnEventGuildClose(tagGameEvent* pGameEvent);
	DWORD OnEventGuildAffairOpen(tagGameEvent *pEvent);
	DWORD OnEventGuildAffairClose(tagGameEvent *pEvent);
	DWORD OnEventPickUpSilverOpen(tagGameEvent *pEvent);
	DWORD OnEventPickUpSilverClose(tagGameEvent *pEvent);
	DWORD OnEventContributeSilverOpen(tagGameEvent *pEvent);
	DWORD OnEventContributeSilverClose(tagGameEvent *pEvent);
	DWORD OnEventCOCOpen(tagGameEvent *pEvent);
	DWORD OnEventCOCClose(tagGameEvent *pEvent);
	DWORD OnEventGuildInvite(tagInviteJoinGuildEvent* pGameEvent);
	DWORD OnEventMsgBoxJoinReq(tagMsgBoxEvent* pGameEvent);
	DWORD OnEventMsgBoxInputName(tagMsgInputBoxEvent* pGameEvent);
	DWORD OnEventMsgBoxDismissCheck(tagMsgBoxEvent* pGameEvent);
	DWORD OnEventMsgBoxQuitCommerceWarning(tagMsgBoxEvent* pGameEvent);
	DWORD OnEventGuildUpgrade(tagGameEvent *pEvent);
	DWORD OnEventQuitCommerce(tagGameEvent *pEvent);
    DWORD OnEventOpenGuildStorage(tagGameEvent *pEvent);
    DWORD OnEventOpenGuildStoragePermission(tagGameEvent *pEvent);
	DWORD OnEventGuildCommerceRankOpen(tagGameEvent *pEvent);
	DWORD OnEventGuildCommerceRankClose(tagGameEvent *pEvent);
	DWORD OnEventGuildGetTenet(tagGameEvent *pEvent);
	

	DWORD OnNetGuildAttInit(tagNS_GetRoleInitState_Guild* pNetCmd, DWORD);
	DWORD OnNetGuildNameGet(tagNS_GetGuildName* pNetCmd, DWORD);
	DWORD OnNetGuildTenetGet(tagNS_GetGuildTenet* pNetCmd, DWORD);
	DWORD OnNetGuildGetAllMembers(tagNS_GuildGetAllMembers* pNetCmd, DWORD);
	DWORD OnNetGuildGetMemberEx(tagNS_GuildGetMemberEx* pNetCmd, DWORD);
	DWORD OnNetGuildRefMember(tagNS_GuildRefMember* pNetCmd, DWORD);
	DWORD OnNetGuildCreateBroad(tagNS_GuildCreateBroad* pNetCmd, DWORD);
	DWORD OnNetGuildDismissBroad(tagNS_GuildDismissBroad* pNetCmd, DWORD);
	DWORD OnNetGuildJoinReq(tagNS_GuildJoinReq* pNetCmd, DWORD);
	DWORD OnNetGuildJoinReqRes(tagNS_GuildJoinReqRes* pNetCmd, DWORD);
	DWORD OnNetGuildJoinBroad(tagNS_GuildJoinBroad* pNetCmd, DWORD);
	DWORD OnNetGuildProcFailed(tagNS_GuildProcFailed* pNetCmd, DWORD);
	DWORD OnNetGuildLeaveBroad(tagNS_GuildLeaveBroad* pNetCmd, DWORD);
	DWORD OnNetGuildKickBroad(tagNS_GuildKickBroad* pNetCmd, DWORD);
	DWORD OnNetGuildTurnoverBroad(tagNS_GuildTurnoverBroad* pNetCmd, DWORD);
	DWORD OnNetGuildDemissBroad(tagNS_GuildDemissBroad* pNetCmd, DWORD);
	DWORD OnNetGuildAppointBroad(tagNS_GuildAppointBroad* pNetCmd, DWORD);
	DWORD OnNetGuildTenetChangeBroad(tagNS_GuildTenetChangeBroad* pNetCmd, DWORD);
	DWORD OnNetGuildSpreadAffair(tagNS_SpreadGuildAffair *pNetCmd, DWORD);
	DWORD OnNetGuildSyncInfo(tagNS_SyncGuildInfo *pNetCmd, DWORD);
	DWORD OnNetGetGuildSkillInfo(tagNS_GetGuildSkillInfo *pNetCmd, DWORD);
	DWORD OnNetSetResearchSkill(tagNS_SetResearchSkill *pNetCmd, DWORD);
	DWORD OnNetLearnGuildSkill(tagNS_LearnGuildSkill *pNetCmd, DWORD);
	DWORD OnNetGuildContribution(tagNS_GuildContribution *pNetCmd, DWORD);
	DWORD OnNetGuildStateSet(tagNS_GuildStateSet *pNetCmd, DWORD);
	DWORD OnNetGuildStateUnSet(tagNS_GuildStateUnset *pNetCmd, DWORD);
	DWORD OnNetGuildSkillUpgrade(tagNS_GuildSkillUpgrade *pNetCmd, DWORD);
	DWORD OnNetGuildSkillLevelUp(tagNS_GuildSkillLevelUp *pNetCmd, DWORD);
	DWORD OnNetAcceptCommerce(tagNS_AcceptCommerce *pNetCmd, DWORD);
	DWORD OnNetGetTaelInfo(tagNS_GetTaelInfo *pNetCmd, DWORD);
	DWORD OnNetCompleteCommerce(tagNS_CompleteCommerce *pNetCmd, DWORD);
	DWORD OnNetAbandonCommerce(tagNS_AbandonCommerce *pNetCmd, DWORD);
	DWORD OnNetGuildUpgrade(tagNS_GuildUpgrade *pNetCmd, DWORD);
	DWORD OnNetGetCommodityInfo(tagNS_GetCommodityInfo *pNetCmd, DWORD);
	DWORD OnNetGetCofCInfo(tagNS_GetCofCInfo *pNetCmd, DWORD);
	DWORD OnNetGetCommerceRank(tagNS_GetCommerceRank *pNetCmd, DWORD);
	DWORD OnNetSwitchCommendation(tagNS_SwitchCommendation *pNetCmd, DWORD);
	DWORD OnNetGuildExploit(tagNS_GuildExploit *pNetCmd, DWORD);
	DWORD OnNetGetSomeGuildName(tagNS_GetSomeGuildName *pNetCmd, DWORD);

	// ��հ�����Ϣ
	VOID ClearGuildInfo();
	// ���ñ������ְλ��Ϣ
	VOID SetLocalPlayerGuildPos(INT8 n8Pos);
	// ��հ��������
	VOID ClearAffairs();
	// ��հ��ɼ��ܱ�
	VOID ClearSkills();
	// ��հ������̱�
	VOID ClearCommerces();
	// �����Ʒ��Ϣ��
	VOID ClearGoodInfo();
	// ����̻���Ϣ��
	VOID ClearCOCInfo();
	

private:
	TSFPTrunk<GuildMgr>		m_Trunk;
	TObjRef<NetSession>		m_pSession;
	TObjRef<NetCmdMgr>		m_pCmdMgr;
	TObjRef<GameFrameMgr>	m_pFrameMgr;

	tagGuildBase*					m_pGuild;		//���ڰ�����Ϣ
	std::map<DWORD, tstring>		m_mapGuildName;	//���������б�
	std::set<DWORD>					m_setNameReq;	//�����������󻺴��
	GuildMembers*					m_pMembers;		//���ɳ�Ա�б�
	tagGuildMember*					m_pLpInfo;		//������ҳ�Ա��Ϣ
	tstring							m_strTenet;		//������ּ
	std::vector<DWORD>				m_vecSkill;		//��ǰ���а��ɼ���
	DWORD							m_dwCurResearchSkillID;	// ��ǰ�о�����
	INT32 							m_nCurResearchProgress; // ��ǰ�о����ܽ���
	INT32							m_nOwnCommerceSilver;   // ��ɫӵ������
	DWORD							m_dwTempCompleteCommercePlayerID;	// ��ʱ����������̽�ɫID
	INT32							m_nTempFund;			// ��ʱ�����������ʱ�������ӵ��ʽ�
	
	DWORD							m_dwCurCOCID;			// ��ǰ�򿪵��̻�ID
	DWORD							m_dwCurTalkCOCNPCID;	// ��ǰ�Ի����̻�NPCID
	
	BYTE							m_byCurHoldCity;		// �ز��̻�
	INT								m_nRunForBusinessLevel; // ��ȡ����ʱ��ҵȼ�
	std::map<DWORD, tagCommerceGoodInfo> m_mapCOCGoods;			// �ݴ浱ǰ�򿪵��̻���ӵ����Ʒ���������ͬ��
	std::map<DWORD, tagCommerceGoodInfo> m_mapPlayerGoods;		// ��ɫӵ����Ʒ���������ͬ��




	std::vector<tagGuildPower>		m_vecPower;		//����ְλְ�ܱ�
	std::vector<tagGuildKick>		m_vecKick;		//����ְλ�����
	std::vector<tagGuildAppoint>	m_vecAppoint;	//����ְλ������
	
	std::vector<tagGuildAffairInfo*> m_vecAffairInfo;	// ���������
	std::map<DWORD, tagGuildSkillInfo*>	 m_mapSkill;	// ���ɼ��ܱ�
	std::vector<tagCommerceInfo*> m_vecCommerceInfo;	// �������̱�
	std::map<DWORD, tagGoodInfo*> m_mapGoodInfo;		// ������Ʒ��
	std::map<DWORD, tagCOCInfo*> m_mapCOCInfo;			// �̻���Ϣ��
	std::deque<tstring>				 m_syncFrameNames;	// ��¼����ͬ����frame������
	

	DWORD							m_dwInviterID;		//������ID
	DWORD							m_dwInviterGuild;	//���������ڰ���
	BOOL							m_bIsInitFinish;	//�Ƿ��ʼ�����
	
	DWORD							m_dwLastSyncTime;	// �ϴη���ͬ��������Ϣ��Ϣ��ʱ��
	DWORD							m_dwLastUpdateTime;	// �ϴ�update��ʱ��
	
	static const DWORD				SYNC_TIME_LIMIT = 0 * 1000;  
	static const DWORD				UPDATE_TIME_LIMIT = 5 * 1000;

    bool                            m_bifNeedInitWarehouse; // �Ƿ���Ҫ��ʼ���ֿ�����
    DWORD                           m_dwWarehouseLastUpTime;// �ֿ��ϴ�ʱ���

	bool m_bSendGetAllMember;
	bool m_bGetTenet;

};
