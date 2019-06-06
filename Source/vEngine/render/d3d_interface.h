//-----------------------------------------------------------------------------
//!\file d3d_interface.h
//!\author Lyp
//!
//!\date 2002-03-08
//! last 2005-11-18
//!
//!\brief d3d interface
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine {
#ifdef _USE_D3D7_
//-------------------------------------------------------------------------
// ƥ�����������ṹ
//-------------------------------------------------------------------------
struct TEXTURESEARCHINFO
{
	DWORD dwDesiredBPP;   // Input for texture format search
	DWORD dwAlphaBitMask;
	DDPIXELFORMAT* pddpf; // Output of texture format search
};

#define LPSURFACE LPDIRECTDRAWSURFACE7
//-----------------------------------------------------------------------------
//!\brief d3d interface
//!	Surface :	һ��ָ��������
//!	Buffer	: 	ָר�ñ��滺����
//-----------------------------------------------------------------------------
class D3DRender : public IRender
{
public:
	virtual BOOL	Init(INT nWidth, INT nHeight);
	virtual VOID	Destroy();

	virtual DWORD	CreateSurface(INT nWidth, INT nHeight, BOOL bAlpha, BOOL bTarget);
	virtual VOID	ReleaseSurface(DWORD dwHandle);

	//!\param ���!P_VALID(dwSurfaceHandle)����Ҫȡ�ú�̨�����HDC
	virtual DWORD	GetDC(DWORD dwHandle);
	virtual VOID	ReleaseDC(DWORD dwHandle, DWORD dwDC);

	virtual VOID	Clear(DWORD dwHandle, const tagRect* prc, DWORD dwColor);
	virtual VOID	Draw(DWORD dwDestHandle, DWORD dwSrcHandle, const tagRect* prcDest, 
		const tagRect* prcSrc, DWORD dwColor);

	virtual BOOL	Render();


	// ������client����ת��Ϊ����view����(view����Ϊ�߼�����)
	virtual VOID	ClientToView(tagPoint& pt);
	virtual VOID	UpdateWindowPos();



    D3DRender();
	~D3DRender();

private:

	GUID				m_GUIDDevice;			// �豸��GUID

    LPDIRECTDRAW7		m_pDD;					// ����ʾ����
 	LPDIRECT3D7			m_pD3D;					// ��ά��ʾ����
    LPDIRECT3DDEVICE7	m_pd3dDevice;			// ��ά��ʾ�豸

	LPSURFACE			m_pddsFrontBuffer;		// ǰ̨������ָ��
    LPSURFACE			m_pddsBackBuffer;		// ��̨������ָ��

	DDPIXELFORMAT		m_PixelFormat;			// ��ͨ�����ʽ
	DDPIXELFORMAT		m_PixelFormat32;		// 32λ�����ʽ
   
	LPSURFACE			m_pLastTexture;		// �Ż���
	LPSURFACE			m_pLastTarget;
	D3DTLVERTEX			m_Mesh4[4];
    HWND				m_hWnd;
   

	HRESULT	CreateWindowedDisplay(INT nWidth, INT nHeight);

	// ���������С
	VOID	SetD3DState();	// ������άRander/Texture״̬
  
	HRESULT	GetCaps(bool bRunTest);	// �õ�Ӳ���豸����
	// ����ö�ٻص�
	static HRESULT CALLBACK TextureSearchCallback(DDPIXELFORMAT* pddpf,VOID* param);

};



__forceinline void SetTLVertex(D3DTLVERTEX *pTLVertex, D3DVALUE x, D3DVALUE y, 
							   D3DVALUE tu, D3DVALUE tv, D3DCOLOR color)
{																
	pTLVertex->sx = x;
    pTLVertex->sy = y;
    pTLVertex->tu = tu;
	pTLVertex->tv = tv;
	pTLVertex->dcColor = color;
}


#endif
} // namespace vEngine {
