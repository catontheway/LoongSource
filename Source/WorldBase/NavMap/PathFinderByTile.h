#pragma once
#include "AStarPathFinder.h"

namespace WorldBase
{
	class NavCollider;
	/** \class PathFinderByTile
		\brief ���ڸ��ӵ�Ѱ·��
	*/
	class PathFinderByTile : public IAStarPathFinderCallBack
	{
	public:
		enum EResult
		{
			ER_Failed,		//Ѱ·ʧ��
			ER_Arrive,		//�����յ�
			ER_GetNearPos,	//���︽��λ��
		};

	public:
		PathFinderByTile(void);
		virtual ~PathFinderByTile(void);

		//--
		void SetCangoMap(BitMap* pCangoMap)		{ m_pCangoMap=pCangoMap;}	
		void SetCollider(NavCollider* pCollider){ m_pCollider=pCollider;}

		/** Ѱ·
		*/
		EResult FindPath(const Vector3& startPos,const Point& destTile,const Vector3& roleSize,bool bHalfPath,list<AStarPathFinder::tagPathPoint>& path);
		/** ƽ��·��
		*/
		void SmoothPath(const Vector3& startPos,const Vector3& roleSize,bool bOnLand,list<AStarPathFinder::tagPathPoint>& tilePath,list<Vector3>& out);

		//--IAStarPathFinderCallBack
		virtual int GetWay(int sx,int sz,int sa,EDir dir,int dx,int dz,int* pda);

	private:
		BitMap*			m_pCangoMap;
		NavCollider*	m_pCollider;
	};
}//namespace WorldBase

