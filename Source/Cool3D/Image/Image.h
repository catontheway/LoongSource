#pragma once
#include "..\Cool3D.h"
#include "..\Util\Noncopyable.h"
#include "..\Device\IDevice.h"//for PixelFormat

class IFS;
namespace Cool3D
{
	enum EImageFilter
	{
		EIS_Normal=0,
		EIS_Fastest,
		EIS_BestQuality,
		EIS_Num
	};
	
	struct tagBGRA
	{
		BYTE B,G,R,A;
	};

	struct tagBGR
	{
		BYTE B,G,R;
	};


	/**	\class Image
		\brief Image�ӿ�,һ��ʵ����Ӧһ��Image����
	*/
	class Cool3D_API Image : private Noncopyable
	{
	public:
		enum EImageMod
		{
			EMod_AverageBlur
		};
	public:
		Image(void);
		virtual ~Image(void);

		/**	���ڴ��д���Image
		*/
		bool Create(UINT w,UINT h,EPixelFormat pixelFmt);
		/**	����ǰ��ͼ��ת����ָ�������ظ�ʽ
		*/
		bool ConvertFmt(EPixelFormat pixelFmt);
		/**	���ö�ȡ���ļ�ϵͳ
		*/
		void SetFS(IFS *pFS);
		/**	���ļ���ȡ����,�����������ݴ���Image
		*/
		bool LoadFromFile(const TCHAR *szPath);
		/*	���ڴ洴��,֧��TGA
		*/
		bool LoadFromMem(void *pMem,UINT size);
		/**	��Image������д���ļ�
		*/
		bool WriteToFile(const TCHAR *szPath);
		/**	�ͷ���ռ�õ��ڴ����Դ
		*/
		virtual void Destroy();
		/**	creates a copy of the current image
		*/
		Image* Clone();

		UINT Width() const;
		UINT Height() const;
		UINT GetBpp() const;//bit per pixel
		EPixelFormat GetPixelFormat() const;
		UINT GetSizeInBytes() const;
		void* GetRawBuffer() const;

		/**	��ָ��ͼ���ָ�����򻭵���ͼ������
		*/
		void Blit(Image *pSrc,int destX,int destY,int srcX,int srcY,int w,int h);
		/**	��ָ��ͼ�������ͼ�񻭵���ͼ������
		*/
		void Blit(Image *pSrc,int destX,int destY);
		/**	���ŵ�ǰͼ��
		*/
		bool Scale(UINT w, UINT h, EImageFilter mode);
		/**	��ת��ǰͼ��
		*/
		bool Rotate(float rad, EImageFilter mode);
		/**	��ֱ��ת��ǰͼ��
		*/
		bool VFlip();
		/** ͼ������Ƿ���UPPER-LEFT
		*/
		bool IsReversed();
		/**	ͼ����	*/
		void DoModifiy(EImageMod mod,int param);
		/** ����һ����ͬ�ߴ��ͼƬ���
		*/
		bool Mul(Image* pOther);
		/** �������ȺͶԱȶ�
		*/
		void AdjuctLumContrast(float lum,float constrast);

		void Blit(Image* pSrc,TCHAR channel);

		/**	ģ��
		*/
		void Blur(UINT blurNum);

		void DiffuseRow(UINT row,UINT startCol,UINT endCol,INT offset);
		void DiffuseCol(UINT col,UINT startRow,UINT endRow,INT offset);

		//--�ڲ�����
		UINT GetID() const;
	private:
		friend class Kernel;//ֻ����Kernel��������������
		/**	��ʼ��ϵͳ,��������ʱ����һ�μ��� 
		*/
		static void InitSys();
		/**	�ر�ϵͳ,�������ʱ����һ��
		*/
		static void ShutDownSys();
	private:
		class Member;
		Member *m_p;
	};
}//namespace Cool3D