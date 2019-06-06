/////////////////////////////////////////////////////////////////////////////
// Name:        PmdPage.h
// Created:     2009-02-04
// Info:		����ƹ���ҳ��
/////////////////////////////////////////////////////////////////////////////

#ifndef PMDPAGE_H_
#define PMDPAGE_H_
#include "UserFrame.h"

class MyListCtrl;
class PmdPage : public wxPanel, public UserFrame
{
public:
	PmdPage(wxBookCtrlBase *book, const wxString& label);
	void ClearTextCtrl();

	const wxString& GetLabel() { return m_label; }

	void InsertServerStatus();

	void SetMyFrame(MyFrame* frame) { m_pMyFrame = frame; }

	void IssuePmdPage(wxCommandEvent& event);

	

	DWORD OnPmdPageEvent(tagUserEvent* pGameEvent);

	void OnColorType(wxCommandEvent& event);
private:
	wxString		m_label;
	wxButton*		m_btnAdd_pmd;		  //�������ƹ���
	wxComboBox*		m_combobox_color_pmd; //����ƹ�����ɫ
	wxTextCtrl*		m_text_pmd;			  //����ƹ�������
	MyListCtrl*     m_lctrlServerStatus;

	MyFrame*		m_pMyFrame;

	DECLARE_EVENT_TABLE()
};



#endif /* PMDPAGE_H_ */