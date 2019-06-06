#pragma once
#include "..\Math\GeomDefine.h"

namespace F3D
{
	class EntityNode;
	class EffectNode;
	class Frustum;
	/**	\class QuadTreeNode 
		\brief �������νṹ�е�һ���ڵ�
	*/
	class QuadTreeNode
	{
	public:
		QuadTreeNode(QuadTreeNode *pFather);
		virtual ~QuadTreeNode(void);

		const vector<EffectNode*>& GetEffectNode() const
		{ return m_effectArray; }
		QuadTreeNode *GetChild(UINT i) const;

		bool IsInFrustum(const Frustum *pFrumstum) const;
		
	protected:
		QuadTreeNode		*m_pFather;
		QuadTreeNode		*m_pChildren[4];
		vector<EffectNode*> m_effectArray;//Ӱ��˽ڵ���ȾЧ���Ľڵ�

		AABBox				m_nodeBox;//��Node��Bounding box
	};
}//namespace F3D