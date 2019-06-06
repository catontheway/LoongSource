#pragma once
#include "..\..\WorldDefine\MapAttDefine.h"
#include "..\NavMap\NavCollider_Move.h"
#include "..\NavMap\NavCollider_Swim.h"

namespace WorldBase
{
	class NavCollider;
	/** \class PlayerCangoMapGen
		\brief ��ҿ��������򵼺�ͼ������
	*/
	class PlayerCangoMapGen
	{
	public:
		PlayerCangoMapGen(void);
		~PlayerCangoMapGen(void);

		void BeginBuild(const list<tagPlayerNavMapBuildStartPoint*>& points,Heightmap* pHMap,BitMap* pManualCangoMap,BitMap* pRoadMap,NavCollider* pCollider);
		bool EndBuild(const TCHAR* szSavePath,const TCHAR* szMapName,IUICallBack* pUI);

		BitMap* GetCangoMap()		{ return &m_cangoMap;		}
		BitMap* GetRoadMap()		{ return &m_roadMap;	}

	private:
		void FloodFill(Vector3& startPos,IUICallBack* pUI);
		void FloodClear(int x,int z);
		bool IfCanGoTile(int x1,int z1,float h1,int x2,int z2,float& h2);
		bool IfCanGoTile(int x,int z);

	private:
		NavCollider*		m_pCollider;
		NavCollider_Move	m_moveCollider;
		NavCollider_Swim	m_swimCollider;

		Heightmap*			m_pHMap;
		BitMap*				m_pManualCangoMap;	//�ֹ��༭�Ŀ���������
		BitMap*				m_pOriginalRoadMap;	//ԭʼ���ֹ��༭������Ѱ·����

		list<tagPlayerNavMapBuildStartPoint*> m_buildStartPoints;

		BitMap				m_cangoMap;
		BitMap				m_roadMap;
		BitMap				m_visitMap;
		float*				m_pFHMap;
		Heightmap			m_tmpHMap;
	};
}//namespace WorldBase

