//-----------------------------------------------------------------------------
// File: image_codec
// Time: 2004-2-24
// Auth: CTCRST
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine{
struct tagImageInfo
{
	DWORD dwWidth;
	DWORD dwHeight;
	DWORD dwChannels;
	DWORD dwBitCount;
	DWORD dwPalEntryNum;
	DWORD dwWholeSize;
	//DWORD dwColorkey;
};

//-----------------------------------------------------------------------------
// ���ִ���ļ���ͳһ�ӿ�
//-----------------------------------------------------------------------------
class ImageCodec
{
public:
	virtual DWORD LoadFrom(LPCVOID pSource, DWORD dwSize=0){return 0;}
	virtual BOOL DrawToHdc(HDC hDC, LPRECT prcArea=NULL, BOOL bFlip=FALSE);

	ImageCodec();
	virtual ~ImageCodec();
	
	DWORD	GetWidth() { return m_Info.dwWidth; }
	DWORD	GetHeight() { return m_Info.dwHeight; }
	DWORD	GetBitCount() { return m_Info.dwBitCount; }

protected:
	tagImageInfo		m_Info;
	LPVOID				m_pDIB;	// ע������m_pDIBָ����ʵ����
	
	BITMAPINFOHEADER*	m_pDIBInfo;	// ָ��m_pDIB�е�����
	LPVOID				m_pBits;	// ָ��m_pDIB�е�����
	LPVOID				m_pPalette;	
};

}