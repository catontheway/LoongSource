#pragma once
#include "..\Cool3D.h"
#include "..\Public\VertexDefine.h"
#include "..\Device\IDevice.h"

	class IFS;
namespace Cool3D
{
	class ResTexture;
	/**	\class IDraw2D
		\brief ��2D��ʽ����Ļ��ͼ�Ĺ���
	*/
	class Cool3D_API IDraw2D
	{
	public:
		enum EBlendMode
		{
			EBlend_Disable, //�ر�alpha���
			EBlend_Alpha,	//���:src.rgb*src.a+(1-src.a)*dest.rgb
		};

		enum EAlphaWriteType
		{
			EAWT_Write,     //дAlphaͨ��
			EAWT_NoWrite,   //��дAlphaͨ��
			EAWT_Add,       //Alphaͨ������ȾĿ�����
		};
		
		IDraw2D(void){} 
		virtual ~IDraw2D(void) {}

		static IDraw2D* Inst();

		/**	����,��ϵͳ����ʱ����һ��,���䱾ϵͳ��Ҫ��������Դ
			\param ��ʹ�õ�Texture Pool���������
		*/
		virtual void Create(UINT texPoolSize=1024*1024*4) =0;
		/**	����,�ڳ������ʱ����һ��
		*/
		virtual void Destroy() =0;

		/**	�õ�һ��Texture�����ָ��
		*/
		virtual ResTexture *NewTexture(const TCHAR *szPath,IFS *pFS=NULL) =0;
		/**	�ͷ�ָ����Texture����
		*/
		virtual void ReleaseTexture(ResTexture *pTex) =0;

		/** Textrue Pool �����ռ�*/
		virtual void TextureGC() =0;
		/** �õ�Texture Pool ��С
		*/
		virtual UINT GetTexPoolSize() =0;

		/** ��ʼDraw������������Ⱦ״̬
			\remark �뾡���ܹر�alpha��Ϻ�zBuffer���Խ��������
		*/
		virtual void BeginDraw(EBlendMode blendMode,bool bZEnable=false) =0;
		/** ����Draw�������ָ���Ⱦ״̬
		*/
		virtual void EndDraw() =0;

		/** ����ͼ���Ƶ���Ļָ������
		*/
		virtual void Draw(const RECT* pDestRect,const RECT* pSrcRect,ResTexture* pTex,DWORD mulColor=0xFFFFFFFF,ETextrueFilter filter=ETFilter_POINT,float fAngle=0,EAlphaWriteType eAlphaWriteType=EAWT_Write, float z=1.0f)=0;
		virtual void Draw(const RECT* pDestRect,const RECT* pSrcRect,IRenderTexture* pTex,DWORD mulColor=0xFFFFFFFF,ETextrueFilter filter=ETFilter_POINT,float fAngle=0,float z=1.0f)=0;



		// ����Draw���� ------------------------------------------------------------
		// ����ͼ���Ƶ���Ļָ������alphaͨ����alphaMap��ȡ (minimap)
		virtual void Draw(const RECT* pDestRect,const RECT* pSrcRectBaseTex,const RECT* pSrcRectAlphaMap,ResTexture* pBaseTex,ResTexture* pAlphaMap,ETextrueFilter filter=ETFilter_POINT,DWORD mulColor=0xFFFFFFFF) =0;
		virtual void Draw(const RECT* pDestRect,const RECT* pSrcRectBaseTex,const RECT* pSrcRectAlphaMap,IRenderTexture* pBaseTex,ResTexture* pAlphaMap,ETextrueFilter filter=ETFilter_POINT,DWORD mulColor=0xFFFFFFFF) =0;
		// ����ͼ���Ƶ���Ļָ�����򣬿�ָ��Tex2��AlphaOp (minimap)
		void virtual Draw(const RECT* pDestRect,ResTexture* pTex1,IRenderTexture* pTex2,const RECT* pSrcRect1,const RECT* pSrcRect2,ETextureOp eAlphaOp) = 0;
		// ��ͼԪ (CoolDown Button)
		virtual void Draw(EDrawType eType, LPPOINT lpPoints, int nVerCount, DWORD color, EAlphaWriteType eAlphaWriteType)=0;
		// -------------------------------------------------------------------------

		/** ����
		*/
		virtual void DrawLine(int sx,int sy,int ex,int ey,DWORD color) =0;
		
		/** ��TexturePool��Ϣ�������־�ļ�
		*/
		virtual void Report() =0;
		/** ����Ļ����ʾTexturePool��С
		*/
		virtual void DrawInfo(int x,int y) =0;

	};
}//namespace Cool3D