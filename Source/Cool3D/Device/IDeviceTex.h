#pragma once
#include "..\Cool3D.h"
#include "..\Image\Image.h"

class IFS;
namespace Cool3D
{
	enum ETexDetail
	{
		ETexDetail_High =0,
		ETexDetail_Mid = 1,
		ETexDetail_Low = 2,
	};

	class Image;
	class ImageTGA;
	/**	\class IDeviceTex
		\brief Device�ϵ�Texture����
		\todo ������Ӳ������mipmap,Ӳ����֧�ֵĻ���û��
	*/
	class Cool3D_API IDeviceTex
	{
	public:
		static EImageFilter s_MipMapQuat;//!mip map����������,Ӱ��mipmap���ɵ��ٶ�
		IDeviceTex(void);
		virtual ~IDeviceTex(void);

		/**	�õ���ͼ����ռ�õ��ڴ�Ĵ�С
			\remarks ���ʹ����mipmap,��Ϊ����level���ܺ�
		*/
		virtual UINT GetSizeInBytes() =0;
		/**	���ļ���ȡ��ͼ����
			\remarks ʹ��hFile������szPath,���Ժ����Ǵ��Ǹ�FileSystem��ȡ������
		*/
		virtual void LoadFromFile(IFS *pFS,const TCHAR* szPath,UINT numLevels=0,bool bManagedPool=true,bool bUseDDS=true,ETexDetail texDetail=ETexDetail_High) =0;
		virtual bool WriteToFile(const TCHAR* szPath) =0;

		/**	��һ��Image���󴴽���ͼ
			\param pImg,Image����
			\param numLevels,mipmap�ĵȼ���,���Ϊ0,��������еĵȼ�(ֱ��1*1��С)
		*/
		virtual void CreateFromImage(Image *pImg,UINT numLevels) =0;
		/**	��Image��������ͼ��
		*/
		virtual void UpdateFromImage(Image *pImg,bool bUpdateMipMap) =0;

		virtual void CreateFromData(int w,int h,EPixelFormat fmt,BYTE *pImageData,int numLevels) =0;

		virtual void CreateFromJPGLump(void *pLump, int lumpSize) = 0;

		virtual UINT Width() =0;
		virtual UINT Height() =0;
		virtual EPixelFormat Format() =0;
	};
}//namespace Cool3D