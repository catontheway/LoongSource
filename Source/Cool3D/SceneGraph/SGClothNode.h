#pragma once
#include "..\Cool3D.h"
#include "SceneNode.h"
#include "..\Public\VertexDefine.h"

namespace Cool3D
{
	class RSkin;
	class IMaterial;
	class AniCloth_SW;
	class IVertBuffer;
	class ResCloth;
	/**	\class SGClothNode
		\brief cloth��scene graph�е�node,һ������avatar node���ӽڵ�
		\remarks
		\par �������:��Updateʱ��Cloth��VB�������������֮
	*/
	class Cool3D_API SGClothNode : public SceneNode
	{
	public:
		SGClothNode(void);
		virtual ~SGClothNode(void);

		//--���л��ӿ�
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);
		//--SceneNode
		virtual bool IsInFrustum(const Frustum *pFrumstum) const;
		virtual UINT GetNumRenderNode() const;
		virtual RenderNode *GetRenderNode(UINT i) const;
		virtual const Matrix4& GetWorldMat() const;
		virtual void SetWorldMat(const Matrix4 *pMat)	{}
		virtual void GetBox(AABBox& out) const;
		/**ֻ����.FCLOTH�ļ�*/
		virtual void LoadRes(const TCHAR* szName);
		virtual UINT GetNumMtl() const;
		virtual IMaterial*	GetMtl(int mtlID);
		virtual void Update(float deltaTime,SceneGraph *pSG);
		virtual void ForceUpdate(float deltaTime,SceneGraph *pSG);
		virtual TResult RayCollide(const Ray& ray);
		/**	�滻ָ����Mtl,pMtl�����ڹ�this����*/
		void ReplaceMtl(int mtlID,IMaterial *pMtl);

		//--
		const ResCloth* GetResCloth() const	{	return m_pRes;}
		EVertType GetVertType() const;
		bool IsCreated()					{	return m_pAniCloth!=NULL && m_pVB!=NULL; }
		IVertBuffer *GetVB()				{	return m_pVB;  }
		int GetLod()						{	return m_nLod; }

		//! ������ʱ���ش˽ڵ�
		void Enable(bool bEnable)			{	m_bEnable=bEnable;}
		//! ���������͸��
		void BeginTransparent();
		void SetTransparent(float alpha);
		void EndTransparent();

		//--BoneMatrixArray
		const tagBoneMatrixArray* const GetBoneMatrixArray( const RSkin* const pSkin );

	protected:
		void UpdateSkinVB(int lod);
		void UpdateClothVB(float deltaTime);
		void CalcBoneMatrix(int lod);

	protected:
		bool				m_bEnable;
		ResCloth			*m_pRes;
		AniCloth_SW			*m_pAniCloth;
		/** ÿ��SGClothNodeӵ�������dynamic vb,����ʹ��RenderSys��vb pool,
		��Ϊ������Ⱦ���sub meshʱ�����ظ���vb pool�з��䲢�ҿ���vb*/
		IVertBuffer			*m_pVB;
		vector<IMaterial*>	m_mtls;
		int					m_nLod;

		vector<tagBoneMatrixArray>  m_vecBoneMatrixArray;

		DECL_RTTI_DYNC(SGClothNode);
	};
}//namespace Cool3D