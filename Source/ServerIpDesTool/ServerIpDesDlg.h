// ServerIpDesDlg.h : ͷ�ļ�
//

#pragma once


// CServerIpDesDlg �Ի���
class CServerIpDesDlg : public CDialog
{
// ����
public:
	CServerIpDesDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CServerIpDesDlg();

// �Ի�������
	enum { IDD = IDD_ServerIpDes_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

private:
	//���ݱ������ļ������ɼ����ļ���(����׺��Ϊ.dat)
	CString GetSaveFileName() const;
	//������ɼ��ܺ�
	INT GenDesCode();

	//��ȡ�������ļ��������ܺ󱣴�
	BOOL EncryptAndSave();
	//��ȡ�����ļ��������ܺ󱣴�(������)
	BOOL DecryptAndSave();

	BOOL IsLegal();
	LPTSTR GetStr( BYTE* pByte, INT nLen );
	VOID RetStr(LPTSTR pRet);
public:
	afx_msg void OnBnClickedButtonBrowse();

private:
	TObjRef<Util>	m_pUtil;

	CString m_strFileName;
};
