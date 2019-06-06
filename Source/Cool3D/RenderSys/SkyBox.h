#pragma once
#include "rendernode.h"
#include "..\ResSys\VertStream.h"
#include "..\Math\CameraBase.h"

namespace F3D
{
	class ResTexture;

	/**	\class SkyBox 
		\brief �򵥵���պ���
		\remarks ��Ϊ��յĻ����������˳��Ҫ��,���Դ�RenderNode����,��û�з���SceneGraph��ȥ����
	*/
	class SkyBox : public RenderNode
	{
	public:
		SkyBox(void);
		~SkyBox(void);

		//--RenderNode
		void Draw(RenderSys *pSys,SceneNode *pSGNode,int nLod =0);	//��֧��Lod��
		ETransparentType GetTransparentType(const SceneNode *pSGNode) const	{return ETrans_None;}
		DWORD GetMtlTrait(SceneNode *pSGNode) const	{return 0;}
		const AABBox& GetAABBox() const;

		//--
		void Create(const char* szTopTex,const char* szWarpTex);
		void SetDrawSize(const Vector3& size);
		EVertType GetVertType()		{	return EVType_PNT; }

	private:
		ResTexture			*m_pTopTex;
		ResTexture			*m_pWarpTex;
		CameraBase			m_camera;
		
		Vert_PNT			m_vert[5*4];//top+warp��5��Quad,ÿ��Quad�ĸ�����
	};
}//namespace F3D