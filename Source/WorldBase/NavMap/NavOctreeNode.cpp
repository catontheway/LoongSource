#include "stdafx.h"
#include "NavOctreeNode.h"
#include "NavOctreeCollider.h"

namespace WorldBase
{
	/** �˲����ռ仮��

   /\
   |
   Z-->x

   �����ĸ����ӣ�
	+----------+----------+
	|          |          |
	|	 2     |	 3    |
	|          |          |
	+----------+----------+
	|          |          |
	|	 0 	   |	 1    |
	|          |          |
	+----------+----------+

	�����ĸ����ӣ�
	+----------+----------+
	|          |          |
	|	 6     |	 7    |
	|          |          |
	+----------+----------+
	|          |          |
	|	 4 	   |	 5    |
	|          |          |
	+----------+----------+
	*/

	NavOctreeNode::NavOctreeNode( void )
	{
		memset(m_pChildren,0,sizeof(m_pChildren));
		m_pContents=NULL;
	}

	NavOctreeNode::~NavOctreeNode( void )
	{
		for(int i=0;i<8;i++)
			SAFE_DELETE(m_pChildren[i]);
	}

	void NavOctreeNode::ReadFile( IFS *pFS,DWORD hFile )
	{
		ASSERT(pFS!=NULL);

		FReadValue(pFS,hFile,m_box.max);
		FReadValue(pFS,hFile,m_box.min);
		FReadValVector(pFS,hFile,m_content);

		if(m_content.size()>0)
			m_pContents=&m_content[0];
		else
			m_pContents=NULL;

		bool bChild=false;
		for(int i=0;i<8;i++)
		{
			FReadValue(pFS,hFile,bChild);
			if(bChild)
			{
				NavOctreeNode *pNode=new NavOctreeNode;
				pNode->ReadFile(pFS,hFile);
				m_pChildren[i]=pNode;
			}
		}
	}

	bool NavOctreeNode::AABBoxCollideBoxTopHighest( const AABBox& box,float& intersectY,NavOctreeCollider* pCollider )
	{
		bool bCollide=false;

		int i;

		//--�����ѵĺ����б���
		if(m_content.size()>0)
		{
			for(i=0;i<(int)m_content.size();i++)
			{	
				const AABBox& collideBox=pCollider->GetCollideBox(m_pContents[i]);
				if(collideBox.AABBoxCollideBoxTopHighest(box,intersectY))
				{
					bCollide=true;
				}
			}
		}

		//--�ݹ��ӽ��
		for(i=0;i<8;i++)
		{
			if(m_pChildren[i]!=NULL
				&&m_pChildren[i]->GetBox().Intersects(box))
			{
				if(m_pChildren[i]->AABBoxCollideBoxTopHighest(box,intersectY,pCollider))
					bCollide=true;
			}
		}

		return bCollide;
	}

	bool NavOctreeNode::AABBoxCollideBox( const AABBox& box,NavOctreeCollider* pCollider )
	{
		int i;

		//--�����ѵĺ����б���
		if(m_content.size()>0)
		{
			for(i=0;i<(int)m_content.size();i++)
			{	
				const AABBox& collideBox=pCollider->GetCollideBox(m_pContents[i]);
				if(collideBox.Intersects(box))
					return true;
			}
		}

		//--�ݹ��ӽ��
		for(i=0;i<8;i++)
		{
			if(m_pChildren[i]!=NULL
				&&m_pChildren[i]->GetBox().Intersects(box))
			{
				if(m_pChildren[i]->AABBoxCollideBox(box,pCollider))
					return true;
			}
		}

		return false;
	}

	TResult NavOctreeNode::RayCollideBox( const Ray& ray, NavOctreeCollider* pCollider )
	{
		TResult ret(false,FLOAT_MAX);

		//--�������ѵĴ������
		TResult tr=ray.Intersect(m_box);
		if(!tr.first)
			return ret;

		//--�����ѵĺ����б���
		int i;
		for(i=0;i<(int)m_content.size();i++)
		{	
			const AABBox& box=pCollider->GetCollideBox(m_pContents[i]);
			tr=ray.Intersect(box);
			if(tr.first)
			{
				ret.first=true;
				if(tr.second<ret.second)
					ret.second=tr.second;
			}
		}

		//--�ݹ��ӽ��
		for(i=0;i<8;i++)
		{
			if(m_pChildren[i]!=NULL)
			{
				tr=m_pChildren[i]->RayCollideBox(ray,pCollider);
				if(tr.first)
				{
					ret.first=true;
					if(tr.second<ret.second)
						ret.second=tr.second;
				}			
			}
		}

		return ret;
	}

	bool NavOctreeNode::RayCollideBoxOnce(const Ray& ray,NavOctreeCollider* pCollider)
	{
		//--�������ѵĴ������
		TResult tr=ray.Intersect(m_box);
		if(!tr.first)
			return false;

		//--�����ѵĺ����б���
		int i;
		for(i=0;i<(int)m_content.size();i++)
		{	
			const AABBox& box=pCollider->GetCollideBox(m_pContents[i]);
			tr=ray.Intersect(box);
			if(tr.first)
				return true;
		}

		//--�ݹ��ӽ��
		for(i=0;i<8;i++)
		{
			if(m_pChildren[i]!=NULL
				&&m_pChildren[i]->RayCollideBoxOnce(ray,pCollider))
				return true;
		}

		return false;
	}
}//namespace WorldBase