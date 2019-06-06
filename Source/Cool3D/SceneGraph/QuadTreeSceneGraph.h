#pragma once

#include "SceneGraph.h"

namespace F3D
{
	class EntityNode;
	class QuadTreeNode;
	class Frustum;
	class RenderSys;
	/**	\class QuadTreeSceneGraph
		\brief ʹ���Ĳ������пռ�ָ�͹����SceneGraph
	*/
	class QuadTreeSceneGraph : public SceneGraph
	{
	public:
		QuadTreeSceneGraph(void);
		virtual ~QuadTreeSceneGraph(void);

		virtual void LoadFromFile();

		virtual EntityNode *CreateEntityNode(const char* szResName);
		virtual void DestroyEntityNode(EntityNode *pNode);

		virtual void Update(float deltaTime);
		virtual void Render();

	private:
		void AddNodeToRenderSysR(QuadTreeNode* pNode,RenderSys *pSys,Frustum *pFrustum);
	private:
		QuadTreeNode *m_pRoot;
		list<EntityNode*> m_entityList;
	};
}//end of namespace F3D