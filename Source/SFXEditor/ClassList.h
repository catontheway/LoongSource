#pragma once
#include "afxwin.h"


// CClassList �Ի���

class CClassList : public CDialog
{
	DECLARE_DYNAMIC(CClassList)

public:
	CClassList(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CClassList();

// �Ի�������
	enum { IDD = IDD_ClassList };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_hList;
	CString	m_szSelectClass;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
