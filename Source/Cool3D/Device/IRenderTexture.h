#pragma once
#include "..\Cool3D.h"
#include "IDevice.h"

namespace Cool3D
{
	class IDeviceTex;
	/**	\class IRenderTexture
		\brief ��������texture,�ֿ�����render target�Ķ���
	*/
	class Cool3D_API IRenderTexture
	{
	public:
		IRenderTexture(void);
		virtual ~IRenderTexture(void);

		virtual bool Create(UINT w,UINT h,EPixelFormat fmt=EPF_A8R8G8B8,bool bDepthStencilBuffer=true,bool bLockable=false,UINT mipLevels=1,bool bThrowExcept=true,bool bUseBackBufferMutiSampleType=false) =0;
		/**	��ʼ��Ⱦ����texture	*/
		virtual bool Begin() =0;
		/** ������Ⱦ���˶���*/
		virtual void End() =0;

		virtual bool HaveDepthBuffer() =0;
		virtual bool HaveStencilBuffer() =0;

		virtual bool WriteToFile(const TCHAR* szPath) =0;

		virtual EPixelFormat Format() =0;
		virtual UINT Width() =0;
		virtual UINT Height() =0;

		virtual void LockRect( tagLockedRect *pLock, CONST RECT *pRect ) =0;
		virtual void UnlockRect() =0;

		virtual void StrechRect(IDeviceTex* pSrc,CONST RECT* pDestRect,CONST RECT* pSrcRect,ETextrueFilter filter) =0;
		virtual void StrechRect(IRenderTexture* pSrc,CONST RECT* pDestRect,CONST RECT* pSrcRect,ETextrueFilter filter) =0;
	};
}//namespace Cool3D