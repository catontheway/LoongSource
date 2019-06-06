#pragma once

struct tagGuildAffairInfo;

class GuildAffairFrame : public GameFrame
{
public:
	GuildAffairFrame();
	~GuildAffairFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

private:
	DWORD EventHandler(tagGUIEvent *pEvent);
	DWORD OnEventRefresh(tagGameEvent *pEvent);
	void ShowQuest();
	bool IsConditionSatisfaction(tagGuildAffairInfo *pInfo);
	void DestroyMsgBoxes();
	
private:
	TSFPTrunk<GuildAffairFrame> m_Trunk;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<GameFrameMgr> m_pGameFrameMgr;

	GUIWnd *m_pWnd;					// ������
	GUIListBox *m_pListAffaires;	// �����б�
	GUIStatic *m_StcpTimes;			// �ɷ�������
	GUIButton *m_pBtnIssueQuest;	// ��������
	GUIButton *m_pBtnClose1;		// ���Ͻ�X
	GUIButton *m_pBtnClose2;		// �ر�	
	
	DWORD m_dwCurRow;
	DWORD m_dwCurChooseRow;
	std::vector<tagGuildAffairInfo*> m_vecAffairInfo;	// ������ʾ�����İ���
	BYTE m_byGuildLv;
};



