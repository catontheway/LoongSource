#pragma once


// CSetChopFieldType �Ի���

class CSetChopFieldType : public CDialog
{
	DECLARE_DYNAMIC(CSetChopFieldType)

public:
	CSetChopFieldType(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetChopFieldType();

// �Ի�������
	enum { IDD = IDD_DIALOG_CHOPFIELDTYPE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	CString   m_szChopFieldType;
public:
	virtual BOOL OnInitDialog();
	int GetChopFieldType();

	CComboBox m_ChopFieldType;
	afx_msg void OnBnClickedOk();
};
