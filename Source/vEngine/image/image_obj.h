//-----------------------------------------------------------------------------
// File: image_obj
// Time: 2004-2-24
// Auth: CTCRST
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine{
class ImageCodec;
//-----------------------------------------------------------------------------
// ����Image��ͳһ�ӿ�
//-----------------------------------------------------------------------------
class ImageObj
{
public:
	DWORD	Load(LPCTSTR szFileName);
	BOOL	DrawToHdc(HDC hDC, LPRECT prcArea=NULL, BOOL bFlip=FALSE);

	DWORD	_DirectCreateSurface(LPCVOID pSource, DWORD dwSize);


	DWORD	GetWidth();
	DWORD	GetHeight();
	DWORD	GetBitCount();

	ImageObj();
	~ImageObj();

protected:
	TObjRef<VirtualFileSys>	m_pVFS;
	ImageCodec*				m_pCodec;	// �����������೧

};

}