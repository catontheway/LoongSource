#pragma once
#include "..\WorldBase.h"
#include "PathFinderByMap.h"
#include "PathFinderByPatch.h"
#include "PathFinderByTile.h"

namespace WorldBase
{
	/** \class PlayerNavMap
		\brief ����Զ�Ѱ·����ͼ
	*/
	class WORLD_BASE_API PlayerNavMap
	{
	public:
		enum EResult		//Ѱ·����ֵ
		{
			ER_Failed,		//ʧ��
			ER_Arrive,		//�����յ�
			ER_GetNearPos,	//���︽����
			ER_EnterRoad,	//�����·����
		};
	public:
		PlayerNavMap(void);
		~PlayerNavMap(void);

		/** ��������
		*/
		void LoadFromFile(const TCHAR* szMapName,IFS* pMapFS,IFS* pSysFS);

		/** �ж��Ƿ��ͨ��
		*/
		bool IfCanGo(int x,int z,bool bRoadMap);
		bool IfCanGo(float x,float z,bool bRoadMap);
		/** ����ͼѰ·
		*/
		bool FindPathByMap(const TCHAR* szSrcMap,const TCHAR* szDestMap,Vector3& switchPos);
		/** ���ؿ�Ѱ·
		*/
		EResult FindPathByPatch(const Vector3& startPos,const Vector3& endPos,bool bRoadMap,list<Point>& path);
		/** ������Ѱ·
		*/
		EResult FindPathByTile(const Vector3& startPos,const Point& destTile,const Vector3& roleSize,bool bRoadMap,bool bRoadCheck,bool bHalfPath,bool bOnLand,NavCollider* pCollider,list<Vector3>& path);
		EResult FindPathByTile(const Vector3& startPos,const Vector3& destPos,const Vector3& roleSize,bool bRoadMap,bool bRoadCheck,bool bHalfPath,bool bOnLand,NavCollider* pCollider,list<Vector3>& path);

	private:
		PathFinderByMap		m_pathFinderByMap;
		PathFinderByPatch	m_pathFinderByPatch_Road;
		PathFinderByPatch	m_pathFinderByPatch_Auto;
		PathFinderByTile	m_pathFinderByTile;
		BitMap				m_cangoMap_Road;
		BitMap				m_cangoMap_Auto;	
	};
}//namespace WorldBase
