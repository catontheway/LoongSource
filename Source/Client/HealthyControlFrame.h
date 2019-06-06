#pragma once

#include "StaticEx.h"
#include "..\WorldDefine\msg_fatigue_guarder.h"

//������ʾ����
enum EHealthyState
{
	EHS_Healthy			=	0,		//����ʱ��
	EHS_Fatigue			=	1,		//ƣ��ʱ��
	EHS_Unhealthy		=	2		//������ʱ��
};

/**
*class HealthyControlFrame
*brief ������ϵͳ���
*author hyu
*remark
*base GameFrame
*/
class HealthyControlFrame :
	public GameFrame
{
public:
	HealthyControlFrame(void);
	~HealthyControlFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();

    void ShowWnd(BOOL bShown);

private:
	DWORD OnGameEvent(tagGameEvent* pGameEvent);

	DWORD EventHandler(tagGUIEvent* pEvent);

	DWORD OnNS_GetFatigueInfo(tagNS_GetFatigueInfo* pMsg, DWORD dwParam);

    DWORD OnNS_FatigueLimit(tagNS_FatigueLimit* pMsg, DWORD dwParam);

	//���÷�����״̬
	VOID SetHealthyState(EHealthyState evalue);

    // ��ʾ��ʾ��Ϣ
    VOID ShowMsg(LPCTSTR szMsg);

	TSFPTrunk<HealthyControlFrame>	m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<Util>					m_pUtil;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<NetSession>				m_pSession;

	GUIWnd*							m_pWnd;
	StaticEx*						m_pStcTips;

};
