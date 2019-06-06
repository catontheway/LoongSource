#pragma once
#include "..\Cool3D.h"
#include "scenenode.h"
#include "..\Math\Math.h"
#include "..\Math\GeomDefine.h"
#include "..\Util\RTTIObj.h"

namespace Cool3D
{
	class DynamicMesh;
	class ResRenderable;
	class AnimationCtrl;
	class VertStream;
	/**	\class EntityNode
		\brief SceneGraph��Actor�Ȼ��ʵ��
		\par
		������Ⱦ���ݿ�����Actor����ResRenderable
		\todo �û������AniCtrl���֧��???
		
	*/
	class Cool3D_API EntityNode : public SceneNode
	{
	public:
		EntityNode(void);
		virtual ~EntityNode(void);

		//--���л��ӿ�
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);
		//-- SceneNode
		virtual bool IsInFrustum(const Frustum *pFrumstum) const;


		void EnableMipMap(bool bEnable)	{	m_bMipMap=bEnable;}
		/**	(�Ӵ���)�����µ���Դ
			\remarks �ȴ���֪���ļ���չ��ƥ��,����ҵ����ResMgr�л��,���򽫴�UserResMgr�л��
		*/
		virtual void LoadRes(const TCHAR* szName);
		virtual UINT GetNumRenderNode() const;
		virtual RenderNode *GetRenderNode(UINT i) const;
		virtual const Matrix4& GetWorldMat() const;
		virtual void SetWorldMat(const Matrix4 *pMat);
		virtual void GetBox(AABBox& out) const;
		virtual void Update(float deltaTime,SceneGraph *pSG);
		virtual void ForceUpdate(float deltaTime,SceneGraph *pSG);
		virtual UINT GetNumMtl() const;
		virtual IMaterial*	GetMtl(int mtlID);
		virtual TResult RayCollide(const Ray& ray);

		/**	�滻ָ����Mtl,pMtl�����ڹ�this����*/
		void ReplaceMtl(int mtlID,IMaterial *pMtl);
		
		//--
		virtual ResRenderable* GetResObj() const;
		virtual AnimationCtrl* GetAniCtrl() const;
		const AABBox& GetAABBox() const;

		TResult RayCollideForDynaNode(const Ray& ray);

		//! ������ʱ���ش˽ڵ�
		void Hide(bool bHide){ m_bHide=bHide; }
		bool IsHide()		 { return m_bHide;}

		//! ���������͸��
		void SetTransparent(float alpha);
		float GetTransparent() const { return m_fTransparent; }
		//! ���������ɫ
		void SetMulColor(const float r, const float g, const float b);
		//! ������ӰЧ��
		void SetShadowEffect(bool bInShadow);
		//! ���������������ǿϵ��
		void SetMtlDiffuseFactor(float factor);

		//--editor support
		void GetVertexBuffer(VertStream* vb);
		void GetIndexBuffer(vector<WORD>& ib);

		//--������������nav��ֻ������fscn
		void GetPosBuffer(VertStream* vb);

		//-- ��Щ����ģ�Ͳ�������ײ���
		static void LoadCDIgnoreList(const TCHAR* szFileName);

		//--
		void BuildDecalMesh( const AABBox& box, DynamicMesh *pMesh, const Vector3* pNormal );

		virtual bool IsLoaded();

		void UpdateBoundingBox();
		const AABBox& GetLocalBox() const;
	private:
		static set<tstring>	m_cdIgnoreList;

	private:
		bool m_bHide;
		float m_fTransparent;

		bool m_bMipMap;//�Ƿ�ʹ��mip map(���糡���еĶ�������ʹ��mip map,��������ϣ��ʹ��mip map)

		class Member;
		Member *m_p;

		DECL_RTTI_DYNC(EntityNode);
	};
}//namespace Cool3D