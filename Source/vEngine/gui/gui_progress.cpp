//-----------------------------------------------------------------------------
//!\file gui_progress.cpp
//!\author Lyp
//!
//!\date 2004-12-08
//! last 2004-12-28
//!
//!\brief ����ϵͳ progress �ؼ�
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "gui_static.h"
#include "gui_progress.h"


namespace vEngine {
//-----------------------------------------------------------------------------
// constructor / destructor
//-----------------------------------------------------------------------------
GUIProgress::GUIProgress()
{
	// ע�����пɱ༭����
	m_pVarMgr->Register(_T("Horizon"), &m_bHorizon, _T("no yes"));
	m_pVarMgr->Register(_T("ForePicFileName"), &m_strForePicFileName);
	m_pVarMgr->Register(_T("ForePicRect"), &m_rcForePic);
	m_pVarMgr->Register(_T("ForePicColor"), &m_dwForePicColor);

	// ����Ĭ������
	m_bHorizon = TRUE;
	m_dwForePicColor = 0xFFFFFFFF;
}


//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL GUIProgress::Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement)
{
	BOOL bResult = GUIStatic::Init(pSystem, pFather, pXmlElement);

	m_fMaxValue = 1.0f;
	m_fCurrentValue = 0.0f;
	m_fTargetValue = 0.0f;
	m_fSpeed = 1.0f;
	m_dwLastTime = timeGetTime();

	// ����ͼƬ
	m_pForePic = m_pRender->CreateImage(m_strForePicFileName, m_rcForePic);

	return bResult;
}



//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
VOID GUIProgress::Destroy()
{
	// ɾ��ͼƬ
	m_pRender->DestroyImage(m_pForePic);

	GUIStatic::Destroy();
}


//-----------------------------------------------------------------------------
// update
//-----------------------------------------------------------------------------
VOID GUIProgress::Update()
{
	GUIStatic::Update();

	DWORD dwTime = timeGetTime() - m_dwLastTime;
	m_dwLastTime = timeGetTime();

	if( m_fCurrentValue < m_fTargetValue )
	{
        m_fCurrentValue += m_fSpeed * (FLOAT)dwTime / 1000.0f; 
		if( m_fCurrentValue >= m_fTargetValue )
			m_fCurrentValue = m_fTargetValue;
		SetRefresh();
	}

	if( m_fCurrentValue > m_fTargetValue )
	{
        m_fCurrentValue -= m_fSpeed * (FLOAT)dwTime / 1000.0f; 
		if( m_fCurrentValue <= m_fTargetValue )
			m_fCurrentValue = m_fTargetValue;
		SetRefresh();
	}
}



//-----------------------------------------------------------------------------
// render
//-----------------------------------------------------------------------------
VOID GUIProgress::Render()
{
	if( !IsRefresh() )
		return GUIWnd::Render();

	// ��Ⱦ��ͼ
	m_pCanvas->rc.SetLTWH(m_ptView, m_ptSize);
	m_pRender->Draw(m_pCanvas, m_pPic, m_dwPicColor, m_eAlphaWrite);

	if( m_bHorizon )	// ����
	{
		INT nOffset = (INT)(m_fCurrentValue * (FLOAT)m_ptSize.x / m_fMaxValue);
		m_pCanvas->rc.SetLTWH(m_ptView.x, m_ptView.y, nOffset, m_ptSize.y);
		m_pRender->Draw(m_pCanvas, m_pForePic, m_dwForePicColor, m_eAlphaWrite);
	}
	else	// ����
	{
		INT nOffset = (INT)(m_fCurrentValue * (FLOAT)m_ptSize.y / m_fMaxValue);
		m_pCanvas->rc.SetLTWH(m_ptView.x, m_ptView.y + m_ptSize.y - nOffset, m_ptSize.x, nOffset);
		m_pRender->Draw(m_pCanvas, m_pForePic, m_dwForePicColor, m_eAlphaWrite);
	}

	// ��Ⱦ�����ַ���
	m_pCanvas->rc = m_rcText + m_ptView;
	m_pRender->Text(m_pCanvas, m_strText, m_pFont, m_dwTextColor, m_dwShadeColor, m_eTextAlign, m_bTextWriteAlpha);

	// ���û�����Ⱦ
	GUIWnd::Render();
}



//-----------------------------------------------------------------------------
// set value
//-----------------------------------------------------------------------------
VOID GUIProgress::SetValue(FLOAT fValue,bool bForceSet)
{ 
	m_fTargetValue = fValue; 
	if( m_fTargetValue < 0 )
		m_fTargetValue = 0;

	if( m_fTargetValue > m_fMaxValue )
		m_fMaxValue = m_fTargetValue;

	if(bForceSet)
		m_fCurrentValue=m_fTargetValue;
	SetRefresh();
} 


//-----------------------------------------------------------------------------
// set max value
//-----------------------------------------------------------------------------
VOID GUIProgress::SetMaxValue(FLOAT fMaxValue)
{ 
	if( fMaxValue < 0.5f )
		return;

	m_fMaxValue = fMaxValue;
	if( m_fTargetValue > m_fMaxValue )
		m_fTargetValue = m_fMaxValue;

	if( m_fCurrentValue > m_fMaxValue )
		m_fCurrentValue = m_fMaxValue;
	
	SetRefresh();
}

}	// namespace vEngine {
