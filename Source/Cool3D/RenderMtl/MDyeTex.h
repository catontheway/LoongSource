#pragma once
#include "imaterial.h"
#include ".\MBitmap.h"

namespace Cool3D
{
	class MConstColor;
	class MBitmap;
	class MMAlphaMask;
	/** \class MDyeTex
		\brief Ⱦɫ��ͼ
	*/
	class Cool3D_API MDyeTex :	public IMaterial
	{
	public:
		MDyeTex(void);
		virtual ~MDyeTex(void);

		//--IMaterial
		virtual bool IsResourceReady() const;
		virtual DWORD GetRenderTrait1() const;
		virtual DWORD GetRenderTrait2() const;

		//--
		void SetAlphaMap(const TCHAR* szPath);
		MMAlphaMask* GetAlphaMask();

	public:
		MConstColor*	m_pColor;
		Color4f			m_dyeColor;//Ⱦ��ȥ����ɫ
		MBitmap*		m_pAlphaMap;//��ʾȾɫ�������ͼ
		MBitmap*		m_pDiffuseMap;//��������

		DECL_RTTI_DYNC(MDyeTex);
	};

}//namespace Cool3D