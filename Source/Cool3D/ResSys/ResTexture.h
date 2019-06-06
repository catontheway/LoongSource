#pragma once
#include "resbase.h"
#include "..\Cool3D.h"

namespace Cool3D
{
	class IDeviceTex;
	/**	\class ResTexture 
		\brief ������Դ���Ե���ͼ����
	*/
	class Cool3D_API ResTexture : public ResBase
	{
	public:
		ResTexture(const TCHAR* szName);
		virtual ~ResTexture(void);

		virtual UINT CreateContent(DWORD param);
		
		void Reload();
		/**	����һ��texture,��this������������
		*/
		void AttachTex(IDeviceTex *pTex);
		IDeviceTex *GetDeviceTex() const
		{ return m_pTex; }

	protected:
		IDeviceTex *m_pTex;

		DECL_RTTI(ResTexture);
	};
}//namespace Cool3D