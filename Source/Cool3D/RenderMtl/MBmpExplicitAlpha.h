#pragma once
#include "mbitmap.h"

namespace F3D
{
	/**	\class MBmpExplicitAlpha 
		\brief ��ɫֵʹ��һ����ͼ,Alphaֵʹ������һ����ͼ
	*/
	class MBmpExplicitAlpha : public MBitmap
	{
	public:
		MBmpExplicitAlpha(void);
		virtual ~MBmpExplicitAlpha(void);

		virtual void SetTexture(UINT index,const char* szPath);
	public:
		ResTexture	*m_pAlpha;

		DECL_RTTI_DYNC(MBmpExplicitAlpha);
	};
}//namespace F3D