#pragma once
#include "..\WorldDefine\container_define.h"

class ItemButton;
class ItemFrame;
class Item;
class NetSession;
struct tagUpdateLPItemEvent;
struct tagRoleYuanbaoChange;
struct tagRoleGetNameEvent;
/**	\class StorageFrame
	\brief �ٱ������洦������
	\remarks ����ٱ����е���Ʒ���� 
*/
class ThroneFrame :
	public GameFrame
{

public:

	ThroneFrame(void);
	~ThroneFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();

	virtual BOOL EscCancel();

private:
	// ����GUI��Ϣ
	DWORD EventHandler(tagGUIEvent* pEvent);
	// ������Ϸ�¼�
	DWORD OnEventUpdateLPItem(tagUpdateLPItemEvent* pGameEvent);
	DWORD OnEventYuanbaoChange(tagRoleYuanbaoChange* pGameEvent);
	DWORD OnEventLoadYuanbao(tagMsgMoneyBoxEvent* pGameEvent);
	DWORD OnEventBaiBaoRecord(tagGameEvent* pGameEvent);
	DWORD OnEventRoleGetName(tagRoleGetNameEvent* pGameEvent);
	// ˢ�°ٱ���
	VOID ReflashThrone();

	// ����Ԫ��
	VOID UpdateYuanbao(INT nYuanbao);

	// ������Ʒ
	VOID UpdateItem(INT16 index);

	// ����Ʒ��������������
	VOID OnItemPosChangeEx(INT16 n16Pos, EItemConType eDstCon = EICT_Bag, INT16 n16DstPos = GT_INVALID);

	// ��ȡ��ǰλ���ϵ���Ʒ;
	Item* GetItem(INT16 index);

	void DestroyMsgBox();

	// ˢ�°ٱ�����¼
	void ReflashRecord();

private:

	TSFPTrunk<ThroneFrame>		m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;

	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndCaption;
	GUIWnd*						m_pWndList;								// ��ϸ��ʾ����
	GUIStatic*					m_pStcThrone;							// �ٱ���ҳ
	GUIButton*					m_pBtnExit;								// �˳���ť
	GUIButton*					m_pBtnSwitch;							// ״̬�л���ť
	GUIButton*					m_pBtnLoading;							// ȡ��
	GUIStatic*					m_pStcYuanbao;							// Ԫ����ʾ
	GUIStaticEx*				m_pStcList;								// ��ϸ��ʾ�б�

	ItemButton*					m_pUnitItem[SPACE_BAIBAO_BAG];			// �ٱ�����Ԫ��

	std::set<DWORD>				m_setRecordRoleNameReq;					// ��¼�н�ɫID��ȡ����
};
