#pragma once

namespace WorldBase
{
	class NavOctreeCollider;
	/**	\class NavSGOctreeNode
		\brief �˲����ռ�ָ�Ľڵ�
	*/
	class NavOctreeNode
	{
	public:
		NavOctreeNode(void);
		virtual ~NavOctreeNode(void);

		void ReadFile(IFS *pFS,DWORD hFile);
		inline const AABBox& GetBox(){ return m_box;}

		bool AABBoxCollideBox(const AABBox& box,NavOctreeCollider* pCollider);
		bool AABBoxCollideBoxTopHighest(const AABBox& box,float& intersectY,NavOctreeCollider* pCollider);
		TResult RayCollideBox(const Ray& ray,NavOctreeCollider* pCollider);
		bool RayCollideBoxOnce(const Ray& ray,NavOctreeCollider* pCollider);

	protected:
		AABBox			m_box;
		vector<WORD>	m_content;
		WORD*			m_pContents;//�����Ż�debug��vector�����ٶ�
		NavOctreeNode	*m_pChildren[8];
	};
}//namespace WorldBase