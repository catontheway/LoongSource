/////////////////////////////////////////////////////////////////////////////
// Name:        mainpage.h
// Purpose:     
// Author:      songg
// Modified by: 
// Created:     15/07/2009 09:38:59
// RCS-ID:      
// Copyright:   songg
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _MAINPAGE_H_
#define _MAINPAGE_H_


/*!
 * Includes
 */
#include "Loong_CommonDef.h"
#include "Resource.h"
////@begin includes
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */




/*!
 * MainPage class declaration
 */
class CServerMerge;
class MainPage: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( MainPage )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    MainPage();
    MainPage( wxWindow* parent, wxWindowID id = SYMBOL_MAINPAGE_IDNAME, const wxString& caption = SYMBOL_MAINPAGE_TITLE, const wxPoint& pos = SYMBOL_MAINPAGE_POSITION, const wxSize& size = SYMBOL_MAINPAGE_SIZE, long style = SYMBOL_MAINPAGE_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_MAINPAGE_IDNAME, const wxString& caption = SYMBOL_MAINPAGE_TITLE, const wxPoint& pos = SYMBOL_MAINPAGE_POSITION, const wxSize& size = SYMBOL_MAINPAGE_SIZE, long style = SYMBOL_MAINPAGE_STYLE );

    /// Destructor
    ~MainPage();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin MainPage event handler declarations
	void OnButtonClick( wxCommandEvent& event );
	void OnButtonExit( wxCommandEvent& event );
	void OnCheckBoxMergeType( wxCommandEvent& event );
////@end MainPage event handler declarations

////@begin MainPage member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end MainPage member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

	//�õ��û�����
	tagConfigDetail *GetLoginUserConfig(){ return stConfigLoginDB; }

	tagConfigDetail *GetLoongUserConfig(){ return stConfigLoongDB; }

public:

	CServerMerge *m_pclsServerMerge;	// �������ϲ��ӿ�

	HANDLE m_MergeEvent; //ͬ���¼�

	//�ؼ�
	wxTextCtrl* m_itemResult; //���
private:
	/*
	* ���û��������ȡ���ݷ���������
	*/
	void Loong_GetValueFromUserSetting();
	/*
	* ����û�������Ϣ�ĺϷ���
	*/
	BOOL Loong_CheckUserSettingValidity();
	/*
	* ���û�������Ϣд�ļ�
	*/
	void Loong_SaveUserSettingToINIFile();
	/*
	* ��ʾ����
	*/
	void Loong_ShowDetailOnScreen();
	/*
	* �������ļ���ֵ�浽������ڴ���
	*/
	void Loong_PutINIValueInToMemory(LPCTSTR pcFileOnePath,LPCTSTR pcFileTwoPath);
	/*
	/	ˢ��ui
	*/
	void OnRefreshUI();
private:
	//Text
	wxTextCtrl* m_itemLoginIP[EM_DB_TYPE_END];

	wxTextCtrl* m_itemLoginUser[EM_DB_TYPE_END];

	wxTextCtrl* m_itemLoginPsd[EM_DB_TYPE_END];

	wxTextCtrl* m_itemLoginName[EM_DB_TYPE_END];

	wxTextCtrl* m_itemLoginPort[EM_DB_TYPE_END];

	wxTextCtrl* m_itemLoongIP[EM_DB_TYPE_END];

	wxTextCtrl* m_itemLoongUser[EM_DB_TYPE_END];

	wxTextCtrl* m_itemLoongPsd[EM_DB_TYPE_END];

	wxTextCtrl* m_itemLoongName[EM_DB_TYPE_END];

	wxTextCtrl* m_itemLoongPort[EM_DB_TYPE_END];

	tagConfigDetail stConfigLoginDB[EM_DB_TYPE_END];	//LoginDB����

	tagConfigDetail stConfigLoongDB[EM_DB_TYPE_END];	//LoongDB����

	BOOL		m_bSelected; //�Ƿ�����

	tstring		m_strSrc2WorldName;//Դ2������

	std::multimap<ERoleCompensationType, tagCompensation*> m_mmCompensation; // ����

};
UINT MergeWorkerThread(LPVOID pParam);
#endif
    // _MAINPAGE_H_
