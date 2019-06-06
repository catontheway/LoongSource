#pragma once
#include "imaterial.h"
#include "..\Public\MtlDesc.h"
#include "MBitmap.h"

namespace Cool3D
{
	class MConstColor;
	class MBitmap;
	/**	\class MGeneral 
		\brief ��õĲ���
	*/
	class Cool3D_API MGeneral : public IMaterial
	{
	public:
		MGeneral(void);
		virtual ~MGeneral(void);

		void CreateFromMaxExport(const tagMtlDesc& desc,UINT mipLevels,const TCHAR* szLightMap,float mtlDiffuseFactor);
		virtual void Destroy();

		virtual bool IsResourceReady() const;
		virtual DWORD GetRenderTrait1() const
		{
			return IMaterial::GetRenderTrait1();
		}
		virtual DWORD GetRenderTrait2() const
		{
			if(m_pDiffuse)
				return m_pDiffuse->GetRenderTrait2();
			else
				return IMaterial::GetRenderTrait2();
		}

		//--ISerializable
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);

		//--��͸��
		void SetTransparent(float alpha,const tagMtlDesc& desc);
		//--���������ɫ
		void SetMulColor(const float r, const float g, const float b);
	private:
		void BeginTransparent();		
		void EndTransparent(const tagMtlDesc& desc);
		bool m_bTransparent;
	public:
		MConstColor	*m_pColor;		//������ɫ����
		MBitmap		*m_pDiffuse;	//diffuse map
		MBitmap		*m_pLightMap;	//light map
		MBitmap		*m_pBump;		//bump map
		bool		m_bEnableSpecular; //�Ƿ�ʹ�þ���߹�
		bool		m_bEnableReflection;

		int			m_uvIndexDiffuse;
		int			m_uvIndexLightMap;
		Color4f     m_color;

		DECL_RTTI_DYNC(MGeneral);
	};
}//namespace Cool3D