#pragma once
#include "..\Cool3D.h"
#include "IDevice.h"

namespace Cool3D
{
	class IDeviceTex;
	/**	\class IRenderCubeTexture
		\brief ��������texture,�ֿ�����render target�Ķ���
	*/
	class Cool3D_API IRenderCubeTexture
	{
	public:
		IRenderCubeTexture(void);
		virtual ~IRenderCubeTexture(void);

		virtual bool Create(UINT size,EPixelFormat fmt=EPF_A8R8G8B8,bool bDepthStencilBuffer=true,UINT mipLevels=1,bool bThrowExcept=true) =0;
		/**	��ʼ��Ⱦ����texture	*/
		virtual bool Begin() =0;
		virtual bool SetPass(int pass) =0;
		/** ������Ⱦ���˶���*/
		virtual void End() =0;

		virtual bool HaveDepthBuffer() =0;
		virtual bool HaveStencilBuffer() =0;

		virtual bool WriteToFile(const TCHAR* szPath) =0;

		virtual EPixelFormat Format() =0;
		virtual UINT Size() =0;
	};
}//namespace Cool3D