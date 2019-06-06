#pragma once
#include "image.h"
#include "..\Cool3D.h"

namespace Cool3D
{
	/**	\class ImageGDI 
		\brief �ṩGDI��ͼ�ӿڵ�Image
	*/
	class Cool3D_API ImageGDI : public Image
	{
	public:
		ImageGDI(void);
		virtual ~ImageGDI(void);

		//--Image
		virtual void Destroy();
		//--
		/**	ʹ���ڲ������bmp handleʧЧ
			\remarks ���ⲿ������Image::Scale()�Ⱥ�����,��Ҫ���ô˺���,�Ը���bmp handle
		*/
		void InvalidateHBmp();
		HBITMAP GetHBmp();
		void DrawDC(HDC hDestDc,int destX,int destY,int srcX,int srcY,int w,int h,DWORD op=SRCCOPY);

	private:
		friend class Kernel;//ֻ����Kernel�����������
		/**	��ʼ��ϵͳ,��������ʱ����һ�μ��� 
		*/
		static void InitSys();
	private:
		HBITMAP	m_hBmp;
	};
}//namespace Cool3D