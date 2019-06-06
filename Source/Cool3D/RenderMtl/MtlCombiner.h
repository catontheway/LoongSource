#include "..\device\IDevice.h"
#pragma once
#include "imaterial.h"

namespace Cool3D
{
	/**	\class MtlCombiner 
		\brief �������������ض��ķ�ʽ�������, ���������������һ�ò������ṹ,��ϲ���ʽ������D3D��Texture Stage
	*/
	class MtlCombiner : public IMaterial
	{
	public:
		MtlCombiner(void);
		virtual ~MtlCombiner(void);

		void SetOperator(ETextureOp colorOp,ETextureOp alphaOp)	{	m_colorOp=colorOp; m_alphaOp=alphaOp;}
		virtual bool IsResourceReady() const;

		virtual DWORD GetRenderTrait1() const
		{
			return IMaterial::GetRenderTrait1();
		}
		virtual DWORD GetRenderTrait2() const
		{
			if(m_pMtl1)
				return m_pMtl1->GetRenderTrait2();
			else
				return IMaterial::GetRenderTrait2();
		}
		
		//--ISerializable
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);
	public:
		IMaterial	*m_pMtl1;
		IMaterial	*m_pMtl2;
		ETextureOp	m_colorOp,
					m_alphaOp;

		DECL_RTTI_DYNC(MtlCombiner);
	};
}//namespace Cool3D