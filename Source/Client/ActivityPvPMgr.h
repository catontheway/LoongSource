#pragma once

//Ӣ������
struct tagPvPHero
{
	DWORD		dwRank;					//����
	DWORD		dwRoleID;				//���ID
	tstring		strRoleName;			//�������
	INT			nScore;				//��һ���
};

//Ⱥ����¹
struct tagPvPMultiHero
{
	DWORD		dwRank;					//����
	DWORD		dwTeamID;				//������
	DWORD		dwTeamLeaderID;			//�ӳ�ID
	tstring		strTeamLeaderName;		//�ӳ�����
	INT			nTeamScore;			//�������
};

struct tagNS_MsgBlob;
struct tagRoleGetNameEvent;

class ActivityPvPMgr 
{
public:
	ActivityPvPMgr(void);
	~ActivityPvPMgr(void);

	static ActivityPvPMgr* Inst();

	void Init();
	void Destroy();
	
	//��ջ���������Ϣ
	void ClearScoreInfo();

	//���
	void SetActivityName(const TCHAR* name, DWORD id);
	//ʣ��ʱ��
	void SetActivityCD(DWORD dwCD);
	//��ǰ����
	void SetActivityScore(DWORD dwScore);
	//��ǰʣ�ิ�����
	void SetActivityRebornTime(DWORD dwRevive);
	//��ǰ����ID
	void SetActivityTeamID(DWORD dwTeamID);
	//��ǰ�������(�ͻ���ר��)
	void SetActivityType(DWORD dwActivityType) { m_dwActivityType = dwActivityType; }

	//--��ȡ������Ϣ
	tstring& GetActivityName()		{ return m_strActivityName; }
	DWORD GetActivityID()			{ return m_dwActivityID; }
	DWORD GetActivityScore()		{ return m_dwScore; }
	DWORD GetActivityRebornTime()	{ return m_dwRevive; }
	DWORD GetActivityTeamID()		{ return m_dwTeamID; }
	DWORD GetActivityType()			{ return m_dwActivityType; }
	DWORD GetPvPType();


	//--���û�������
	void SetPvPActivityScoreSort( DWORD dwRank, DWORD dwRoleID, DWORD dwScore );
	void SetPvPMultiActivityScoreSort( DWORD dwRank, DWORD dwRoleID, DWORD dwScore, DWORD dwTeamID );

	//--��ȡ����������Ϣ
	vector<tagPvPHero>& GetPvPActivityScoreSort()			{ return m_vecPvPHero; }
	vector<tagPvPMultiHero>& GetPvPMultiActivityScoreSort() { return m_vecPvPMhero; }

	void RefreshScoreSort();

	void ClearPvPUI();

private:
	//��Ϸ�¼�
	DWORD OnRoleGetNameEvent(tagRoleGetNameEvent* pGameEvent);

private:
	TSFPTrunk<ActivityPvPMgr>		m_Trunk;
	TObjRef<NetCmdMgr>				m_pNetCmdMgr;
	TObjRef<GameFrameMgr>			m_pGameFrameMgr;
	TObjRef<NetSession>				m_pSession;

	vector<tagPvPHero>				m_vecPvPHero;
	vector<tagPvPMultiHero>			m_vecPvPMhero;

	vector<tagPvPHero>				m_vecPvPHeroTmp;			//û�������Ļ����
	vector<tagPvPMultiHero>			m_vecPvPMheroTmp;			//ͬ��

	DWORD							m_dwActivityType;			//�����(�ͻ�����)
	DWORD							m_dwActivityID;				//�ID;
	tstring							m_strActivityName;			//�����
	DWORD							m_dwScore;					//�����(���˺Ͷ���)
	DWORD							m_dwRevive;					//����ʣ�����
	DWORD							m_dwTeamID;					//������

};