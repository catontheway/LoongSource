#pragma once
#include "..\WorldBase.h"
#include "..\NavMap\NavNullSceneGraph.h"
#include "..\..\WorldDefine\MapAttDefine.h"

namespace WorldBase
{
	class NavCollider;
	/**	\class NavMapEditor
		\brief ����ͼ�༭��������
	*/
	class WORLD_BASE_API NavMapEditor
	{
	public:
		struct tagBuildOptions
		{
			bool bColliderData;//��ײ����
			bool bNPCNavMap;//NPC���ӵ���ͼ
			bool bNPCNavGraph;//NPC·�㵼��ͼ
			bool bPlayerNavMap;//����Զ�Ѱ·����ͼ
			float fNPCPassHeight;//NPCͨ���߶�
			float fNPCNavGraphPassHeight; //NPC·�㵼��ͼ��ͨ���߶�
		};
	public:
		NavMapEditor();
		virtual ~NavMapEditor();

		void BuildSetHeightmap(Heightmap *pHMap);
		void BuildSetManualCangoMap(BitMap* pManualCangoMap);
		void BuildSetPlayerRoadMap(BitMap* pPlayerRoadMap);
		void BuildSetPathPoints(const vector<tagMapPathPoint>& points);
		void BuildSetNPCNavMapBuildStartPoints(const list<tagNPCNavMapBuildStartPoint*>& points);
		void BuildSetPlayerNavMapBuildStartPoints(const list<tagPlayerNavMapBuildStartPoint*>& points);
		void BuildSetWaterArea(BitMap* pBitmap);
		void BuildSetWaterHeight(Heightmap* pHMap);
		void BuildSetGroundType(Heightmap* pGroundType);
		void BuildSetDoors(const vector<tagMapDoor>& doors);
		bool Build(const TCHAR* szSavePath,const TCHAR* szMapName,IUICallBack* pUI);

		bool Save(const TCHAR *szMapName,BitMap *pManualCangoMap,BitMap *pPlayerRoadMap);
		bool Load(const TCHAR *szMapName,BitMap *pManualCangoMap,BitMap *pPlayerRoadMap);

		bool LoadNPCNavMap(const TCHAR *szMapName, BitMap *pNPCNavBMap, Heightmap* pNPCNavHMap);
		bool LoadPlayerNavMap(const TCHAR *szMapName, BitMap *pPlayerBMap, Heightmap* pPlayerHMap);


		NavNullSceneGraph* GetSceneGraph(){ return &m_sg;}
		tagBuildOptions& GetBuildOptions(){ return m_buildOptions;}
	
	private:
		bool SaveWaterData(const TCHAR* szSavePath,const TCHAR* szMapName);

	private:
		Heightmap				*m_pTerrainHMap;		//���θ߶�ͼ
		BitMap					*m_pManualCangoMap;		//�ֹ��༭�Ĳ�����������
		BitMap					*m_pPlayerRoadMap;		//�ֹ��༭�����Ѱ·��·����
		vector<tagMapPathPoint> m_pathPoints;			//�ⲿ��������Ѱ·������
		BitMap					*m_pWaterArea;			//ˮ������
		Heightmap				*m_pWaterHeight;		//ˮ��߶�
		Heightmap				*m_pGroundType;			//�ر�����

		list<tagNPCNavMapBuildStartPoint*>		m_npcNavMapBuildStartPoints;
		list<tagPlayerNavMapBuildStartPoint*>	m_playerNavMapBuildStartPoints;

		vector<tagMapDoor>		m_doors;

		NavNullSceneGraph		m_sg;					//������ײ���ļ��׳���������
		tagBuildOptions			m_buildOptions;			//Buildѡ��

	};
}//namespace WorldBase