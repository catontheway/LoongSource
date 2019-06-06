#include "stdafx.h"
#include "RoleFloatWnd.h"

RoleFloatWnd::RoleFloatWnd( void )
{
}

BOOL RoleFloatWnd::Init( GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement )
{
	BOOL bResult = GUIStatic::Init(pSystem, pFather, pXmlElement);

	m_bUnClickable = TRUE;	// ����������
	return bResult;
}

VOID RoleFloatWnd::Destroy()
{
	GUIStatic::Destroy();
}

VOID RoleFloatWnd::Render()
{
	if( !m_bInvisible )
		return GUIStatic::Render();
}


tagPoint RoleFloatWnd::ChageFontSize( const TCHAR* szInfo, INT nWidth, INT nHeight, INT nWeight )
{
	if(m_nFontWidth==nWidth && m_nFontHeight==nHeight)
	{
		return m_ptSize;
	}

	if( P_VALID(m_pFont) )
		m_pRender->DestroyFont(m_pFont);
	m_pFont = m_pRender->CreateFont(g_StrTable[_T("Font_Hanyi")], nWidth, nHeight, nWeight);
	// ����ʹ�ø��ؼ�����
	if( !P_VALID(m_pFont) )	
		m_pFont = m_pRender->CloneFont(m_pFather->GetFont());

	m_nFontWidth = nWidth;
	m_nFontHeight = nHeight;

	tagPoint txtSize;	
	m_pRender->GetTextSize(szInfo, m_pFont, txtSize);

	//�ı������С����������������ؼ��Ĵ�С���µ���
	m_rcText.right = txtSize.x;
	m_rcText.bottom = txtSize.y;

	m_ptSize.x = txtSize.x;
	m_ptSize.y = txtSize.y;

	return txtSize;
}

