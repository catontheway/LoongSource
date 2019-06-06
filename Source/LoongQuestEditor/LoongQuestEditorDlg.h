// LoongQuestEditorDlg.h : header file
//

#pragma once
#include "afxcmn.h"

#include "MyTabCtrl.h"
#include "afxwin.h"

struct tagQuestProto;

// CLoongQuestEditorDlg dialog
class CLoongQuestEditorDlg : public CDialog
{
// Construction
public:
	CLoongQuestEditorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_LOONGQUESTEDITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CMyTabCtrl m_ctrlTab;
	CTreeCtrl m_ctrlQuestTree;
	CButton m_ctrlNewQuest;
	CButton m_ctrlSaveQuest;
	CButton m_ctrlSaveAsQuest;
	CButton m_ctrlDeleteQuest;
	CButton m_ctrlQuestPath;
	CButton m_ctrlRefreshQuestTree;
	CButton m_ctrlExportQuestAcceptRelation;
	CButton m_ctrlExportQuestCompleteRelation;
private:
	void InitTabCtrl();
	void SaveQuest( tagQuestProto* pQuest, LPCTSTR szFileName );
	
	//��ע����������·��
	BOOL ReadQuestPathFromReg();
	//������·��д��ע���
	BOOL WriteQuestPathToReg();

	//���������ļ�
	BOOL LoadQuest();

	//��������·���µ������ļ��к��ļ�
	void FindAllQuestXMLFile( LPCTSTR szQuestPath );

	//���� Tab �ؼ�����
	void UpdateTabCtrl( tagQuestProto* pQuest );

	//���� Tree �ؼ�����
	void UpdateTreeCtrl();

	//
	void LoadQuestDataFromXML();

	//��ȡ����NPC����������ļ�
	BOOL SaveAcceptQuestNPCXml( LPCTSTR szFileName );

	//�������NPC����������ļ�
	BOOL SaveCompleteQuestNPCXml( LPCTSTR szFileName );

public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnSetQuestPath();
	
	LRESULT ButtonPressed(WPARAM w, LPARAM l);

private:
	//����·��
	wstring m_wstrQuestPath;

	//�Ի���ͬʱֻ����ʾһ���������ݣ�������Ҫ���浱ǰ�����һЩ��Ϣ
	//��ǰ�����ļ���
	wstring m_wstrCurrentQuestFileName;
	//��ǰ����ָ��
	tagQuestProto* m_pCurrentQuest;

	//Ŀ¼����
	vector< wstring > m_vecPath;
	//�ļ�����
	vector< wstring > m_vecQuestXMLFile;

	//�����ļ����� Ŀ¼����	( RXS_001.xml  d:\quest\���� )
	map< wstring, wstring > m_mapQuestXMLFile;

	//
	map< wstring, tagQuestProto* > m_mapQuestData;

	list< tagQuestProto* > m_listDeleteQuest;

	list< wstring > m_listDeleteQuestFile;

public:
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNewQuest();
	afx_msg void OnDeleteQuest();
	afx_msg void OnOk();
	afx_msg void OnRefreshQuesTree();
	
	afx_msg void OnBtnExportQuestAcceptRelation();
	
	afx_msg void OnBtnExportQuestCompleteRelation();
};
