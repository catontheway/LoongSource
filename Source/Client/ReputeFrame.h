#pragma once

#include "CurrencyMgr.h"
#include "ReputeMgr.h"
#include "ProgressEx.h"
#include "..\WorldDefine\reputation.h"
#include "..\WorldDefine\msg_famehall.h"

class FameHallFrame;

/*!
	\class ReputeFrame
	\brief ����ϵͳ����
	\author hyu
	\base GameFrame
	\remark
*/
class ReputeFrame : public GameFrame
{
public:
	ReputeFrame(void);
	~ReputeFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	
	//ˢ��������ʾ
	VOID UpdateRepute();

private:
	

	DWORD OnGameEvent(tagGameEvent* pGameEvent);

	DWORD OnFamousUIOpen(tagGameEvent* pMsg);
	DWORD OnFamousUIClose(tagGameEvent* pMsg);

    DWORD OnReputeChange(tagReputeChange* pMsg);
    DWORD OnBecomeEvent(tagBecomeEvent* pMsg);
    DWORD OnRoleClanContributeChange(tagRoleClanContributeChange* pMsg);

	DWORD EventHandler(tagGUIEvent* pEvent);

	TSFPTrunk<ReputeFrame>		m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;

	//GUI�ؼ�
	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndFather;

	GUIButton*					m_pBtEnterFameHall;

	struct tagReputeGUICollect;
	tagReputeGUICollect* m_pReputeGUICollect;

	/*! ���� */
	bool						m_bisFamousOpen;	/*!< �����ô���״̬ */
	FameHallFrame*				m_pFameHallFrame;	/*!< �����ô���frame */

    bool                        m_bneedRefreshRepute;
    DWORD                       m_timeFirst;
};
