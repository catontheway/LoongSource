#pragma once
#include "..\..\Cool3D\scenegraph\scenenode.h"


namespace Cool3D
{
	/**	\class SGRoleGlow
		\brief ��ɫ����Ч��
		\remarks ������ΪSGAvatarNode��childʹ��
	*/
	class Cool3D_API SGRoleGlow : public SceneNode
	{
	public:
		enum EStyle//���η��
		{
			ES_Round,//����Billboard
			ES_Bucket,//Ͱ��Billboard
		};

		struct tagParam//����
		{
			tstring szHeadBone;//��ʼ��������
			tstring szTailBone;//������������
			int uDim;//��ͼ����֡��
			int vDim;//��ͼ����֡��
			float fps;//��ͼ�����ٶ�
			float width;//Ƭ���(�뾶)
			float heightExtend;//Ƭ�߶��ӳ�
		};

	public:
		SGRoleGlow(void);
		virtual ~SGRoleGlow(void);

		//--SceneNode
		virtual bool IsInFrustum(const Frustum *pFrumstum) const;
		virtual void LoadRes(const TCHAR *szName);
		virtual UINT GetNumRenderNode() const;
		virtual RenderNode *GetRenderNode(UINT i) const;
		virtual const Matrix4& GetWorldMat() const;
		virtual void SetWorldMat(const Matrix4 *pMat);
		virtual void GetBox(AABBox& out) const;
		virtual IMaterial*	GetMtl(int mtlID);
		virtual void Update(float deltaTime,SceneGraph *pSG);
		virtual void ForceUpdate(float deltaTime,SceneGraph *pSG);

		//--
		void Init(EStyle style,tagParam& param);
		void Enable(bool bEnable) {m_bEnable=bEnable;}
		void SetColor(Color4ub& color);
		void SetScale(float scale);
		void SetBlendOne(bool bBlendOne);

	private:
		bool m_bEnable;		

		class Member;
		Member *m_p;

		DECL_RTTI_DYNC(SGRoleGlow);
	};

}//namespace Cool3D