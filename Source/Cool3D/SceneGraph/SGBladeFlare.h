#pragma once
#include "scenenode.h"

namespace Cool3D
{
	/**	\class SGBladeFlare 
		\brief ����
		\remarks ������ΪSGAvatarAttchment����EntityNode��childʹ��,
		���ǵ���ĸ��ڵ����Դ������ResStaticMesh
	*/
	class Cool3D_API SGBladeFlare : public SceneNode
	{
	public:
		SGBladeFlare(void);
		virtual ~SGBladeFlare(void);

		//--���л��ӿ�
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);

		//--scene node
		virtual bool IsInFrustum(const Frustum *pFrumstum) const;
		virtual void LoadRes(const TCHAR* szName);
		virtual UINT GetNumRenderNode() const;
		virtual RenderNode *GetRenderNode(UINT i) const;
		virtual const Matrix4& GetWorldMat() const;
		virtual void SetWorldMat(const Matrix4 *pMat);
		virtual void GetBox(AABBox& out) const;
		virtual IMaterial*	GetMtl(int mtlID);
		virtual void Update(float deltaTime,SceneGraph *pSG);
		virtual void ForceUpdate(float deltaTime,SceneGraph *pSG);

		bool Init(float trackTimeLen);
		void Enable(bool bEnable);
		void SetColor(Color4ub color);

	private:
		bool m_bEnable;

		class Member;
		Member *m_p;

		DECL_RTTI_DYNC(SGBladeFlare);
	};
}//namespace Cool3D