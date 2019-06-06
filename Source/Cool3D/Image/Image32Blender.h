#pragma once
#include "..\Math\Color.h"

namespace Cool3D
{
	class Image;
	/**	\class Image32Blender
		\������32bit��image�������
	*/
	class Image32Blender
	{
	public:
		struct tagPixel
		{
			BYTE B,G,R,A;
		};
	public:
		Image32Blender(void);
		~Image32Blender(void);

		/**��src image��ϵ�dst image��ָ��λ����*/
		static void AlphaBlend(Image *pDst,Image *pSrc,UINT dstx,UINT dsty);
		/**��src imageʹ��ָ����alpha ͨ����ϵ�dst image��ָ��λ����*/
		static void AlphaBlend(Image *pDst,Image *pSrc,Image *pSrcAlpha,UINT dstx,UINT dsty);
		/**��ָ����Image����ָ������ɫ*/
		static void MulColor(Image *pImg,Color4f color);
		/**��ָ����Image����ָ������ɫ*/
		static void AddColor(Image *pImg,int a,int r,int g,int b);
		/**��ָ������ɫ����alpha���*/
		static void BlendColor(Image *pImg,Color4f srcColor);
		/** ��ָ����ͼ��ת���ɻҶ�ͼ��	*/
		static void GrayScale(Image *pImg);
		/** ��ָ����ͼ��ת���ɵ�ɫ(ֻ�а׺�ָ����ɫ��������ɫ)ͼ�� */
		static void NoColor(Image *pImg,BYTE color);
	};
}//namespace Cool3D