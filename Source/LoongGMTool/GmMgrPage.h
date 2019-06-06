/////////////////////////////////////////////////////////////////////////////
// Name:        GmMgrPage.h
// Created:     2009-05-20
// Info:		GM����ҳ��
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "UserFrame.h"

class MyListCtrl;

class GmMgrPage : public wxPanel, public UserFrame
{
public:
	GmMgrPage(wxBookCtrlBase *book, const wxString& label);
	const wxString& GetLabel() { return m_label; }

	void SetMyFrame(MyFrame* frame) { m_pMyFrame = frame; }

	//--����,�޸�,ɾ��
	void OnGMCreateOperation(wxCommandEvent& event);
	void OnGMModifyOperation(wxCommandEvent& event);
	void OnGMDeleteOperation(wxCommandEvent& event);

	DWORD OnGmMgrPageEvent(tagUserEvent* pGameEvent);

private:
	wxString				m_label;

	wxTextCtrl*				m_text_account;				//����
	wxTextCtrl*				m_text_psw;
	wxComboBox*				m_combox_Pr_cre;
	wxButton*				m_btn_create;

	wxTextCtrl*				m_text_account_modify;		//�޸�
	wxComboBox*				m_combox_Pr_modify;
	wxButton*				m_btn_modify;
	
	wxTextCtrl*				m_text_account_del;			//ɾ��
	wxButton*				m_btn_del;
	
	MyFrame*				m_pMyFrame;	

	DECLARE_EVENT_TABLE()
};