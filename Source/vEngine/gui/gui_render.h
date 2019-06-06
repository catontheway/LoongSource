//-----------------------------------------------------------------------------
//!\file gui_render.h
//!\author Lyp
//!
//!\date 2004-12-09
//! last 2007-12-19
//!
//!\brief ����ϵͳ������Ⱦ�ӿ�
//-----------------------------------------------------------------------------
#pragma once

namespace vEngine {

struct tagGUIImage
{
	DWORD		dwID;
	DWORD		dwHandle;	// ���
	tagRect		rc;			// ����
	tagPoint	ptSize;		// ��С
	tagPoint	ptOffset;	// Renderƫ��
	BOOL		bRefresh;	// �Ƿ���Ҫ�ػ�
	BOOL		bRenderTarget;
	tagGUIImage():dwID(GT_INVALID), dwHandle(GT_INVALID),bRefresh(TRUE),bRenderTarget(FALSE){}
};

struct tagGUIFont
{
	DWORD		dwID;
	DWORD		dwHandle;	// ���
	tagGUIFont():dwID(GT_INVALID), dwHandle(GT_INVALID){}
};

enum EGUITextAlign	// ���ֶ��뷽ʽ
{
	EGUITA_LeftTop = 0,		// ����
	EGUITA_CenterTop,		// ����
	EGUITA_RightTop,		// ����
	EGUITA_LeftCenter,		// ����
	EGUITA_Center,			// ����
	EGUITA_RightCenter,		// ����
	EGUITA_LeftBottom,		// ����
	EGUITA_CenterBottom,	// ����
	EGUITA_RightBottom,		// ����
};

enum EGUIAlphaWrite
{
	EGUIAW_NoWrite = 0,		// ��дAlphaͨ��
	EGUIAW_Write,			// дAlphaͨ��
	EGUIAW_Add,				// Alpha���
};

//-----------------------------------------------------------------------------
//!\brief ����ϵͳ��Ҫ����Ⱦ�ӿ�
//-----------------------------------------------------------------------------
class VENGINE_API IGUIRender
{
public:
	// ����ͼƬ��ʧ�ܷ���0,�ɹ����ؾ��(���strImage.empty()�򷵻�NULL)
	// ����޷���ȡָ��ͼƬ�򷵻ؿ�������ΪRenderTarget�Ŀ�����
	virtual tagGUIImage* CreateImage(const tstring str, const tagRect& rc)=0;
	// ɾ��ͼƬ,pImage����Ϊ��
	virtual VOID DestroyImage(tagGUIImage* pImage)=0;

	// ��������
	virtual tagGUIFont* CreateFont(const tstring str, INT nWidth, INT nHeight, INT nWeight, BOOL bAntiAliase=TRUE)=0;
	// ��¡����,һ������һ���ؼ�Ҳʹ��ͬ������
	virtual tagGUIFont* CloneFont(tagGUIFont* pFont)=0;
	// ɾ������,pFont����Ϊ��
	virtual VOID DestroyFont(tagGUIFont* pFont)=0;

	// ��ͼ,���!P_VALID(pDest->dwHandle)����Ⱦ����̨����, pSrc����Ϊ��
	virtual VOID Draw(const tagGUIImage* pDest, const tagGUIImage* pSrc, DWORD dwColor, EGUIAlphaWrite eAlphaWrite, FLOAT z=1.0f)=0;
	// ����,pFont����Ϊ��
	virtual VOID Text(const tagGUIImage* pDest, const tstring& szText, tagGUIFont* pFont, 
		DWORD dwColor, DWORD dwShadeColor, EGUITextAlign eAlign, BOOL bWriteAlpha)=0;
	// ����
	virtual VOID Line(const tagGUIImage* pDest, DWORD dwColor, BOOL bBox=FALSE)=0;

	// �õ��ı���С,pFont����Ϊ��
	virtual VOID GetTextSize(const LPCTSTR szText, tagGUIFont* pFont, tagPoint& ptSize)=0;

	// ����Ļ����ת��Ϊ����view����
	virtual VOID ClientToView(tagPoint& pt)=0;

	// ˢ�´�������
	virtual VOID UpdateWindowPos()=0;

	// ˢ����Ļ(�ײ�ֻ�ðѺ�̨���忽����ǰ̨���弴��)
	virtual VOID Present()=0;

	// ����render target
	virtual VOID PushRenderTarget(const tagGUIImage* pDest)=0;
	virtual VOID PopRenderTarget(const tagGUIImage* pDest)=0;
	virtual VOID ClearRenderTarget()=0;
	virtual DWORD CreateRenderTarget(const tagRect& rc)=0;
	virtual VOID ReleaseRenderTarget(DWORD dwHandle)=0;

	// ���ü�������
	virtual VOID SetScissorRect(tagRect* pRect)=0;

	// ��������
	virtual VOID PlaySound(LPCTSTR szName, FLOAT fVolume=1.0f)=0;

	virtual ~IGUIRender(){}


};




//-----------------------------------------------------------------------------
//!\brief vEngine�ڲ�ʵ�ֵĽ���ϵͳ��Ⱦ�ӿ�
//-----------------------------------------------------------------------------
struct tagGUIImageHandle
{
	DWORD		dwHandle;
	tagPoint	ptSize;
	INT			nRefCount;
	BOOL		bRenderTarget;
	tagGUIImageHandle():dwHandle(GT_INVALID), nRefCount(0), bRenderTarget(FALSE){}
};
struct tagGUIFontEx : public tagGUIFont
{
	INT	nRefCount;	// ���ü���
	tagGUIFontEx():nRefCount(0){}
};


class VENGINE_API IGUIRenderNative : public IGUIRender
{
public:
	BOOL Init(IRender* pRender);

	virtual tagGUIImage* CreateImage(const tstring str, const tagRect& rc);
	virtual VOID DestroyImage(tagGUIImage* pImage);
	virtual tagGUIFont* CreateFont(const tstring str, INT nWidth, INT nHeight, INT nWeight, BOOL bAntiAliase=TRUE);
	virtual tagGUIFont* CloneFont(tagGUIFont* pFont);
	virtual VOID DestroyFont(tagGUIFont* pFont);
	virtual VOID Draw(const tagGUIImage* pDest, const tagGUIImage* pSrc, DWORD dwColor, EGUIAlphaWrite eAlphaWrite, FLOAT z=1.0f);
	virtual VOID Text(const tagGUIImage* pDest, const tstring& szText, tagGUIFont* pFont, 
		DWORD dwColor, DWORD dwShadowColor, EGUITextAlign eAlign, BOOL bWriteAlpha);
	virtual VOID Line(const tagGUIImage* pDest, DWORD dwColor, BOOL bBox=FALSE);
	virtual VOID GetTextSize(const LPCTSTR szText, tagGUIFont* pFont, tagPoint& ptSize);
	virtual VOID ClientToView(tagPoint& pt);
	virtual VOID UpdateWindowPos();
	virtual VOID Present();

	// ����render target
	virtual VOID PushRenderTarget(const tagGUIImage* pDest){ m_pCurrentDest = (tagGUIImage*)pDest; }
	virtual VOID PopRenderTarget(const tagGUIImage* pDest) { m_pCurrentDest = NULL; }
	virtual VOID ClearRenderTarget();
	virtual VOID EnableAlphaWrite(){};	// ֻ��renderTarget�ı���������Ҫ�޸�Alphaͨ��
	virtual VOID DisableAlphaWrite(){};
	virtual DWORD CreateRenderTarget(const tagRect& rc);
	virtual VOID ReleaseRenderTarget(DWORD dwHandle);

	// ���ü�������
	virtual VOID SetScissorRect(tagRect* pRect){}

	// ��������
	virtual VOID PlaySound(LPCTSTR szName, FLOAT fVolume=1.0f){}

private:
	IRender*						m_pRender;
	TObjRef<ImageObj>				m_pImageObj;
	TObjRef<Util>					m_pUtil;

	TMap<DWORD, tagGUIFontEx*>		m_mapFont;
	TMap<DWORD, tagGUIImageHandle*>	m_mapImageHandle;
	tagGUIImage*					m_pCurrentDest;

};

}	// namespace vEngine {
