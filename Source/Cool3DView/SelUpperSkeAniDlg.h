#pragma once
#include "afxwin.h"


// CSelUpperSkeAniDlg �Ի���

class CSelUpperSkeAniDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelUpperSkeAniDlg)

public:
	CSelUpperSkeAniDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSelUpperSkeAniDlg();

// �Ի�������
	enum { IDD = IDD_SELUPPERSKEANIDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_szUpperSkeAniName;
	CString m_szLowerSkeAniName;
	BOOL m_bIsOneTrack;
	CEdit m_UpperSkeAniEdit;
};
