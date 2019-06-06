#pragma once
#include "..\Cool3D\public\propertylist.h"


// CUVSetDlg �Ի���

class CUVSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CUVSetDlg)

public:
	CUVSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUVSetDlg();

// �Ի�������
	enum { IDD = IDD_UVSet };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	CPropertyList m_hUVList;

	void UpdateUVList(bool bSave);
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
