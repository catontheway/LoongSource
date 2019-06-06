#pragma once
#include "NavBaseCollider.h"

namespace WorldBase
{
	class NavOctreeNode;
	/**	\class NavOctreeCollider
		\brief �˲�������ײ�����
	*/
	class NavOctreeCollider : public NavBaseCollider
	{
	public:
		NavOctreeCollider();
		virtual ~NavOctreeCollider();

		virtual void ReadFile(IFS *pFS,DWORD hFile);
		virtual const AABBox& GetBigBox();

		virtual bool AABBoxCollideBox(const AABBox& box);
		virtual bool AABBoxCollideBoxTopHighest(const AABBox& box,float& intersectY);
		virtual TResult RayCollideBox(const Ray& ray);
		virtual bool RayCollideBoxOnce(const Ray& ray);

	private:
		NavOctreeNode*		m_pRoot;
	};
}//namespace WorldBase