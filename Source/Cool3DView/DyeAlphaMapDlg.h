#pragma once


// CDyeAlphaMapDlg �Ի���

class CDyeAlphaMapDlg : public CDialog
{
	DECLARE_DYNAMIC(CDyeAlphaMapDlg)

public:
	CDyeAlphaMapDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDyeAlphaMapDlg();

// �Ի�������
	enum { IDD = IDD_DyeAlphaMap };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_szBaseTexPath;
	afx_msg void OnBnClickedBrowse();
	afx_msg void OnBnClickedOk();
	int m_nAlphaTexWidth;
	int m_nAlphaTexHeight;
	float m_fLumMask;
};
