#pragma once
#include "scenenode.h"

namespace Cool3D
{
	/**	\class SGSkyNode 
		\brief ������յĻ���
		\remarsk ��Ϊ��սڵ���Renderʱ��Ҫ���⴦��������SG��Ҳʹ������ӿ�
		\see SceneGraph::SetSkyNode()
	*/
	class Cool3D_API SGSkyNode : public SceneNode
	{
	public:
		SGSkyNode(void);
		virtual ~SGSkyNode(void);

		//--��ЩSceneNode�����ﶼû����
		virtual bool IsInFrustum(const Frustum *pFrumstum) const;
		virtual void LoadRes(const TCHAR* szName);
		virtual const Matrix4& GetWorldMat() const;
		virtual void SetWorldMat(const Matrix4 *pMat);
		virtual void GetBox(AABBox& out) const;

	private:
		DECL_RTTI(SGSkyNode);
	};
}//namespace Cool3D