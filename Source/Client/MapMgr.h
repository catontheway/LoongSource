#pragma once

/** \class MapMgr
	\brief ��Ϸ����������
*/
class MapMgr
{
public:
	enum{MAX_VIEW_DIST=4};
public:
	MapMgr(void);
	~MapMgr(void);

	/** ��ʼ����������Ϸ����ʱ����
	*/
	void Init();
	/** �ͷ����ݣ��뿪��Ϸ����ʱ����
	*/
	void Destroy();

	void OpenMap(const TCHAR* szMapName,DWORD dwMapID,CameraBase* pBindCamera);
	void CloseMap();

	DWORD GetCurMapID(){ return m_curMapID;}
	const tstring& GetCurMapName() { return m_curMapName; }

	/** \Զ����Ұ
	*/
	void SetViewDist(int viewDist);
	int GetViewDist(){ return m_viewDist;}
	/** \����ϸ��
	*/
	void SetSceneDetailLevel(int level);
	int GetSceneDetailLevel(){ return m_sceneDetailLevel;}
	/** \̫������
	*/
	void EnableSunFlare( const bool bEnable );

	void Update();
	DWORD OnGameEvent(tagGameEvent* pEvent);

	/** ͬ�����¿�����
	*/
	void SyncInvalidViewZone();

	SceneGraph* GetSceneGraph(){ return m_pSG;}
	GameMap& GetGameMap(){ return m_gameMap;}
	NavMap*  GetNavMap() { return &m_navMap; }

	int		GetMapAreaIndex( const Vector3& vPos, const Vector3& vSize, EMapArea eAreaType = EMA_Null );
	DWORD	GetCurTriggerID() const;
	BOOL	GetTriggerPos(DWORD questSerial,Vector3& pos,AABBox& box);
	BOOL	GetTriggerPos(DWORD triggerID, Vector3& pos);
	static MapMgr* Inst();
private:
	void SetViewZoneSize();
	void UpdateTriggers();
	void UpdateMapArea();
	void ResetSceneFog( const bool bFade );

private:
	int						m_viewDist;
	int                     m_sceneDetailLevel;
	bool                    m_bEnableSunFlare;
	HugeQuadTreeSG*			m_pSG;
	GameMap					m_gameMap;
	NavMap					m_navMap;
	DWORD					m_curMapID;
	tstring					m_curMapName;

	vector<HRGN>			m_triggerRgn;			//����������
	vector<HRGN>			m_mapAreaRgn;			//��ͼ����
	bool					m_bInTrigger;			//��ҵ�ǰ�Ƿ��ڴ�������
	int						m_curTriggerIndex;		//��ǰ������ڴ���������
	int						m_curMapAreaIndex;		//��ǰ������ڵ�ͼ��������

	float					m_lastUpdateMapAreaTime;//���һ�θ���MapArea��ʱ��

	TObjRef<NetSession>		m_pZoneSession;
	TObjRef<GameFrameMgr>	m_pGameFrameMgr;
	TSFPTrunk<MapMgr>		m_trunk;
};
