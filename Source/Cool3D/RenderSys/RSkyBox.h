#pragma once
#include "rendernode.h"

namespace Cool3D
{
	/**	\class   RSkyBox 
		\brief   �򵥵���պ���
		\remarks ��Ϊ��յĻ����������˳��Ҫ��,���Դ�RenderNode����,��û�з���SceneGraph��ȥ����
	*/
	class RSkyBox : public RenderNode
	{
	public:
		RSkyBox(void);
		~RSkyBox(void);

		//--RenderNode
		virtual void Draw(RenderSys *pSys,SceneNode *pSGNode,int nLod=0,RenderItem* pItem=NULL,SceneGraph* pSG=NULL);
		virtual EMtlTrait GetMtlTrait(SceneNode *pSGNode) const;
		virtual DWORD GetMtlTrait1(SceneNode *pSGNode) const;
		virtual DWORD GetMtlTrait2(SceneNode *pSGNode) const;
		virtual const AABBox& GetAABBox() const;
		virtual int GetMtlID() const;

		void CreateMeshs( const Vector3& drawSize );

		void EnableRenderShade( const bool bEnable );
		void SetShadeColor( const DWORD dwColor );
	private:
		class Member;
		Member *m_p;

		DECL_RTTI_DYNC(RSkyBox);
	};
}//namespace Cool3D