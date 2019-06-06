/////////////////////////////////////////////////////////////////////////////
// Program:     RTClient
// Name:        GmMgrPage.h
// Created:     2009-06-25
// Info:		�����͹رշ�����
/////////////////////////////////////////////////////////////////////////////

#ifndef GMMGRPAGE_H_
#define GMMGRPAGE_H_

#include "PageAssist.h"

class GmMgrPage : public wxPanel
{
public:
	GmMgrPage(wxBookCtrlBase* book, const wxString label);

	const wxString& GetLabel() { return label_; }
	void SetPageAssist(PageAssist* pPageAssit) { page_assist_ = pPageAssit; }

	//--����,�޸�,ɾ��
	void OnGMCreateOperation(wxCommandEvent& event);
	void OnGMModifyOperation(wxCommandEvent& event);
	void OnGMDeleteOperation(wxCommandEvent& event);
	void OnGMQueryOperation(wxCommandEvent& event);

private:
	//ҳ�����
	wxString label_;

	wxTextCtrl*				m_text_account;				//����
	wxTextCtrl*				m_text_psw;
	//wxComboBox*				m_combox_Pr_cre;
	wxButton*				m_btn_create;

	wxTextCtrl*				m_text_account_modify;		//�޸�
	wxTextCtrl*				m_text_bind_ip;
	//wxComboBox*				m_combox_Pr_modify;
	wxButton*				m_btn_modify;

	wxTextCtrl*				m_text_account_del;			//ɾ��
	wxButton*				m_btn_del;

	wxButton*				m_btn_query;				//��ѯ

	PageAssist*				page_assist_;

	DECLARE_EVENT_TABLE()
};

#endif /* OPENAANDCLOSESERVERPAGE_H_ */

