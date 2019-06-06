#pragma once
#include "..\Cool3D\Cool3DEngine.h"
using namespace Cool3D;
#include "..\Cool3D\public\iplugeditor.h"

#include "..\WorldBase\gameMapEditor\gamemapeditor.h"
using namespace WorldBase;

#include".\MapObjDlg.h"

#define DEFAULT_WAYPOINT_TITLE_SIZE 1
#define QUAD_OFFSET 100

class CPlugMapEditor :public IPlugEditor
{
public:
	CPlugMapEditor(void);
	~CPlugMapEditor(void);

	//-- IPlugEditor
	virtual void AttachEngineInterface(void *pInterface,void *pExInterface=NULL);
	virtual void SetCamera(CameraBase *pCamera);

	virtual void Update(bool bActive);
	virtual void Render(bool bActive);
	virtual void RenderPreview();

	virtual const TCHAR* GetUserName();
	virtual const TCHAR* GetClassName();

	virtual void Create(DWORD pParentWnd);
	virtual void Destroy();
	virtual void ShowWindow(bool bShow);
	virtual HWND GetRenderWnd() {	return NULL; }
	virtual bool Build(const TCHAR* szMapName,const TCHAR* szSaveBasePath,DWORD dwItems,tagBuildOptions& options,IUICallBack* pUI);
	void AddMazeToSG(NullSceneGraph *pSG,const TCHAR* szMazeFile);

	virtual void OnLButtonDown(UINT nFlags, UINT x,UINT y);
	virtual void OnLButtonUp(UINT nFlags, UINT x,UINT y);
	virtual void OnMouseMove(UINT nFlags, UINT x,UINT y);
	virtual void OnMouseWheel(UINT nFlags, short zDelta, UINT x,UINT y);
	virtual void OnRButtonDown(UINT nFlags, UINT x,UINT y);
	virtual void OnRButtonUp(UINT nFlags, UINT x,UINT y);

	MapObjEdit* GetLock() const { return m_pLockSelected;}
	void SetLock(MapObjEdit* pEdit) { m_pLockSelected = pEdit;}
	GameMapEditor* GetMap() const { return m_pEngine;}
	CameraBase*	 GetCamera()  { return m_pCamera;}
    
	//����������
	bool BuildWayPoint(UINT x,UINT y);
	//ʰȡ������
	bool PickWayPoint(UINT x,UINT y);
	//�ƶ�ʰȡ�ĵ�����
	bool MovePickedWayPoint(UINT x,UINT y);

	//�Ҽ�����������
	bool BuildTriggerRButton(UINT x, UINT y);
	//ʰȡ������
	bool PickTrigger(UINT x,UINT y);
	//ʰȡ�������ĵ�
	bool PickTriggerPoint(UINT x, UINT y);
	//�ƶ�ʰȡ�Ĵ�����
	bool MovePickedTrigger(UINT x,UINT y);
	//�ƶ�ʰȡ�Ĵ������ĵ�
	bool MovePickedTriggerPoint(UINT x, UINT y);

	//�ƶ�ʰȡ�Ĳɼ�����
	bool MovePickedGather(UINT x,UINT y);
	//������Ч
	bool BuildSound(UINT x,UINT y);
	//ʰȡ��Ч
	bool PickSound(UINT x,UINT y);
	//�ƶ�ʰȡ��Ч
	bool MovePickedSound(UINT x,UINT y);

	//����ʰȡ��ʾ��ʶ
	void SetPickFlag();
	
	//�������Դ
	bool BuildPointLight(UINT x, UINT y);
	//ʰȡ���Դ
	bool PickPointLight(UINT x, UINT y);
	//�ƶ�ʰȡ�ĵ��Դ
	bool MovePickedPointLight(UINT x, UINT y);

	//����Ѱ·������
	bool BuildPathPoint(UINT x, UINT y);
	//ʰȡѰ·������
	bool PickPathPoint(UINT x, UINT y);
	//�ƶ�ʰȡ��Ѱ·������
	bool MovePickedPathPoint(UINT x, UINT y);

	//�Ҽ�������ͼ����
	bool BuildMapRectRButton(UINT x, UINT y);
	//ʰȡ��ͼ����
	bool PickMapRect(UINT x, UINT y);
	//ʰȡ��ͼ����ĵ�
	bool PickMapAreaPoint(UINT x, UINT y);
	//�ƶ���ͼ����
	bool MovePickMapRect(UINT x, UINT y);
	//�ƶ�ʰȡ�Ĵ������ĵ�
	bool MovePickedMapAreaPoint(UINT x, UINT y);

	//����ˢ�ֵ�
	bool BuildSpawnPoint(UINT x, UINT y);
	//ʰȡˢ�ֵ�
	bool PickSpawnPoint(UINT x, UINT y);
	//�ƶ�ˢ�ֵ�
	bool MovePickedSpawnPoint(UINT x, UINT y);

	//--NPC���ӵ���ͼ�������
	bool BuildNPCNavStartPoint(UINT x, UINT y);
	bool PickNPCNavStartPoint(UINT x, UINT y);
	bool MovePickedNPCNavStartPoint(UINT x, UINT y);

	//--��Ҹ��ӵ���ͼ�������
	bool BuildPlayerNavStartPoint(UINT x, UINT y);
	bool PickPlayerNavStartPoint(UINT x, UINT y);
	bool MovePickedPlayerNavStartPoint(UINT x, UINT y);


	//���ƾ�̬���巽��
	void DrawStatiObjQuad();

	//ͨ����̬���巽��ʰȡ��̬����
	bool PickStaticObjByQuad(UINT x, UINT y);
	//ͨ���Թ�ǽ�ڷ���ʰȡ�Թ�ǽ��
	//bool PickMazeWallByQuad(UINT x, UINT y);

	//������ɸѡ
	bool MouseFilterSelected(MapObjEdit* pSelected);

	CMapObjDlg* GetCMapObjDlg(){return &m_mapObjDlg;}

private:
	bool _Build(const TCHAR* szMapName,const TCHAR* szSaveBasePath,DWORD dwItems,tagBuildOptions& options,IUICallBack* pUI);
	bool BuildMiniMap( const AABBox& box, const DWORD nRTSize, TCHAR* szFileName, HugeQuadTreeSG* pSG );
private:
	GameMapEditor		*m_pEngine;
	CMapObjDlg			m_mapObjDlg;
	bool				m_bDraged;
	CameraBase			*m_pCamera;		
	MapObjEdit*        m_pLockSelected;

	bool                m_bBuildWayPoint;
	bool                m_bBuildTrigger;
	bool				m_bBuildMapRect;

	bool                m_bDragedSoundBox;
	bool                m_bDragedWayPoint;
	bool                m_bDragedTrigger;
	bool				m_bMoveTriggerPoint;
	bool				m_bMoveMapAreaPoint;
	bool                m_bPickObject;
	bool				m_bDragedPointlight;
	bool				m_bDragedMapRect;
	bool				m_bDragedPathPoint;
	bool				m_bDragedSpawnPoint;
	bool				m_bDragedDynamicBlock;
	bool				m_bDragedNPCNav;
	bool				m_bDragedPlayerNav;

	bool                m_bBuildChopField;
	bool                m_bPickChopField;
	bool                m_bDragedChopField;

	bool				m_bDragedGather;
	bool				m_bFirstMove;
	bool				m_bIsEqualType;

	tagMapCarrier		*m_MapCarrier;
	vector<Vector3>	m_vecCarrierPoint;
	INT					m_nCarrierPoint;		//��ǰ�ؾ��ƶ����ڼ�����
};

extern CPlugMapEditor* g_mapEditor;
extern void BeginMFCRes();
extern void EndMFCRes();
