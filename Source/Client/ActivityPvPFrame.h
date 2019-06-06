#pragma once

class ActivityPvPFrame : public GameFrame
{
public:
	enum 
	{
		E_PvP			= 0,
		E_PvPMulti,
	};

	enum 
	{
		E_Sort_TeamID	= 2000,
		E_Sort_Name,
		E_Sort_Score,
	};

public:
	ActivityPvPFrame(void);
	~ActivityPvPFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();

	void RefreshScoreUI();
	void RefreshSelfUI();

private:
	//�ؼ��¼�������
	BOOL EventHandler(tagGUIEvent* pEvent);

	//���Ͳ�ѯ������Ϣ
	void SendQueryScore();

	//���������Ϣ
	void SendQuitActivity();

private:
	TSFPTrunk<ActivityPvPFrame>		m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<Util>					m_pUtil;
	TObjRef<NetSession>				m_pSession;
	TObjRef<NetCmdMgr>				m_pCmdMgr;


	GUIWnd*							m_pWnd_Rank;
	GUIListBox*						m_pLB_RankInfo;		//������Ϣ
	GUIButton*						m_pBtn_TeamID;		//����ID
	GUIButton*						m_pBtn_NameSort;	//��������
	GUIButton*						m_pBtn_Score;		//����
	GUIButton*						m_pBtn_Cancel;		//�ر�������Ϣ

	GUIWnd*							m_pWnd_Self;
	GUIStatic*						m_pStc_Info;		//������Ϣ
	GUIButton*						m_pBtn_Rank;		//���а�ť
	GUIButton*						m_pBtn_Exit;		//�뿪��ť

	DWORD							m_dwActivityID;
};