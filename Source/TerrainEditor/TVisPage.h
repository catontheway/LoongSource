#pragma once
#include "afxwin.h"
#include "ColorButton.h"


// TVisPage �Ի���

class TVisPage : public CPropertyPage
{
	DECLARE_DYNAMIC(TVisPage)

public:
	TVisPage();
	virtual ~TVisPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_VIS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CColorButton m_diffuse;
	CColorButton m_ambient;
	CColorButton m_specular;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedDiffuse();
	afx_msg void OnBnClickedAmbient();
	afx_msg void OnBnClickedSpecular();
	UINT m_terrainPow;
	afx_msg void OnEnChangeTerrainpow();
};
