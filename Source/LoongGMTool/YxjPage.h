/////////////////////////////////////////////////////////////////////////////
// Name:        YxjPage.h
// Created:     2009-02-04
// Info:		���½ǹ���ҳ��
/////////////////////////////////////////////////////////////////////////////

#ifndef YXJPAGE_H_
#define YXJPAGE_H_
#include "UserFrame.h"

class MyListCtrl;
class YxjPage : public wxPanel, public UserFrame
{
public:
	enum EYxjListCol
	{
		EYLC_NULL	= -1,
		EYLC_0		= 0,
		EYLC_1,
		EYLC_2,
		EYLC_3,
		EYLC_4,
		EYLC_5,
		EYLC_6,
	};
	struct tagYxj
	{
		DWORD						dwNetCrc;
		tstring						strTitle;
		tstring						strContent;
		tstring						strLink;		
		tstring						strYear;
		tstring						strMonth; 
		tstring						strDay;
		tstring						strHour; 
		tstring						strMin; 
		tstring						strSec; 
		bool						bLoginHint; 
		std::vector<tstring>		vecWorld;
		tagYxj()
		{
			
		}
	};

public:
	YxjPage(wxBookCtrlBase *book, const wxString& label);
	void ClearTextCtrl();

	const wxString& GetLabel() { return m_label; }

	void InsertShowItem(tstring szWorldName, tstring szTester, tstring szTitle, tstring szContent, tagDWORDTime dwStartTime);
	void ClearShowItem();
	void YxjOperationStatus(BOOL);

	void IssueYxjPage(wxCommandEvent& event);
	void CancelYxj(wxCommandEvent& event);

	void SetMyFrame(MyFrame* frame) { m_pMyFrame = frame; }

	//DWORD OnYxjPageEvent(tagUserEvent* pGameEvent);

private:
	// �ؼ�
	wxString		m_label;
	wxTextCtrl*		m_text_title;
	wxTextCtrl*		m_text_link;
	wxTextCtrl*		m_text_year;
	wxTextCtrl*		m_text_month;
	wxTextCtrl*		m_text_day;
	wxTextCtrl*		m_text_hour;
	wxTextCtrl*		m_text_min;
	//wxTextCtrl*		m_text_sec;
	//wxCheckBox*     m_chkOnlinePrompt;
	wxTextCtrl*		m_text_yxj;					//���½ǹ�������
	wxButton*		m_btnAdd_yxj;				//������½ǹ���
	wxButton*		m_btnAdd_cancelyxj;			//ȡ�����½ǹ���
	MyListCtrl*     m_lctrl_yxj;				//���½ǹ���״̬

	MyFrame*		m_pMyFrame;

	std::map<DWORD, tagYxj>		m_mapYxj;		//����

	DECLARE_EVENT_TABLE()
};



#endif /* YXJPAGE_H_ */