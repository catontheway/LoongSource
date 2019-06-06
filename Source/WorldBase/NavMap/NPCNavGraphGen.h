#pragma once
#include "..\NavMap\PathPointDistMap.h"
#include "..\NavMap\NPCNavGraph.h"
#include "..\..\WorldDefine\MapAttDefine.h"
#include "..\NavMap\NavCollider_NPCMove.h"

namespace WorldBase
{
	/** \class NPCNavGraphGen
		\brief NPC·�㵼��ͼ������
	*/
	class NPCNavGraphGen
	{
	public:
		NPCNavGraphGen(void);
		~NPCNavGraphGen(void);

		void BeginBuild(vector<tagMapPathPoint>& pathPoints,NavCollider* pCollider,float fPassHeight);
		bool EndBuild(const TCHAR* szSavePath,const TCHAR* szMapName);

	private:
		/** �������ڵ�������룬�����ڷ���-1
		*/
		float CalcNearPathPointDist(int curPoint,int nearPoint,NavCollider* pCollider);
		/** ����һ�������㵽�������е��������̾���
		*/
		void CalcPointShortestDist(int srcPoint,vector<NPCNavGraph::tagPathPointRT>& points,PathPointDistMap& distMap);

	private:
		vector<tagMapPathPoint>     m_pathPoints;
		NavCollider*				m_pCollider;
		NavCollider_NPCMove			m_moveCollider;
		float                       m_fPassHeight;
	};

}//namespace WorldBase
