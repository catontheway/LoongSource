//-----------------------------------------------------------------------------
// File: image_jpeg
// Time: 2004-3-10
// Auth: CTCRST
//-----------------------------------------------------------------------------
#pragma once
#include "image_codec.h"

namespace vEngine{

//-----------------------------------------------------------------------------
// ���ִ���ļ���ͳһ�ӿ�
//-----------------------------------------------------------------------------
class ImageBmp:public ImageCodec
{
public:
	virtual DWORD	LoadFrom(LPCVOID pSource, DWORD dwSize);

	virtual BOOL DrawToHdc(HDC hDC, LPRECT prcArea=NULL, BOOL bFlip=FALSE)
	{
		// BMP�ǵ��Ŵ�ŵ�,���ǰ���������
		return ImageCodec::DrawToHdc(hDC, prcArea, !bFlip);
	}

};




}