#pragma once
#include "..\Cool3D.h"
#include "SceneNode.h"

namespace Cool3D
{
	class NodeAnimationCtrl;
	class ResSkeleton;
	/**	\class AvatarNode
		\brief ʹ�ù�������,֧�ֻ�װ��Entity
	*/
	class Cool3D_API AvatarNode :	public SceneNode
	{
	public:
		AvatarNode(void);
		virtual ~AvatarNode(void);

		//--���л��ӿ�
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);
		//--SceneNode
		virtual bool IsInFrustum(const Frustum *pFrumstum) const;
		/**	ֻ������Skeleton Resource */
		virtual void LoadRes(const TCHAR* szName);
		virtual UINT GetNumRenderNode() const;
		virtual RenderNode *GetRenderNode(UINT i) const;
		virtual const Matrix4& GetWorldMat() const;
		virtual void SetWorldMat(const Matrix4 *pMat);
		virtual void GetBox(AABBox& out) const;
		virtual void Update(float deltaTime,SceneGraph *pSG);
		virtual void ForceUpdate(float deltaTime,SceneGraph *pSG);
		virtual IMaterial*	GetMtl(int mtlID);
		virtual TResult RayCollide(const Ray& ray);

		//--
		/**	�ⲿ���Կ���AniCtrl�Ĳ���(���Բ�ʹ��const)*/
		NodeAnimationCtrl* GetSkeletonAniCtrl() 	{	return m_pAniCtrl;}
		ResSkeleton* GetResource() const {	return m_pRes;}

		/** ����Ƥ���ӽ���Ƿ��Ѵ��� */
		bool IsCreated();

		void Hide(bool bHide){ m_bHide=bHide; }
		bool IsHide()		 { return m_bHide;}

		/** \͸��Ч��
		*/
		void SetTransparent(float alpha);

		/** \��ɫ����
		*/
		void SetMulColor(const float r, const float g, const float b);

		/** Editor sport */
		bool ReloadMsgTable();

	private:
		void UpdateBoundingBox();

	private:
		ResSkeleton			*m_pRes;
		NodeAnimationCtrl	*m_pAniCtrl;
		Matrix4				m_toWorldMat;

		AABBox				m_box;//in world space

		//--���������Ż�
		bool			m_bHide;//�Ƿ�����

		DECL_RTTI_DYNC(AvatarNode);
	};
}//namespace Cool3D