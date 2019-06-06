#pragma once
#include "NPCNavGraph.h"

namespace WorldBase
{
	class PathPointOctreeNode;
	/** \class PathPointOctree
		\brief ����Ѱ·������İ˲���
	*/
	class PathPointOctree
	{
	public:
		PathPointOctree(void);
		~PathPointOctree(void);

		bool LoadFromFile(const TCHAR* szMapName,IFS* pFS);
		void Clone(PathPointOctree* pTarget);

		void QueryPathPoints(const AABBox& box,const vector<NPCNavGraph::tagPathPointRT>& points,list<pair<int,float> >& out);
	private:
		PathPointOctreeNode* m_pRoot;
	};
};//namespace WorldBase