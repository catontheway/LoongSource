#pragma once
#include "..\WorldDefine\container_define.h"



class ItemButton;
class ItemFrame;
class Item;
class NetSession;
class ItemMenuButton;
struct tagUpdateLPItemEvent;
struct tagItemPosChangeExEvent;
struct tagRoleSilverChange;
struct tagRoleYuanbaoChange;

/**	\class StorageFrame
	\brief �ֿ���洦������
	\remarks ����ֿ��е���Ʒ��Ǯ��ʾ����Ʒ���� 
*/
class StorageFrame :
	public GameFrame
{

public:

	StorageFrame(void);
	~StorageFrame(void);
	
	/** \GameFrame
	*/
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();

    void SetWalkWareEnable(bool bEnabled) { m_bWalkWare = bEnabled; }
	bool GetWalkWareMode(){ return m_bWalkWare; }
private:
	/** \����GUI��Ϣ
	*/
	DWORD EventHandler(tagGUIEvent* pEvent);
	/** \������Ϸ�¼�
	*/
	DWORD OnEventUpdateLPItem(tagUpdateLPItemEvent* pGameEvent);
	DWORD OnEventItemPosChange(tagItemPosChangeExEvent* pGameEvent);
	DWORD OnEventRoleWareSilverChange(tagRoleSilverChange* pGameEvent);
	/*VOID OnEventRoleWareYuanbaoChange(tagRoleYuanbaoChange* pGameEvent);*/
	DWORD OnEventSaveMoneyToWare(tagMsgMoneyBoxEvent* pGameEvent);
	DWORD OnEventLoadMoneyFromWare(tagMsgMoneyBoxEvent* pGameEvent);
	DWORD OnEventWareExtend(tagGameEvent* pGameEvent);

	/** \��ʾ��ǰҳ
	*/
	VOID ShowCurPage(INT16 page);
	

	/** \������Ʒ
		\param indexΪ��Ʒ��ţ�����Ʒ�������е���ţ�
	*/
	VOID UpdateItem(INT16 index);
	/** \���»��ң�������Ǯ��Ԫ����
	*/
	VOID UpdateMoney();

	
	/** \���ý�Ǯ����
	*/
	VOID SetMoneyValue(INT64 n64Value);
	/** \����Ԫ������
	*/
	/*VOID SetTreasureValue(INT nValue);*/

	/** \��ȡ��ǰλ���ϵ���Ʒ;
		\param indexΪ��Ʒ���
	*/
	Item* GetItem(INT16 index);

	/** \ˢ����Ʒ��ť
		\param indexΪ��Ʒ���
	*/
	VOID RefreshItem(INT16 index);


	/** \������Ʒ�ƶ�
		\param n16PosSrcΪ�ƶ�����Ʒ���
		\param n16PosDstΪ�ƶ�����λ�ã�Ĭ��ֵΪ�ƶ�λ�ò�ָ��
	*/
	VOID OnItemChange(INT16 n16PosSrc, INT16 n16PosDst = GT_INVALID);
	/** \������Ʒ����ֿ�
		\param ePosSrcΪ��Ʒԭ����
		\param ePosDstΪ��ƷĿ������
		\param n16PosSrcΪ��Ʒԭ���
		\param n16PosDstΪ�ƶ�����λ�ã�Ĭ��ֵΪ�ƶ�λ�ò�ָ��
	*/
	VOID OnItemChangeEx(EItemConType ePosSrc, EItemConType ePosDst, INT16 n16PosSrc, INT16 n16PosDst = GT_INVALID);


	/** \�Ƿ��ڵ�ǰҳ
		\param indexΪ��Ʒ���
	*/
	BOOL IsCurPage(INT16 index){return ((SPACE_ONE_WARE * m_n16CurPage <= index) && (SPACE_ONE_WARE * (m_n16CurPage + 1) > index));}

	/** \�����Ӵ���
	*/
	VOID DestroyChild();
	/** \������صĶԻ���
	*/
	VOID DestroyMsgBox();

private:

	TSFPTrunk<StorageFrame>		m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>		m_pSession;

	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndCaption;
	ItemMenuButton*				m_pPbnStorageIndex[MAX_WARE_NUM];		// �ֿ�ѡ��ť
	GUIStatic*					m_pStcStorage;							// �ֿ�ҳ
	GUIButton*					m_pBtnSaving;							// �ֿ��ť
	GUIButton*					m_pBtnLoading;							// �ֿ�ȡ�ť
	GUIButton*					m_pBtnSettle;							// �ֿ�����ť
	GUIButton*					m_pBtnExit;								// �˳���ť
	GUIStatic*					m_pStcGold;								// �����ʾ��
	GUIStatic*					m_pStcSilver;							// ������ʾ��
	//GUIStatic*					m_pStcTreasure;							// Ԫ����ʾ��

	ItemButton*					m_pUnitItem[SPACE_ONE_WARE];			// �ֿⵥԪ��

	INT16						m_n16CurPage;							// Ϊ��ǰ�ֿ�ҳ

	DWORD						m_dwNPCID;								// �ֿ�NPCID

    bool                        m_bWalkWare;                            // �Ƿ�������ֿ�
};
