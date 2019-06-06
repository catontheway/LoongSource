#pragma once
#include "..\WorldBase.h"

namespace WorldBase
{
	class NavCollider;
	class PathPointDistMap;
	/** \class NPCNavGraph
		\brief NPC·�㵼��ͼ
	*/
	class WORLD_BASE_API NPCNavGraph
	{
	public:
		struct tagPathPointRT 
		{
			Vector3 pos;				
			vector<int> nearPoints;
			vector<float> nearDists;
		};
	public:
		NPCNavGraph(void);
		~NPCNavGraph(void);

		//--
		void LoadFromFile(const TCHAR* szMapName,IFS* pFS);
		void Clone(NPCNavGraph* pTarget);

		/** ������Ѱ·
			\param searchRange ����������ķ�Χ
		*/
		bool FindPath(const Vector3& startPos,const Vector3& endPos,const Vector3& roleSize,float searchRange,NavCollider* pCollider,list<Vector3>& out);
		
		//--Editor support
		const vector<tagPathPointRT>& GetPathPoints();
		static bool Save(const TCHAR* szMapName,const vector<tagPathPointRT>& pathPoints,PathPointDistMap* pDistMap);
		
	private:
		/** Ѱ����ʼ�����㣬�Ҳ�������-1
		*/
		int FindStartPoint(const Vector3& startPos,const Vector3& roleSize,int tarPoint,float searchRange,NavCollider* pCollider);
		/** Ѱ�ҽ��������㣬�Ҳ�������-1
		*/
		int FindTargetPoint(const Vector3& targetPos,const Vector3& roleSize,float searchRange,NavCollider* pCollider);
		/** Ѱ������������֮������·��
		*/
		bool FindPointPath(int srcPoint,int tarPoint,list<Vector3>& out);
		/** Ѱ����һ��Ѱ���㣬�Ҳ�������-1
		*/
		int FindNextPoint(int curPoint,int tarPoint);
	private:
		class Member;
		Member* m_p;
	};
}//namespace WorldBase
