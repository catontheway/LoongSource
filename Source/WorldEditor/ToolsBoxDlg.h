#pragma once
#include "afxwin.h"


// CToolsBoxDlg �Ի���

class CToolsBoxDlg : public CDialog
{
	DECLARE_DYNAMIC(CToolsBoxDlg)

public:
	CToolsBoxDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CToolsBoxDlg();

// �Ի�������
	enum { IDD = IDD_ToolsBox };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CListBox m_list;
	afx_msg void OnLbnSelchangeToolslist();
};
