#pragma once

/** \
	\
	\
	\
*/
enum EItemStrategy
{
	EIST_ItemPocket = 0,	// ����
	EIST_ItemStorage,		// �ֿ�
	EIST_ItemPlayerTrade,	// ��ҽ���
	EIST_ItemShopVend,		// �̵�����
	EIST_ItemStall,			// ��̯
	EIST_ItemSmithCompose,	// �ϳ�
	EIST_ItemInsurance,		// ���׷�
    EIST_GuildStorage,      // ���ɲֿ�
	EIST_GuildUpgrade,      // ��������

	// ע��ÿ����һ��������Ҫ�ں���ItemFrame::SetCurStrategy()�м�����Ӧ�Ĺرս�����Ϣ
	//--todo: more
	EIST_ItemEnd = 32		
};

class PocketFrame;
class ThroneFrame;
class StorageFrame;
class ItemProduceFrame;
class ShopShelfFrame;
class PlayerTradeFrame;
class Item;
class NetSession;
struct tagClickItemEvent;
struct tagOnTheRoleOfItemEvent;
struct tagPutDownItemEvent;
struct tagStartNPCTalkEvent;
/** class   ItemFrame
	breif   ��Ʒ��������
	remarks ������Ʒ��ؽ���֮��Ĳ���
*/
class ItemFrame :
	public GameFrame
{
	friend class FishMgr;
public:
	ItemFrame(void);
	~ItemFrame(void);
	
	// GameFrame
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();

	// ��ȡ��ǰ����
	EItemStrategy GetCurStrategy(){return m_eCurStrategy;}
	// ���õ�ǰ����
	VOID SetCurStrategy(EItemStrategy eType);

private:
	// ������Ϸ�¼�
	DWORD OnOpenPackage(tagGameEvent* pGameEvent);
	DWORD OnClosePackage(tagGameEvent* pGameEvent);
	DWORD OnOpenWare(tagGameEvent* pGameEvent);
	DWORD OnCloseWare(tagGameEvent* pGameEvent);
	DWORD OnCloseWareNow(tagGameEvent* pGameEvent);
	DWORD OnNpcTalk(tagStartNPCTalkEvent* pGameEvent);

	DWORD OnTryToUseItem(tagClickItemEvent* pGameEvent);
	DWORD OnTheRoleOfItem(tagOnTheRoleOfItemEvent* pGameEvent);
	DWORD OnRolePutDownItem(tagPutDownItemEvent* pGameEvent);
	DWORD OnPutDownItemCheck(tagMsgBoxEvent* pGameEvent);
	DWORD OnUseSpecItemCheck(tagMsgBoxEvent* pGameEvent);
	DWORD OnAddPotValue(tagMsgBoxEvent* pGameEvent);

	VOID SendPutDownItem(INT64 n64ItemSerial);

	VOID OnUseItem(Item* pItem);
	VOID OnUseSpecItem(Item* pItem);

	VOID ShowUseItemCheckDlg(Item* pSrcItem, Item* pDstItem = NULL);

private:

	TSFPTrunk<ItemFrame>		m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>		m_pSession;

	PocketFrame*				m_pPocket;		// ���ҽ���
	
	EItemStrategy				m_eCurStrategy;	// ��ǰ����
	DWORD						m_dwNpcGlobalID;// ��ǰNPCID
	DWORD						m_dwNpcTypeID;	// ��ǰNPCTypeID

	Item*						m_pOperSrcItem;	// ���ڲ�������Ʒ�Ļ���
    INT64                       m_n64OperSrcItem;   // ���ڲ�������Ʒ�ڱ����е�ID
	Item*						m_pOperDstItem;	// ������ĳ����Ʒ�Ļ���
};

/** \function SetStrategy
	\blief	  ������Ʒ�Ҽ�����
	\param	  eTypeΪ��Ҫ���õĹ���
	\param	  eComTypeΪ��Ҫ�ȽϵĹ���EIST_ItemEndΪ���Ƚϣ�������Ҫ����ǰ�Ĺ�����ȽϹ�����ͬ������
*/
inline void SetStrategy(EItemStrategy eType, EItemStrategy eComType = EIST_ItemEnd)
{
	ItemFrame* pFrame = (ItemFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("Item"));
	if(P_VALID(pFrame))
	{
		if(eComType == EIST_ItemEnd || eComType == pFrame->GetCurStrategy())
			pFrame->SetCurStrategy(eType);
	}
}
