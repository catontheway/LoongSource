#pragma once
#include "rendernode.h"
#include "..\ResSys\VertStream.h"
#include "..\Device\IDevice.h"

namespace Cool3D
{
	/**	\class DynamicMesh 
		\brief �ɳ���̬������Mesh,��Ҫ����Decl,projector,��Ч��
	*/
	class Cool3D_API DynamicMesh :	public RenderNode
	{
	public:
		DynamicMesh(void);
		virtual ~DynamicMesh(void);

		//--RenderNode
		virtual bool SupportLod();
		virtual void Draw(RenderSys *pSys,SceneNode *pSGNode,int nLod=0,RenderItem* pItem=NULL,SceneGraph* pSG=NULL);
		virtual EMtlTrait GetMtlTrait(SceneNode *pSGNode) const;
		virtual DWORD GetMtlTrait1(SceneNode *pSGNode) const;
		virtual DWORD GetMtlTrait2(SceneNode *pSGNode) const;
		virtual const AABBox& GetAABBox() const;

		//--
		void Create(int mtlID,EDrawType drawType,EVertType vertType,UINT numVert);
		
		void EnablePointSprites(bool bSprites,float size)	{	m_bDrawAsSprites=bSprites; m_spriteSize=size;}
		VertStream *GetVertStream()				{	return &m_vertStream;}

		const EDrawType GetDrawType() const { return m_drawType; }  
		const int       GetNumDraw()  const { return m_numDraw; } 

		/**	�ⲿ������VertStream����Ե������,���¼���bounding box*/
		void UpdateAABBox()						{	m_vertStream.BuildAABBox(m_box); }

		void BeginBuild(int mtlID,EDrawType primitiveType);
		void AddPos(const Vector3& pos);
		void AddTexCoord(const Vector3& uvw);
		void AddDiffuse(const Color4f& diffuse);
		int BuildGetNumPos() const	{	return m_posBuffer.size();}
		Vector3& BuildGetPos(int i);
		void EndBuild();

		//�޶������ٸ�ͼԪ,������ֱ���С���ڲ�vert steam���ܴ�����ͼԪ�ĸ���
		//��Ҫ��������ϵͳ����Ч,������Ҫ��ʼʱ����̶���vert steam,Ԫ������ʱ������ƶ�������������
		void SetDrawLimit(int numPrimitive);
		void SetMtlID(int ID)	{	m_mtlID=ID;}
		void SetIdentityWorldMat(bool bIdentityWorldMat)	{	m_bIdentityWorldMat = bIdentityWorldMat;}
		const bool IsIdentityWorldMat() const { return m_bIdentityWorldMat; }

		/**�Ƿ�zƫ��
		*/
		void SetZBias( bool bZBias ) { m_bZBias = bZBias; }
	private:
		VertStream			m_vertStream;
		EDrawType			m_drawType;
		int					m_mtlID;
		AABBox				m_box;
		int					m_numPrimitive;
		int					m_numDraw;

		float				m_spriteSize;
		bool				m_bDrawAsSprites;

		bool				m_bIdentityWorldMat;
		bool                m_bZBias;

		//--build buffers
		vector<Vector3>		m_posBuffer;
		vector<Vector3>		m_uvwBuffer;
		vector<Color4f>		m_diffuseBuffer;

		DECL_RTTI_DYNC(DynamicMesh);
	};
}//namespace Cool3D