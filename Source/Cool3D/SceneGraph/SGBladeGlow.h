#pragma once
#include "scenenode.h"
#include "entitynode.h"
#include "..\RenderMtl\MBitmap.h"

namespace Cool3D
{

	/**	\class SGBladeGlow 
			\brief ��������Ч��
			\remarks ������ΪSGAvatarAttchment����EntityNode��childʹ��,
			���Ǹ��ڵ����Դ������ResStaticMesh
	*/
	class BladeGlow;
	class Cool3D_API SGBladeGlow :	public SceneNode
	{
	public:
		SGBladeGlow(void);
		virtual ~SGBladeGlow(void);

		//--SceneNode
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

		//--
		void Enable(bool bEnable){ m_bEnable = bEnable; }
		void SetColor(Color4ub color){ m_color = color; }
		void SetBlendOne(bool bBlendOne);

	private:
		//! ��������Ƭ
		void Init( EntityNode *pFatherNode );

		bool m_bEnable;		
		vector<BladeGlow*>		m_vecBladeGlow;
		MBitmap					m_mtl;
		Color4ub				m_color;
		bool					m_blendOne;

		DECL_RTTI_DYNC(SGBladeGlow);
	};

}//namespace Cool3D