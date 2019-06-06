#pragma once
#include "..\WorldDefine\shop_define.h"

class ItemButton;
class NetSession;
class Item;
class Equipment;
struct tagMsgInputBoxEvent;
struct tagMsgBoxEvent;
struct tagBuyItemEvent;
struct tagItemPosChangeExEvent;
struct tagNS_FeedbackFromShop;
struct tagNS_GetShopItems;
struct tagNS_GetShopEquips;
struct tagNS_BuyShopItem;
struct tagNS_BuyShopEquip;

struct tagShopEquipClient
{
	INT16 n16Index;		// ����������Ʒ�±�
	INT16 n16RemainNum;	// ��ֵΪGT_INVALIDʱ����ʾ����Ʒ������
	Equipment* pEquip;
	const tagShopRareItemProto	*pRareProto;
};
struct tagShopItemClient
{
	INT16 n16Index;		// ����������Ʒ�±�
	INT16 n16RemainNum;	// ��ֵΪGT_INVALIDʱ����ʾ����Ʒ������
	DWORD dwTypeID;
	const tagShopRareItemProto	*pRareProto;
	const tagItemProto			*pItemProto;
};

/** \class ShopShelfFrame
	\brief ��ͨ���̻��ܽ���
*/
class ShopShelfFrame :
	public GameFrame
{
public:
	ShopShelfFrame(void);
	~ShopShelfFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();

	virtual BOOL EscCancel();

    void ShowWnd(BOOL bVisable);
    bool IsOpen();

private:
	// ����GUI��Ϣ
	DWORD EventHandler(tagGUIEvent* pEvent);
	// ��ʾ��ǰ�˵�
	VOID ShowCurMenu(INT nMenu);
	// ��ʾ��ǰҳ
	VOID ShowCurPage(INT nPage);

	// ������Ϸ�¼�
	DWORD OnEventCheckSellItem(tagMsgBoxEvent* pGameEvent);
	DWORD OnEventClickGoodsInput(tagMsgInputBoxEvent* pGameEvent);
	DWORD OnEventDragGoodsInput(tagMsgInputBoxEvent* pGameEvent);
	DWORD OnEventBuyGoods(tagBuyItemEvent* pGameEvent);
	DWORD OnEventItemPosChange(tagItemPosChangeExEvent* pGameEvent);

	// ����������Ϣ
	DWORD OnNetFeedbackFromShop(tagNS_FeedbackFromShop* pNetCmd, DWORD);
	DWORD OnNetGetShopItems(tagNS_GetShopItems* pNetCmd, DWORD);
	DWORD OnNetGetShopEquips(tagNS_GetShopEquips* pNetCmd, DWORD);
	DWORD OnNetBuyShopItem(tagNS_BuyShopItem* pNetCmd, DWORD);
	DWORD OnNetBuyShopEquip(tagNS_BuyShopEquip* pNetCmd, DWORD);

	// ��ʾ�̵������
	VOID ShowShopErrorMsg(DWORD dwErrorCode);

	// ���õ�ǰҳ��
	VOID SetCurPage();
	// ���õ�������
	VOID SetSignboard(LPCTSTR szSignboard);
	// ���õ��̻���
	VOID SetShelfName(INT nIndex, LPCTSTR szShelfName);

	// ��ҳ��TRUEΪ���£�FALSEΪ����
	VOID PageDown(bool bFlag = TRUE);

	// �����̵����
	BOOL StartShopShelf();

	// �����̵���ܲ�ͬ��Ʒ�б�
	VOID SetShelfNormalList(const tagShopProtoClient* pShop);
	// ���Ե�������е���Ʒ
	VOID TryToClickItem(INT nPos, bool bShift=FALSE);
	// �����϶������е���Ʒ
	VOID TryToDragItem(INT nPos, bool bShift=FALSE);
	// �϶������е���Ʒ
	VOID OnDragItem(INT nPos, INT nKey, DWORD dwTypeID, INT16 n16Num);
	// ����Ʒ
	VOID OnBuyShopItem(INT nKey, DWORD dwTypeID, INT16 n16Num);
	// ������
	VOID OnSellItemToShop(INT16 n16Pos, DWORD dwTypeID);
	// ������������Ϣ
	VOID SendSellToShop(INT64 nItemSerial);
	// ��ʾTip
	VOID ShowGoodsTip(INT nPos);
	// ����������Ʒ�б�����
	VOID SendShelfSpecList(BYTE byMemu);

	// ��ȡ��ǰ��ƷTypeID,�����������
	DWORD GetGoodsTypeID(INT nKey, INT& nMaxCanBuy);
	DWORD GetGoodsTypeID(INT nKey);
	// ��ȡ��ǰ��Ʒ�Ƿ���Թ���,�����ظ���Ʒ64λid
	BOOL IsGoodsCanBuy(INT nKey, INT16 n16Num, INT64 &n64Serial);
	// ��ȡ��ǰ��Ʒ�ɹ��������
	INT GetGoodsMaxCanBuy(INT32	nSilver, INT32 nCostNum, BYTE byLevel);

	// �������
	VOID FreeMap();

	VOID DestroyMsgBox();

private:

	TSFPTrunk<ShopShelfFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;

	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndCaption;
	GUIPushButton*				m_pPbnMenu[MAX_SHOP_SHELF_NUM];		// �̵���ܣ���ʾ��ͬ�������Ʒ
	GUIButton*					m_pBtnExit;							// �˳���ť
	GUIButton*					m_pBtnClose;						// �رհ�ť
	GUIButton*					m_pBtnPageDown;						// ���·�ҳ 
	GUIButton*					m_pBtnPageUp;						// ���Ϸ�ҳ
	GUIStatic*					m_pStcPage;							// ��ǰҳ
	GUIStatic*					m_pStcTitle;						// �̵�����

	ItemButton*					m_pUnitItem[MAX_SHOP_ONE_SHELF];	// ��Ԫ��
	
	INT							m_nCurMenu;							// ��ǰ����
	INT							m_nCurPage[MAX_SHOP_SHELF_NUM];		// ��ǰҳ
	INT							m_nCurMaxPage[MAX_SHOP_SHELF_NUM];	// ��ǰ�������ҳ
	INT							m_nFirstBlank;						// ��ǰҳ��һ����λ
			
	DWORD						m_dwShopNpcID;						// �̵�NpcID
	Item*						m_pSellItem;						// �۳���Ʒ����
	INT							m_nClickKey;						// �����������λ�û���
	INT							m_nDragPos;							// �϶���������λ�û���
	const tagShopProtoClient*	m_pShop;							// �̵꾲̬ģ��


	TMap<INT, const tagShopItemProto*>		m_mapNormalGoods[MAX_SHOP_SHELF_NUM];	// ��ͨ��Ʒ�б�
	TMap<INT, tagShopItemClient*>			m_mapSpecItem[MAX_SHOP_SHELF_NUM];		// ������Ʒ�б�
	TMap<INT, tagShopEquipClient*>			m_mapSpecEquip[MAX_SHOP_SHELF_NUM];		// ����װ���б�

};
