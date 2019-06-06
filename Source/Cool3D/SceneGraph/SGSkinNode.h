#pragma once
#include "..\Cool3D.h"
#include "scenenode.h"
#include "..\Public\VertexDefine.h"

namespace Cool3D
{
	class RSkin;
	class IMaterial;
	class AniSkin_SW;
	class IVertBuffer;
	class ResSkin;
	struct tagMtlDesc;
	/**	\class SGSkinNode
		\brief skin��scene graph�е�node,һ������avatar node���ӽڵ�
		\remarks
		\par �������:��Updateʱ��Skin��VB�������������֮
	*/
	class Cool3D_API SGSkinNode : public SceneNode
	{
	public:
		SGSkinNode(void);
		virtual ~SGSkinNode(void);

		//--���л��ӿ�
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);
		//--SceneNode
		virtual bool IsInFrustum(const Frustum *pFrumstum) const;
		virtual UINT GetNumRenderNode() const;
		virtual RenderNode *GetRenderNode(UINT i) const;
		virtual const Matrix4& GetWorldMat() const;
		virtual void SetWorldMat(const Matrix4 *pMat);
		virtual void GetBox(AABBox& out) const;
		/**ֻ����.FSKIN�ļ�*/
		virtual void LoadRes(const TCHAR* szName);
		virtual UINT GetNumMtl() const;
		virtual IMaterial*	GetMtl(int mtlID);
		virtual void Update(float deltaTime,SceneGraph *pSG);
		virtual void ForceUpdate(float deltaTime,SceneGraph *pSG);
		virtual TResult RayCollide(const Ray& ray);
		/**	�滻ָ����Mtl,pMtl�����ڹ�this����*/
		void ReplaceMtl(int mtlID,IMaterial *pMtl);
		
		//--
		const ResSkin* GetResSkin() const	{	return m_pRes;}
		EVertType GetVertType() const;
		bool IsCreated()					{	return m_pAniSkin!=NULL && m_pVB!=NULL; }
		IVertBuffer *GetVB()				{	return m_pVB;  }
		int GetLod()						{	return m_nLod; }

		//! ������ʱ���ش˽ڵ�
		void Enable(bool bEnable)			{	m_bEnable=bEnable;}
		//! ���������͸��
		void SetTransparent(float alpha);
		float GetTransparent() const { return m_fTransparent; }
		//! ���������ɫ
		void SetMulColor(const float r, const float g, const float b);

		//--BoneMatrixArray
		const tagBoneMatrixArray* const GetBoneMatrixArray( const RSkin* const pSkin );
		//--��ȡ��������
		const tagMtlDesc& GetMtlDesc( const int i ) const;
	protected:
		void UpdateVB(int lod);
		void CalcBoneMatrix(int lod);
	protected:
		bool				m_bEnable;
		ResSkin				*m_pRes;
		AniSkin_SW			*m_pAniSkin;
		/** ÿ��SGSkinNodeӵ�������dynamic vb,����ʹ��RenderSys��vb pool,
		��Ϊ������Ⱦ���sub meshʱ�����ظ���vb pool�з��䲢�ҿ���vb*/
		IVertBuffer			*m_pVB;
		vector<IMaterial*>	m_mtls;
		int					m_nLod;
		float				m_fTransparent;

		vector<tagBoneMatrixArray>  m_vecBoneMatrixArray;

		DECL_RTTI_DYNC(SGSkinNode);
	};
}//namespace Cool3D