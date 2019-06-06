#pragma once
#include "sceneprojector.h"

namespace Cool3D
{
	class VertStream;
	/**	\class SGStaticProjector
		\brief ��̬projector,��texture��vert buffer�����ı�
	*/
	class Cool3D_API SGStaticProjector : public SceneProjector
	{
	public:
		SGStaticProjector(void);
		virtual ~SGStaticProjector(void);

		//--���л��ӿ�
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);
		//--scene node
		virtual bool IsInFrustum(const Frustum *pFrumstum) const;
		/**	û��ʵ��*/
		virtual void LoadRes(const TCHAR* szName);
		virtual UINT GetNumRenderNode() const;
		virtual RenderNode *GetRenderNode(UINT i) const;
		virtual const Matrix4& GetWorldMat() const;
		virtual void SetWorldMat(const Matrix4 *pMat);
		virtual IMaterial*	GetMtl(int mtlID);
		virtual void GetBox(AABBox& out) const;

		virtual void ForceUpdate(float deltaTime,SceneGraph *pSG);

		//--
		void Create(VertStream *pVertStream,const TCHAR *szTexPath);
	private:
		class Member;
		Member *m_p;

		DECL_RTTI_DYNC(SGStaticProjector);
	};
}//namespace Cool3D