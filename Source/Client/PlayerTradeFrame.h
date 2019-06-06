#pragma once
#include "GameFrame.h"
#include "..\WorldDefine\exchange_define.h"

class TemporaryContainer;
class ItemButton;
class LockStatic;
class Item;
struct tagTemporaryItem;
struct tagNS_ExchangeAdd_2Src;
struct tagNS_ExchangeAdd_2Dst;
struct tagNS_ExchangeDec;
struct tagNS_ExchangeLock;
struct tagNS_ExchangeMoney;
struct tagItemPosChangeExEvent;

/** \class	PlayerTradeFrame
	\brief	��ҽ��׽���
*/
class PlayerTradeFrame :
	public GameFrame
{
public:
	PlayerTradeFrame();
	~PlayerTradeFrame();
	
	/** \GameFrame
	*/
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();

	VOID SetTargetPlayerID(DWORD dwTargetID);

	VOID LockSrcControl(BOOL bLock);
	VOID LockDstControl(BOOL bLock);
private:


	BOOL EventHandler(tagGUIEvent* pEvent);

	VOID SetExchangeEnable();

	// ����������Ϣ
	DWORD OnNetExchangeAdd2Src(tagNS_ExchangeAdd_2Src* pNetCmd, DWORD);
	DWORD OnNetExchangeAdd2Dst(tagNS_ExchangeAdd_2Dst* pNetCmd, DWORD);
	DWORD OnNetExchangeDec(tagNS_ExchangeDec* pNetCmd, DWORD);
	DWORD OnNetExchangeMoney(tagNS_ExchangeMoney* pNetCmd, DWORD);
	DWORD OnNetExchangeLock(tagNS_ExchangeLock* pNetCmd, DWORD);
	
	// ����������Ϣ
	VOID SendCancelExchange();
	VOID SendVerifyExchange();
	VOID SendLockExchange();
	VOID SendAddItemExchange(Item* pItem);
	VOID SendDecItemExchange(Item* pItem);

	// ������Ϸ�¼�
	DWORD OnEventItemPosChange(tagItemPosChangeExEvent* pGameEvent);
	DWORD OnEventExchangeMoney(tagMsgMoneyBoxEvent* pGameEvent);

	// ����������Ʒ
	VOID OnItemAdd(INT16 n16Pos, DWORD dwTypeID);
	// ���������Ʒ
	VOID OnItemDec(INT16 n16Pos);
	
	VOID SetTitle();
	VOID SetMoneySrc(INT64 n64Silver);
	VOID SetMoneyDst(INT64 n64Silver);

	VOID ReflashItemSrc(INT16 n16Pos);
	VOID ReflashItemDst(INT16 n16Pos);

	// ��ʾ������ʾ
	VOID ShowExchangeErrorMsg(DWORD dwErrorCode);

	// ������صĶԻ���
	VOID DestroyMsgBox();

private:
	TSFPTrunk<PlayerTradeFrame>		m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<Util>					m_pUtil;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	
	TemporaryContainer*				m_pContainerSrc;
	TemporaryContainer*				m_pContainerDst;
	TList<Item*>					m_listItemDst;
	
	GUIWnd*							m_pWnd;
	GUIStatic*						m_pWndCaption;
	GUIStatic*						m_pStcBackSrc;
	GUIStatic*						m_pStcGoldSrc;
	GUIStatic*						m_pStcSilverSrc;
	GUIStatic*						m_pStcPlayerNameSrc;
	LockStatic*						m_pStcLockSrc;
	GUIStatic*						m_pStcBackDst;
	GUIStatic*						m_pStcGoldDst;
	GUIStatic*						m_pStcSilverDst;
	GUIStatic*						m_pStcPlayerNameDst;
	LockStatic*						m_pStcLockDst;
	ItemButton*						m_pItemSrc[MAX_EXCHANGE_ITEM];
	ItemButton*						m_pItemDst[MAX_EXCHANGE_ITEM];
	GUIButton*						m_pBtnClose;
	GUIButton*						m_pBtnExchange;
	GUIButton*						m_pBtnOK;
	GUIButton*						m_pBtnCancel;

	DWORD							m_dwTargetID;
};
