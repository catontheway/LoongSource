#pragma once
#include "scenenode.h"

namespace Cool3D
{
	class CameraBase;

	/**	\class SGSunNode 
		\brief ̫��
		\see 
	*/
	class Cool3D_API SGSunNode : public SceneNode
	{
	public:
		SGSunNode(void);
		virtual ~SGSunNode(void);

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

		/** \����
		    \szTexBasePath ��ͼ·������ͼ�ļ����ڲ�ָ��Ϊ��sun, flare:sun_1,sun2...��
			 szExt �ļ���չ��
		*/
		bool Create( const TCHAR* szTexBasePath );
		/** \��ȡ̫��λ�õ���Ϣ
		*/
		Vector3 GetPos() const;
		POINT   GetScreenPos() const;
		float   GetNumPixels() const;
		/** \�Ƿ�����׶��
		*/
		bool IsInFrustum() const;
		/** \��/�رչ���
		*/
		void EnableFlare( const bool bEnable ) { m_bEnableFlare = bEnable; }
		bool IsEnableFlare() const { return m_bEnableFlare; }
	private:
		class Member;
		Member *m_p;
		bool m_bEnableFlare;    // �Ƿ�򿪹���

		DECL_RTTI(SGSunNode);
	};
}//namespace Cool3D