#pragma once
#include "resrenderable.h"
#define VFS_READ
#include "..\Public\TagNodeDefine.h"
#include "..\Math\Ray.h"
#include "..\Math\math.h"
#include "..\Animation\KeyFrame.h"

namespace Cool3D
{
	class AniMsgTable;
	class VertStream;
	/**	\class ResKeyFrame 
		\brief �ؼ�֡����
	*/
	class Cool3D_API ResKeyFrame :	public ResRenderable
	{
		struct TrackInfo{
			int nBegin;
			int nEnd;
			float fTime;
		};
	public:
		ResKeyFrame(const TCHAR* szName);
		virtual ~ResKeyFrame(void);

		//--ResBase
		virtual UINT CreateContent(DWORD param);

		//--ResRenderable
		virtual UINT GetNumRenderNode() const;
		virtual RenderNode *GetRenderNode(UINT i) const;
		virtual const AABBox	*GetAABBox() const;
		virtual int GetNumMtl() const;
		virtual void CreateMtlArray(vector<IMaterial*>&	mtls,const TCHAR* szLightMap,float mtlDiffuseFactor=1.0f) const;

		//--
		void EnableMipMap(bool bEnable)	{	m_bMipMap=bEnable;}
		const AniMsgTable* GetMsgTable() const;
		const vector<VertexKeyFrame*>& GetFrameArray() const;
		const vector<TagNodeFrame>& GetTagFrameArray() const;   

		UINT GetNumFrame() const;
		VertexKeyFrame* GetFrame(UINT index) const;
		float ResKeyFrame::GetFrmTime( int nFrame )	 const;

		float GetTrackTime( DWORD dwTrackName ) ;
		int	  GetTrackRange( DWORD dwTrackName, int& nBegin, int& nEnd ); 

		//- ȡ�ö����汾���Ƶ��б�
		int	  GetTrackName( vector<tstring>& trackNames );

		TResult RayCollide(const Ray& ray,const VertStream* pVS,const Matrix4* pObj2World);

		//--editor support
		void GetIndexBuffer(vector<WORD>& ib);

		void DrawOctree();

		//--lightmap
		int GetLightMapSize(void) const;
		bool SetLightMapSize(int size);

		//--��ȡ��������
		const tagMtlDesc& GetMtlDesc( const int i ) const;
	private:
		bool	m_bMipMap;//�Ƿ�ʹ��mip map

		class Member;
		Member	*m_p;

		DECL_RTTI(ResKeyFrame);
	};
}//namespace Cool3D