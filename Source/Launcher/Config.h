#pragma once


// CConfig dialog

class VarContainer;
class CConfig : public CDialog
{
	DECLARE_DYNAMIC(CConfig)

public:
	CConfig(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfig();

	virtual BOOL OnInitDialog();

	VOID Load(LPCTSTR szFileName);
	VOID Save();

// Dialog Data
	enum { IDD = IDD_CONFIG };

	CButton m_ctrlWindow0;		// ����ģʽ0
	CButton m_ctrlWindow1;		// ����ģʽ1
	CButton m_ctrlWindow2;		// ����ģʽ2
	CButton m_ctrlFullScreen;	// ȫ��ģʽ
	CStatic m_ctrlTips;

	CButton m_ctrlVerticalSync;		// ��ֱͬ��
	CButton m_ctrlTextureDetail;	// �߾�������
	CButton m_ctrlGroundPrecision;	// �߾��ȵر�
	CButton m_ctrlGPUAni;			// GPU����
	CButton m_ctrlGroundHighLight;	// �ر����
	CButton m_ctrlNormalMap;		// ������ͼ
	CButton m_ctrlMusic;			// ����
	CButton m_ctrlSound;			// ��Ч

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

private:
	VarContainer*	m_pVarContainer;
	CString			m_strFileName;
};
