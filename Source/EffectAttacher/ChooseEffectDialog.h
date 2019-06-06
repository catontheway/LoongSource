#pragma once
#include "afxwin.h"


// CChooseEffectDialog �Ի���

class CChooseEffectDialog : public CDialog
{
	DECLARE_DYNAMIC(CChooseEffectDialog)

public:
	CChooseEffectDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChooseEffectDialog();

// �Ի�������
	enum { IDD = IDD_CHOOSEEFFECT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EffectAddress_Edit;
	CButton m_AddAddress_Button;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	bool UpdateAttachPointList(void);
	CListBox m_AttachPoint_List;
	
	CString m_EffectAddress;
	CString m_AttachPoint;

	afx_msg void OnBnClickedEffectaddressButton();
	afx_msg void OnBnClickedOk();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
