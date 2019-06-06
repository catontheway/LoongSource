#pragma once
#include "..\GameMap\GameMap.h"
#include "MapObjEdit.h"
#include "..\..\Cool3D\SceneGraph\SGSkyBox.h"
#include "CreatureDataReader.h"
using namespace Cool3D;

#define DISTENCE_WATPOINT_PICK 50   //waypoint's pick distance
#define DISTENCE_TRIGGER_PICK 50
#define HEIGHT_SHOW_BOX 100    //���������� ʰȡ�������͵����㷶Χ���ӵĸ߶� chopfield���Ӹ߶�
#define DISTENCE_SOUNDBOX_PICK 100  
#define DISTENCE_CHOPFIELD_TREEZONE 64
#define DISTENCE_CHOPFIELD_WHEATZONE 32
#define DISTENCE_MAZEWALL_ZONE 20
#define DISTENCE_POINTLIGHT_PICK 100	//���Դ
#define DIS_PATHPOINT_Y 50
#define DIS_PATHPOINT_XZ 30
#define DIS_SPAWNPOINT_Y 30
#define DIS_SPAWNPOINT_XZ 20
#define DIS_NPCNAVSTARTPOINT_PICK 50
#undef VFS_READ
namespace WorldBase
{
	class MapObjEdit;
	class WORLD_BASE_API GameMapEditor : public GameMap
	{
	public:
		GameMapEditor(void);
		virtual ~GameMapEditor(void);

		//--GameMap
		virtual void Destroy();

		//--
		void Create(TerrainEditor *pTerrain,NullSceneGraph *pSG,NavNullSceneGraph *pNavSG);
		void LoadAttDataBase();

		bool MousePick(UINT cursorX,UINT cursorY,Vector3& out);

		bool WriteEditFile(const TCHAR* szFileName);
		bool LoadFromEditFile(const TCHAR* szFileName);

		bool SelectObjByMouse(UINT cursorX,UINT cursorY);
		bool SelectObjByMouseEx(UINT cursorX,UINT cursorY,MapObjEdit::EType type);
		bool SelectObjByID(WORD mapID);

		void AddMapObjEdit(MapObjEdit* pEdit);

		bool RemoveMapObjEdit(MapObjEdit* pEdit);
		MapObjEdit* RemoveMapObjEdit(MapObjEdit::EType Type, DWORD mapID);

		const list<MapObjEdit*>& GetMapObjEditList();
		const list<tagNPCNavMapBuildStartPoint*>& GetNPCNavMapBuildStartPoint() { return m_NPCNavStartPointList; }
		const list<tagPlayerNavMapBuildStartPoint*>& GetPlayerNavMapBuildStartPoint() { return m_PlayerNavStartPointList; }

		void ClearEditList(bool bAll=true);

		MapObjEdit *GetSelectedObj() const			{	return m_pSelectedObj;}

		CreatureDataReader* GetAttDataBase()		{	return &m_createData;}
		
		SceneGraph*  GetSceneGraph()				{	return m_pSceneGraph;}
		NavNullSceneGraph* GetNavSG()				{	return m_pNavSG;	 }

		void SetSelectObj(MapObjEdit* pSelected)	{   m_pSelectedObj = pSelected;}

		//GNavMap* GetNav()						    {	return &m_navMap;}
		void DrawEdit();
		void DrawWayPoint();		//�����������
		void DrawTrigger();			//�����������
		void DrawSound();			//����Ч���
		void DrawPointLight();		//�����Դ
		void DrawMapRect();			//����ͼ����
		void DrawPathPoint();		//��Ѱ·������
		void DrawSpawnPoint();		//��ˢ�ֵ�
		void DrawNPCNavStartPoint(); //��NPC���ӵ���ͼ�������
		void DrawPlayerNavStartPoint();//����Ҹ��ӵ���ͼ�������

		
		void SetSunLight(const tagLight& sunlight) {	m_SunLight = sunlight; }
		void SetFog(const tagDistFog& fog)	{	m_Fog = fog;}
		void SetSkyCol(const Color4f& skyCol) { m_SkyCol = skyCol; }
		void SetRenderSkyShade(const bool bRenderskyShade);
		void SetSkyShadeCol(const DWORD dwSkyShadeCol);
		void SetDynamicDiffCol(const DWORD dwCol) { m_dwDynamicDiffCol = dwCol; }
		void SetDynamicAmbCol(const DWORD dwCol) { m_dwDynamicAmbCol = dwCol; }
		void SetDynamicSpecCol(const DWORD dwCol) { m_dwDynamicSpecCol = dwCol; }

		//������պ�
		void SetSkyBox(const TCHAR* szTopTex, const TCHAR* szFrontRightTex, const TCHAR* szBackLeftTex, 
			const Vector3& drawSize, const Vector3& offSize);
		//����̫������ǿϵ��
		void SetSunModulus(FLOAT fVal) { m_fSunModulus = fVal; }
		//������պ���ת�Ƕ�
		void SetSkyYaw(FLOAT fVal);

		//�õ�������һ������Ӧ�ĸ������꣬�Ҹ���z�����Ѿ��任
		void GetTerrainTileXZ(Vector3 &pos,POINT &tileXZ);
		//�õ�����
		TerrainEditor * GetHeightMap()              {   return m_pTerrain;  }
		//��������
		MapObjEdit* FindMapObjEdit(MapObjEdit::EType Type, DWORD ObjID);

		//��������
		//���һ�������㣬������ͬһ�鵼�����е�λ������
		void AddWayPoint(tagMapWayPoint *pWayPoint,int &nIndex);
		//���������õ�Ѱ·������
		tagMapWayPoint* FindWayPoint(DWORD dwIndex);
		//�õ��������б�
		void GetWayPointList(list<tagMapWayPoint*>& out)      {  out = m_WayPointList;  }
		//��յ�����
		void ClearWayPointList();
		//set current draw waypoint
		void SetCurPosForWayPoint(Vector3 pos,bool bIsCurDrawWayPoint)  
		{ 
			m_bCurDrawWayPoint=bIsCurDrawWayPoint;
			if(bIsCurDrawWayPoint)
			   m_vCurPosForWayPoint=pos;
		}
		//set pick way point
		void SetCurPosForPickWayPoint(Vector3 pos)            {  m_vCurPickWayPoint=pos;  }
		void SetDrawPickFlagForWayPoint(bool bIsPickWayPoint) {  m_bPickWayPoint=bIsPickWayPoint;  }
		//�ҵ�һ�����������ݣ����û�ҵ�����NULL
        tagMapWayPoint* GetWayPointData(DWORD nObjId);
		tagMapWayPoint* GetWayPointData(const TCHAR* szName, BYTE byIndex);
		//��ѯ�Ƿ��Ѿ�����ͬ���ĵ�������
		bool IsExistWayPointWithSameName(TCHAR* szName);
		//pick way point
		void  PickWayPoint(UINT x, UINT y);
		//get current picked way point
		tagMapWayPoint* GetCurPickedWayPoint()                {  return m_pSelectWayPoint;  }
		//set current picked way point 
		void SetCurPickedWayPoint(tagMapWayPoint* pWayPoint)  {  m_pSelectWayPoint=pWayPoint;  }
		//delete a waypoint
		void DeleteWayPoint(DWORD dwObjID);
		//ֱ���޸��ض�����������
		void ModifyWayPointProp(tagMapWayPoint WayPoint);
        //����ǰ������������ȫ�ı�
		void ModifyWayPointProp(DWORD dwOriID, TCHAR * szOldName, int nOldIndex,tagMapWayPoint WayPoint);
		//����/��� �Ƿ���Ƶ�����
		void SetIsDrawWayPoint(bool bVal) { m_bDrawWayPoint = bVal; }
		bool GetIsDrawWayPoint()const { return m_bDrawWayPoint; }

		//���������
		//�¼Ӵ�����
		void  AddTrigger(tagMapTrigger *pTrigger);
		//modify trigger
		void ModifyTriggerProp(tagMapTrigger Trigger);
		//����IDɾ��һ��������
		void DeleteTrigger(DWORD dwObjID);
		//��������ɾ��һ��������
		void DeleteTrigger(int nIndex);
		//get a trigger list
		void GetTriggerList(list<tagMapTrigger*>& out)        {  out = m_TriggerList;  }
		//clear trigger list
		void ClearTriggerList();
		//set current draw trigger
		void SetCurPosForTrigger(Vector3 pos,bool bIsCurDrawTrigger)  
		{ 
			m_bCurDrawTrigger=bIsCurDrawTrigger;
			if(m_bCurDrawTrigger)
				m_vCurPosForTrigger=pos;
		}
		void GetCurPosForTrigger(Vector3 &out)                {  out=m_vCurPosForTrigger;   }

		void SetPosForTrigger(Vector3& pos, int curPos)	
		{	 
			if(curPos>=(int)m_vTriggerPos.size())
				m_vTriggerPos.push_back(pos);
			else
				m_vTriggerPos[curPos] = pos; 
		}
		void SetPosForTrigger(Vector3& pos)		
		{	 
			m_vTriggerPos.push_back(pos); 
			m_nCurTriggerNum++;
		}
		void GetPosForTrigger(Vector3&out, int curPos)		  {  out = m_vTriggerPos[curPos];}
		void GetPosForTrigger(vector<Vector3>& vec)			  {	 vec = m_vTriggerPos; }
		//set pick trigger
		void SetCurPosForPickTrigger(Vector3 pos)             {  m_vCurPickTrigger=pos;     }
		void SetDrawPickFlagForTrigger(bool bIsPickTrigger)   {  m_bPickTrigger=bIsPickTrigger;  }
		//pick trigger
		void  PickTrigger(UINT x, UINT y);
		//ʰȡ��������ĳһ����
		void PickTriggerPoint(UINT x, UINT y, bool& bHave);
		//get current trigger
		tagMapTrigger* GetCurPickedTrigger()                  {  return m_pSelectTrigger;  }
		//set current trigger
		void SetCurPickedTrigger(tagMapTrigger* pTrigger)     {  m_pSelectTrigger=pTrigger;  }
		//ͨ��ID���Ҵ�����
		tagMapTrigger* GetTriggerData(DWORD dwObjID);
		//��õ�ǰ��������������������ĸ���
		int GetCurTriggerNum()								  { return m_nCurTriggerNum; }
		void ClearTriggerNum()								  { m_nCurTriggerNum = 0; }
		//���㴥������AABBOX
		AABBox CalTriggerAABBox();
		//���¼��㴥������AABBOX
		void ReCalTriggerAABBox(tagMapTrigger* pTri);
		//
		void ClearTriggerPos();
		//�õ�ʰȡ������ĳ�����ǵڼ�����
		int GetTriggerNum()								{	return m_nTriggerNum; }
		void ClearPickTriggerPoint()					{	m_bPickTriggerPoint = false; m_nTriggerNum = 0; }
		//����/��� �Ƿ���ƴ�����
		void SetIsDrawTrigger(bool bVal) { m_bDrawTrigger = bVal; }
		bool GetIsDrawTrigger()const { return m_bDrawTrigger; }
		

		//��Ч���
		//����һ����Ч
		void AddSound(tagMapSound *pSound);
		//ɾ��һ����Ч
		void DeleteSound(tagMapSound *pSound);
		//ɾ��һ����Чͨ������
		void DeleteSound(DWORD nIndex);
		//�õ���Ч�б�
		void GetSoundList(list<tagMapSound*>& out)            {  out = m_SoundList;  }
		//�����Ч�б�
		void ClearSoundList();
		//ʰȡ��Ч
		void PickSoundBox(UINT x,UINT y);
		//���õ�ǰѡ�����Ч
		void SetSelectedSound(tagMapSound *pSound)            {  m_pSelectSound=pSound;  }
		//�õ���ǰѡ�����Ч
		tagMapSound *GetSelectedSound()                       {  return m_pSelectSound;  }
		//���������õ���Ч
		tagMapSound *FindSound(DWORD nIndex);
		//������Ч��ʶ
		void SetDrawSoundFlag(bool bVal)	{ m_bPickSound = bVal; }
		//��Ч��ʶλ��
		void SetPosForSound(Vector3& pos)	{ m_vCurPosSound = pos; }

		//���Դ���
		//����һ�����Դ
		void AddPointLight(MapObjEdit* pPointLight);
		//ɾ��һ�����Դ
		void DeletePointLight(tagMapPointLight *pPointLight);
		//ɾ��һ�����Դͨ������
		void DeletePointLight(DWORD nIndex);
		//�õ����Դ�б�
		void GetPointLightList(list<MapObjEdit*>& out)	{ out = m_PointLightList;}
		//��յ��Դ�б�
		void ClearPointLightList();
		//ʰȡ���Դ
		void PickPointLight(UINT x, UINT y);
		//���õ�ǰѡ��ĵ��Դ
		void SetSelectedPointLight(MapObjEdit *pPointLight) { m_pSelectPointLight = pPointLight;}
		//�õ���ǰѡ��ĵ��Դ
		MapObjEdit* GetSelectedPointLight()	{ return m_pSelectPointLight;}
		//���������õ����Դ
		MapObjEdit* FindPointLight(DWORD nIndex);
		//���Ƶ��Դ��ʶ
		void SetDrawPointLightFlag(bool bVal)		{ m_bPickPointLight = bVal;}
		//���Դ��ʶλ��
		void SetPosForPointLight(Vector3& pos)		{ m_vCurPosPointLight = pos;}

		//--��ͼ�������
		//������һ����ͼ����
		void AddMapRect(MapObjEdit* pMapRect);
		//ɾ��һ����ͼ����
		void DeleteMapRect(tagMapArea * pMapRect);
		//ͨ������ɾ��һ����ͼ����
		void DeleteMapRect(DWORD dwIndex);
		//�õ����ͼ�����б�
		void GetMapRectList(list<MapObjEdit*>& out)	{	out = m_pMapRectList; }
		//��յ�ͼ�����б�
		void ClearMapRectList();
		//���õ�ǰѡ��ĵ�ͼ����
		void SetSelectedMapRect(MapObjEdit* pMapRect)	{ m_pSelectMapRect = pMapRect;}
		//�õ���ǰѡ��ĵ�ͼ����
		MapObjEdit* GetSelectedMapRect()	{return m_pSelectMapRect;}
		//���������õ���ͼ����
		MapObjEdit* FindMapRect(DWORD dwIndex);

		void SetCurPosForMapRect(Vector3 pos,bool bIsCurDrawMapRect)
		{
			m_bDrawMapRect = bIsCurDrawMapRect;
			if(m_bDrawMapRect)
				m_vCurPosForMapRect = pos;
		}
		
		void SetCurPosForPickMapRect(Vector3 pos)             {  m_vCurPickMapRect = pos;     }
		void GetCurPosForMapRect(Vector3 &out)                {  out = m_vCurPosForMapRect; }

		void SetPosForMapArea(Vector3& pos, int curPos)
		{ 
			if(curPos>=(int)m_vMapAreaPos.size())
				m_vMapAreaPos.push_back(pos);
			else
				m_vMapAreaPos[curPos] = pos; 
		}
		void SetPosForMapArea(Vector3& pos)					  
		{ 
			m_vMapAreaPos.push_back(pos); 
			m_nCurMapAreaNum++;
		}
		void GetPosForMapArea(Vector3&out, int curPos)		  { out = m_vMapAreaPos[curPos]; }
		void GetPosForMapArea(vector<Vector3>& vec)			  {	vec = m_vMapAreaPos; }
			
		void SetDrawPickFlagForMapRect(bool bIsPickMapRect)   {  m_bPickMapRect = bIsPickMapRect;  }
		//�õ�ʰȡ�ĵ�ͼ����
		void PickMapRect(UINT x, UINT y);
		//ʰȡ��ͼ�����ĳһ����
		void PickMapAreaPoint(UINT x, UINT y, bool& bHave);
		//����/��� �Ƿ���Ƶ�ͼ����AABBox
		void SetIsDrawMapRect(bool bVal) { m_bIsDrawMapRect = bVal; }
		bool GetIsDrawMapRect()const { return m_bIsDrawMapRect; }

		//��õ�ǰ������ͼ�������������ĸ���
		int GetCurMapAreaNum()								  { return m_nCurMapAreaNum; }
		void ClearMapAreaNum()								  { m_nCurMapAreaNum = 0; }
		//���㴥������AABBOX
		AABBox CalMapAreaAABBox();
		//���¼��㴥������AABBOX
		void ReCalMapAreaAABBox(tagMapArea* pArea);
		//
		void ClearMapAreaPos();
		//�õ�ʰȡ������ĳ�����ǵڼ�����
		int GetMapAreaNum()								{	return m_nMapAreaNum; }
		void ClearPickMapAreaPoint()					{	m_bPickMapAreaPoint = false; m_nMapAreaNum = 0; }


		//--Ѱ·���������
		//������һ��Ѱ·������
		void AddPathPoint(MapObjEdit* pEdit);
		//ɾ��һ��Ѱ·������
		void DeletePathPoint(tagMapPathPoint * pPathPoint);
		//ͨ������ɾ��һ��Ѱ·������
		void DeletePathPoint(DWORD dwIndex);
		//�õ���Ѱ·�������б�
		void GetPathPointList(list<MapObjEdit*>& out)		{ out = m_pPathPointList; }
		void GetPathPointVector(vector<tagMapPathPoint>& out)
		{
			vector<tagMapPathPoint> vecResult;
			tagMapPathPoint pathpoint;
			list<MapObjEdit*>::iterator it = m_pPathPointList.begin();
			for( ; it!=m_pPathPointList.end(); ++it )
			{
				if( (*it)->GetType() == MapObjEdit::PathPoint )
				{
					(*it)->GetData(pathpoint);
					vecResult.push_back(pathpoint);
				}
			}
			out = vecResult;
		}

		//���Ѱ·�������б�
		void ClearPathPointList();
		//���õ�ǰѡ���Ѱ·������
		void SetSelectedPathPoint(MapObjEdit* pPathPoint)	{ m_pSelectPathPoint = pPathPoint;}
		//�õ���ǰѡ���Ѱ·������
		MapObjEdit* GetSelectedPathPoint()					{ return m_pSelectPathPoint;}
		//���������õ�Ѱ·������
		MapObjEdit* FindPathPoint(DWORD dwIndex);
		//ʰȡѰ·������
		void PickPathPoint(UINT x, UINT y);
		//���û�����Ѱ·�������ʶ
		void SetDrawPathPoint(bool bVal)					{ m_bDrawPathPoint = bVal; }
		//���û�����Ѱ·������λ��
		void SetCurPosForPathPoint(Vector3 pos)				{ m_vCurPosForPathPoint = pos; }

		//--ˢ�ֵ����
		//���һ��ˢ�ֵ�
		void AddSpawnPoint(MapObjEdit* pEdit);
		//ɾ��һ��ˢ�ֵ�
		void DeleteSpawnPoint(tagMapSpawnPoint* pSpawnPoint);
		//ͨ������ɾ��һ��ˢ�ֵ�
		void DeleteSpawnPoint(DWORD dwIndex);
		//�õ�ˢ�ֵ��б�
		void GetSpawnPointList(list<MapObjEdit*>& out)		{ out = m_pSpawnPointList;}
		//���ˢ�ֵ��б�
		void ClearSpawnPointList();
		//���õ�ǰѡ���ˢ�ֵ�
		void SetSelectedSpawnPoint(MapObjEdit* pSpawnPoint)	{ m_pSelectSpawnPoint = pSpawnPoint; }
		//�õ���ǰѡ���ˢ�ֵ�
		MapObjEdit* GetSelectedSpawnPoint()					{ return m_pSelectSpawnPoint; }
		//ͨ�������õ�ˢ�ֵ�
		MapObjEdit* FindSpawnPoint(DWORD dwIndex);
		//ʰȡˢ�ֵ�
		void PickSpawnPoint(UINT x, UINT y);
		//���û�����ˢ�ֵ��ʶ
		void SetDrawSpawnPoint(bool bVal)					{ m_bDrawSpawnPoint = bVal; }
		//���û�����ˢ���λ��
		void SetCurPosForSpawnPoint(Vector3 pos)			{ m_vCurPosForSpawnPoint = pos; }

		//--�¼����������
		//���һ���¼�������
		void AddEventTrigger(MapObjEdit* pEdit);
		//ɾ��һ���¼�������
		void DeleteEventTrigger(tagMapEventTrigger* pEventTrigger);
		//ͨ������ɾ��һ���¼�������
		void DeleteEventTrigger(DWORD dwIndex);
		//�õ��¼��������б�
		void GetEventTriggerList(list<MapObjEdit*>& out)	{ out = m_pEventTriggerList; }
		//����¼�������
		void ClearEventTrigger();
		//ͨ�������õ��¼�������
		MapObjEdit* FindEventTrigger(DWORD dwIndex);

		//--NPC���ӵ���ͼ�������
		void AddNPCNavStartPoint(tagNPCNavMapBuildStartPoint* pNPCNav);
		void DeleteNPCNavStartPoint(tagNPCNavMapBuildStartPoint* pNPCNav);
		void DeleteNPCNavStartPoint(DWORD dwIndex);
		void ClearNPCNavStartPoint();
		void SetSelectedNPCNavStartPoint(tagNPCNavMapBuildStartPoint* pNPCNav)  { m_pSelectNPCNavStartPoint = pNPCNav;}
		tagNPCNavMapBuildStartPoint* GetSelectedNPCNavStartPoint()				{ return m_pSelectNPCNavStartPoint; }
		tagNPCNavMapBuildStartPoint* FindNPCNavStartPoint(DWORD dwIndex);
		void PickNPCNavStartPoint(UINT x, UINT y);
		void SetDrawNPCNavStartPoint(bool bVal)									{ m_bDrawNPCNavStartPoint = bVal; }
		void SetCurPosForNPCNavStartPoint(Vector3 pos)							{ m_vCurPosForNPCNav = pos; }

		//--���Ѱ·����ͼ�������
		void AddPlayerNavStartPoint(tagPlayerNavMapBuildStartPoint* pPlayerNav);
		void DeletePlayerNavStartPoint(tagPlayerNavMapBuildStartPoint* pPlayerNav);
		void DeletePlayerNavStartPoint(DWORD dwIndex);
		void ClearPlayerNavStartPoint();
		void SetSelectedPlayerNavStartPoint(tagPlayerNavMapBuildStartPoint* pPlayerNav)  { m_pSelectPlayerNavStartPoint = pPlayerNav;}
		tagPlayerNavMapBuildStartPoint* GetSelectedPlayerNavStartPoint()				{ return m_pSelectPlayerNavStartPoint; }
		tagPlayerNavMapBuildStartPoint* FindPlayerNavStartPoint(DWORD dwIndex);
		void PickPlayerNavStartPoint(UINT x, UINT y);
		void SetDrawPlayerNavStartPoint(bool bVal)									{ m_bDrawPlayerNavStartPoint = bVal; }
		void SetCurPosForPlayerNavStartPoint(Vector3 pos)							{ m_vCurPosForPlayerNav = pos; }



		//!	�ҵ���һ�����������way point�Ľ�ɫ
		MapObjEdit *FindRoleByWayPoint(const TCHAR* szWayName);

		//--
		DWORD GetGlobalID()const			{ return m_dwGlobalID; }
		void SetGlobalID(DWORD id)			{ m_dwGlobalID = id; }

	private:
		void GetTransList(IFS *pFS, LPCTSTR szFileName, vector<tstring>& fsmList);
		void SaveShowAreaData(LPCTSTR szFileName);
	protected:
		list<MapObjEdit*>			m_objList;
		MapObjEdit*					m_pSelectedObj;
		CreatureDataReader			m_createData;

		NullSceneGraph				*m_pSceneGraph;
		NavNullSceneGraph			*m_pNavSG;

		TerrainEditor				*m_pTerrain;
		CameraBase					*m_pCamera;

		//way point list
		list<tagMapWayPoint*>        m_WayPointList;
        //current selected way point
		tagMapWayPoint*              m_pSelectWayPoint;
		//deal with drawing waypoint
		bool                         m_bCurDrawWayPoint;
		Vector3                      m_vCurPosForWayPoint;
        //deal with picking waypoint
		bool                         m_bPickWayPoint;
		Vector3                      m_vCurPickWayPoint;
		//
		bool						 m_bDrawWayPoint;

		//Trigger list
		list<tagMapTrigger*>         m_TriggerList;
		//current selected trigger
		tagMapTrigger*               m_pSelectTrigger;
		//ʰȡ��������ĳ�����ǵڼ�����
		int							 m_nTriggerNum;
		bool						 m_bPickTriggerPoint;
		//����������������Ѿ��������ĸ���
		int							 m_nCurTriggerNum;
		//���������������ĵ�λ��
		vector<Vector3>				 m_vTriggerPos;
		//deal with drawing Trigger
		bool                         m_bCurDrawTrigger;
		//Vector3                      m_vStartPosForTrigger;
		Vector3                      m_vCurPosForTrigger;
		//deal with picking trigger
		bool                         m_bPickTrigger;
		Vector3                      m_vCurPickTrigger;
		//
		bool						 m_bDrawTrigger;

		//sound list
		list<tagMapSound*>           m_SoundList;
		//current selected sound
		tagMapSound*                 m_pSelectSound;
		//����Ч��ʶ
		bool						 m_bPickSound;
		//��Ч��ʶλ��
		Vector3						 m_vCurPosSound;

		//���Դ
		list<MapObjEdit*>			 m_PointLightList;
		//��ǰѡ��ĵ��Դ
		MapObjEdit*					 m_pSelectPointLight;
		//�����Դ��ʶ
		bool						 m_bPickPointLight;
		//���Դ��ʶλ��
		Vector3						 m_vCurPosPointLight;

		//��պ�SceneGraph���	
		SGSkyBox*					 m_pSGSkyBoxNode;	

		//--��ͼ����
		list<MapObjEdit*>			 m_pMapRectList;
		//��ǰѡ��ĵ�ͼ����
		MapObjEdit*					 m_pSelectMapRect;
		//ʰȡ��ͼ�����ĳ�����ǵڼ�����
		int							 m_nMapAreaNum;
		bool						 m_bPickMapAreaPoint;
		//��ͼ�������������Ѿ��������ĸ���
		int							 m_nCurMapAreaNum;
		//��ͼ������������ĵ�λ��
		vector<Vector3>				 m_vMapAreaPos;
		//������ͼ����
		bool						 m_bDrawMapRect;
		Vector3						 m_vStartPosForMapRect;
		Vector3						 m_vCurPosForMapRect;
		//����ʰȡ��ͼ����
		bool						 m_bPickMapRect;
		Vector3						 m_vCurPickMapRect;
		//�����Ƿ���Ƶ�ͼ����
		bool						 m_bIsDrawMapRect;

		//--Ѱ·������			by add xtian 2008-8-6
		list<MapObjEdit*>			m_pPathPointList;
		//��ǰѡ���Ѱ·������
		MapObjEdit*					m_pSelectPathPoint;
		//����Ѱ·Ѱ����
		bool						m_bDrawPathPoint;
		Vector3						m_vCurPosForPathPoint;

		//--ˢ�ֵ�				by add xtian 2008-8-11
		list<MapObjEdit*>			m_pSpawnPointList;
		//��ǰѡ���ˢ�ֵ�
		MapObjEdit*					m_pSelectSpawnPoint;
		//����ˢ�ֵ�
		bool						m_bDrawSpawnPoint;
		Vector3						m_vCurPosForSpawnPoint;

		//--�¼�������			by add xtian 2008-9-10
		list<MapObjEdit*>			m_pEventTriggerList;

		//ȫ��ΨһID
		DWORD						m_dwGlobalID;

		//--NPC���ӵ���ͼ�������	 by add xtian 2008-10-20
		list<tagNPCNavMapBuildStartPoint*>	m_NPCNavStartPointList;
		tagNPCNavMapBuildStartPoint*		m_pSelectNPCNavStartPoint;
		bool								m_bDrawNPCNavStartPoint;
		Vector3								m_vCurPosForNPCNav;

		//--���Ѱ·����ͼ�������
		list<tagPlayerNavMapBuildStartPoint*>	m_PlayerNavStartPointList;
		tagPlayerNavMapBuildStartPoint*			m_pSelectPlayerNavStartPoint;
		bool									m_bDrawPlayerNavStartPoint;
		Vector3									m_vCurPosForPlayerNav;

                		
	};
}//namespace WorldBase


