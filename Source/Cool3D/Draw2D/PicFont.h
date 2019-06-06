#pragma once
#include "..\Cool3D.h"
#include ".\idraw2d.h"

class IFS;

namespace Cool3D
{
	/**	\class PicFont
		\brief ʹ��ͼƬ������
		\remarks .ʹ��һ��256*256�ĻҶ�ͼƬ(��Alphaͨ��),�洢ASCII��Ϊ0~127�����ַ�
	*/
	class Cool3D_API PicFont
	{
	public:
		PicFont(void);
		~PicFont(void);


		//--�ر�����
		/**���벢����ASCII�������������һ��PicFont�����ʱ�����ȵ��øú���
			par��szPicFileName	ָ��Ascii����ŵ�����ͼƬ
		*/
		bool Create(const TCHAR* szPicFileName,IFS *pFS);

		//! ���û���ʱÿ���ַ��Ĵ�С��Ĭ��Ϊ16*16����
		void SetDrawCharSize(const int w=16,const int h=16);

		/**����ָ����Ļ�ϵ���ʼ�������ASCII�ַ�����
			par:	szText  ָ����ASCII�ַ���
					x,y	    ��Ļ���ڻ��Ƶ���ʼ����
					dwColor Ϊָ�����Ƶ���ɫ��Ĭ��Ϊ��ɫ,
					nCount  ָ�������ַ������ַ��ĸ���,Ĭ��Ϊ-1ȫ������
					dwTextEffectColor	���������ɫ0x00000000��ʾ�����
		*/
		void DrawString(const TCHAR* szText,int x,int y,float z,DWORD dwColor=0xFFFFFFFF,
			DWORD dwTextEffectColor=0x00000000, int nCount=-1);
	
		void Destroy();

		//--��������
		void	SetSpaceSize(const int w =2, const int h =8);	//���������ַ��ļ����м��
		void	SetSpaceWidth(const int w);			//�������ڵ��ַ����
		void	SetSpaceHeight(const int h);		//�����м��
		void	SetDrawCharWidth(const int nW);		//�����ַ��Ŀ��
		void	SetDrawCharHeight(const int nH);	//�����ַ��ĸ߶�
		int		GetDrawCharWidth();					//ȡ���ַ��Ŀ��
		int		GetDrawCharHeight();				//ȡ���ַ��ĸ߶�
		int		GetSpaceWidth();					//ȡ�����ڵ��ַ����
		int		GetSpaceHeight();					//ȡ���м��
		const TCHAR* GetASCIITexPath() const;		//ȡ������ͼƬ��·��
	
	private:
		class Member;
		Member *m_p;
	};


	/**	\class PicFontEx
	\brief ʹ��ͼƬ������
	\remarks .ʹ��ָ�ַ��Ͷ�Ӧ��ͼƬ�������ַ���֮ǰһ��Ҫ�����ַ���Ӧ��ͼƬ
	*/
	class Cool3D_API PicFontEx
	{
	public:
		PicFontEx(void);
		~PicFontEx(void);


		//--�ر�����
		/**���벢����ASCII�������������һ��PicFont�����ʱ�����ȵ��øú���
		par��szPicFileName	ָ��Ascii����ŵ�����ͼƬ
				ch			ָ��ͼƬ��Ӧ���ַ�
		*/
		bool LoadPic(TCHAR ch,const TCHAR* szPicFileName,IFS *pFS);

		/**����ָ����Ļ�ϵ���ʼ�������ASCII�ַ�����
		par:	szText  ָ����ASCII�ַ���
		x,y	    ��Ļ���ڻ��Ƶ���ʼ����
		dwColor Ϊָ�����Ƶ���ɫ��Ĭ��Ϊ��ɫ,
		nCount  ָ�������ַ������ַ��ĸ���,Ĭ��Ϊ-1ȫ������
		nSyle	���ֶ��뷽ʽ=0:����룬1������룬 2���Ҷ���
		nZoom	��������ű���
		*/
		void DrawString(const TCHAR* szText,int x,int y,float z,DWORD dwColor=0xFFFFFFFF,int nCount=-1,int nSyle=1,float fZoom=1.0f);
		void GetDrawStringSize(const TCHAR* szText, int& nWidth, int& nHeight);

		void Destroy();

		//--��������
		void	SetSpaceSize(const int w =2, const int h =2);	//���������ַ��ļ����м��
		void	SetSpaceWidth(const int w);			//�������ڵ��ַ����
		void	SetSpaceHeight(const int h);		//�����м��
		int		GetSpaceWidth();					//ȡ�����ڵ��ַ����
		int		GetSpaceHeight();					//ȡ���м��

	private:
		//nStyle=0:����룬1������룬 2���Ҷ���
		void CalStringPos(const TCHAR* szText,int nStyle ,int& x,int& y);

	private:
		class Member;
		Member *m_p;
	};
}//namespace Cool3D