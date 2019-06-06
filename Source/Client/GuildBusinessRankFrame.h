#pragma once

#include "..\WorldDefine\role_info.h"
struct tagRoleGetSomeNameEvent;

struct tagRefreshGuildBusinessRankEvent;
struct tagRefreshBusinessRankCommendStateEvent;

class GuildBusinessRankFrame : public GameFrame
{
public:
	GuildBusinessRankFrame();
	~GuildBusinessRankFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	bool IsShow() const;
	void Show(bool bShow);
private:
	DWORD EventHandler(tagGUIEvent *pEvent);
	DWORD OnRefreshEvent(tagRefreshGuildBusinessRankEvent *pEvent);
	DWORD OnGetSomeNameOKEvent(tagRoleGetSomeNameEvent *pEvent);
	DWORD OnRefreshStateEvent(tagRefreshBusinessRankCommendStateEvent *pEvent);
	
private:
	TSFPTrunk<GuildBusinessRankFrame> m_Trunk;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<GameFrameMgr> m_pGameFrameMgr;

	GUIWnd *m_pWnd;
	GUIListBox *m_pListRank;
	GUIStatic *m_pStcCommendationState;	// ������رռν���״̬
	GUIButton *m_pBtnCommendationOpen;	// �����ν�
	GUIButton *m_pBtnCommendationClose;	// �رռν�
	GUIButton *m_pBtnClose;				// �ر�
	BOOL m_bCommend;					// �ν�״̬
	bool m_bShow;
	std::vector<DWORD> m_ReqIDList;		// ��Ҫ�������ֵı�
	std::vector<tagCommerceRank> m_vecSaveRank;	// ���û�ҵ����ֱ���������Ϣ
};

