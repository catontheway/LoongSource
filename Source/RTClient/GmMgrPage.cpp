/////////////////////////////////////////////////////////////////////////////
// Program:     RTClient
// Name:        GmMgrPage.cpp
// Created:     2009-06-25
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GmMgrPage.h"
#include "RTServerManager.h"
#include "UIWords.h"
#include "..\ServerDefine\msg_gm_tool_c.h"

//�ؼ�ID
enum
{
	GmMgrPage_Create = 600,
	GmMgrPage_Modify,
	GmMgrPage_Delete,
	GmMgrPage_Query,

	GmMgrPage_Privilege,
};

BEGIN_EVENT_TABLE(GmMgrPage, wxPanel)
	EVT_BUTTON(GmMgrPage_Create, GmMgrPage::OnGMCreateOperation)
	EVT_BUTTON(GmMgrPage_Modify, GmMgrPage::OnGMModifyOperation)
	EVT_BUTTON(GmMgrPage_Delete, GmMgrPage::OnGMDeleteOperation)
	EVT_BUTTON(GmMgrPage_Query, GmMgrPage::OnGMQueryOperation)
END_EVENT_TABLE()

GmMgrPage::GmMgrPage( wxBookCtrlBase* book, const wxString label ) : label_(label),
	wxPanel(book, wxID_ANY, wxDefaultPosition, wxDefaultSize, 
	wxNO_FULL_REPAINT_ON_RESIZE | wxCLIP_CHILDREN | wxTAB_TRAVERSAL)
{
	//����Ȩ���ַ�������
	wxString strings[EP_End - EP_Pri_Rt_B];
	for (int n=1; n<=EP_End-EP_Pri_Rt_B; n++)
	{
		strings[n-1] << w_Privilege << (EP_Pri_Rt_B+n);
	}

	//�޴�Ĺ��캯��
	wxSizer* sizerAll = new wxBoxSizer(wxVERTICAL);

	//����-��
	wxSizer* sizerTop = new wxBoxSizer(wxVERTICAL);
	wxStaticBox *box = new wxStaticBox(this, wxID_ANY, w_CreateGM);
	wxSizer* sizerBox = new wxStaticBoxSizer(box, wxVERTICAL);

	//wxSizer* sizerTmp = new wxBoxSizer(wxHORIZONTAL);
	sizerBox->Add(CreateSizerWithTextAndLabel(this, w_GMAccount, wxID_ANY, &m_text_account),
		0,  wxALL, CONTROL_BORDER);
	sizerBox->Add(CreateSizerWithTextAndLabel(this, w_LoginPsd, wxID_ANY, &m_text_psw, true),
		0,  wxALL, CONTROL_BORDER);
	m_btn_create = new wxButton(this, GmMgrPage_Create, w_CreateGM);
	//sizerTmp->Add(CreateSizerWithTextAndComboBox(this, GmMgrPage_Privilege, wxT("GMȨ��"), &m_combox_Pr_cre, EP_End - EP_Pri_Rt_B, strings), 0, /*wxGROW |*/ wxALL, CONTROL_BORDER);
	sizerBox->Add(m_btn_create, 0, wxALL, CONTROL_BORDER);
	//sizerBox->Add(sizerTmp, 0, wxGROW | wxALL, CONTROL_BORDER);

	//����-��
	wxSizer* sizerMin = new wxBoxSizer(wxVERTICAL);
	wxStaticBox *boxMin = new wxStaticBox(this, wxID_ANY, w_ChangeBindIP);
	wxSizer* sizerBoxMin = new wxStaticBoxSizer(boxMin, wxVERTICAL);

	sizerBoxMin->Add(CreateSizerWithTextAndLabel(this, w_GMAccount, wxID_ANY, &m_text_account_modify),
		0,  wxALL, CONTROL_BORDER);
	sizerBoxMin->Add(CreateSizerWithTextAndLabel(this, w_IPBinded, wxID_ANY, &m_text_bind_ip),
		0,  wxALL, CONTROL_BORDER);

	m_btn_modify = new wxButton(this, GmMgrPage_Modify, w_ChangeBindIP);
	
	sizerBoxMin->Add(m_btn_modify, 0, wxALL, CONTROL_BORDER);

	//����-��
	wxSizer* sizerBom = new wxBoxSizer(wxVERTICAL);
	wxStaticBox *boxBom = new wxStaticBox(this, wxID_ANY, w_DeleteGM);
	wxSizer* sizerBoxBom = new wxStaticBoxSizer(boxBom, wxVERTICAL);
	wxSizer* sizerTmpBom = new wxBoxSizer(wxVERTICAL);
	sizerTmpBom->Add(CreateSizerWithTextAndLabel(this, w_GMAccount, wxID_ANY, &m_text_account_del),
		0,  wxALL, CONTROL_BORDER);
	m_btn_del = new wxButton(this, GmMgrPage_Delete, w_DeleteGM);
	sizerTmpBom->Add(m_btn_del, 0, wxALL, CONTROL_BORDER);
	sizerBoxBom->Add(sizerTmpBom, 0, wxGROW | wxALL, CONTROL_BORDER);

	sizerTop->Add(sizerBox, 1, wxGROW | wxALL, CONTROL_BORDER);
	sizerMin->Add(sizerBoxMin, 1, wxGROW | wxALL, CONTROL_BORDER);
	sizerBom->Add(sizerBoxBom, 1, wxGROW | wxALL, CONTROL_BORDER);

	sizerAll->Add(sizerTop, 1, wxGROW|wxALL, CONTROL_BORDER);
	sizerAll->Add(sizerMin, 1, wxGROW|wxALL, CONTROL_BORDER);
	sizerAll->Add(sizerBom, 1, wxGROW|wxALL, CONTROL_BORDER);
	SetSizer(sizerAll);
}

// ����GM
void GmMgrPage::OnGMCreateOperation( wxCommandEvent& event )
{
	wxString strName = m_text_account->GetValue();
	wxString strPsw = m_text_psw->GetValue();
	//EPrivilege ePri = (EPrivilege)(m_combox_Pr_cre->GetSelection());

	//�˺ź�����ĺϷ����ж�
	if (strName.empty())
	{
		wxMessageBox(w_NullAccount, w_Attention, wxOK|wxCENTRE|wxICON_ERROR, this);
		return;
	}
	else if (strPsw.empty())
	{
		wxMessageBox(w_NullPsd, w_Attention, wxOK|wxCENTRE|wxICON_ERROR, this);
		return;
	}
	//else if (ePri == EP_Null)
	//{
	//	wxMessageBox(wxT("��ѡ��GMȨ��"), wxT("����"), wxOK|wxCENTRE|wxICON_ERROR, this);
	//	return;
	//}

	//�������Ȩ���ж�
	if (!sRTServerManager.VerifyGMPrivilege(Crc32("NGMTC_GMCreate")))
	{
		wxMessageBox(w_NoAuthCreateGM, w_Attention, wxOK|wxCENTRE|wxICON_ERROR, this);
		return;
	}
	
	//��ʽ������Ϣ
	if (page_assist_ != NULL)
	{
		page_assist_->SendCreateGMMessage(strName.c_str(), strPsw.c_str(), EP_Pri_Rt_B);
		m_text_account->Clear();
		m_text_psw->Clear();
		//m_combox_Pr_cre->SetSelection(GT_INVALID);
	}
}

// �޸�GM��IP
void GmMgrPage::OnGMModifyOperation( wxCommandEvent& event )
{
	wxString strName = m_text_account_modify->GetValue();
	wxString strIP	= m_text_bind_ip->GetValue();
	//EPrivilege ePri = (EPrivilege)(m_combox_Pr_modify->GetSelection());

	//�˺ŵĺϷ����ж�
	if (strName.empty())
	{
		wxMessageBox(w_NullAccount, w_Attention, wxOK|wxCENTRE|wxICON_ERROR, this);
		return;
	}
	else if (strIP.empty())
	{
		wxMessageBox(w_NullIP, w_Attention, wxOK|wxCENTRE|wxICON_ERROR, this);
		return;
	}
	//else if (ePri == EP_Null)
	//{
	//	wxMessageBox(wxT("��ѡ��GMȨ��"), wxT("����"), wxOK|wxCENTRE|wxICON_ERROR, this);
	//	return;
	//}

	//�ж�IP�ĸ�ʽ�Ƿ�Ϸ�
	CHAR szIP[X_LONG_STRING] = {0};
	UnicodeToUnicode8(strIP.c_str(), szIP);
	DWORD dwIP = sRTServerManager.IsIPValid(szIP);
	if (dwIP == INADDR_NONE)
	{
		wxMessageBox(w_InvalidIP, w_Attention, wxOK|wxCENTRE|wxICON_ERROR, this);
		return;
	}

	//�������Ȩ���ж�
	if (!sRTServerManager.VerifyGMPrivilege(Crc32("NRTC_BindIP")))
	{
		wxMessageBox(w_NoAuthBindIP, w_Attention, wxOK|wxCENTRE|wxICON_ERROR, this);
		return;
	}

	//������Ϣ
	if (page_assist_ != NULL)
	{
		page_assist_->SendModifyGMMessage(strName.c_str(), dwIP);
		m_text_account_modify->Clear();
		m_text_bind_ip->Clear();
		//m_combox_Pr_modify->SetSelection(GT_INVALID);
	}
}

// ɾ��GM
void GmMgrPage::OnGMDeleteOperation( wxCommandEvent& event )
{
	wxString strName = m_text_account_del->GetValue();

	//�˺ŵĺϷ����ж�
	if (strName.empty())
	{
		wxMessageBox(w_NullAccount, w_Attention, wxOK|wxCENTRE|wxICON_ERROR, this);
		return;
	}

	//�������Ȩ���ж�
	if (!sRTServerManager.VerifyGMPrivilege(Crc32("NGMTC_GMDelete")))
	{
		wxMessageBox(w_NoAuthDeleteGM, w_Attention, wxOK|wxCENTRE|wxICON_ERROR, this);
		return;
	}

	//������Ϣ
	if (page_assist_ != NULL)
	{
		page_assist_->SendDeleteGMMessage(strName.c_str());
		m_text_account_del->Clear();
	}
}

// 7��GM��ѯ
void GmMgrPage::OnGMQueryOperation( wxCommandEvent& event )
{
	//�������Ȩ���ж�
	if (!sRTServerManager.VerifyGMPrivilege(Crc32("NRTC_GMQuery")))
	{
		wxMessageBox(w_NoAuthQuery, w_Attention, wxOK|wxCENTRE|wxICON_ERROR, this);
		return;
	}

	//������Ϣ
	if (page_assist_ != NULL)
	{
		page_assist_->SendQueryGMMessage();
		//m_text_account_Query->Clear();
	}

}