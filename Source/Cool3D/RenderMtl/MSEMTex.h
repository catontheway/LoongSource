#pragma once
#include "imaterial.h"
#include ".\MBitmap.h"

namespace Cool3D
{
	class MBitmap;
	/** \class MSEMTex
	\brief ���λ���ӳ��
	*/
	class Cool3D_API MSEMTex :	public IMaterial
	{
	public:
		MSEMTex(void);
		virtual ~MSEMTex(void);

		//--IMaterial
		virtual bool IsResourceReady() const;
		virtual DWORD GetRenderTrait1() const;
		virtual DWORD GetRenderTrait2() const;

		//--
		void SetEnvMap(const TCHAR* szPath);

	public:
		MBitmap*			m_pEnvMap;		//���λ���ӳ����ͼ
		MBitmap*			m_pBaseMap;		//��������

		DECL_RTTI_DYNC(MSEMTex);
	};
}//namespace Cool3D