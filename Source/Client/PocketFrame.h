#pragma once
#include "..\WorldDefine\container_define.h"

class ItemButton;
class ItemMenuButton;
class ItemFrame;
class Item;
class NetSession;
class ThroneFrame;
struct tagUpdateLPItemEvent;
struct tagItemPosChangeExEvent;
struct tagRoleSilverChange;
struct tagRoleYuanbaoChange;

/**	class	PocketFrame
	brief	���ҽ��洦������
	remarks	���������е���Ʒ��Ǯ��ʾ����Ʒ���� 
*/
class PocketFrame :
	public GameFrame
{

public:

	PocketFrame(void);
	~PocketFrame(void);
	
	/** \GameFrame
	*/
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();
	
	/** \��ʾ���ҽ���
	*/
	VOID ShowWnd(BOOL bShow);

private:
	/** \����GUI��Ϣ
	*/
	DWORD EventHandler(tagGUIEvent* pEvent);
	/** \������Ϸ�¼�
	*/
	DWORD OnUpdateLPItem(tagUpdateLPItemEvent* pGameEvent);
	DWORD OnItemPosChangeEvent(tagItemPosChangeExEvent* pGameEvent);
	DWORD OnBagSilverChange(tagRoleSilverChange* pGameEvent);
	DWORD OnBagYuanbaoChange(tagRoleYuanbaoChange* pGameEvent);
	DWORD OnInputItemNumber(tagMsgInputBoxEvent* pGameEvent);
	DWORD OnUpdateEQDataChange(tagUpdateLPItemEvent* pGameEvent);
	DWORD OnUpdateItemCD(tagGameEvent* pGameEvent);
	DWORD OnEventBagExtend(tagGameEvent* pGameEvent);
	DWORD OnOpen_Encrypt(tagGameEvent* pGameEvent);

	DWORD OnEventSetPsdFirst(tagMsgInputBoxEvent* pGameEvent);
	DWORD OnEventSetPsdSecond(tagMsgInputBoxEvent* pGameEvent);
	DWORD OnEventModPsdFirst(tagMsgInputBoxEvent* pGameEvent);
	DWORD OnEventModPsdSecond(tagMsgInputBoxEvent* pGameEvent);
	DWORD OnEventModPsdThird(tagMsgInputBoxEvent* pGameEvent);
	DWORD OnEventDelPsdFirst(tagMsgInputBoxEvent* pGameEvent);
	DWORD OnEventOpenBagPsd(tagMsgInputBoxEvent* pGameEvent);
	
	/** \��ʾ��ǰҳ
	*/
	VOID ShowCurPage(INT16 page);
	
	/** \�������ҳ��
	*/
	VOID UpdateMaxPage();
	/** \������Ʒ
		\param indexΪ��Ʒ��ţ�����Ʒ������ţ�
	*/
	VOID UpdateItem(INT16 index);
	/** \���»���(������Ǯ��Ԫ��)
	*/
	VOID UpdateMoney();

	/** \���ý�Ǯ����
		\param n64ValueΪ��Ǯ����
	*/
	VOID SetMoneyValue(INT64 n64Value);
	/** \����Ԫ������
		\param nValueΪԪ������
	*/
	VOID SetTreasureValue(INT nValue);
	
	/** \��ȡ��ǰλ���ϵ���Ʒ 
		\param indexΪ��Ʒ���
	*/
	Item* GetItem(INT16 index);

	/** \ˢ����Ʒ��ť
		\param indexΪ��Ʒ���
	*/
	VOID RefreshItem(INT16 index);
	/** \ˢ����ƷTips
		\param pItemΪ��Ʒָ��
		\param pWndΪ��Ӧ��Ʒ��ťָ��
	*/
	VOID ReflashItemTips(Item* pItem, GUIWnd* pWnd);

	/** \������Ʒ�ƶ�
		\param n16PosSrcΪ�ƶ�����Ʒ���
		\param n16PosDstΪ�ƶ�����λ�ã�Ĭ��ֵΪ�ƶ�λ�ò�ָ��
	*/
	VOID OnItemChange(INT16 n16PosSrc, INT16 n16PosDst = GT_INVALID, INT16 n16NumDst = 0);
	/** \������Ʒ���뱳��
		\param ePosSrcΪԭ��Ʒ����
		\param n16PosSrcΪԭ��Ʒ���
		\param n16PosDstΪ�ƶ�����λ�ã�Ĭ��ֵΪ�ƶ�λ�ò�ָ��
		\param n16NumDstΪ����Ŀ��λ�õ�������Ĭ��ֵΪȫ������
	*/
	VOID OnItemChangeEx(EItemConType ePosSrc, INT16 n16PosSrc, INT16 n16PosDst = GT_INVALID);
	/** \������Ʒ����
		\param nShelfPosΪ����Ʒ���̵��е���ţ��������ԭ��
		\param dwTypeIDΪ����Ʒ����Ʒԭ��ID
		\param n16NumΪ���������
	*/
	VOID OnBuyItem(INT nShelfPos, DWORD dwTypeID, INT16 n16Num = 1);
	/** \����װ��ж��
		\param ePosΪ��Ʒ��װ�����е�λ��
		\param n16PosDstΪ�ƶ�����λ�ã�Ĭ��ֵΪ�ƶ�λ�ò�ָ��
	*/
	VOID OnUnEquipment(EEquipPos ePos, INT16 n16PosDst = GT_INVALID);
    VOID OnPetUnEquipment(DWORD dwPetID, INT8 n8PebPos, INT16 n16PosDst = GT_INVALID);
	/** \��������Ҽ������Ʒ�¼�
		\param indexΪ��Ʒ���
	*/
	VOID TryToUseItem(INT16 index);
	/** \��������������϶���Ʒ�¼�
		\param indexΪ��Ʒ���
		\param bShiftΪ�Ƿ�סShift����־
	*/
	VOID TryToDragItem(INT16 index, bool bShift);
	/** \��Ʒ�϶�
		\param pItemΪ��Ʒ�϶�ָ��
		\param n16NumΪҪ�϶�������
	*/
	VOID OnItemDrag(Item* pItem, INT16 n16Num, bool bSplit = false);

		
	/** \�����Ƿ��ڵ�ǰҳ
		\param indexΪ��Ʒ���
	*/
	BOOL IsCurPage(INT16 index){return ((SPACE_ONE_BAG * m_n16CurPage <= index) && (SPACE_ONE_BAG * (m_n16CurPage + 1) > index));}
	/** \�����Ӵ���
	*/
	VOID DestroyChild();
	/** \������صĶԻ���
	*/
	VOID DestroyMsgBox();

	/** \��ʾTip
		\param n16IndexΪ��Ʒ���
	*/
	VOID ShowItemTip(GUIWnd* pWnd, INT16 n16Index);

private:

	TSFPTrunk<PocketFrame>		m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>		m_pSession;

	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndCaption;
	GUIStatic*					m_pStcPocket;							// ����ҳ
	GUIButton*					m_pBtnThorne;							// �ٱ��䰴ť
	GUIButton*					m_pBtnSettle;							// ��������ť
	GUIButton*					m_pBtnEncrypt;							// ���Ҽ��ܰ�ť
	GUIButton*					m_pBtnStall;							// �رհ�ť
	GUIButton*					m_pBtnExit;								// �˳���ť
	GUIStatic*					m_pStcGold;								// �����ʾ��
	GUIStatic*					m_pStcSilver;							// ������ʾ��
	GUIStatic*					m_pStcTreasure;							// Ԫ����ʾ��
	
	ItemMenuButton*				m_pPbnPocketIndex[MAX_BAG_NUM+1];		// ����ѡ��ť��Ҫ����������Ʒ��
	ItemButton*					m_pUnitItem[SPACE_ONE_BAG];				// ������Ԫ��

	INT16						m_n16CurPage;							// ��ǰҳ

	Item*						m_pSliptItem;							// Ҫ��ֵ���Ʒ����


	DWORD							m_dwNewPsdCrc;		// ������������CRC��
	DWORD							m_dwOldPsdCrc;		// ����ľ������CRC��
};
