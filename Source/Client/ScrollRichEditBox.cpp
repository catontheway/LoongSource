#include "StdAfx.h"
#include "ScrollRichEditBox.h"

ScrollRichEditBox::ScrollRichEditBox(void)
{
}

ScrollRichEditBox::~ScrollRichEditBox(void)
{
}

BOOL ScrollRichEditBox::Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement)
{
	BOOL bRes = GUIRichEditBox::Init(pSystem, pFather, pXmlElement);
	if(!bRes)
		return bRes;

	//��ʼ�����ֹ����ٶ�
	m_fSclSpeed = 50.0f;
	m_fSclTimeCount = 0.0f;
	//��ʼ������ʱ��
	m_fSclTimeStart = 2.0f;
	m_bNextText = true;

	m_bUnClickable = TRUE;
	return bRes;
}

VOID ScrollRichEditBox::Render()
{
	if( IsInvisible() == TRUE )
		return;
	GUIRichEditBox::Render();
}


VOID ScrollRichEditBox::Update()
{
	if( IsInvisible() == TRUE )
		return;

	float deltaTime =  Cool3D::Kernel::Inst()->GetDeltaTime();

	if(m_fSclTimeStart > 0.0f)
		m_fSclTimeStart -= deltaTime;
	else
	{
		m_fSclTimeCount -= deltaTime;							
		if(m_fSclTimeCount < 0.0f && m_ptView.x+m_ptSize.x < 0)	//�������ʱ�䵽��,��������ܳ����ؼ������
		{
			ClsText();
			SetInvisible(TRUE);									//�������������
			m_bNextText = true;
			return;
		}

		if( m_ptView.x+m_ptSize.x < 0)			//ȫ���������Ϣ���ܳ����ؼ����������
		{
			SetClient(tagPoint(m_lnSlcFatherWndX, m_lnSlcFatherWndY));		//�ָ�����ƿ�ʼ��λ��Ϊ���Ҷ�
		}
		SetMovable(TRUE);
		INT nSpeed = (INT)(deltaTime*m_fSclSpeed);
		if( nSpeed<=0 )
			nSpeed=1;
		Move(tagPoint(-nSpeed, 0));
		SetMovable(FALSE);
	}
		
	GUIRichEditBox::Update();	
}


BOOL ScrollRichEditBox::PushScrollText(LPCTSTR szText, float fTime)
{
	BOOL bRes = FALSE;
	if( szText==NULL )
		return bRes;

	m_fSclTimeStart = 0.0f;
	SetClient(tagPoint(m_lnSlcFatherWndX, m_lnSlcFatherWndY));		//��������ƿ�ʼ��λ��	
	RefreshViewCoord();
	SetScrollTime(fTime);
	
	ClsText();
	m_ptSize.x = 1024;
	m_rcText.left = 0;
	m_rcText.right = m_ptSize.x;

	bRes = AddText(szText);

	//ÿ�ζ����¼�������Ƶĳ���,�������Է�ֹ�����������Ĵ�������,��ʾ������������
	INT nRowW = this->GetRowRenderWidth(0);
	if( nRowW > 0 )
	{
		this->SetWidth(nRowW+14);
	}

	m_fSclSpeed = (float)((m_lnSlcFatherWndX+nRowW+14)/(fTime/3));

	m_bNextText = false;
	SetInvisible(FALSE);								//�����������ʾ
	SetRefresh();
	return bRes;
}
