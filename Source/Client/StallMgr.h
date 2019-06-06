#pragma once

class TemporaryContainer;
class Item;
class LocalPlayer;
struct tagTemporaryItem;
struct tagRolePickEvent;
struct tagVIPStall;
struct tagNS_StallStart;
struct tagNS_StallClose;
struct tagNS_StallGetTitle;
struct tagNS_StallBuyRefresh;
struct tagNS_GetAllVIPStallInfo;
struct tagNS_UpdateVIPStallInfo;
struct tagNS_StallSetFinish;
struct tagRoleGetNameEvent;

struct tagStallAppearEvent : public tagGameEvent
{
	DWORD	dwRoleID;
	Vector3 vPos;
	FLOAT	fYaw;
	BYTE    byStallLevel;

	tagStallAppearEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName, pSenderFrame){}
};

struct tagStallUpdateItemEvent : public tagGameEvent
{
	INT16	n16Pos;

	tagStallUpdateItemEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName, pSenderFrame){}
};


struct tagStallStarEvent : public tagGameEvent
{
	INT nLevel;
	tagStallStarEvent(LPCTSTR szEventName, GameFrame *pSenderFrame) : tagGameEvent(szEventName, pSenderFrame){}

};

struct tagStall
{
	DWORD			dwRoleID;		// ˭��̯λ
	BYTE			byLevel;		// ̯λ�ȼ�
	BYTE			byType;			// ��������
	BYTE			byDummy[2];		
	ObjectSpace		space;			// ̯λ3Dλ��
	EntityNode*		pNode;			// ̯λģ��
	TCHAR			szName[X_SHORT_NAME]; // ̯λ����
    bool            bCustomName;    // �Ƿ��Զ���̯λ����

    tagStall() :
        dwRoleID(GT_INVALID),
        byLevel(0),
        byType(0),
        pNode(0),
        bCustomName(false)
    {
        byDummy[0]  = 0;
        byDummy[1]  = 0;
        szName[0] = 0;
    }
};

struct tagVipStallEvent : public tagGameEvent
{
	DWORD			dwRoleID;		// vip��̯λ
	BYTE			byIndex;		// vip̯λ���

	tagVipStallEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName, pSenderFrame)
	{
		dwRoleID = GT_INVALID;
		byIndex	 = GT_INVALID;
	}
};

class StallMgr
{
public:
	enum EOperateStallType
	{
		EOperateStallType_NULL		= -1,
		EOperateStallType_Normal,				//�����򿪷�ʽ
		EOperateStallType_Vip,					//VIP�򿪷�ʽ
	};

	struct tagStallModelConfig
	{
		int nStallLevel;	// ̯λ�ȼ�
		int nModel;			// ̯λģ��
		int nSignBoard;		// ����ģ��
	};

	typedef std::map<int, tagStallModelConfig> StallModelConfigMap;

public:
	StallMgr(void);
	~StallMgr(void);

	VOID Init();
	VOID Destroy();

	tagStall* PickStall(const Ray& rayMouse);

	BOOL AddItem(Item* pItem, INT16 &nIndex, INT64 n64Price = 0);
	BOOL DelItem(INT16 nIndex, BOOL bUnlock = TRUE);
	tagTemporaryItem* GetItem(INT16 nIndex);

	tstring GetStallStars(INT64 nTotalTax);
	
	VOID ShowStallErrorMsg(DWORD dwErrorCode);

	BOOL IsStallState();

	BOOL IsOutValidDist();
	
	// �Ƿ���԰�̯
	BOOL CanStall(BYTE byLevel);

	// ����/��ȡ̯λ����
	VOID SetStallName(DWORD dwRoleID, LPCTSTR szName, bool bCustom);
	LPCTSTR GetStallName(DWORD dwRoleID);

    // ������NameTable��ɫ�����¼����ش���
    DWORD OnGetPlayerName(tagRoleGetNameEvent* pGameEvent);

	// ����/��ȡ������ҹ���־λ
	VOID SetStallAdFlag(BOOL bFlag){m_bSetAdFlag = bFlag;}
	BOOL GetStallAdFlag(){return m_bSetAdFlag;}

	// ��ȡ̯λ�б�
	TMap<DWORD,tagStall*>& GetStallMap() { return m_mapStall; }

	//���ô�̯λ��ʽ
	void SetOperateStallType(EOperateStallType eType=EOperateStallType_Normal){ m_eOperateStallType = eType; }
	EOperateStallType GetOperateStallType() { return m_eOperateStallType; }

	//--���vip̯λ״̬
	BOOL IsCanCheckVipStall(BYTE byIndex);
	BOOL IsCanRentVipStall(BYTE byIndex);

	//����Լ���û��vip̯λ
	BOOL IsSelfVipStall();

	//ˢ��VIP̯����ʾ
	void RefreshVipStallState(DWORD dwRoleID);

	// ��ȡvip̯λ��Ϣ
	map<BYTE, tagVIPStall>& GetVipStallInfo() { return m_mapVipStall; }
	tagVIPStall* FinVipStallInfo(BYTE byKey);

	static StallMgr* Inst();

	// ����̯λ�ȼ�ȡ��̯λģ��·��
	tstring GetStallModelPath(int nStallLevel);

	// ����̯λ�ȼ�ȡ������·��
	tstring GetStallSignBoardPath(int nStallLevel);

private:

	DWORD OnEventStallAppear(tagStallAppearEvent* pGameEvent);
	DWORD OnEventStallDisappear(tagStallAppearEvent* pGameEvent);
	DWORD OnEventPickStall(tagRolePickEvent*	pGameEvent);
	DWORD OnEventOpenStall(tagGameEvent* pGameEvent);
	DWORD OnEventCloseStall(tagGameEvent* pGameEvent);
	DWORD OnEventCloseVend(tagGameEvent* pGameEvent);
	DWORD OnEventOpenVipStall(tagGameEvent* pGameEvent);
	DWORD OnCheckVipStallEvent(tagVipStallEvent* pGameEvent);

	DWORD OnNetStallStart(tagNS_StallStart* pNetCmd, DWORD);
	DWORD OnNetStallClose(tagNS_StallClose* pNetCmd, DWORD);
	DWORD OnNetStallGetTitle(tagNS_StallGetTitle* pNetCmd, DWORD);
	DWORD OnNetStallBuyRefresh(tagNS_StallBuyRefresh* pNetCmd, DWORD);
	DWORD OnNetGetAllVIPStallInfo(tagNS_GetAllVIPStallInfo* pNetCmd, DWORD);
	DWORD OnNetUpdateVIPStallInfo(tagNS_UpdateVIPStallInfo* pNetCmd, DWORD);
	DWORD OnNetStallSetFinish( tagNS_StallSetFinish* pNetCmd, DWORD );

	BOOL LoadStallModelConfig(tstring strConfigPath);


	VOID SendToGetStallTitle(DWORD dwRoleID);

	// ��ʼ��̯λ����
	VOID InitStallContainer();	
	// �洢����������������
	VOID RecordStallList();	

	VOID OpenStallShelf();
	VOID CloseStallShelf();
	VOID OpenStallVend();
	VOID CloseStallVend();

	BOOL ShowStall(tagStall* pStall);
	VOID HideStall(tagStall* pStall);

	BOOL IsAABBCollideStall(const AABBox &box);

	BOOL IsAABBCollideGround(const AABBox &box, Vector3 &wpt);
	
	// ����̯λ�ȼ���ȡһ��̯λ��Ӧ��AABBox
	VOID GetStallAABBox(AABBox& out, Vector3& wpt, BYTE byLevel);

private:
	TSFPTrunk<StallMgr>			m_Trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<GameFrameMgr>		m_pFrameMgr;

	TMap<DWORD, tagStall*>		m_mapStall;	
	TemporaryContainer*			m_pContainerStall;
	TMap<INT64, INT64>			m_mapStallItem;
	LocalPlayer*				m_pLp;
	DWORD						m_dwStallRoleID;
	tstring						m_strLPStallName;
	BOOL						m_bSetAdFlag;

	//--VIP̯λ
	EOperateStallType			m_eOperateStallType;
	map<BYTE, tagVIPStall>		m_mapVipStall;
	DWORD						m_dwVipTime;
	
	StallModelConfigMap			m_mapStallModelConfigs;
};
