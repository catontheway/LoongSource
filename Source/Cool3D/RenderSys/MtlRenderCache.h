#pragma once
#include "..\Device\StateBlock.h"
#include <set>
using namespace std;

namespace Cool3D
{
	enum EMtlTrait			
	{
		EMT_Unknow=0,				 //δ֪
		EMT_Opacity,				 //��͸��
		EMT_AlphaTest,				 //�ο�
		EMT_AlphaBlend,				 //͸��
		EMT_AlphaBlendDisableZ,		 //͸�����ر�Z����
		EMT_AlphaBlendDisableZWrite, //͸�����ر�Zд��
		EMT_AlphaTestAndBlend,		 //��͸��
	};

	class IMaterial;
	class IMtlModifier;
	/**	\class MtlRenderCache
		\brief IMaterial��RenderSys/Device friendly ����
	*/
	class MtlRenderCache
	{
	public:
		MtlRenderCache(void);
		virtual ~MtlRenderCache(void);

		virtual void CompileMaterial(IMaterial *pMtl) =0;
		//!	��ĳ��MtlModifier Updateʱ�������,������Щ��������(����uv����)��Mod
		virtual void UpdateMtlMod(IMtlModifier *pMod) =0;
		virtual int GetNumPass() const =0;
		virtual void Begin(IVertShader* pVS=NULL,IPixelShader* pPS=NULL);
		virtual bool BeginPass(int i) =0;
		virtual void EndPass(bool clearTex=true) =0;
		virtual void End();
		virtual void UpdateMtl(IMaterial* pMtl) =0;
		virtual EMtlTrait GetMtlTrait() =0;

	protected:
		set<ERStateType>	m_influenceState;//����passӰ�쵽��render state,combileʱ����
		StateBlock			m_stateSave;	//begin()ʱ������Ӱ�쵽��render state����,endʱ���û�ȥ
	};
}//namespace Cool3D