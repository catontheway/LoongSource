#pragma once

//! \class	StaticEx
//! \brief	���ṩ����Ȧѡ����,�Լ����Ը�����̬�ؼ���ͼƬ
//! \author	hyu
//! \date	2009-3-5
//! \base	GUIStatic
//! \sa		GUIStatic

class StaticEx :
	public GUIStatic
{
public:
	StaticEx(void);
	~StaticEx(void);

	//! �ػ���Ⱦ����
	virtual VOID Render();

	VOID SetPicFile(const tstring& strFileName);

	VOID SetFontHeight(INT hValue);

	//! ���ø���״̬
	VOID SetHighLight(BOOL bVal) {bisHighLight = bVal;}

	//! �õ���ǰ����״̬
	BOOL GetHightLight() {return bisHighLight;}

private:
	BOOL		bisHighLight;	//!< ����״̬
};
