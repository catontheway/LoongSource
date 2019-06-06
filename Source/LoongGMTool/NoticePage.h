/////////////////////////////////////////////////////////////////////////////
// Program:     loong����༭��
// Name:        NoticePage.h
// Created:     2009-02-04
// Info:		����ҳ��
/////////////////////////////////////////////////////////////////////////////


#ifndef NOTICEPAGE_H_
#define NOTICEPAGE_H_

class MyListCtrl;

//----------------------------------------------------------------------------
// ����
//----------------------------------------------------------------------------
class NoticePage : public wxPanel
{
public:
	NoticePage(wxBookCtrlBase *book, const wxString& label);
	const wxString& GetLabel() { return m_label; }

private:
	wxString		m_label;
	wxButton*		m_btnAdd_pmd;		  //�������ƹ���
	wxComboBox*		m_combobox_color_pmd; //����ƹ�����ɫ
	wxTextCtrl*		m_text_pmd;			  //����ƹ�������
	wxButton*		m_btnAdd_zdxh;		  //����Զ�ѭ������
	wxComboBox*		m_combobox_color_zdxh; //�Զ�ѭ��������ɫ
	MyListCtrl*     m_lctrl_zdxh;		   //�Զ�ѭ������״̬
	wxTextCtrl*		m_text_interval;
	wxTextCtrl*		m_text_loop;
	wxTextCtrl*		m_text_title;
	wxTextCtrl*		m_text_link;
	wxTextCtrl*		m_text_year;
	wxTextCtrl*		m_text_month;
	wxTextCtrl*		m_text_day;
	wxTextCtrl*		m_text_hour;
	wxTextCtrl*		m_text_min;
	wxTextCtrl*		m_text_sec;
	wxCheckBox*     m_chkOnlinePrompt;
	wxTextCtrl*		m_text_yxj;				//���½ǹ�������
	wxButton*		m_btnAdd_yxj;			//������½ǹ���
	MyListCtrl*     m_lctrl_yxj;		   //���½ǹ���״̬

	DECLARE_EVENT_TABLE()
};

#endif /* NOTICEPAGE_H_ */