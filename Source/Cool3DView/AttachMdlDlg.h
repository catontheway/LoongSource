#pragma once
#include "afxwin.h"


// CAttachMdlDlg �Ի���

class CAttachMdlDlg : public CDialog
{
	DECLARE_DYNAMIC(CAttachMdlDlg)

public:
	CAttachMdlDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAttachMdlDlg();

// �Ի�������
	enum { IDD = IDD_AttachMdl };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMdlfile();
	afx_msg void OnBnClickedOk();
	CString m_szMdlPath;
	CString m_szTagNodeName;
};
