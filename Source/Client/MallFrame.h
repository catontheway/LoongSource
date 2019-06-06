#pragma once
#include "..\WorldDefine\mall_define.h"
#include "..\WorldDefine\msg_mall.h"

/** class MallFrame
	brief �̳�
*/
class NetSession;
class IconStatic;
class PlayerTryDressWnd;
class ItemButton;
class MallButton;
struct tagMsgMallGiftEvent;
struct tagRoleYuanbaoChange;
struct tagExchangeVolumeChange;
class HttpPicEx;
#define MALL_MAXITEM_PERPAGE 18
#define MALL_ITEM_STATE 5
#define MALL_MAX_PROMOTE_ITEM 5
#define MALL_ITEM_SUBTYPE	8
#define SPECIAL_TYPE 2
#define MALL_MAXPACK_PERPAGE 7
#define MALL_PACKPRO_NUM 5
class MallFrame : public GameFrame
{
public:
	enum EMallItemType 
	{
		EMIT_All		= -2,
		EMIT_Package	= -1,
	};

	enum EMallItemStateType			//��Ʒ״̬ͼ��:λ����Ʒͼ���·�
	{
		EMIST_Promote	= 0,		//����
		EMIST_GroupBuy	= 1,		//�Ź�
		EMIST_Present	= 2,		//����
		EMIST_Num		= 3,		//����
		EMIST_Exchange	= 4,		//�һ�
	};

	enum EMamllItemTipsType
	{
		EMITT_All		= 0,
		EMITT_Package,
		EMITT_Promote,
	};

	enum EMallItemOperateState
	{
		EMIOS_NULL				= 0,
		EMIOS_NormalItem,
		EMIOS_PackItem,
		EMIOS_Promote,
		EMIOS_Free,
	};

public:
	MallFrame(void);
	~MallFrame(void);

    tstring GetGUIFullName()
    {
        if(P_VALID(m_pWnd))
            return m_pWnd->GetFullName();
        else 
            return _T("");
    };

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Render3D();
	virtual VOID Update();
	virtual BOOL EscCancel();

	// �Ǵ����Ʒ����
	void ClassifiedMallItem();

	void ShowWnd(BOOL bVal);

	//����avatar
	void UpdateAvatar();

	//���������Ʒ
	DWORD GetMallItemHopeTypeID() { return m_dwSelectItemID; }
	EMallItemOperateState GetOpereateState() { return m_eOperateState; }


private:
	// ����GUI��Ϣ
	DWORD EventHandler(tagGUIEvent* pEvent);

	//ˢ����Ʒѡ���
	void RefreshMallItemFlag();

	//ˢ���Ƽ���Ʒ
	void RefreshPromoteItem();

	//ˢ�������ȡ��Ʒ
	void RefreshFreeItem();

	//��ʾ��ǰҳ����Ʒ(�����)
	void ShowCurPageMallItemByType(INT nPage);

	//��ʾ��ǰҳ����Ʒ(ȫ��)
	void ShowCurPageMallItemByAll(INT nPage);

	//��ʾ��ǰҳ�����
	void ShowCurPageMallPack(INT nPage);

	//�ж���ʾ��Ʒ
	void JudgeShowMallItem(INT nMenu);

	//--��ҳ
	void PageUp();
	void PageDown();

	//��ʾ��ǰҳ��
	void SetCurrentPage();

	//����Ҫ�������ƷID
	void SetHopeBuyItemID(DWORD dwID) { m_dwSelectItemID = dwID; }

	//����Ԫ��
	void SetYuanBao();
	void SetExchangeVolume();

	//�һ���Ʒ
	void ExchangeMallItem();

	//������Ʒ
	void BuyMallItem();
	void BuyMallItemPromote();

	//��ȡ�����Ʒ
	void FreeGetMallItem();

	//����û����Ʒ����ͨ��Ʒ�ؼ�
	void HideCtrlMallItem();
	void HideCtrlPromoteItem();
	void HideCtrlMallPack();

	//--������Ϸ�¼�
	DWORD OnBuyMallItemInputEvent(tagMsgInputBoxEvent* pGameEvent);
	DWORD OnBuyMallPromoteItemInputEvet(tagMsgInputBoxEvent* pGameEvent);
	DWORD OnAffirmBuyMallItemEvent(tagMsgBoxEvent* pGameEvent);
	DWORD OnMsgMallGiftEvent(tagMsgMallGiftEvent* pGameEvent);
	DWORD OnBagYuanbaoChange(tagRoleYuanbaoChange* pGameEvent);
	DWORD OnExchangeVolumeChange(tagExchangeVolumeChange* pGameEvent);
	DWORD OnExchangePackItem(tagMsgInputBoxEvent* pGameEvent);
	DWORD OnExchangeMallItem(tagMsgInputBoxEvent* pGameEvent);
	DWORD OnAffirmExchangePackItem(tagMsgBoxEvent* pGameEvent);
	DWORD OnAffirmExchangeMallItem(tagMsgBoxEvent* pGameEvent);

	//--����������Ϣ

	//--����������Ϣ
	VOID OnSendBuyMallItem(const tagMallItemProto* pItem, INT16 n16Num); 
	VOID OnSendBuyMallPack(const tagMallPackProto* pPack, INT16 n16Num);

	//--����������Ϣ
	DWORD OnNS_MallBuyItem(tagNS_MallBuyItem* pMsg, DWORD dwParam);
	DWORD OnNS_MallFreeGetItem(tagNS_MallFreeGetItem* pMsg, DWORD dwParam);
	DWORD OnNS_MallPresentItem(tagNS_MallPresentItem* pMsg, DWORD dwParam);
	DWORD OnNS_MallBuyPack(tagNS_MallBuyPack* pMsg, DWORD dwParam);
	DWORD OnNS_MallPresentPack(tagNS_MallPresentPack* pMsg, DWORD dwParam);
	DWORD OnNS_MallItemExchange(tagNS_MallItemExchange* pMsg, DWORD dwParam);
	DWORD OnNS_MallPackExchange(tagNS_MallPackExchange* pMsg, DWORD dwParam);

	//--��ʾ��ƷTips
	void ShowMallGoodsTips(int nGoodsPos, int nStatePos);
	void ShowMallPromoteGoodsTips(int nGoodsPos, int nStatePos);
	void ShowMallPackTips(int nGoodsPos, int nStatePos);

	//--������Tips
	void ShowPromoteProTips(const tagMallItemProtoBase* pItem, int nGoodsPos, int nStatePos, EMamllItemTipsType eType=EMITT_All);
	void ShowGroupBuyProTips(const tagMallItemProtoBase* pItem, int nGoodsPos, int nStatePos, EMamllItemTipsType eType=EMITT_All);
	void ShowPresentProTips(const tagMallItemProtoBase* pItem, int nGoodsPos, int nStatePos, EMamllItemTipsType eType=EMITT_All);
	void ShowNumProTips(const tagMallItemProtoBase* pItem, int nGoodsPos, int nStatePos, EMamllItemTipsType eType=EMITT_All);
	void ShowExchangeProTips(const tagMallItemProtoBase* pItem, int nGoodsPos, int nStatePos, EMamllItemTipsType eType=EMITT_All);

	//--�������ж�
	bool JudgePromotePro(const tagMallItemProtoBase* pItem);
	bool JudgeGroupBuyPro(const tagMallItemProtoBase* pItem);
	bool JudgePresentPro(const tagMallItemProtoBase* pItem);
	bool JudgeNumPro(const tagMallItemProtoBase* pItem);
	bool JudgeExchangePro(const tagMallItemProtoBase* pItem);

	// �Դ�
	void JudgeTryDress(DWORD dwTypeID);

	// ��������������
	void SetPresentMaxNum();

	void SetOperateState(EMallItemOperateState eType=EMIOS_NULL);	
	
	//ֱ��
	void YuanBaoAbundance();

	//������
	void ShowMallNotice(BOOL bVal);

	//-------------------------------------------------------------------------------------------
	//Ԫ������
	//-------------------------------------------------------------------------------------------

private:

	TSFPTrunk<MallFrame>		m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;

	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndCaption;		
	PlayerTryDressWnd*			m_pWndTryDress;							// �Դ�����
	GUIStatic*					m_pStcYuanBao;							// Ԫ��
	GUIStatic*					m_pExchangeVolume;						// �һ���
	GUIButton*					m_pBtnClose;							// �ر�
	GUIPushButton*				m_pPBtnYuanBaoTrade;					// Ԫ������
	GUIPushButton*				m_pPBtnGroupBuy;						// �����Ź�
	GUIPushButton*				m_pPBtnNotice;							// �̳ǹ���
	GUIButton*					m_pBtnYuanBaoAbundance;					// Ԫ����ֵ
	GUIButton*					m_pBtnReset;							// �Դ�����

	GUIStatic*					m_pStcFetchBack;						// �����ȡ����
	IconStatic*					m_pStcFetchSel;							// �����ȡѡ���
	ItemButton*					m_pFetchUnitItem;						// �����ȡ��Ʒ
	GUIButton*					m_pBtnFetch;							// �����ȡ��ť	
	GUIStatic*					m_pBtnFetchTitle;						// �����ȡ��Ʒ��

	GUIStatic*					m_pStcGoodsBack;						// ����������
	GUIStatic*					m_pStcGoodsBackEx;						// ��ͨ��Ʒ
	GUIStatic*					m_pUnitItemBack[MALL_MAXITEM_PERPAGE];	// ��Ʒ��
	IconStatic*					m_pUnitItemSel[MALL_MAXITEM_PERPAGE];	// ��Ʒѡ���
	MallButton*					m_pUnitItem[MALL_MAXITEM_PERPAGE];		// ��Ʒ
	GUIStatic*					m_pStcItemName[MALL_MAXITEM_PERPAGE];	// ��Ʒ����
	GUIStatic*					m_pStcInfo[MALL_MAXITEM_PERPAGE];		// ��Ʒ��Ϣ
	IconStatic*					m_pStcState[MALL_MAXITEM_PERPAGE][MALL_ITEM_STATE];// ��Ʒ״̬
	GUIButton*					m_pBtnPDItem;							// ��һҳ(��ͨ��Ʒ)
	GUIButton*					m_pBtnPUItem;							// ��һҳ(��ͨ��Ʒ)
	GUIStatic*					m_pStcCPItem;							// ��ǰҳ��ʾ(��ͨ��Ʒ)
	GUIButton*					m_pBtnPDPack;							// ��һҳ(�����Ʒ)
	GUIButton*					m_pBtnPUPack;							// ��һҳ(�����Ʒ)
	GUIStatic*					m_pBtnCPPack;							// ��ǰҳ��ʾ(�����Ʒ)

	GUIButton*					m_pBtnBuy;								// ����
	GUIButton*					m_pBtnPresent;							// ����
	GUIButton*					m_pBtnGroupBuy;							// �Ź�
	GUIButton*					m_pBtnExchange;							// �һ�

	GUIStatic*					m_pStcProBack;							// �Ƽ���Ʒ����
	GUIStatic*					m_pProItemBack[MALL_MAX_PROMOTE_ITEM];
	IconStatic*					m_pUnitProItemSel[MALL_MAX_PROMOTE_ITEM];
	MallButton*					m_pUnitProItem[MALL_MAX_PROMOTE_ITEM];
	IconStatic*					m_pProItemState[MALL_MAX_PROMOTE_ITEM][MALL_ITEM_STATE];
	GUIStatic*					m_pProItemName[MALL_MAX_PROMOTE_ITEM];
	GUIStatic*					m_pProItmeInfo[MALL_MAX_PROMOTE_ITEM];

	GUIStatic*					m_pStcPackBack;							//�������
	IconStatic*					m_pUnitPackBack[MALL_MAXPACK_PERPAGE];	//ÿ�����
	IconStatic*					m_pUnitPackSel[MALL_MAXPACK_PERPAGE][MALL_PACKPRO_NUM];	//���ѡ���
	MallButton*					m_pUnitPack[MALL_MAXPACK_PERPAGE][MALL_PACKPRO_NUM];	//��������Ʒ
	IconStatic*					m_pStcPackName[MALL_MAXPACK_PERPAGE];	// �������
	IconStatic*					m_pStcPackInfo[MALL_MAXPACK_PERPAGE];	// �����Ϣ
	IconStatic*					m_pStcPackState[MALL_MAXPACK_PERPAGE][MALL_ITEM_STATE];// ���״̬
	GUIStatic*					m_pStcPackNew[MALL_MAXPACK_PERPAGE];	// ���������ʾ
	GUIStatic*					m_pStcPackHot[MALL_MAXPACK_PERPAGE];	// ���������ʾ



	GUIPushButton*				m_pPBtnGoods;							// �̳���Ʒ�����ǩ
	GUIPushButton*				m_pPBtnAll;								// ȫ����Ʒ��ť
	GUIPushButton*				m_pPBtnPackage;							// �����Ʒ��ť
	GUIPushButton*				m_pPBtnSubMall[MALL_ITEM_SUBTYPE];		// ��Ʒ���ఴť

	GUIPushButton*				m_pPBtnMall;							// ˢ����Ʒ����ť

	GUIStatic*					m_pWndNotice;							// ����
	HttpPicEx*					m_pHttp_MallNotice;						


	TMap<DWORD, tagMallItemProto*>	m_mapMallItem[MALL_ITEM_SUBTYPE];	// ��Ʒ�����б�

	INT							m_nCurMenu;								// ��ǰ��Ʒ�������:0-7 ȫ��:-2 ���:-1
	INT							m_nCurPage[MALL_ITEM_SUBTYPE];			// ��ǰҳ
	INT							m_nCurMaxPage[MALL_ITEM_SUBTYPE];		// ��ǰ�������ҳ

	INT							m_nSpecialPage[SPECIAL_TYPE];			// ȫ��:0 ���:1
	INT							m_nSpecialMaxPage[SPECIAL_TYPE];

	DWORD						m_dwSelectItemID;						// Ҫ�������ƷID
	INT16						m_n16BuyNum;							// ���������
	vector<tagMsgMallGiftEvent>	m_vecGiftEvent;							// ��������
	bool						m_bSelectPromote;
	EMallItemOperateState		m_eOperateState;
	tstring						m_strMallNotice;						// ������̳ǹ���

	//-------------------------------------------------------------------------------------------
	//Ԫ������
	//-------------------------------------------------------------------------------------------
    GUIStatic*					m_pStcYuanbaoBack;						// Ԫ�����ױ���

	//-------------------------------------------------------------------------------------------
	//�Ź�
	//-------------------------------------------------------------------------------------------
	GUIStatic*					m_pStcGroupBuyBack;						// �Ź�����
};