#pragma once


// CBladeGlow �Ի���

class CBladeGlow : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CBladeGlow)

public:
	CBladeGlow(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBladeGlow();
// ��д
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// �Ի�������
	enum { IDD = IDD_BladeGlow, IDH = IDR_HTML_BLADEGLOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
};
