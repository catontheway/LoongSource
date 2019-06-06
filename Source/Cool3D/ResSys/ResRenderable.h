#pragma once
#include "resbase.h"
#include "..\Math\GeomDefine.h"
#include "..\Util\FArchive.h"
#include "..\Public\MtlDesc.h"

namespace Cool3D
{
	class RenderNode;
	class IMaterial;
	/**	\class ResRenderable 
		\brief ���п���Ⱦ����Դ�Ļ���
		\todo ����������л�֧������
	*/
	class Cool3D_API ResRenderable : public ResBase
	{
	public:
		enum ETexPathMode
		{
			ETexPath_Lib,	//texture����ָ����Ŀ¼��
			ETexPath_Mdl	//texture����ģ������Ŀ¼��
		};
	public:
		ResRenderable(const TCHAR* szName);
		virtual ~ResRenderable(void);

		virtual UINT GetNumRenderNode() const =0;
		virtual RenderNode *GetRenderNode(UINT i) const =0;
		virtual const AABBox	*GetAABBox() const =0;
		virtual int GetNumMtl() const =0;
		/**	���ڲ�������Ҫ�Ĳ��ʴ�����һ��������,�ɵ����߹�����������
			\remarks ע��,��ʹ��resource �첽IOʱ,ֻ��resource�����ɹ���,�˵��ò���Ч
		*/
		virtual void CreateMtlArray(vector<IMaterial*>&	mtls,const TCHAR* szLightMap,float mtlDiffuseFactor=1.0f) const =0;
	protected:
		void CreatemtlArrayFromDesc(vector<IMaterial*>&	mtls,const vector<tagMtlDesc>& descs,UINT mipLevels,const TCHAR* szLightMap) const;
		DECL_RTTI(ResRenderable);
	};
}//namespace Cool3D