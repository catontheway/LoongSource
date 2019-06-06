#pragma once
#include "ResRenderable.h"
#include "..\Math\Ray.h"

namespace Cool3D
{
	class RenderNode;
	class TagNodeFrame;
	class VertStream;
	/**	\class ResStaticMesh 
		\brief ��̬Mesh��Դ
		\remarks ���𱣴�������ڴ���Դ,RenderDevice��Դ;�����RenderSys�Ĳ�ͬ������RenderDevice��Դ
	*/
	class Cool3D_API ResStaticMesh : public ResRenderable
	{
	public:
		ResStaticMesh(const TCHAR* szName);
		virtual ~ResStaticMesh(void);

		//--ResBase
		virtual UINT CreateContent(DWORD param);
		//--ResRenderable
		virtual UINT GetNumRenderNode() const;
		virtual RenderNode *GetRenderNode(UINT i) const;
		virtual const AABBox	*GetAABBox() const;
		virtual int GetNumMtl() const;
		virtual void CreateMtlArray(vector<IMaterial*>&	mtls,const TCHAR* szLightMap,float mtlDiffuseFactor=1.0f) const;

		const TagNodeFrame& GetTagNodes() const;
		void EnableMipMap(bool bEnable)	{	m_bMipMap=bEnable;}

		void SaveTagNodes();
		TResult RayCollide(const Ray& ray,const Matrix4* pObj2World);

		//--editor support
		VertStream* GetVertStream();
		void GetIndexBuffer(vector<WORD>& ib);

		//--lightmap
		int GetLightMapSize(void) const;
		bool SetLightMapSize(int size);

		void DrawOctree();

		//--��ȡ��������
		const tagMtlDesc& GetMtlDesc( const int i ) const;
	private:
		bool	m_bMipMap;//�Ƿ�ʹ��mip map
		class Member;
		Member	*m_p;

		DECL_RTTI(ResStaticMesh);
	};
}//namespace Cool3D