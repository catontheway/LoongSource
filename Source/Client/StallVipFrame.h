#pragma once

struct tagNS_ApplyVIPStall;
struct tagNS_SpecVIPStallGet;
/*
	class: StallVipFrame
	brief: VIP̯λ

*/
class StallVipFrame : public GameFrame
{
public:
	StallVipFrame(void);
	~StallVipFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();


private:
	DWORD EventHandler(tagGUIEvent* pEvent);

	//������Ϣ
	DWORD OnNS_ApplyVIPStall(tagNS_ApplyVIPStall* pMsg, DWORD dwParam);
	DWORD OnNS_SpecVIPStallGet(tagNS_SpecVIPStallGet* pMsg, DWORD dwParam);

	//��Ϸ�¼�
	DWORD OnAllVipStallInfoEvent(tagGameEvent* pEventGame);
	DWORD OnRentVipStallEvent(tagMsgBoxEvent* pEventGame);

private:
	TSFPTrunk<StallVipFrame>		m_Trunk;
	TObjRef<NetSession>				m_pSession;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<Util>					m_pUtil;
	TObjRef<GUISystem>				m_pGUI;

	GUIWnd*							m_pWnd;
	GUIButton*						m_pBtnRent;				//����
	GUIButton*						m_pBtnCheck;			//�鿴
	GUIButton*						m_pBtnCancel;			//�ر�
	GUIListBox*						m_pLVipShop;			//̯λ


	INT								m_nCurSelectedRow;		//��ǰ��ѡ̯λ

};