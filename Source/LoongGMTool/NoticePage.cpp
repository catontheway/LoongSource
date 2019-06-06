/////////////////////////////////////////////////////////////////////////////
// Program:     loong����༭��
// Name:        NoticePage.cpp
// Created:     2009-02-04
// Info:		����ҳ��
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "wx/wx.h"
#include "wx/busyinfo.h"
#include "wx/panel.h"
#include "wx/bookctrl.h"
#include "wx/combobox.h"
#include "LoongGMTool.h"
#include "common.h"
#include "NoticePage.h"
#include "mylistctrl.h"


//�ؼ�ID
enum
{
	NoticePage_BTN_PMD = 100,
	NoticePage_Color_PMD,
	NoticePage_BTN_ZDXH,
	NoticePage_Color_ZDXH,
	NoticePage_List_ZDXH,
	NoticePage_OnlinePrompt,
	NoticePage_List_YXJ,
};

BEGIN_EVENT_TABLE(NoticePage, wxPanel)
END_EVENT_TABLE()

NoticePage::NoticePage(wxBookCtrlBase *book, const wxString& label) : m_label(label),wxPanel(book, wxID_ANY, wxDefaultPosition, wxDefaultSize, 
														  wxNO_FULL_REPAINT_ON_RESIZE | wxCLIP_CHILDREN | wxTAB_TRAVERSAL)
{
	//�޴�Ĺ��캯��
	wxSizer* sizerAll = new wxBoxSizer(wxHORIZONTAL);
	
	//����-��
	wxSizer* sizerLeft = new wxBoxSizer(wxVERTICAL);
	wxStaticBox *box1 = new wxStaticBox(this, wxID_ANY, wxT("�����"));
	wxSizer* sizerBox_pmd = new wxStaticBoxSizer(box1, wxVERTICAL);

	wxSizer* sizeRow = NULL;
	sizeRow = CreateSizerWithTextAndLabel(this, wxT("��������"), wxID_ANY, &m_text_pmd);
	sizerBox_pmd->Add(sizeRow, 0, wxGROW | wxALL, CONTROL_BORDER);

	wxSizer* sizerTmp = new wxBoxSizer(wxHORIZONTAL);
	sizerTmp->Add(CreateSizerWithTextAndComboBox(this, NoticePage_Color_PMD, wxT("��ɫ"), &m_combobox_color_pmd), 0, wxGROW | wxALL, CONTROL_BORDER);
	m_btnAdd_pmd = new wxButton(this, NoticePage_BTN_PMD, wxT("����"));
	sizerTmp->Add(m_btnAdd_pmd, 0, wxGROW | wxALL, CONTROL_BORDER);

	sizerBox_pmd->Add(sizerTmp, 0, wxGROW | wxALL, CONTROL_BORDER);

	sizerLeft->Add(sizerBox_pmd, 0, wxGROW | wxALL, CONTROL_BORDER);


	//����-��
	wxSizer* sizerMiddle = new wxBoxSizer(wxVERTICAL);
	
	wxStaticBox *box2 = new wxStaticBox(this, wxID_ANY, wxT("�Զ�ѭ������"));
	wxSizer* sizerBox_zdxh = new wxStaticBoxSizer(box2, wxVERTICAL);

	sizerBox_zdxh->Add(CreateSizerWithTextAndLabels(this, wxT("���ʱ��"), wxT("����"), wxID_ANY, &m_text_interval), 
			0, wxGROW | wxALL, CONTROL_BORDER);
	sizerBox_zdxh->Add(CreateSizerWithTextAndLabels(this, wxT("ѭ��ʱ��"), wxT("����"), wxID_ANY, &m_text_loop), 
			0, wxGROW | wxALL, CONTROL_BORDER);

	wxSizer* sizerTmp1 = new wxBoxSizer(wxHORIZONTAL);
	sizerTmp1->Add(CreateSizerWithTextAndComboBox(this, NoticePage_BTN_ZDXH, wxT("��ɫ"), &m_combobox_color_pmd), 0, wxGROW | wxALL, CONTROL_BORDER);
	m_btnAdd_zdxh = new wxButton(this, NoticePage_Color_ZDXH, wxT("���"));
	sizerTmp1->Add(m_btnAdd_zdxh, 0, wxGROW | wxALL, CONTROL_BORDER);
	sizerBox_zdxh->Add(sizerTmp1, 0, wxGROW | wxALL, CONTROL_BORDER);
	
	m_lctrl_zdxh = new MyListCtrl(this, NoticePage_List_ZDXH, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxSUNKEN_BORDER | wxLC_EDIT_LABELS);
	wxListItem itemCol;
	itemCol.SetText(wxT("��������"));
	m_lctrl_zdxh->InsertColumn(0, itemCol);

	itemCol.SetText(wxT("������ɫ"));
	itemCol.SetAlign(wxLIST_FORMAT_CENTER);
	m_lctrl_zdxh->InsertColumn(1, itemCol);

	itemCol.SetText(wxT("ѭ������ʱ��"));
	itemCol.SetAlign(wxLIST_FORMAT_CENTER);
	m_lctrl_zdxh->InsertColumn(2, itemCol);
	sizerBox_zdxh->Add(m_lctrl_zdxh, 1, wxGROW | wxALL, CONTROL_BORDER);

	sizerMiddle->Add(sizerBox_zdxh, 1, wxGROW | wxALL, CONTROL_BORDER);

	//����-��
	wxSizer* sizerRight = new wxBoxSizer(wxVERTICAL);
	wxStaticBox *box3 = new wxStaticBox(this, wxID_ANY, wxT("���½ǹ���"));
	wxSizer* sizerBox_yxj = new wxStaticBoxSizer(box3, wxVERTICAL);
	
	sizerBox_yxj->Add(CreateSizerWithTextAndLabel(this, wxT("����"), wxID_ANY, &m_text_title),
				0, wxGROW | wxALL, CONTROL_BORDER);
	sizerBox_yxj->Add(CreateSizerWithTextAndLabel(this, wxT("����"), wxID_ANY, &m_text_link),
				0, wxGROW | wxALL, CONTROL_BORDER);
	
	sizerBox_yxj->Add(CreateSizerWithTextAndLabel(this, wxT("��������"), wxID_ANY, &m_text_yxj),
				0, wxGROW | wxALL, CONTROL_BORDER);

	m_chkOnlinePrompt = CreateCheckBoxAndAddToSizer(this, sizerBox_yxj, wxT("������ʾ"), NoticePage_OnlinePrompt);

	wxSizer* sizerTmp5 = new wxBoxSizer(wxHORIZONTAL);
	sizerTmp5->Add(CreateSizerWithTextAndLabel(this, wxT("��"), wxID_ANY, &m_text_year), 1, wxGROW | wxALL, CONTROL_BORDER);
	sizerTmp5->Add(CreateSizerWithTextAndLabel(this, wxT("��"), wxID_ANY, &m_text_month), 1, wxGROW | wxALL, CONTROL_BORDER);
	sizerBox_yxj->Add(sizerTmp5, 0, wxGROW | wxALL, CONTROL_BORDER);

	wxSizer* sizerTmp6 = new wxBoxSizer(wxHORIZONTAL);
	sizerTmp6->Add(CreateSizerWithTextAndLabel(this, wxT("��"), wxID_ANY, &m_text_day), 1, wxGROW | wxALL, CONTROL_BORDER);
	sizerTmp6->Add(CreateSizerWithTextAndLabel(this, wxT("Сʱ"), wxID_ANY, &m_text_hour), 1, wxGROW | wxALL, CONTROL_BORDER);
	sizerBox_yxj->Add(sizerTmp6, 0, wxGROW | wxALL, CONTROL_BORDER);

	wxSizer* sizerTmp7 = new wxBoxSizer(wxHORIZONTAL);
	sizerTmp7->Add(CreateSizerWithTextAndLabel(this, wxT("��"), wxID_ANY, &m_text_min), 1, wxGROW | wxALL, CONTROL_BORDER);
	sizerTmp7->Add(CreateSizerWithTextAndLabel(this, wxT("��"), wxID_ANY, &m_text_sec), 1, wxGROW | wxALL, CONTROL_BORDER);
	sizerBox_yxj->Add(sizerTmp7, 0, wxGROW | wxALL, CONTROL_BORDER);

	m_lctrl_yxj = new MyListCtrl(this, NoticePage_List_YXJ, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxSUNKEN_BORDER | wxLC_EDIT_LABELS);
	itemCol.SetText(wxT("����"));
	m_lctrl_yxj->InsertColumn(0, itemCol);

	itemCol.SetText(wxT("����"));
	itemCol.SetAlign(wxLIST_FORMAT_CENTER);
	m_lctrl_yxj->InsertColumn(1, itemCol);

	itemCol.SetText(wxT("ʱ��"));
	itemCol.SetAlign(wxLIST_FORMAT_CENTER);
	m_lctrl_yxj->InsertColumn(2, itemCol);

	itemCol.SetText(wxT("������"));
	itemCol.SetAlign(wxLIST_FORMAT_CENTER); 
	m_lctrl_yxj->InsertColumn(3, itemCol);

	itemCol.SetText(wxT("������ʾ"));
	itemCol.SetAlign(wxLIST_FORMAT_CENTER);
	m_lctrl_yxj->InsertColumn(4, itemCol);
	sizerBox_yxj->Add(m_lctrl_yxj, 1, wxGROW | wxALL, CONTROL_BORDER);

	sizerRight->Add(sizerBox_yxj, 1, wxGROW | wxALL, CONTROL_BORDER);

	sizerAll->Add(sizerLeft, 0, wxGROW|wxALL, CONTROL_BORDER);
	sizerAll->Add(sizerMiddle, 0, wxGROW|wxALL, CONTROL_BORDER);
	sizerAll->Add(sizerRight, 1, wxGROW|wxALL, CONTROL_BORDER);

	SetSizer(sizerAll);
	sizerAll->Fit(this);
}