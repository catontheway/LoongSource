#pragma once
#include "gameframe.h"

struct tagNS_ExchangeReq;
struct tagNS_ExchangeReqRes;
struct tagNS_ExchangeCancel;
struct tagNS_ExchangeFinish;
struct tagFollowRoleEvent;
class NetSession;
/** \class TradeFrame
	\brief ����ϵͳģ�飬��������ص����� 
*/
class TradeFrame :
	public GameFrame
{
public:
	TradeFrame(void);
	~TradeFrame(void);
	
	// GameFrame
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual VOID Update();

	// ���״�������ʾ
	VOID ShowExchangeErrorMsg(DWORD dwErrorCode, BOOL bNotMine = FALSE);

private:
	// ����������Ϣ
	DWORD OnNetExchangeReq(tagNS_ExchangeReq* pNetCmd, DWORD);
	DWORD OnNetExchangeReqRes(tagNS_ExchangeReqRes* pNetCmd, DWORD);
	DWORD OnNetExchangeCancel(tagNS_ExchangeCancel* pNetCmd, DWORD);
	DWORD OnNetExchangeFinish(tagNS_ExchangeFinish* pNetCmd, DWORD);
	
	// ������Ϸ�¼�
	DWORD OnEventOpenShop(tagGameEvent* pGameEvent);
	DWORD OnEventCloseShop(tagGameEvent* pGameEvent);
	DWORD OnEventCloseShopNow(tagGameEvent* pGameEvent);
	DWORD OnEventReqTrade(tagFollowRoleEvent* pGameEvent);
	DWORD OnEventReqReply(tagMsgBoxEvent* pGameEvent);

	// ����ҽ��׽���
	VOID OpenPlayerTrade(DWORD dwTargetID);
	// �ر���ҽ��׽���
	VOID ClosePlayerTrade();
	// ������ҽ��׽���
	VOID UnlockPlayerTrade();

private:
	TSFPTrunk<TradeFrame>	m_Trunk;
	TObjRef<NetCmdMgr>		m_pCmdMgr;
	TObjRef<NetSession>		m_pSession;

	DWORD					m_dwReqTime;  // ����������Ӧʱ��	

	DWORD					m_dwTargetID; // ���׶���ID
};
