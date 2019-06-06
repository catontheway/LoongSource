#pragma once

namespace WorldBase
{
	class NavBintreeCollider;
	/**	\class NavBintreeNode
		\brief �������ռ�ָ�Ľڵ�
	*/
	class NavBintreeNode
	{
		enum { EChildNum = 2 };

	public:
		NavBintreeNode(void);
		virtual ~NavBintreeNode(void);

		void ReadFile(IFS *pFS,DWORD hFile);
		inline const AABBox& GetBox(){ return m_box;}

		bool AABBoxCollideBox(const AABBox& box,NavBintreeCollider* pCollider);
		bool AABBoxCollideBoxTopHighest(const AABBox& box,float& intersectY,NavBintreeCollider* pCollider);
		TResult RayCollideBox(const Ray& ray,NavBintreeCollider* pCollider);
		bool RayCollideBoxOnce(const Ray& ray,NavBintreeCollider* pCollider);

	protected:
		AABBox			m_box;
		vector<WORD>	m_content;
		WORD*			m_pContents;//�����Ż�debug��vector�����ٶ�
		NavBintreeNode	*m_pChildren[EChildNum];
	};
}//namespace WorldBase