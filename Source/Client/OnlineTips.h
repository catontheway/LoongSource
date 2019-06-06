#pragma once

/*************
\!brief (40������)��ɫ��½��ʾ����
*************/


class OnlineTips : public GameFrame
{
public:
	OnlineTips();
	~OnlineTips();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	BOOL IsShow() const { return m_bShow; }
	void ShowWnd(BOOL bShow);
private:
	DWORD EventHandler(tagGUIEvent *pEvent);
	DWORD OnRefreshTips(tagGUIEvent *pEvent);
	
	void RefreshCanAcceptQuestsNum();
	void RefreshNotCompleteQuestsNum();
	void RefreshCanAssignAttNum();
	void RefreshCanAssignTalentNum();
	void RefreshBBItemNum();
	void RefreshBBYuanBaoNum();

private:
	TSFPTrunk<OnlineTips> m_Trunk;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<NetSession> m_pSession;
	TObjRef<GameFrameMgr> m_pGameFrameMgr;

	GUIWnd *m_pWnd;
	GUIButton *m_pBtnCanAcceptQuestsNum;	// �ɽ���������
	GUIButton *m_pBtnNotCompleteQuestsNum;	// δ�����������
	GUIButton *m_pBtnCanAssignAttNum;		// �ɷ������Ե�
	GUIButton *m_pBtnCanAssignTalentNum;	// �ɷ������ʵ�
	GUIButton *m_pBtnBBItemNum;				// �ٱ�����Ʒ����
	GUIButton *m_pBtnBBYuanBaoNum;			// �ٱ���Ԫ������
	GUIButton *m_pBtnClose;					// �ر�

	BOOL m_bShow;
};

