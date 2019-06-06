
#pragma once
#include "..\Math\GeomDefine.h"
#include "..\Device\IVertBuffer.h"
#include "..\Util\SharedPtr.h"
#include "..\Util\RTTIObj.h"
#include "..\Public\VertexDefine.h"
#include "..\ResSys\VertStream.h"
#include "..\RenderMtl\IMaterial.h"
#include ".\RenderSys.h"
#include ".\MtlRenderCache.h"

namespace Cool3D
{
	class SceneNode;
	class RenderSys;
	class IVertBuffer;
	class IStreamDefine;
	class IIndexBuffer;
	class IMaterial;
	
	typedef SharedPtr<IVertBuffer,SharePtrDelete<IVertBuffer> > SharedVB;
	typedef SharedPtr<IMaterial,SharePtrDelete<IMaterial> >	SharedMtl;

	/**	\class RenderNode
		\brief �ύ��RenderSys��һ����Ⱦ��λ����SceneGraph��RenderSys�Ľӿ�
		\remarks �����RenderNode�����ڴ���Render Device Resourceʱ���RenderSys�����,�Ӷ���ȡ��ͬ�Ĳ���
		\see RenderSys, SceneNode
		\todo ���е�GetMtlTrait()����Ҫ�Ķ�, ��ΪMtl�������scene node����, ����mtl����ָ�벻�پ�����������
	*/
	class Cool3D_API RenderNode : public RTTIObj
	{
	public:
		enum ERenderPri
		{
			EPri_Hightest=100,
			EPri_Normal=50,//default
			EPri_Low=25,
			EPri_Lowest=0
		};
	public:
		RenderNode(void);
		virtual ~RenderNode(void);

		virtual bool SupportLod();	//�Ƿ�֧��LOD,Ĭ��Ϊ��֧��
		virtual void Draw(RenderSys *pSys,SceneNode *pSGNode,int nLod=0,RenderItem* pItem=NULL,SceneGraph* pSG=NULL);	//֧��Lod��
		virtual EMtlTrait GetMtlTrait(SceneNode *pSGNode) const =0;
		virtual DWORD GetMtlTrait1(SceneNode *pSGNode) const =0;
		virtual DWORD GetMtlTrait2(SceneNode *pSGNode) const =0;
		virtual const AABBox& GetAABBox() const =0;
		virtual int GetMtlID() const{ return 0;}

	public:
		static IMaterial	*m_pSpecialMtl;//�����Mtl��ΪNULL,��ʹ��BeginMtl()����ָ����mtl
		static IVertShader	*m_pSpecialVS;
		static IPixelShader	*m_pSpecialPS;
		/**	\return number of passes	*/
		int BeginMtl(RenderSys *pSys,IMaterial *pMtl);
		void BeginMtlPass(int passIndex);
		void EndMtlPass(bool clearTex=false);
		void EndMtl();
		EMtlTrait GetMtlTrait(int mtlID,SceneNode* pSGNode) const;
		DWORD GetMtlTrait1(int mtlID,SceneNode *pSGNode) const;
		DWORD GetMtlTrait2(int mtlID,SceneNode *pSGNode) const;

		int GetPriority() const		{	return m_renderPri;}
		void SetPriority(int pri)	{	m_renderPri=pri;}
	private:
		IMaterial	*m_pCurMtl;//begin mtl��������mtl
		int			m_renderPri;//��Ⱦ���ȼ���,priority,����Խ��Խ����;ĿǰֻӰ�첻͸���������Ⱦ˳��
        
		DECL_RTTI(RenderNode);
	};
}//namespace Cool3D