#pragma once
#include "scenenode.h"
#include ".\sgsunnode.h"
#include"SGSkyDome.h"

namespace F3D
{
	/**	\class SGMoonNode 
		\brief ����Ч��
		\remarsk �������ȳ�ʼ��̫�����
		\see 
	*/
	class F3D_API SGMoonNode : public SceneNode
	{
	public:
		SGMoonNode(void);
		virtual ~SGMoonNode(void);

		//--��ЩSceneNode�����ﶼû����
		virtual bool IsInFrustum(const Frustum *pFrumstum) const;
		virtual void LoadRes(const TCHAR* szName);
		virtual const Matrix4& GetWorldMat() const;
		virtual void SetWorldMat(const Matrix4 *pMat);
		virtual void GetBox(AABBox& out) const;

		virtual UINT GetNumRenderNode() const;
		virtual RenderNode *GetRenderNode(UINT i) const;
		virtual IMaterial*	GetMtl(int mtlID);
		virtual void Update(float deltaTime,SceneGraph *pSG);
		virtual void ForceUpdate(float deltaTime,SceneGraph *pSG);

		//����
		void Create(Vector3 InitMoonPos, Vector3 InitMoonRotateAxis,float fSize,SGSunNode* pSunNode,SGSkyDome* pSkyDome);
		//--����������Ϣ
		void SetDateInfo(int nDate);
		//����������λ��
		void UpdateMoonPos(float deltaTime,Vector3 CamPos);
		//�õ��¹����䷽��
		Vector3 GetLightDir();


	private:
		class Member;
		Member *m_p;

		DECL_RTTI(SGMoonNode);
	};
}//namespace F3D