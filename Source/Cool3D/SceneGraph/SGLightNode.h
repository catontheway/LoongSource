#pragma once
#include "scenenode.h"
#include "..\Device\Light.h"

namespace Cool3D
{
	/**	\class SGLightNode 
		\brief ���Դ,������Ϊ�����ڵ㴦��
		\par ��̬�Ͷ�̬��Դ��������ദ��
		\par �ƹ�Node������Ϊ����SceneNode��child,�游�ڵ��ƶ�
	*/
	class Cool3D_API SGLightNode :	public SceneNode
	{
	public:
		SGLightNode(void);
		virtual ~SGLightNode(void);

		//--���л��ӿ�
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);
        
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

		void SetLight(const tagLight *pLgt);
		void SetDynamic(bool bDynamic);
		bool IsDynamic();
		tagLight* GetLight();
		float GetNoBlockRange() const;
		void SetNoBlockRange(float range);
		void SetLightModulus(float modulus);
		float GetLightModulus() const;
		
		//! �������ĳ������(������)��Ӱ��ϵ��,ϵ��Խ��Ӱ��Խ��,С�ڵ���0��ûӰ��
		float LightCost(const AABBox& box);
	private:
		class Member;
		Member *m_p;

		DECL_RTTI_DYNC(SGLightNode);
	};
}//namespace Cool3D