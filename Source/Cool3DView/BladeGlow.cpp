// BladeGlow.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "F3DView.h"
#include "BladeGlow.h"


// CBladeGlow �Ի���

IMPLEMENT_DYNCREATE(CBladeGlow, CDHtmlDialog)

CBladeGlow::CBladeGlow(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CBladeGlow::IDD, CBladeGlow::IDH, pParent)
{
}

CBladeGlow::~CBladeGlow()
{
}

void CBladeGlow::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CBladeGlow::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

BEGIN_MESSAGE_MAP(CBladeGlow, CDHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CBladeGlow)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// CBladeGlow ��Ϣ�������

HRESULT CBladeGlow::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

HRESULT CBladeGlow::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
