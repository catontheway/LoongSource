//-----------------------------------------------------------------------------
//!\file xui_richeditbox.cpp
//!\author tidy / bwen / zewu / lyp
//!
//!\date 2005-4-13
//! last 2008-1-17
//!
//!\brief ����ϵͳ richeditbox �ؼ�
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "gui_static.h"
#include "gui_richeditbox.h"
#include "gui_event_define.h"

#define CURSOR_FLICKER_TIME 500  // �����˸���

#define GUI_MAX_TRANSFER_LEN	50
#define GUI_DEFAULT_ROW_SPACING_ADDON 0

#define SET_FONT_SIZE	 0
#define SET_FONT_COLOR   1
#define SET_FONT_STYLE	 2
#define SET_KEY_START	 3     //���ùؼ���  �ؼ��ֲ��ܳ����������л���100�ֽ�
#define SET_KEY_END		 4
#define SET_LINK_START	 5	   //���ó�������Ϣ
#define SET_LINK_END     6     

namespace vEngine {
//-----------------------------------------------------------------------------
// construction
//-----------------------------------------------------------------------------
GUIRichEditBox::GUIRichEditBox()
{
	m_pVarMgr->Register(_T("Disable"), &m_bDisable, _T("no yes"));
	m_pVarMgr->Register(_T("ReadOnly"), &m_bReadOnly, _T("no yes"));
	m_pVarMgr->Register(_T("CursorColor"), &m_dwCursorColor);
	m_pVarMgr->Register(_T("CursorTimer"), &m_dwCursorTimer);
	m_pVarMgr->Register(_T("RowSpacing"), &m_nRowSpacing);
	m_pVarMgr->Register(_T("MaxEditTextCount"), &m_nMaxEditTextCount);

	m_dwCursorColor = 0xFF000000;
	m_dwCursorTimer = 100;
	m_nMaxEditTextCount = 20480;
	m_bEditable = TRUE;
}


//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL GUIRichEditBox::Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement)
{
	BOOL bResult = GUIStatic::Init(pSystem, pFather, pXmlElement);

	m_VecCharIntro.clear();

	m_bScrollLast = TRUE;
	m_bEqualTextSize = FALSE;

	// �õ��ı��߶�
	tagPoint ptSize;
	m_pRender->GetTextSize(_T("��"), m_pFont, m_ptSizeChinese);
	m_pRender->GetTextSize(_T("a"), m_pFont, m_ptSizeLetter);

	m_nTextNormalHeight = m_ptSizeChinese.y;

	if( 0 == m_nRowSpacing )	// ���δ�����и�������һ��
	{
		// ���һ���ı��ĸ߶ȣ�������GUI_DEFAULT_ROW_SPACING_ADDON
		m_nRowSpacing = m_nTextNormalHeight + GUI_DEFAULT_ROW_SPACING_ADDON;
	}

	m_bShowCursor = FALSE;
	m_bHoldCursor = TRUE;
	m_bCanSelect  = TRUE;

	m_bMiddleRender = FALSE;
	m_bDownLine = FALSE;
	m_dwLinkColor = GT_INVALID;

	m_nSelectStart = 0;	// ѡ��ʼ��
	m_nSelectEnd = 0;// ѡ�������
	m_nCursorPos = 0;	// ���λ��
	m_nStartDisplayLine = 0;	// ��ʾ�ĵ�һ���ַ�����ʵ�ʵĵڼ����ַ�
	
	m_dwMaxTextCount = 100;
	m_VecCharIntro.resize(m_dwMaxTextCount+2);

	m_strText.clear();
	m_strText.push_back(0x0d);
	m_strText.push_back(0x0a);
	m_VecCharIntro[0].nByte = 2;
	m_VecCharIntro[0].dwHeight = m_nRowSpacing;
	m_VecCharIntro[1].nByte = -1;
	SendScrollEvent();

	//��ʼ��ע����������ת���ַ���
	m_mapFont.Add(_T("<color="), SET_FONT_COLOR); // <color=xxx> ��<color=(xxx,xxx,xxx)>
	m_mapFont.Add(_T("<="), SET_FONT_COLOR); // <color=xxx> ��<color=(xxx,xxx,xxx)>

	//���size>1000��ô���彫������Ϊб��,ʵ�ʴ�СΪsize-1000,if = 0,��ΪĬ�������С
	m_mapFont.Add(_T("<size="), SET_FONT_SIZE);
	m_mapFont.Add(_T("<style="), SET_FONT_STYLE);

	//���ùؼ��ָ�ʽ(<keystart>�ؼ���<keyend>
	m_mapKey.Add(_T("<keystart>"), SET_KEY_START);
	m_mapKey.Add(_T("<keyend>"), SET_KEY_END);

	//���ó����Ӹ�ʽ
	m_mapLink.Add(_T("<linkstart="), SET_LINK_START);   //<linkstart=xxxxxx>yyyyy<linkend>
	//xxxxx��ʾ����Ŀ����Ϣ,yyyy��ʾ��ʾ��������Ϣ
	m_mapLink.Add(_T("<linkend>"), SET_LINK_END);

	m_dwLastMoveTime = GetTickCount();
	m_dwTransDepth = 0xff000000;

	//test
	AddText(_T("<=(146,243,255)>Series Quest: Sansheng Stone - The Truth in Dreams (2/8)\n	Fortuneteller Chunyun said you need to find the Sansheng Stone to help Zhou Lingyun. Bring this information to Zhou Lingyun in Liuhe (531,470)."));
	AddText(_T("Yicheng Yamen Watchman (2/3)\nReturn to the Yicheng Yamen Watchman <<=(85,245,183)><linkstart=0>Yicheng,662,261<linkend><=(146,243,255)>>."));


	return bResult;
}

//-----------------------------------------------------------------------------
//����
//-----------------------------------------------------------------------------
VOID GUIRichEditBox::Destroy()
{
	m_mapPic.ResetIterator();
	tagGUIImage* pPic = NULL;
	while(m_mapPic.PeekNext(pPic))
	{
		m_pRender->DestroyImage(pPic);
	}

	GUIStatic::Destroy();
}

//-----------------------------------------------------------------------------
// update
//-----------------------------------------------------------------------------
VOID GUIRichEditBox::Update()
{
	static DWORD dwLastTime = timeGetTime();
	// ���������ʾ����˸
	if( m_pSystem->GetActive() == this )
	{
		if( timeGetTime() - dwLastTime > m_dwCursorTimer )
		{
			m_bShowCursor = !m_bShowCursor;
			dwLastTime = timeGetTime();
			this->SetRefresh();
		}
	}

	GUIStatic::Update();
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VOID GUIRichEditBox::Render()
{
	if( !IsRefresh() )
		return GUIWnd::Render();

	// ��Ⱦ��ͼ
	m_pCanvas->rc.SetLTWH(m_ptView, m_ptSize);
	m_pRender->Draw(m_pCanvas, m_pPic, m_dwPicColor, m_eAlphaWrite);

	if( m_bCanSelect == FALSE  )
		m_nSelectEnd = m_nSelectStart;

	INT nS1 = m_nSelectStart;
	INT nS2 = m_nSelectEnd;
	if( m_nSelectStart > m_nSelectEnd )
	{
		std::swap(nS1, nS2);
	}

	//��ʼ���������
	INT nLine = 0;
	INT nHeigth = m_rcText.top;	//��top�����Ѿ����˵ĸ߶�
	INT nRowHeight = 0;	  //���� ��Ҫ����ʾ�߶�
	INT nIntelHeight = 0; //����ʵ�ʿ��õĸ߶�

	INT nLeft = m_ptView.x + m_rcText.left; //����
	INT nCursorLeft = m_ptView.x + m_rcText.left;

	INT nTextSize = m_strText.size();
	{
		//���ҳ�ʼ�еĸ߶�
		INT nPos = m_strText.find_first_of(0x0d, 0);
		ASSERT( nPos >= 0 );
		nRowHeight = m_VecCharIntro[nPos].dwHeight;

		if( TRUE == m_bMiddleRender )
		{
			//���ж��룬�����г�ʼ��Ⱦλ��
			nLeft += (m_rcText.right - m_rcText.left - m_VecCharIntro[nPos].nCharSize) / 2;
			nCursorLeft += (m_rcText.right - m_rcText.left - m_VecCharIntro[nPos].nCharSize) / 2;
		}

		if( m_rcText.bottom - m_rcText.top -nHeigth >= nRowHeight )
			nIntelHeight = nRowHeight;
		else
			nIntelHeight = m_rcText.bottom - m_rcText.top - nHeigth;

		if( m_nStartDisplayLine > nLine )
		{
			nIntelHeight = 0;
		}
	}

	//������������
	DWORD dwTextColor = m_dwTextColor;	 
	tagGUIFont* pFont = m_pFont;
	DWORD dwTextShade = m_dwShadeColor;
	dwTextColor |= m_dwTransDepth;

	INT nStep = 1;
	TCHAR szTemp[3];
	ZeroMemory(szTemp, sizeof(szTemp));

	//������Ⱦ���
	tstring strRender;
	INT nTextWidth = 0;
	BOOL bTransparent = FALSE;
	INT nTextHeight = 0;


	BOOL bLink = FALSE;		//ָʾ��ǰ�Ƿ��ǳ�����
	DWORD dwLinkColor = GT_INVALID;		//��ǰ��������꾭����������ɫ
	//�ؼ��ֵ��»������
	INT nDownLineLeft = -1;
	INT nDownLineWidth = 0;

	//��Ⱦ��ѭ��
	for(INT n=0; n<nTextSize; n+=nStep)
	{
		szTemp[0] = m_strText[n];
		nStep = m_VecCharIntro[n].nByte;

		if( m_bHoldCursor && m_bShowCursor && m_nCursorPos == n )	//��ʾ���
		{
			m_pCanvas->rc.left  = nCursorLeft-1;
			m_pCanvas->rc.right  = m_pCanvas->rc.left + 2;
			m_pCanvas->rc.bottom = nHeigth + nIntelHeight + m_ptView.y;
			m_pCanvas->rc.top = m_pCanvas->rc.bottom - m_nTextNormalHeight;
			m_pRender->Draw(m_pCanvas, NULL, m_dwCursorColor, m_eAlphaWrite);
		}

		if( nStep == -1 || nStep == 0 )
			break;

		if( szTemp[0] == 0x0d )	//���з�
		{
			//����֮ǰ����ʾ֮ǰ������
			if( strRender.size() > 0 )
			{
				INT nbottom = nHeigth + m_ptView.y + nIntelHeight;
				INT nRHeight = 0;

				if( nIntelHeight >= m_nTextNormalHeight )
					nRHeight = m_nTextNormalHeight;
				else
					nRHeight = nIntelHeight;

				if( GT_INVALID == dwLinkColor )
				{
					RenderText(strRender.c_str(), nLeft, nbottom, nTextWidth, 
						nRHeight, dwTextColor, pFont, bTransparent);
				}
				else
				{
					RenderText(strRender.c_str(), nLeft, nbottom, nTextWidth, 
						nRHeight, dwLinkColor, pFont, bTransparent);
				}

				nLeft += nTextWidth;

				strRender.clear();
				nTextWidth = 0;
			}

			//��ʾ�ؼ�����Ҫ��ʾ���»���
			if( (TRUE == m_bDownLine || TRUE == bLink)
				&& nDownLineWidth > 0 )
			{
				m_pCanvas->rc.left = nDownLineLeft;
				m_pCanvas->rc.top = nHeigth + m_ptView.y + nIntelHeight;
				m_pCanvas->rc.right = nDownLineLeft+nDownLineWidth;
				m_pCanvas->rc.bottom = nHeigth + m_ptView.y + nIntelHeight;
				m_pRender->Line(m_pCanvas, dwTextColor);

				nDownLineLeft = m_ptView.x + m_rcText.left;
				nDownLineWidth = 0;
			}

			nLine++;	//����һ��
			if( m_nStartDisplayLine > nLine )
			{
				nIntelHeight = 0;
				continue;
			}

			// ��ʼ���µ�һ��-----------------------------------------------------------
			// ������õ���ʼλ��
			nLeft = m_ptView.x + m_rcText.left; 
			nCursorLeft = m_ptView.x + m_rcText.left;

			//->add by bwen
			if(nDownLineLeft!=-1)
				nDownLineLeft = m_ptView.x + m_rcText.left;
			//<-

			//����һ���еĸ߶�
			if( n+1 < nTextSize )
			{
				INT nPos = m_strText.find_first_of(0x0d, n+1);
				if( nPos > 0 )
				{
					nRowHeight = m_VecCharIntro[nPos].dwHeight;

					if( TRUE == m_bMiddleRender )
					{
						//���ж��룬�����г�ʼ��Ⱦλ��
						nLeft += (m_rcText.right-m_rcText.left - m_VecCharIntro[nPos].nCharSize) / 2;

						if( nDownLineLeft != -1 )
							nDownLineLeft = nLeft;	 //��������

						nCursorLeft += (m_rcText.right-m_rcText.left - m_VecCharIntro[nPos].nCharSize) / 2;
					}
				}
			}

			nHeigth += nIntelHeight;

			if( nHeigth >= m_rcText.bottom || m_rcText.bottom-nHeigth <= GUI_DEFAULT_ROW_SPACING_ADDON ) 
				break;	//�Ѿ�û�и߶ȿ���ʾ������

			if( m_rcText.bottom-nHeigth >= nRowHeight )
				nIntelHeight = nRowHeight;
			else
			{
				//���õĸ߶Ȳ���,������ȫ��ʾ�������ֻ���ͼƬ
				//--ע�����϶����ڲ�ȥ�����һ�л���zewu 06-3-1
				//--ע2������bug������ȥ�����һ���ˣ�yanliang 06-05-11
				break;// û�п��õĸ߶ȾͲ�Ҫ����һ����Ⱦ����
			}
		}
		else if( szTemp[0] < 0 || nStep == 1 ) //������
		{
			if( m_nStartDisplayLine > nLine )	
				continue;

			DWORD bTrans = TRUE;
			if( n >= nS1 && n < nS2 )	// ����ѡ���������Ⱦ
			{
				bTrans = FALSE;
			}

			if( bTransparent != bTrans && nTextWidth != 0 )
			{
				//�Ƿ������ʾ
				INT nbottom = nHeigth + m_ptView.y + nIntelHeight;
				INT nRHeight = 0;

				if( nIntelHeight >= m_nTextNormalHeight )
					nRHeight = m_nTextNormalHeight;
				else
					nRHeight = nIntelHeight;

				if( GT_INVALID == dwLinkColor )
				{
					RenderText(strRender.c_str(), nLeft, nbottom, nTextWidth, 
						nRHeight, dwTextColor, pFont, bTransparent);
				}
				else
				{
					RenderText(strRender.c_str(), nLeft, nbottom, nTextWidth, 
						nRHeight, dwLinkColor, pFont, bTransparent);
				}

				nLeft += nTextWidth;
				strRender.clear();
				nTextWidth = 0;
			}

			bTransparent = bTrans;

			strRender.push_back(szTemp[0]);
			if( szTemp[0] < 0 )
			{
				//˫�ֽ��ַ�
				ASSERT( nStep == 2 );
				strRender.push_back( m_strText[n+1] );
			}

			nTextWidth += m_VecCharIntro[n].nCharSize;
			nTextHeight = m_VecCharIntro[n].dwHeight;
			nCursorLeft += m_VecCharIntro[n].nCharSize;

			//�»��ߵ�ͳ��
			if( nDownLineLeft != -1 )
			{
				nDownLineWidth += m_VecCharIntro[n].nCharSize;
			}
		}
		else if( nStep > 1 )
		{
			//ת���ַ���֮ǰ,��������Ⱦ֮ǰ������
			if( nTextWidth > 0 )
			{
				INT nbottom = nHeigth + m_ptView.y + nIntelHeight;
				INT nRHeight = 0;

				if( nIntelHeight >= m_nTextNormalHeight )
					nRHeight = m_nTextNormalHeight;
				else
					nRHeight = nIntelHeight;

				if( GT_INVALID == dwLinkColor )
				{
					RenderText(strRender.c_str(), nLeft, nbottom, nTextWidth, 
						nRHeight, dwTextColor, pFont, bTransparent);
				}
				else
				{
					RenderText(strRender.c_str(), nLeft, nbottom, nTextWidth, 
						nRHeight, dwLinkColor, pFont, bTransparent);
				}

				nLeft += nTextWidth;
				strRender.clear();
				nTextWidth = 0;
			}

			// ת���ַ���
			if( 0 == m_VecCharIntro[n].nCharSize )
			{
				//��ռ��ȣ�˵������������
				INT nFont = (INT)(m_VecCharIntro[n].pPic);
				if( nFont == SET_FONT_COLOR )
				{
					dwTextColor = m_VecCharIntro[n].dwReserve;
					dwTextColor |= m_dwTransDepth;
				}
				else if( nFont == SET_FONT_SIZE )
					pFont = (tagGUIFont*)m_VecCharIntro[n].dwReserve;
				else if( nFont == SET_FONT_STYLE )
					dwTextShade = m_VecCharIntro[n].dwReserve;
				else if( nFont == SET_KEY_START )
				{
					//��ʼ���»���
					nDownLineLeft = nCursorLeft;
					nDownLineWidth = 0;

				}
				else if( nFont == SET_KEY_END )
				{
					//���»���

					//��ʾ�ؼ�����Ҫ��ʾ���»���
					if( TRUE == m_bDownLine && nDownLineWidth > 0 )
					{
						m_pCanvas->rc.left = nDownLineLeft;
						m_pCanvas->rc.top = nHeigth + m_ptView.y + nIntelHeight;
						m_pCanvas->rc.right = nDownLineLeft+nDownLineWidth;
						m_pCanvas->rc.bottom = nHeigth + m_ptView.y + nIntelHeight;
						m_pRender->Line(m_pCanvas, dwTextColor);
					}

					//���³�ʼ��
					nDownLineLeft = -1;
					nDownLineWidth = 0;

				}
				else if( nFont == SET_LINK_START )
				{
					bLink = TRUE;	//�����ӿ�ʼ
					if( n == m_nMouseLinkIndex )
					{
						//��ǰ��괦�ڴ˳����ӵ�λ��
						dwLinkColor = m_dwLinkColor;
					}

					//��ʼ���»���
					nDownLineLeft = nCursorLeft;
					nDownLineWidth = 0;	
				}
				else if( nFont == SET_LINK_END )
				{

					if( nDownLineWidth > 0 )
					{
						m_pCanvas->rc.left = nDownLineLeft;
						m_pCanvas->rc.top = nHeigth + m_ptView.y + nIntelHeight;
						m_pCanvas->rc.right = nDownLineLeft+nDownLineWidth;
						m_pCanvas->rc.bottom = nHeigth + m_ptView.y + nIntelHeight;
						m_pRender->Line(m_pCanvas, dwTextColor);
					}

					bLink = FALSE;	 //�����ӽ���
					dwLinkColor = GT_INVALID;

					//���³�ʼ���»�������
					nDownLineLeft = -1;
					nDownLineWidth = 0;
				}


				continue;
			}

			if( m_nStartDisplayLine > nLine )
				continue;

			//��Ⱦ��ͼƬ
			m_VecCharIntro[n].pPic->rc.left = 0;
			m_pCanvas->rc.left = nLeft;
			m_VecCharIntro[n].pPic->rc.right = m_VecCharIntro[n].pPic->rc.left + m_VecCharIntro[n].pPic->ptSize.x;
			m_pCanvas->rc.right = m_pCanvas->rc.left + m_VecCharIntro[n].nCharSize;
			m_VecCharIntro[n].pPic->rc.top = 0;

			if( nIntelHeight >= (INT)m_VecCharIntro[n].pPic->ptSize.y )
			{
				m_pCanvas->rc.top = nHeigth + m_ptView.y + GUI_DEFAULT_ROW_SPACING_ADDON + \
					nRowHeight - m_VecCharIntro[n].pPic->ptSize.y;
				m_VecCharIntro[n].pPic->rc.bottom = m_VecCharIntro[n].pPic->rc.top + m_VecCharIntro[n].pPic->ptSize.y;
				m_pCanvas->rc.bottom = nHeigth + m_ptView.y + nIntelHeight;
			}
			else
			{
				m_pCanvas->rc.top = nHeigth + m_ptView.y + GUI_DEFAULT_ROW_SPACING_ADDON;
				m_VecCharIntro[n].pPic->rc.bottom = m_VecCharIntro[n].pPic->rc.top + nIntelHeight;
				m_pCanvas->rc.bottom = nHeigth + m_ptView.y + nIntelHeight;
			}

			// ����ѡ���������Ⱦ����ͼƬ���������ɫ���ʾ��ѡ��,add by zewu 2005-2-9 20:49
			if( n >= nS1 && n < nS2 )	
				m_pRender->Draw(m_pCanvas, NULL, 0xFFFFFFFF, m_eAlphaWrite);//ע����Ⱦ˳�򣬱�������ȾͼƬ֮ǰ������Ϊ����ɫ

			m_pRender->Draw(m_pCanvas, m_VecCharIntro[n].pPic, m_dwPicColor|0xFF000000, EGUIAW_Add);

			nLeft += m_VecCharIntro[n].nCharSize;
			nCursorLeft += m_VecCharIntro[n].nCharSize;

			//�»��ߵ�ͳ��
			if( nDownLineLeft != -1 )
				nDownLineWidth += m_VecCharIntro[n].nCharSize;
		}
		//....
	}

	return GUIWnd::Render();
}


//-----------------------------------------------------------------------------
//��Ⱦ����
//-----------------------------------------------------------------------------
VOID GUIRichEditBox::RenderText(LPCTSTR szText, INT nLeft, INT nBottom, INT nWidth, INT nHeight,
					   DWORD dwColor, tagGUIFont* pFont, BOOL bTransparent)
{
	m_pCanvas->rc.left = nLeft;
	m_pCanvas->rc.right = m_pCanvas->rc.left + nWidth;
	m_pCanvas->rc.bottom = nBottom;
	m_pCanvas->rc.top = m_pCanvas->rc.bottom - nHeight;

	DWORD dwMyColor = dwColor;
	if( FALSE == bTransparent )	// ����ѡ���������Ⱦ
	{
		DWORD dwAlpha = dwColor & 0xff000000;
		dwMyColor = ( (~dwColor) & (0x00ffffff) ); // ��ɫ
		dwMyColor |= dwAlpha;//��֤Alphaֵ����

		m_pRender->Draw(m_pCanvas, NULL, dwColor, m_eAlphaWrite);
		m_pRender->Text(m_pCanvas, szText, pFont, dwMyColor, m_dwShadeColor, m_eTextAlign, m_bTextWriteAlpha);
	}
	else
	{
		m_pRender->Text(m_pCanvas, szText, pFont, dwColor, m_dwShadeColor, m_eTextAlign, m_bTextWriteAlpha);
	}

}

//-----------------------------------------------------------------------------
// ������Ϣ����
//-----------------------------------------------------------------------------
BOOL GUIRichEditBox::OnInputMessage(tagGUIInputMsg* pMsg)
{
	switch( pMsg->dwType )
	{
	case WM_LBUTTONDOWN:
		{
			m_bShowCursor = TRUE;
			// �ҵ���������ַ�
			INT nChar = GetCharIndex(pMsg->pt.x, pMsg->pt.y, FALSE);
			if( GT_INVALID != nChar )
			{
				m_nSelectStart = m_nSelectEnd = m_nCursorPos = nChar;	// ȡ��ѡ��

				while( m_nCursorPos > GetLastDisplayIndex() )
					m_nStartDisplayLine++;
			}

			//�Ƿ���������
			BOOL bLink = ClickLink(pMsg);
			//�ı���
			AdptiveCursor(TRUE, bLink);
			this->SetRefresh();
		}
		break;
	case WM_LBUTTONUP:
		{
			//�Ƿ���������
			//BOOL bLink = ClickLink(pMsg);
		}
		break;

	case WM_LBUTTONDBLCLK:
		{	//˫��
			if( m_bCanSelect == TRUE )
			{
				//����˫��������,�������ֿ�ѡ
				CalSelectForDoubleClick();
				this->SetRefresh();
			}
		}
		break;
	case WM_MOUSEMOVE:
		{
			if( m_pSystem->GetActive() != this )
				break;

			//----------------------------------------------
			// ���������״�仯,add by zewu 2005-2-9 20:49

			//�Ƿ��ڳ����ӵ�λ��
			INT nPos = GetCharIndex(pMsg->pt.x, pMsg->pt.y, TRUE);
			m_nMouseLinkIndex = GetLinkIndex(nPos);

			if( GT_INVALID == m_nMouseLinkIndex )
				AdptiveCursor(FALSE, FALSE );
			else
				AdptiveCursor(FALSE, TRUE);
			//-----------------------------------------------

			if( TRUE == PointInWnd(pMsg->pt) )
			{
				// ��������϶�ѡ��
				if( FALSE == m_bCanSelect )
					break;

				if( pMsg->dwParam1 == MK_LBUTTON )
				{
					//�����������ѡ��,��ѡ��״̬����Ϊԭ��,��������Ϊԭ��
					INT nChar = GetCharIndex(pMsg->pt.x, pMsg->pt.y, TRUE);
					if( GT_INVALID != nChar )
					{ //
						if( nChar < m_nSelectStart )
							m_nSelectEnd = m_nCursorPos = nChar;
						else
						{
							nChar  = GetCharIndex(pMsg->pt.x, pMsg->pt.y, FALSE);
							if( GT_INVALID != nChar )
								m_nSelectEnd = m_nCursorPos = nChar;
						}
						this->SetRefresh();
					}
				}
			}
			else
			{
				if( (pMsg->dwParam1 != MK_LBUTTON ) || (GetTickCount()-m_dwLastMoveTime) < 20 )
					break;

				if( FALSE == m_bCanSelect )
					break;

				m_dwLastMoveTime = GetTickCount();

				if( pMsg->pt.y < m_ptView.y )
				{
					//���ƶ�
					CalCursonForUpDown(TRUE);
					while( m_nCursorPos < GetRowHead(m_nStartDisplayLine) )
						m_nStartDisplayLine--;

				}
				else if( pMsg->pt.y > m_ptView.y + m_ptSize.y )
				{
					//���ƶ�
					CalCursonForUpDown(FALSE);
					while( m_nCursorPos > GetLastDisplayIndex() )
						m_nStartDisplayLine++;
				}

				m_nSelectEnd = m_nCursorPos;
				SendScrollEvent();
				this->SetRefresh();
			}
		}
		break;
	case WM_MOUSEWHEEL:	// ����������
		{
			if( pMsg->dwParam1 & 0x80000000 )	// down
			{
				ScrollText(FALSE);
			}
			else // up
			{
				ScrollText(TRUE);
			}
			SendScrollEvent();
			this->SetRefresh();
		}
		break;
	case WM_KEYDOWN:
		this->SetRefresh();
		return OnMsgForKeyDown(pMsg->dwParam1);

	case WM_SYSCHAR:
	case WM_CHAR:
		{
			this->SetRefresh();
			//���ȴ���ctrl+ ()�Ŀ�ݼ�
			if( pMsg->dwParam1 == ('a' & 0x1F) )	// ctrl+a ��Сд�޹�
			{
				m_nSelectStart = 0;
				m_nSelectEnd = m_strText.size() - 2;
				m_nCursorPos = 0;
			}
			else if( pMsg->dwParam1 == ('c' & 0x1F) )	//ctrl+c  ��Сд�޹�
			{
				this->CopyToClipboard();
			}
			else if( pMsg->dwParam1 == ('x' & 0x1F) )	//ctrl+x  ��Сд�޹�
			{
				this->CopyToClipboard();

				if( m_bReadOnly )
					break;

				this->DelSelected();
			}
			else if( pMsg->dwParam1 == ('v' & 0x1F) )	//ctrl+v  ��Сд�޹�
			{
				if( m_bReadOnly )
					break;

				this->DelSelected();
				this->PasteFromClipboard();
			}

			if( m_bReadOnly )
				break;

			if( pMsg->dwParam1 <= 0x1F && pMsg->dwParam1 > 0 )
			{
				// ��������Ч�ַ�,��������ʾ
				return TRUE;
			}

			// ���������������
			// ������ѡ����ɾ��ѡ�񲿷�
			if( m_nSelectStart != m_nSelectEnd )
				this->DelSelected();

			//���ܳ����ֽ�����
			if( (INT)m_strText.size() + 1 < m_nMaxEditTextCount )
			{
				if( m_strText.size() + 1 >= m_dwMaxTextCount )
				{
					m_dwMaxTextCount = m_strText.size() + 2;
					m_VecCharIntro.resize(m_dwMaxTextCount+2);
				}

				InsertText((TCHAR*)&pMsg->dwParam1, m_nCursorPos, m_dwTextColor, m_dwShadeColor);
			}

			while( m_nCursorPos > GetLastDisplayIndex() )
				m_nStartDisplayLine++;

			SendScrollEvent();
			return TRUE;
		}
		break;
	default:
		break;
	}

	return GUIStatic::OnInputMessage(pMsg);
}


//-------------------------------------------------------------------------------------
//�͸�����λ���ж������ĳ����ӵ�index,����index=-1
//-------------------------------------------------------------------------------------
INT GUIRichEditBox::GetLinkIndex(INT nPos)
{
	INT nStartPos = GT_INVALID;
	INT nEndPos = GT_INVALID;
	//�����ڹ��λ��֮ǰ�ĸ����Լ���һ�в����Ƿ���SET_LINK_START	

	if( nPos == 0 )
		return -1;

	INT nRowStart = m_strText.find_last_of(0x0d, nPos-1);
	if( nRowStart <= 0 )
	{
		nRowStart = 0;
	}
	else
	{
		nRowStart = m_strText.find_last_of(0x0d, nRowStart-1);
		if( nRowStart <= 0 )
			nRowStart = 0;
	}

	//����<keystart>
	INT i = 0;
	for( i=nPos-1; i>=nRowStart; i--)
	{
		if( m_VecCharIntro[i].nCharSize == 0 && (INT)(m_VecCharIntro[i].pPic) == SET_LINK_END )
		{
			//���ҵ�keyend,����Ч
			break;
		}

		if( m_VecCharIntro[i].nCharSize == 0 && (INT)(m_VecCharIntro[i].pPic) == SET_LINK_START )
		{
			nStartPos = i + m_VecCharIntro[i].nByte;
			break;
		}
	}
	if( nStartPos == GT_INVALID )
	{
		//δ�ҵ�
		return -1;
	}

	//�����ڹ��λ��֮��ĸ����Լ���һ�в����Ƿ���<keyend>

	INT nRowEnd = m_strText.find_first_of(0x0d, nPos);
	ASSERT( nRowEnd >= 0 );

	INT nTemp =  m_strText.find_first_of(0x0d, nRowEnd+1);
	if( nTemp >= 0 )
	{
		nRowEnd = nTemp;
	}

	//����<keyend>
	for( INT j=nPos; j<nRowEnd; j++)
	{
		if( m_VecCharIntro[j].nCharSize == 0 && (INT)(m_VecCharIntro[j].pPic) == SET_LINK_START )
		{
			break;
		}

		if( m_VecCharIntro[j].nCharSize == 0 && (INT)(m_VecCharIntro[j].pPic) == SET_LINK_END )
		{
			nEndPos = j;
			break;
		}
	}
	if( nEndPos == GT_INVALID )
	{
		//δ�ҵ�
		return -1;
	}

	if( nEndPos <= nStartPos )
	{
		//�ҵ���Ҳ���Ϸ�,���ܹؼ���Ϊ��
		return -1;
	}

	return i;
}


//-------------------------------------------------------------------------------------
//����Ƿ����˳�����
//-------------------------------------------------------------------------------------
BOOL GUIRichEditBox::ClickLink(tagGUIInputMsg* pMsg)
{
	//�����Ƿ��ǳ�����
	m_nMouseLinkIndex = GetLinkIndex(m_nCursorPos);

	if( -1 == m_nMouseLinkIndex )
		return FALSE;

	tagGUIEvent event(this, EGUIE_RichLinkClick, pMsg);
	event.dwParam1 = pMsg->dwType;
	event.dwParam2 = m_nMouseLinkIndex;

	SendEvent(&event);	//�����¼����ϲ㴦��
	return TRUE;

}


//-------------------------------------------------------------------------------------
//����index�õ���������ָ�������(����false��ʾ�޴˳���), 
//-------------------------------------------------------------------------------------
BOOL GUIRichEditBox::GetLinkContent(INT nIndex,tstring &strText)
{
	if( nIndex < 0 || (DWORD)nIndex >= m_strText.size() )
		return FALSE;

	if( m_VecCharIntro[nIndex].nCharSize != 0 
		|| (INT)(m_VecCharIntro[nIndex].pPic) != SET_LINK_START )
	{
		return FALSE;
	}

	INT nTextLen = m_VecCharIntro[nIndex].nByte;
	ASSERT( nTextLen >= 1 );

	INT i = 0;
	for(i=nIndex; i<nIndex+nTextLen-1; i++)
	{
		if( m_strText[i] == '=' )
			break;
	}

	if( i >= nIndex+nTextLen-1 )
		return FALSE;


	INT nLen = nTextLen + nIndex - 2 -  i;
	strText = m_strText.substr(i+1, nLen).c_str();
	return TRUE;
}


//-------------------------------------------------------------------------------------
//�õ����λ�ô���key, szKeyWord���ⲿ�ṩ�ռ�
//-------------------------------------------------------------------------------------
BOOL GUIRichEditBox::GetCursorKey(LPTSTR szKeyWord, INT& nStartPos, INT& nEndPos)
{
	nStartPos = GT_INVALID;
	nEndPos = GT_INVALID;

	//�����ڹ��λ��֮ǰ�ĸ����Լ���һ�в����Ƿ���<keystart>	

	if( m_nCursorPos == 0 )
		return FALSE;

	INT nRowStart = m_strText.find_last_of(0x0d, m_nCursorPos-1);
	if( nRowStart <= 0 )
	{
		nRowStart = 0;
	}
	else
	{
		nRowStart = m_strText.find_last_of(0x0d, nRowStart-1);
		if( nRowStart <= 0 )
			nRowStart = 0;
	}

	//����<keystart>
	for( INT i=m_nCursorPos-1; i>=nRowStart; i--)
	{
		if( m_VecCharIntro[i].nCharSize == 0 && (INT)(m_VecCharIntro[i].pPic) == SET_KEY_END )
		{
			//���ҵ�keyend,����Ч
			break;
		}

		if( m_VecCharIntro[i].nCharSize == 0 && (INT)(m_VecCharIntro[i].pPic) == SET_KEY_START )
		{
			nStartPos = i + m_VecCharIntro[i].nByte;
			break;
		}
	}
	if( nStartPos == GT_INVALID )
	{
		//δ�ҵ�
		return FALSE;
	}

	//�����ڹ��λ��֮��ĸ����Լ���һ�в����Ƿ���<keyend>

	INT nRowEnd = m_strText.find_first_of(0x0d, m_nCursorPos);
	ASSERT( nRowEnd >= 0 );

	INT nTemp =  m_strText.find_first_of(0x0d, nRowEnd+1);
	if( nTemp >= 0 )
	{
		nRowEnd = nTemp;
	}

	//����<keyend>
	for( INT i=m_nCursorPos; i<nRowEnd; i++)
	{
		if( m_VecCharIntro[i].nCharSize == 0 && (INT)(m_VecCharIntro[i].pPic) == SET_KEY_START )
		{
			break;
		}

		if( m_VecCharIntro[i].nCharSize == 0 && (INT)(m_VecCharIntro[i].pPic) == SET_KEY_END )
		{
			nEndPos = i;
			break;
		}
	}
	if( nEndPos == GT_INVALID )
	{
		//δ�ҵ�
		return FALSE;
	}

	if( nEndPos <= nStartPos )
	{
		//�ҵ���Ҳ���Ϸ�,���ܹؼ���Ϊ��
		return FALSE;
	}

	//�����ؼ���
	_tcsncpy(szKeyWord, m_strText.substr(nStartPos, nEndPos-nStartPos+1).c_str(), nEndPos-nStartPos);
	szKeyWord[nEndPos-nStartPos] = 0;

	return TRUE;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VOID GUIRichEditBox::CalSelectForDoubleClick()
{
	//���Ȳ��Ҹùؼ���
	TCHAR szKeyWord[100];
	if( TRUE == GetCursorKey(szKeyWord, m_nSelectStart, m_nSelectEnd) )
		return ;

	m_nSelectStart = m_nSelectEnd = m_nCursorPos;

	//���ҵ����е����׺���β
	INT nRowHead = 0;
	INT nRowTail = 0;
	if( m_nCursorPos == 0 )
		nRowHead = 0;
	else
	{
		nRowHead = m_strText.find_last_of(0x0d, m_nCursorPos-1);
		if( nRowHead < 0 )
			nRowHead = 0;
		else 
			nRowHead += m_VecCharIntro[nRowHead].nByte;
	}
	nRowTail = m_strText.find_first_of(0x0d, m_nCursorPos);
	ASSERT( nRowTail >= 0 );

	//��������ѡ��
	INT nStep = 1;
	TCHAR ch = 0;
	INT n = 0;
	for(n=m_nCursorPos; n < nRowTail; n+=nStep )
	{
		ch = m_strText.at(n);
		if( ch == _T(' ') || m_VecCharIntro[n].nCharSize == 0 )
			break;

		nStep = m_VecCharIntro[n].nByte;
	}
	m_nSelectEnd = n;

	//��ǰ��ѡ��
	if( m_nCursorPos == 0 )
		m_nSelectStart = 0;
	else 
	{
		for(n=m_nCursorPos-1; n >= nRowHead; n--)
		{
			ch = m_strText.at(n);
			if( ch == _T(' ') || 
				(m_VecCharIntro[n].nCharSize == 0 && m_VecCharIntro[n].nByte != GT_INVALID ) )
			{
				m_nSelectStart = n + m_VecCharIntro[n].nByte;
				break;
			}

			if( n == nRowHead )
			{
				m_nSelectStart = n;
				break;
			}
		}
	}
	m_nCursorPos = m_nSelectEnd;
}


//-----------------------------------------------------------------------------
// ������Ϊ����
//-----------------------------------------------------------------------------
GUIWnd* GUIRichEditBox::OnActive()
{
	// �����뷨
//	m_pSystem->OpenCloseIme( TRUE );

	// ���ý�����������־
//	m_pSystem->SetFocusInEditBox( TRUE );

	// ����ƶ����ַ���ĩβ
	m_bShowCursor = TRUE;
	return GUIStatic::OnActive();

}


//-----------------------------------------------------------------------------
// ��ʧ����
//-----------------------------------------------------------------------------
VOID GUIRichEditBox::OnDeactive(GUIWnd* pWnd)
{
	// ȡ��ѡ��
	m_bShowCursor = FALSE;
	this->SetRefresh();

	// �ر����뷨
//	m_pSystem->OpenCloseIme( FALSE );

	// ��ս�����������־
//	m_pSystem->SetFocusInEditBox( FALSE );
	return GUIStatic::OnDeactive(pWnd);

}

//-----------------------------------------------------------------------------
// ָ�����Ƿ������Ǵ�����
//-----------------------------------------------------------------------------
BOOL GUIRichEditBox::PointInWnd(tagPoint& pt)
{
	if(m_bShowCursor==FALSE&&m_bCanSelect==FALSE&&m_bReadOnly==TRUE)
	{
		//����ʾ��꣬����ѡ��,�������죬���Ҳ��ɱ༭��ʵ������ؼ���Ϊ
		if( m_bDisable || m_bInvisible )
			return FALSE;

		// �����Ƿ������ǵ������Ӵ�����
		std::list<GUIWnd*>::iterator it;
		for(it=m_listChild.begin(); it!=m_listChild.end(); ++it)
		{
			if( TRUE == (*it)->PointInWnd(pt) )
				return TRUE;
		}
		return FALSE;
	}
	return GUIStatic::PointInWnd(pt);
}


//-----------------------------------------------------------------------------
// ��view����õ���ӽ��ַ������к�
//-----------------------------------------------------------------------------
INT GUIRichEditBox::GetCharIndex(INT nViewX, INT nViewY, BOOL bLeft)
{
	// �ҵ���������ַ�
	INT nX = nViewX - m_ptView.x - m_rcText.left;
	INT nY = nViewY - m_ptView.y - m_rcText.top;

	//�����ж�������һ��
	INT nHeight = 0;
	INT nWidth = 0;

	INT nNewLineIndex = GetRowHead(m_nStartDisplayLine);
	while( nY > nHeight && nNewLineIndex < (INT)m_strText.size() )
	{
		nNewLineIndex = m_strText.find_first_of(0x0d, nNewLineIndex);
		ASSERT( nNewLineIndex >= 0 );
		nHeight += m_VecCharIntro[nNewLineIndex].dwHeight;
		nWidth = m_VecCharIntro[nNewLineIndex].nCharSize;

		nNewLineIndex += m_VecCharIntro[nNewLineIndex].nByte;
	}

	if( nY > nHeight )
	{ 
		//�㵽�ַ��ĺ����ˣ����������һ���ַ�
		return m_strText.size()-2;
	}

	INT nStartPos = 0;
	if( nNewLineIndex <=2 )
		nStartPos = 0;
	else 
	{
		nStartPos = m_strText.find_last_of(0x0d, nNewLineIndex-3);
		if( nStartPos < 0 )
			nStartPos = 0;
		else
			nStartPos += m_VecCharIntro[nStartPos].nByte;
	}


	INT nLeft = 0;
	if( m_bMiddleRender == TRUE )
		nLeft = (m_rcText.right - m_rcText.left - nWidth) / 2;

	if( nX < nLeft )	//��߽簢
		return nStartPos;

	INT n = 0;
	for(n=nStartPos; n<nNewLineIndex-2; )
	{
		if( TRUE == bLeft )
		{
			if( nLeft <= nX && nLeft+m_VecCharIntro[n].nCharSize > nX )
				return n;
		}
		else
		{
			if( nLeft < nX && nLeft+m_VecCharIntro[n].nCharSize >= nX )
				return n+m_VecCharIntro[n].nByte;
		}

		nLeft += m_VecCharIntro[n].nCharSize;
		n += m_VecCharIntro[n].nByte;
	}

	return n;
}


//-----------------------------------------------------------------------------
//ɾ���ı�
//-----------------------------------------------------------------------------
VOID GUIRichEditBox::ClsText()
{
	m_strText.clear();

	m_VecCharIntro.clear();
	m_VecCharIntro.resize(m_dwMaxTextCount+2);
	m_strText.push_back(0x0d);
	m_strText.push_back(0x0a);
	m_VecCharIntro[0].nByte = 2;
	m_VecCharIntro[0].dwHeight = m_nRowSpacing;
	m_VecCharIntro[1].nByte = -1;

	m_nSelectStart = 0;	// ѡ��ʼ��
	m_nSelectEnd = 0;// ѡ�������
	m_nCursorPos = 0;	// ���λ��
	m_nStartDisplayLine = 0;
}


//-----------------------------------------------------------------------------
// ɾ��ѡ����ַ�
//-----------------------------------------------------------------------------
VOID GUIRichEditBox::DelSelected()
{
	if( m_nSelectStart == m_nSelectEnd ) //��û���ı���ѡ��
		return;

	if( m_nSelectStart > m_nSelectEnd )	// ���ѡ��
		std::swap(m_nSelectStart, m_nSelectEnd);
	INT nInteChar = m_nSelectEnd - m_nSelectStart;

	DeleteText(m_nSelectStart, m_nSelectEnd);
	m_nCursorPos = m_nSelectEnd = m_nSelectStart;
	while( GT_INVALID == GetRowHead(m_nStartDisplayLine) )
		m_nStartDisplayLine--;

	while( m_nCursorPos < GetRowHead(m_nStartDisplayLine) )
		m_nStartDisplayLine -- ;
	SendScrollEvent();
}


//-----------------------------------------------------------------------------
// ��ѡ�����ݿ��������� 
//-----------------------------------------------------------------------------
VOID GUIRichEditBox::CopyToClipboard()
{
	if( m_nSelectStart != m_nSelectEnd )	// ȷ����ѡ�е�����
	{
		if( !OpenClipboard(NULL) )
			return;
		EmptyClipboard(); 

		INT nLen = abs(m_nSelectEnd - m_nSelectStart);
		HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, (nLen+1) * sizeof(TCHAR)); 
		if( NULL == hglbCopy ) 
		{ 
			CloseClipboard();
			return;
		} 

		// Lock the handle and copy the text to the buffer. 
		LPTSTR lptstrCopy = (LPTSTR)GlobalLock( hglbCopy ); 
		INT nStart = m_nSelectStart < m_nSelectEnd ? m_nSelectStart : m_nSelectEnd;
		memcpy(lptstrCopy, &m_strText[nStart], nLen * sizeof(TCHAR)); 
		lptstrCopy[nLen] = (TCHAR)0;    // null character 
		GlobalUnlock(hglbCopy); 

		// Place the handle on the clipboard. 
		SetClipboardData(CF_UNICODETEXT, hglbCopy); 
		CloseClipboard();
	}
}


//-----------------------------------------------------------------------------
// �Ӽ����忽���ı�����ǰλ��
//-----------------------------------------------------------------------------
VOID GUIRichEditBox::PasteFromClipboard()
{
	if( FALSE == IsClipboardFormatAvailable(CF_UNICODETEXT) ) 
		return; 

	if( FALSE == OpenClipboard(NULL) )
		return; 

	HGLOBAL hglb = GetClipboardData(CF_UNICODETEXT); 
	if( hglb != NULL ) 
	{ 
		LPTSTR lpstr = (LPTSTR)GlobalLock(hglb); 
		if( NULL != lpstr ) 
		{ 
			AddText(lpstr, FALSE);
			GlobalUnlock(hglb); 
		} 
	} 

	CloseClipboard(); 
	return; 
}


//----------------------------------------------------------------------------
//�����Ա���������ʾ���һ��
//----------------------------------------------------------------------------
VOID GUIRichEditBox::ScrollForLastLine(BOOL bReset)
{
	if( m_strText.size() <= 2 )
		return;

	if( bReset == TRUE )
	{
		m_nStartDisplayLine = 0;
	}

	m_nSelectStart = m_nSelectEnd = m_nCursorPos = m_strText.size() - 2;
	while( m_nCursorPos > GetLastDisplayIndex() )
		m_nStartDisplayLine++;

	SendScrollEvent();
}


//----------------------------------------------------------------------------
//�����ı�bNexLine=TRUE��ʾ����һ�п�ʼ�����������ӵ����һ�У�
//dwColor=-1��ʾĬ����m_Data����ɫ����
//----------------------------------------------------------------------------
BOOL GUIRichEditBox::AddText(LPCTSTR szText, BOOL bNextLine, DWORD dwColor, DWORD dwShade, 
							  BOOL bMatchPic, BOOL bEqualTextSize)
{
	BOOL bShowLastLine = IsShowLastLine();

	INT nSize = lstrlen(szText);
	if( nSize == 0 )
		return TRUE;

	if( (INT)m_strText.size() + nSize + 10 > m_nMaxEditTextCount )
		return FALSE;

	if( m_strText.size() + nSize + 10 > m_dwMaxTextCount ) // Ԥ���ռ�����з���
	{
		m_dwMaxTextCount = m_strText.size() + nSize + 10;
		m_VecCharIntro.resize(m_dwMaxTextCount+2);
	}

	//�ݴ�ȫ�ֱ���
	BOOL bEqual = m_bEqualTextSize;
	m_bEqualTextSize = bEqualTextSize;

	INT nVirtualPos = m_nCursorPos;
	if( TRUE == m_bReadOnly )
	{
		//ֻ��ֻ������ֱ������, ����ʾ�Ͳ��ܼ�������
		nVirtualPos = m_strText.size() - 2;
	}

	if( TRUE == bNextLine )
	{
		ASSERT( (INT)m_strText.size() > nVirtualPos );
		if( FALSE == NewLine(nVirtualPos) )
		{
			m_bEqualTextSize = bEqual;
			return FALSE;
		}

		nVirtualPos += 2;
	}

	if( dwColor != GT_INVALID && dwColor != m_dwTextColor )
	{
		TCHAR szFontColor[50];
		_stprintf(szFontColor, _T("<color=%ld>"), dwColor);
		InsertText(szFontColor, nVirtualPos, dwColor, dwShade, FALSE, FALSE);
	}


	TCHAR* szInsertText = new TCHAR[nSize+1];
	ZeroMemory(szInsertText, (nSize+1)*sizeof(TCHAR));
	INT nIndex = 0;
	for(INT n=0; n<nSize; n++)
	{
		//����������е����
		if( szText[n] == 0x0d || szText[n] == 0x0a)
		{
			InsertText(szInsertText, nVirtualPos, dwColor, dwShade, FALSE, bMatchPic);
			if( FALSE == NewLine(nVirtualPos) )
			{
				m_bEqualTextSize = bEqual;
				delete[](szInsertText);	///Add by zewu �˴�ֱ��return�ᵼ���ڴ�й©
				return FALSE;
			}

			nVirtualPos += 2;
			if( szText[n] == 0x0d && n+1 < nSize && szText[n+1] == 0x0a )
				n++;

			ZeroMemory(szInsertText, (nSize+1)*sizeof(TCHAR));
			nIndex = 0;
		}
		else if( szText[n] == _T('\t') )
		{
			InsertText(_T("    "), nVirtualPos, dwColor, dwShade, TRUE, FALSE);
		}
		else
		{
			szInsertText[nIndex] = szText[n];
			nIndex++;
		}
	}

	//�����ַ���
	InsertText(szInsertText, nVirtualPos, dwColor, dwShade, FALSE, bMatchPic);

	//�Զ�����
	if( TRUE == m_bScrollLast )
	{
		m_nSelectStart = m_nSelectEnd = m_nCursorPos = nVirtualPos;
		while( m_nCursorPos > GetLastDisplayIndex() )
			m_nStartDisplayLine++;
	}
	SendScrollEvent();

	delete[](szInsertText);
	m_bEqualTextSize = bEqual;
	this->SetRefresh();
	return TRUE;
}


//----------------------------------------------------------------------------
// ע�⣺szText���ܰ������з�!!!,����λ��Ҫ�������
// bLinkLastLine : �Ƿ���ϵ��һ�й��ɹؼ���
// bMatchPic : �Ƿ�ƥ��ͼƬ
//----------------------------------------------------------------------------
VOID GUIRichEditBox::InsertText(LPCTSTR szText, INT& nVirtualPos, DWORD dwColor, 
									DWORD dwShade, BOOL bLinkLastLine, BOOL bMatchPic) 
{
	if( 0 == lstrlen(szText) )
		return;

	if( GT_INVALID == dwColor )
		dwColor = m_dwTextColor;

	if( GT_INVALID == dwShade )
		dwShade = m_dwShadeColor;

	INT nOriginPos = nVirtualPos;
	tstring str(szText);
	MoveCharIntro(nVirtualPos, str.size());
	m_strText.insert(nVirtualPos, str);
	INT nStrPos = nVirtualPos + str.size();

	//������Ҫ�������Ļ��з�
	INT nStartPos = m_strText.find_last_of(0x0d, nOriginPos);
	if( nStartPos < 0 )
		nStartPos = 0;
	else
	{
		if( nStartPos == 0 )
		{
			nStartPos++;
			if( nStartPos+1 < (INT)m_strText.size() && m_strText[nStartPos] == 0x0a )
				nStartPos++;
		}
		else if( bLinkLastLine == TRUE )
		{
			nStartPos = m_strText.find_last_of(0x0d, nStartPos-1);
			if( nStartPos < 0 )
				nStartPos = 0;
			else 
			{
				nStartPos++;
				if( nStartPos+1 < (INT)m_strText.size() && m_strText[nStartPos] == 0x0a )
					nStartPos++;
			}
		}
		else
		{
			nStartPos++;
			if( nStartPos+1 < (INT)m_strText.size() && m_strText[nStartPos] == 0x0a )
				nStartPos++;
		}
	}

	INT nEndPos = m_strText.find_first_of(0x0d, nVirtualPos);
	ASSERT( nEndPos > 0 );

	RecalCharIntro(nStartPos, nEndPos, nStrPos, nVirtualPos, bMatchPic);
}


//----------------------------------------------------------------------------
//�����������ƶ�ʱ�Ĺ��λ��
//----------------------------------------------------------------------------
VOID GUIRichEditBox::CalCursorForLeftRight(BOOL bLeft)
{
	if( TRUE == bLeft )
	{
		if( m_nCursorPos == 0 )
			return;

		INT nPos = m_nCursorPos - 1;
		while( nPos >= 0 )
		{
			if( m_VecCharIntro[nPos].nByte != -1 )
			{
				m_nCursorPos = nPos;
				break;
			}
			nPos--;
		}
	}
	else 
	{
		if( m_nCursorPos >= (INT)m_strText.size()-2 )
			return;

		m_nCursorPos += m_VecCharIntro[m_nCursorPos].nByte;
	}

	return;
}


//----------------------------------------------------------------------------
//�����������ƶ�ʱ�µĹ��λ��
//----------------------------------------------------------------------------
VOID GUIRichEditBox::CalCursonForUpDown(BOOL bUp)
{
	if( TRUE == bUp )
	{
		if( m_nCursorPos == 0)
			return;

		INT nPreLineEnd = m_strText.find_last_of(0x0d, m_nCursorPos-1);
		if( nPreLineEnd < 0 )
			return;  //��Ϊ��������Ѿ������
		INT nIntelIndex = m_nCursorPos - (nPreLineEnd+m_VecCharIntro[nPreLineEnd].nByte);	
		ASSERT( nIntelIndex >= 0 );
		INT nPreLineBeing = 0;
		if( nPreLineEnd > 0 )
			nPreLineBeing = m_strText.find_last_of(0x0d, nPreLineEnd-1);
		if( nPreLineBeing < 0 )
			nPreLineBeing = 0;
		else
			nPreLineBeing += m_VecCharIntro[nPreLineBeing].nByte;
		if( nPreLineEnd-nPreLineBeing > nIntelIndex )
		{
			for(int i= 0; i<=nIntelIndex; )
			{
				m_nCursorPos = nPreLineBeing + i;
				i += m_VecCharIntro[m_nCursorPos].nByte;
			}
		}
		else
			m_nCursorPos = nPreLineEnd;
	}
	else
	{//�������
		INT nBegin = 0;
		if( m_nCursorPos > 0 )
		{	
			nBegin = m_strText.find_last_of(0x0d, m_nCursorPos-1);
			if( nBegin < 0 )
				nBegin = 0;
			else
				nBegin += m_VecCharIntro[nBegin].nByte;
		}
		else
			nBegin = 0;

		INT nIntelIndex = m_nCursorPos - nBegin;	
		ASSERT( nIntelIndex >= 0 );

		INT nNextLineBegin = m_strText.find_first_of(0x0d, m_nCursorPos);
		if( nNextLineBegin == m_strText.size()-2 )
			return;	//�Ѿ��������һ��

		nNextLineBegin += m_VecCharIntro[nNextLineBegin].nByte;
		INT nNextLineEnd = m_strText.find_first_of(0x0d, nNextLineBegin);
		ASSERT( nNextLineEnd >= 0 );

		if( nNextLineEnd-nNextLineBegin > nIntelIndex )
		{
			for(int i= 0; i<=nIntelIndex; )
			{
				m_nCursorPos = nNextLineBegin + i;
				i += m_VecCharIntro[m_nCursorPos].nByte;
			}
		}
		else
			m_nCursorPos = nNextLineEnd;
	}
}


//----------------------------------------------------------------------------------------------
//ע��ת���ַ���Ӧ��ͼƬ,�涨������������ĸ��϶���
//----------------------------------------------------------------------------------------------
BOOL GUIRichEditBox::RegisterPic(LPCTSTR szStr, LPCTSTR szPicFileName)
{
	INT nLen = lstrlen(szStr);
	if( nLen <= 1 )
	{
		ASSERT( FALSE && "��ת���ַ�������" );
		return FALSE;
	}
	//���뱣֤ȫ���ǵ��ֽ�
	for(INT i=0; i<nLen; i++)
	{
		ASSERT( szStr[i] > 0 && "ת���ַ������ܺ���˫�ֽ�" );
	}

	tagGUIImage* pPic = m_mapPic.Peek(tstring(szStr));
	if( P_VALID(pPic) )
	{
		ASSERT( FALSE && "��ת���ַ����Ѿ���ע��" );
		return FALSE;
	}

	tagRect rc;
	pPic = m_pRender->CreateImage(szPicFileName, rc);
	if( !P_VALID(pPic) )
	{
		ASSERT( FALSE && "richeditbox load image failed!" );
		return FALSE;
	}

	m_mapPic.Add(tstring(szStr), pPic);
	return TRUE;
}


//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
VOID GUIRichEditBox::MoveCharIntro(INT nPos, INT IncSize)
{
	if( IncSize > 0 )
	{
		//��ǰ�������Ų
		tagCharIntro var;
		ZeroMemory(&var, sizeof(var));
		m_VecCharIntro.insert(m_VecCharIntro.begin() + nPos, IncSize, var);
	}
	else if( IncSize <= 0 )
	{ 
		//�Ѻ����Ų��ǰ��
		m_VecCharIntro.erase(m_VecCharIntro.begin() + nPos, m_VecCharIntro.begin() + (nPos-IncSize));
	}
}


//----------------------------------------------------------------------------------------------
//��ĳ������
//----------------------------------------------------------------------------------------------
BOOL GUIRichEditBox::NewLine(INT nPos)
{
	if( (INT)m_strText.size() + 2 >= m_nMaxEditTextCount )
		return FALSE;

	if( m_strText.size() + 2 >= m_dwMaxTextCount )
	{
		m_dwMaxTextCount = m_strText.size() + 4;
		m_VecCharIntro.resize(m_dwMaxTextCount + 2);
	}


	MoveCharIntro(nPos, 2);

	TCHAR szNewLine[3];
	ZeroMemory(szNewLine, 3*sizeof(TCHAR));
	szNewLine[0] = 0x0d;
	szNewLine[1] = 0x0a;

	m_strText.insert(nPos, szNewLine);

	m_VecCharIntro[nPos].nByte = 2;
	m_VecCharIntro[nPos].dwHeight = m_nRowSpacing;
	m_VecCharIntro[nPos+1].nByte = -1;

	INT nFirstHead = 0;
	if( nPos > 0 )
		nFirstHead = m_strText.find_last_of(0x0d, nPos-1);
	if( nFirstHead < 0 )
		nFirstHead = 0;
	else 
		nFirstHead += m_VecCharIntro[nFirstHead].nByte;
	INT nLastHead = m_strText.find_first_of(0x0d, nPos+2);
	ASSERT( nLastHead > 0 );
	if(nLastHead==std::string::npos)
	{
		return FALSE;
	}

	INT nHeight = 0;
	INT nWidth = 0;

	INT nStep = 1;
	for(INT n=nFirstHead; n<nPos; n+=nStep)
	{
		if( (INT)m_VecCharIntro[n].dwHeight+GUI_DEFAULT_ROW_SPACING_ADDON > nHeight )
			nHeight = m_VecCharIntro[n].dwHeight+GUI_DEFAULT_ROW_SPACING_ADDON;

		nWidth += m_VecCharIntro[n].nCharSize;
		
		nStep = m_VecCharIntro[n].nByte;
	}

	m_VecCharIntro[nPos].dwHeight = nHeight;
	if( nHeight > m_nRowSpacing )
		m_VecCharIntro[nPos].dwHeight = nHeight;
	else
		m_VecCharIntro[nPos].dwHeight = m_nRowSpacing;

	m_VecCharIntro[nPos].nCharSize = nWidth;

	nHeight = 0;
	nWidth = 0;
	for(INT n=nPos+2; n<nLastHead; n+=nStep)
	{
		if( (INT)m_VecCharIntro[n].dwHeight+GUI_DEFAULT_ROW_SPACING_ADDON > nHeight )
			nHeight = m_VecCharIntro[n].dwHeight+GUI_DEFAULT_ROW_SPACING_ADDON;

		nWidth += m_VecCharIntro[n].nCharSize;
		nStep = m_VecCharIntro[n].nByte;
	}

	m_VecCharIntro[nLastHead].dwHeight = nHeight;
	if( nHeight < m_nRowSpacing )
		m_VecCharIntro[nLastHead].dwHeight = m_nRowSpacing;
	else
		m_VecCharIntro[nLastHead].dwHeight = nHeight;

	m_VecCharIntro[nLastHead].nCharSize = nWidth;
	return TRUE;
}


//----------------------------------------------------------------------------------------------
//ɾ��ĳ���ַ���
//----------------------------------------------------------------------------------------------
VOID GUIRichEditBox::DeleteText(INT nBeginPos, INT nEndPos)
{
	ASSERT( nBeginPos >= 0 );
	if( nEndPos <= nBeginPos )
		return;
	if( nEndPos > (INT)m_strText.size() -2 )
		return;

	INT nOriginPos = nBeginPos;
	MoveCharIntro(nBeginPos, nBeginPos-nEndPos);
	m_strText.erase(nBeginPos, nEndPos-nBeginPos);
	INT nStartPos = 0;
	if( nBeginPos > 0 )
	{
		nStartPos = m_strText.find_last_of(0x0d, nBeginPos-1);
		if( nStartPos < 0 )
			nStartPos = 0;
		else
		{
			if( nStartPos == 0 )
				nStartPos += m_VecCharIntro[nStartPos].nByte;
			else
			{
				nStartPos = m_strText.find_last_of(0x0d, nStartPos-1);
				if( nStartPos < 0 )
					nStartPos = 0;
				else 
					nStartPos += m_VecCharIntro[nStartPos].nByte;
			}
		}
	}
	else
		nStartPos = 0;

	INT nFinishPos = m_strText.find_first_of(0x0d, nBeginPos);
	ASSERT( nFinishPos >= 0 );

	RecalCharIntro(nStartPos, nFinishPos, nOriginPos, m_nCursorPos, TRUE);
}


//----------------------------------------------------------------------------------------------
// �ַ���ƥ��ת��ͼƬ
//----------------------------------------------------------------------------------------------
tagGUIImage* GUIRichEditBox::MatchPic(tstring &str, INT& nPos)
{
	tagGUIImage* pPic = (tagGUIImage*)GT_INVALID;
	INT nTextSize = str.size();
	for(INT n=0; n<nTextSize; n++)	
	{
		tstring temp = str.substr(n, nTextSize-n);
		pPic = m_mapPic.Peek( str.substr(n, nTextSize-n) );
		if( P_VALID(pPic) )
		{
			nPos = n;
			return pPic;
		}
	}
	return pPic;
}


//----------------------------------------------------------------------------------------------
// �ַ���ƥ��ת�������������
//----------------------------------------------------------------------------------------------
INT GUIRichEditBox::MatchFont(tstring &str, INT &nPos, DWORD &nValue)
{
	INT nFont = GT_INVALID;

	//�ȿ����ǲ������ùؼ���
	INT nTextSize = str.size();
	if( nTextSize <= 0 )
		return GT_INVALID;

	if( str[0] != '<' )
		return GT_INVALID;

	//�����Ϲؼ���
	if( str[nTextSize-1] != '>' )
		return GT_INVALID;


	tstring temp;
	m_mapKey.ResetIterator();
	while( m_mapKey.PeekNext(temp, nFont) )
	{
		nPos = str.find(temp, 0);
		if( nPos != str.npos )
		{
			nValue = 0;
			return nFont;
		}
	}


	m_mapLink.ResetIterator();
	while( TRUE == m_mapLink.PeekNext(temp, nFont) )
	{
		if( nFont != SET_LINK_END )
			continue;

		nPos = str.find(temp, 0);
		if( nPos != str.npos )
		{
			nValue = 0;
			return nFont;
		}
	}

	nFont = GT_INVALID;

	//�Ƿ�ﵽ����Ҫ��,��������ͨ����
	if( nTextSize <=2 || str[nTextSize-1] != '>' )
		return nFont;

	INT nEqualPos = str.find_last_of('=', nTextSize );
	if( nEqualPos <= 0 || nEqualPos == nTextSize-2 )	//û�ҵ����߽�����'>'Ҳ��Ч
		return nFont;

	INT nSubNum = 0;
	for(INT n=nEqualPos-1; n>=0; n--)	
	{
		nSubNum = nEqualPos - n + 1;
		tstring temp = str.substr(n, nSubNum);

		//����Ƿ���ɫ��������
		nFont = m_mapFont.Peek( str.substr(n, nSubNum) );
		if( nFont != GT_INVALID)
		{
			nSubNum = str.size()-2 + 1 - (nEqualPos+1);
			tstring temp = str.substr(nEqualPos+1, nSubNum);
			if( temp.size() == 0 )
				return GT_INVALID;

			if( nFont == SET_FONT_COLOR )
			{//������ɫ,�����õ�ֵ��ʽΪ(255,255,255);
				if( temp[0] != _T('(') || temp[temp.size()-1] != _T(')') )
				{
					nValue = _ttol(temp.c_str());
					nPos = n;
					return nFont;
				}
				INT r,g,b;
				r = g = b = 0;
				INT nPos = 1;
				INT nPausePos = temp.find_first_of(_T(','), nPos);
				if( nPausePos < 0 ) return GT_INVALID;
				r = _ttol( temp.substr(nPos, nPausePos-nPos).c_str() );
				nPos = nPausePos + 1;
				nPausePos = temp.find_first_of(_T(','), nPos);
				if( nPausePos < 0 ) return GT_INVALID;
				g = _ttol( temp.substr(nPos, nPausePos-nPos).c_str() );
				nPos = nPausePos + 1;
				if( nPos >= (INT)temp.size()-1 )
					return GT_INVALID;
				b = _ttol( temp.substr(nPos, temp.size()-1-nPos).c_str() );

				nValue = RGB(r,g,b);
			}
			else if( nFont == SET_FONT_SIZE )
			{//�����ַ���С
				nValue = _ttol( temp.c_str() );
			}
			else if( nFont == SET_FONT_STYLE )
			{//��������
				nValue = _ttol( temp.c_str() );
			}
			else 
			{
				ASSERT( FALSE );
			}
			nPos = n;
			return nFont;
		}

		//����Ƿ���������
		nFont = m_mapLink.Peek( str.substr(n, nSubNum) );
		if( SET_LINK_START == nFont )
		{
			nPos = n;
			return nFont;
		}
	}
	return nFont;
}

//----------------------------------------------------------------
//��ǰ�Ƿ���ʾ���������һ��
//----------------------------------------------------------------
BOOL GUIRichEditBox::IsShowLastLine()
{
	INT nHeight = 0;
	INT nLineEnd = 0;
	INT nSaveEnd = 0;
	INT i = 0;
	for(i=GetRowHead(m_nStartDisplayLine); i<(INT)m_strText.size();)
	{
		nLineEnd = m_strText.find_first_of(0x0d, i);
		ASSERT( nLineEnd >= 0 );
		nHeight += m_VecCharIntro[nLineEnd].dwHeight;
		if( nHeight > m_rcText.bottom - m_rcText.top )
		{//��ʾ������
			break;	
		}
		i = nLineEnd + m_VecCharIntro[nLineEnd].nByte;
		nSaveEnd = nLineEnd;
	}
	if( i == GetRowHead(m_nStartDisplayLine) )
		return FALSE;	// һ�ж��޷���ʾ����

	if( (m_strText.size() - nSaveEnd) <= 2 )
		return TRUE;
	else 
		return FALSE;
}

//----------------------------------------------------------------
//�ܹ�������ʾ�����һ�еĽ���������
//----------------------------------------------------------------
INT GUIRichEditBox::GetLastDisplayIndex()
{
	INT nHeight = 0;
	INT nLineEnd = 0;
	INT nSaveEnd = 0;
	INT i = 0;
	for(i=GetRowHead(m_nStartDisplayLine); i<(INT)m_strText.size();)
	{
		nLineEnd = m_strText.find_first_of(0x0d, i);
		ASSERT( nLineEnd >= 0 );
		if( nLineEnd < 0 )
			return 0;

		nHeight += m_VecCharIntro[nLineEnd].dwHeight;
		if( nHeight > m_rcText.bottom - m_rcText.top )
		{//��ʾ������
			break;	
		}
		i = nLineEnd + m_VecCharIntro[nLineEnd].nByte;
		nSaveEnd = nLineEnd;
	}

	if( i == GetRowHead(m_nStartDisplayLine) )
		nSaveEnd = m_strText.find_first_of(0x0d, i);	//һ�ж��޷���ʾ����

	return nSaveEnd;
}


//----------------------------------------------------------------------------------------------
//�õ����к����׵��ַ����к�
//----------------------------------------------------------------------------------------------
INT GUIRichEditBox::GetRowHead(INT nRowIndex)
{
	if( nRowIndex == 0 )
		return 0;
	INT nLine = 0;
	INT nLineEnd = 0;
	for(INT i=0; i<(INT)m_strText.size(); )
	{
		nLineEnd = m_strText.find_first_of(0x0d, i);
		ASSERT(nLineEnd >= 0);
		if(nLineEnd < 0)
			return 0;

		nLine++;
		if( nLine == nRowIndex )
			return nLineEnd + m_VecCharIntro[nLineEnd].nByte;
		i = nLineEnd + m_VecCharIntro[nLineEnd].nByte;
	}
	return GT_INVALID;
}


//---------------------------------------------------------------------------------
//��ǰ�ж�����
//---------------------------------------------------------------------------------
INT GUIRichEditBox::GetRowNum(INT& nVisibleRowNum)
{
	INT nRowNum = 0;
	nVisibleRowNum = 0;
	INT nHeight = 0;
	for(INT i=0; i<(INT)m_strText.size(); )
	{
		i = m_strText.find_first_of(0x0d, i);
		ASSERT( i >= 0 );
		if( nRowNum >= m_nStartDisplayLine && nHeight < m_rcText.bottom - m_rcText.top )
		{
			nHeight += m_VecCharIntro[i].dwHeight;

			if( nHeight <= m_rcText.bottom - m_rcText.top )
			{
				//������������ǿ��ʾ��һ��(���������߶ȵ�Ŀǰ���ᱻ��ǿ��ʾ)
				nVisibleRowNum++;
			}
		}
		tagCharIntro* p = &m_VecCharIntro[i];
		i += m_VecCharIntro[i].nByte;
		nRowNum++;
	}
	return nRowNum;
}


//-----------------------------------------------------------------------------
// �������ѷ����¼�
//-----------------------------------------------------------------------------
VOID GUIRichEditBox::SendEvent(tagGUIEvent* pEvent)
{
	switch(pEvent->eEvent)
	{
	case EGUIE_Scroll:
		this->SetCurrentTextRow(pEvent->dwParam1);
		SetRefresh(TRUE);
		break;
	}

	GUIStatic::SendEvent(pEvent);
}


//-----------------------------------------------------------------------------
//���������¼�
//-----------------------------------------------------------------------------
BOOL GUIRichEditBox::SendScrollEvent()
{
	// ����������Ϣ
	INT nVisibleRowNum = 0;
	tagGUIEvent event(this, EGUIE_Scroll);
	event.dwParam1 = (DWORD)m_nStartDisplayLine;
	event.dwParam2 = GetRowNum(nVisibleRowNum); 
	event.dwParam3 = nVisibleRowNum;
	OnEvent(&event);

	return TRUE;
}


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GUIRichEditBox::SetCurrentTextRow(INT nRowIndex )
{
	INT nVisibleRowNum = 0;
	if( nRowIndex >=0 && nRowIndex < GetRowNum(nVisibleRowNum) )
	{
		m_nStartDisplayLine = nRowIndex;
		while( m_nCursorPos < GetRowHead(m_nStartDisplayLine) )
			CalCursonForUpDown(FALSE);
		while( m_nCursorPos > GetLastDisplayIndex() )
			CalCursonForUpDown(TRUE);
	}
}


//----------------------------------------------------------------------------------
//���ֹ���(��Ϊ��λ)
//----------------------------------------------------------------------------------
DWORD	GUIRichEditBox::ScrollText(BOOL bUp)
{
	if( TRUE == bUp )
	{
		if( m_nStartDisplayLine > 0 )		
		{//��ʾ�Ĳ��ǵ�һ��
			m_nStartDisplayLine--;
			if( m_nCursorPos > GetLastDisplayIndex() )
				CalCursonForUpDown(TRUE);
		}
	}
	else
	{
		INT nLastDisplayIndex = GetLastDisplayIndex();
		if( nLastDisplayIndex < (INT)m_strText.size()-2 )
		{//û����ʾ�����һ��,�������¹���
			m_nStartDisplayLine++;
			if( m_nCursorPos < GetRowHead(m_nStartDisplayLine) )
				CalCursonForUpDown(FALSE);
		}
	}

	return m_nStartDisplayLine;
}


//----------------------------------------------------------------------------------
//���¼���ʼĩλ��֮����ַ����е���Ϣ����,�����Ű��ԭ��,nEndPos���ܻ��������
//----------------------------------------------------------------------------------
VOID GUIRichEditBox::RecalCharIntro(INT nStartPos, INT nEndPos, INT& nCursorPos, INT& nVirtualPos, BOOL bMatchPic)
{
	tagGUIImage* pPic = (tagGUIImage*)GT_INVALID;

	tagPoint ptSize;
	TCHAR szTemp[3];
	ZeroMemory(szTemp, 3*sizeof(TCHAR));

	INT nHeight = m_nRowSpacing;
	INT nWidth = 0;
	INT nSaveWidth = 0;
	INT nSaveHeight = 0;

	INT nStep = 1;
	tstring str;
	tstring strLastLine;
	tstring strTotal;

	tstring strPic;
	tstring strLastLinePic;
	tstring strTotalPic;

	nVirtualPos = nStartPos;

	INT n = 0;
	for(n=nStartPos; n<nEndPos; n+=nStep)
	{
		szTemp[0] = m_strText[n];	

		if( szTemp[0] == 0x0d )
		{
			if( n+1 < (INT)m_strText.size() && m_strText[n+1] == 0x0a )
			{
				nStep = 2;
				m_VecCharIntro[n+1].nByte = -1;		
			}
			else 
				nStep = 1;
			m_VecCharIntro[n].nByte = nStep;
			m_VecCharIntro[n].dwHeight = nHeight;
			m_VecCharIntro[n].nCharSize = nWidth;


			strLastLine = str;
			str.clear();

			if( TRUE == bMatchPic )
			{
				strLastLinePic = str;
				strPic.clear();
			}


			nSaveHeight = nHeight;
			nSaveWidth = nWidth;
			nHeight = m_nRowSpacing;
			nWidth = 0;
			continue;			
		}
		else if( szTemp[0] < 0 )
		{
			//˫�ֽ��ַ�
			str.clear();
			strLastLine.clear();  //��Ϊƥ���ַ������ܰ���˫�ֽ��ַ�
			strTotal.clear();

			if( TRUE == bMatchPic )
			{
				strPic.clear();
				strLastLinePic.clear();  //��Ϊƥ���ַ������ܰ���˫�ֽ��ַ�
				strTotalPic.clear();
			}

			szTemp[1] = m_strText[n+1];

			if( nWidth + m_ptSizeChinese.x > m_rcText.right - m_rcText.left )
			{ 
				//�Ų�����,ֻ����ӻ��з���ͬʱʣ�µĺ���һ����һ��Ҳ����ȡ������֮��Ļ��з�
				if( n <= nCursorPos) //��ΪҪ���У���������2���ַ�
					nCursorPos += 2; 

				TCHAR szNewLine[3];
				ZeroMemory(szNewLine, 3*sizeof(TCHAR));

				if( m_strText.size() + 2 >= m_dwMaxTextCount)
				{
					m_dwMaxTextCount = m_strText.size() + 4;
					m_VecCharIntro.resize(m_dwMaxTextCount + 2);
				}

				szNewLine[0] = 0x0d;
				szNewLine[1] = 0x0a;
				MoveCharIntro(n, 2);
				m_strText.insert(n, szNewLine);
				m_VecCharIntro[n].nByte = 2;
				m_VecCharIntro[n].dwHeight = nHeight;
				m_VecCharIntro[n].nCharSize = nWidth;
				m_VecCharIntro[n+1].nByte = -1;

				nEndPos += 2;
				nStep = 2;


				nSaveHeight = nHeight;
				nSaveWidth = nWidth;
				nHeight = m_nRowSpacing;
				nWidth = 0;


				continue;
			}

			m_VecCharIntro[n].nByte = 2;
			m_VecCharIntro[n].nCharSize = m_ptSizeChinese.x;
			m_VecCharIntro[n].dwHeight = m_ptSizeChinese.y;
			m_VecCharIntro[n+1].nByte = -1;

			if( m_ptSizeChinese.y+GUI_DEFAULT_ROW_SPACING_ADDON > nHeight )
			{
				nHeight = m_ptSizeChinese.y + GUI_DEFAULT_ROW_SPACING_ADDON;
			}

			nWidth += m_ptSizeChinese.x;
			nStep = 2;

			if( n < nCursorPos && n+nStep >= nCursorPos )
				nVirtualPos = n + nStep;

		}
		else
		{
			//���ֽ�
			if( strLastLine.size() + str.size() >= GUI_MAX_TRANSFER_LEN )
			{
				//ƥ����ַ���������
				if( strLastLine.size() > 0 )
					strLastLine.erase(0, 1);
				else
					str.erase(0, 1);
			}

			if( TRUE == bMatchPic )
			{
				if( strLastLinePic.size() + strPic.size() >= GUI_MAX_TRANSFER_LEN )
				{
					//ƥ����ַ���������
					if( strLastLinePic.size() > 0 )
						strLastLinePic.erase(0, 1);
					else 
						strPic.erase(0, 1);
				}

				strPic.push_back(szTemp[0]);
				strTotalPic = strLastLinePic + strPic;
			}


			if( (strLastLine.size()+str.size()) == 0 )
			{
				//�յ�ƥ���ַ���
				if( szTemp[0] == _T('<') )
				{
					//'<'�ǹؼ��ֵ���ʼ��־,����϶����ǹؼ���
					str.push_back(szTemp[0]);
					strTotal = strLastLine + str;
				}
			}
			else if( szTemp[0] != _T('<') )
			{
				//���ܷ��Ϲؼ���
				str.push_back(szTemp[0]);
				strTotal = strLastLine + str;
			}
			else
			{
				//�µ�'<'����,ǰ��Ļ�ûƥ���,�ǿ϶�����
				str.clear();
				strLastLine.clear();

				str.push_back(szTemp[0]);
				strTotal = str;
			}

			INT nPicPos = 0;
			if( !bMatchPic || !P_VALID((pPic=MatchPic(strTotalPic, nPicPos))) )
			{
				//����ͼƬ
				DWORD dwValue = 0;
				INT nFont = MatchFont(strTotal, nPicPos, dwValue);	//ƥ����������
				if( nFont != GT_INVALID )
				{
					//�ɹ�
					INT nFontHead = 0;
					INT nRemoveWidthIndex = 0;
					INT nBytes = strTotal.size() - nPicPos;
					if( nBytes > (INT)str.size() )
					{
						//˵��������һ�е��ַ�,�ָ��ֳ�
						INT nLastLineEnd = m_strText.find_last_of(0x0d, n);
						ASSERT( nLastLineEnd );
						nHeight = nSaveHeight;
						nWidth = nSaveWidth;
						INT nIntelLineSize = m_VecCharIntro[nLastLineEnd].nByte;
						MoveCharIntro(nLastLineEnd, -nIntelLineSize);
						m_strText.erase(nLastLineEnd, nIntelLineSize);

						if( n<= nCursorPos) //��ΪҪ���У���������
							nCursorPos -= nIntelLineSize; 

						n = n - nIntelLineSize;
						nEndPos -= nIntelLineSize;

						nFontHead = n - nBytes + 1;
						INT nLastLineChar = nBytes - str.size();	//��һ�в�����ַ�
						nRemoveWidthIndex = nLastLineEnd;

					}
					else
					{
						nFontHead = n - nBytes + 1;
						nRemoveWidthIndex = n;
					}

					for(INT i=nFontHead+1; i<=n ;i++)
					{
						m_VecCharIntro[i].nByte = -1;
					}

					INT nInteWidth = 0;
					for(INT i=nFontHead; i<nRemoveWidthIndex; i++)
					{
						nInteWidth += m_VecCharIntro[i].nCharSize;
					}
					nWidth -= nInteWidth;

					m_VecCharIntro[nFontHead].nByte = nBytes;
					m_VecCharIntro[nFontHead].nCharSize = 0;
					m_VecCharIntro[nFontHead].dwReserve = dwValue;
					m_VecCharIntro[nFontHead].pPic = (tagGUIImage*)nFont;
					m_VecCharIntro[nFontHead].dwHeight = 0;

					str.clear();
					strLastLine.clear();
					strTotal.clear();

					if( TRUE == bMatchPic )
					{
						strPic.clear();
						strLastLinePic.clear();
						strTotalPic.clear();
					}

					nStep = nBytes;
					n = nFontHead;

					if( n < nCursorPos && n+nStep >= nCursorPos )
						nVirtualPos = n + nStep;

					continue;
				}

				szTemp[1] = 0;

				if( m_bEqualTextSize == TRUE )
					ptSize = m_ptSizeLetter;
				else
					m_pRender->GetTextSize(szTemp, m_pFont, ptSize);

				if( nWidth + ptSize.x > m_rcText.right - m_rcText.left )
				{ 
					//�Ų�����,ֻ����ӻ��з���ͬʱʣ�µĺ���һ����һ��Ҳ����ȡ������֮��Ļ��з�
					if( n<= nCursorPos) //��ΪҪ���У���������2���ַ�
						nCursorPos += 2; 

					if( m_strText.size() + 2 >= m_dwMaxTextCount)
					{
						m_dwMaxTextCount = m_strText.size() + 4;
						m_VecCharIntro.resize(m_dwMaxTextCount + 2);
					}

					//Ѱ�һ���λ��
					n=FindBreakLinePos(n);

					//ɾ�����׵Ŀո�
					if( m_strText[n] == _T(' ') )
					{
						m_strText.erase(n,1);
						MoveCharIntro(n,-1);
						nEndPos--;
						nCursorPos--;
					}

					TCHAR szNewLine[3];
					ZeroMemory(szNewLine, 3*sizeof(TCHAR));
					szNewLine[0] = 0x0d;
					szNewLine[1] = 0x0a;
					MoveCharIntro(n, 2);
					m_strText.insert(n, szNewLine);
					m_VecCharIntro[n].nByte = 2;
					m_VecCharIntro[n].dwHeight = nHeight;
					m_VecCharIntro[n].nCharSize = nWidth;
					m_VecCharIntro[n+1].nByte = -1;

					nEndPos += 2;
					nStep = 2;

					if( str.size() > 0 )
					{
						str.erase(str.size()-1, 1);
					}
					strLastLine = str;
					str.clear();

					if( TRUE == bMatchPic )
					{
						if( strPic.size() > 0 )
						{
							strPic.erase(strPic .size()-1, 1);
						}
						strLastLinePic = strPic;
						strPic.clear();
					}



					nSaveHeight = nHeight;
					nSaveWidth = nWidth;
					nHeight = m_nRowSpacing;
					nWidth = 0;

					if( n < nCursorPos && n+nStep >= nCursorPos )
						nVirtualPos = n + nStep;

					continue;
				}

				m_VecCharIntro[n].nByte = 1;
				m_VecCharIntro[n].nCharSize = ptSize.x;
				m_VecCharIntro[n].dwHeight = ptSize.y;

				if( ptSize.y+GUI_DEFAULT_ROW_SPACING_ADDON > nHeight )
					nHeight = ptSize.y + GUI_DEFAULT_ROW_SPACING_ADDON;

				nWidth += ptSize.x;
				nStep = 1;

				if( n < nCursorPos && n+nStep >= nCursorPos )
					nVirtualPos = n + nStep;
			}
			else
			{	
				//ƥ��ɹ�,��ת��ͼƬ
				INT nPicHead = 0;
				INT nRemoveWidthIndex = 0;
				INT nBytes = strTotalPic.size() - nPicPos;

				if( nBytes > (INT)strPic.size() )
				{
					//˵��������һ�е��ַ�,�ָ��ֳ�
					INT nLastLineEnd = m_strText.find_last_of(0x0d, n);
					INT nIntelLineSize = m_VecCharIntro[nLastLineEnd].nByte;
					ASSERT( nLastLineEnd );
					nHeight = nSaveHeight;
					nWidth = nSaveWidth;
					MoveCharIntro(nLastLineEnd, -nIntelLineSize);
					m_strText.erase(nLastLineEnd, nIntelLineSize);
					//��ΪҪ���У���������2���ַ�
					if( n<= nCursorPos) 
						nCursorPos -= nIntelLineSize; 

					n = n - nIntelLineSize;
					nEndPos -= nIntelLineSize;

					nPicHead = n - nBytes + 1;
					//��һ�в�����ַ�
					INT nLastLineChar = nBytes - strPic.size();	
					nRemoveWidthIndex = nLastLineEnd;

				}
				else
				{
					nPicHead = n - nBytes + 1;
					nRemoveWidthIndex = n;
				}


				INT nInteWidth = 0;
				for(INT i=nPicHead; i<nRemoveWidthIndex; i++)
				{
					nInteWidth += m_VecCharIntro[i].nCharSize;
				}
				if( nWidth + pPic->ptSize.x - nInteWidth > m_rcText.right - m_rcText.left )
				{ 
					// �Ų�����,ֻ����ӻ��з���ͬʱʣ�µĺ���һ����һ��Ҳ����ȡ������֮��Ļ��з�
					if( n<= nCursorPos) //��ΪҪ���У���������2���ַ�
						nCursorPos += 2;

					if( nEndPos < (INT)m_strText.size()-2 )
					{
						MoveCharIntro(nEndPos, -2);
						m_strText.erase(nEndPos, 2);
					}

					if( m_strText.size() + 2 >= m_dwMaxTextCount)
					{
						m_dwMaxTextCount = m_strText.size() + 4;
						m_VecCharIntro.resize(m_dwMaxTextCount + 2);
					}

					TCHAR szNewLine[3];
					ZeroMemory(szNewLine, 3*sizeof(TCHAR));

					szNewLine[0] = 0x0d;
					szNewLine[1] = 0x0a;
					MoveCharIntro(nPicHead, 2);
					m_strText.insert(nPicHead, szNewLine);
					m_VecCharIntro[nPicHead].nByte = 2;
					m_VecCharIntro[nPicHead].dwHeight = nHeight;
					m_VecCharIntro[nPicHead].nCharSize = nWidth;
					m_VecCharIntro[nPicHead+1].nByte = -1;

					nEndPos += 2;
					n = nPicHead;
					nStep = 2;

					str.clear();
					strLastLine.clear();
					strTotal.clear();

					if( TRUE == bMatchPic )
					{
						strPic.clear();
						strLastLinePic.clear();
						strTotalPic.clear();
					}


					nSaveHeight = nHeight;
					nSaveWidth = nWidth;
					nHeight = m_nRowSpacing;
					nWidth = 0;
					continue;
				}

				m_VecCharIntro[nPicHead].nByte = n - nPicHead + 1;
				m_VecCharIntro[nPicHead].nCharSize = pPic->ptSize.x;
				m_VecCharIntro[nPicHead].dwHeight = pPic->ptSize.y;
				m_VecCharIntro[nPicHead].pPic = pPic;
				for(INT i=nPicHead+1; i<=n ;i++)
				{
					m_VecCharIntro[i].nByte = -1;
				}

				if( pPic->ptSize.y > nHeight )
					nHeight = pPic->ptSize.y;

				nWidth += pPic->ptSize.x - nInteWidth;
				n = nPicHead;
				nStep = m_VecCharIntro[nPicHead].nByte;

				if( nPicHead < nCursorPos && nPicHead+nStep >= nCursorPos )
					nVirtualPos = nPicHead + nStep;

				str.clear();
				strLastLine.clear();
				strTotal.clear();

				if( TRUE == bMatchPic )
				{
					strPic.clear();
					strLastLinePic.clear();
					strTotalPic.clear();
				}
			}
		}
	}

	if( m_strText[n] == 0x0d && m_strText[n+1] == 0x0a )
	{
		m_VecCharIntro[n].dwHeight = nHeight;	//ˢ�¸��еĸ߶�
		m_VecCharIntro[n].nCharSize = nWidth;
		if( nHeight < m_nRowSpacing )
			m_VecCharIntro[n].dwHeight = m_nRowSpacing;
	}

}

//-----------------------------------------------------------------------------
//��ȡ�ı���Ϣ����ӿ�
//-----------------------------------------------------------------------------
INT	GUIRichEditBox::GetText(tstring &str) 
{ 
	INT nTextSize = (INT)(m_strText.size()-2);
	str = m_strText.substr(0, nTextSize); 
	ASSERT( str.size() == nTextSize );
	return nTextSize; 
}

LPCTSTR GUIRichEditBox::GetText(INT& nTextSize)  
{
	//ע����Ч���ȣ�����
	nTextSize = (INT)(m_strText.size()-2);
	return (m_strText.c_str()); 
}

//-----------------------------------------------------------------------------
//�������ɵ��������
//-----------------------------------------------------------------------------
VOID GUIRichEditBox::SetMaxTextCount(DWORD dwMaxTextCount)
{
	m_nMaxEditTextCount = dwMaxTextCount;
}


//-----------------------------------------------------------------------------
//ɾ���ı�ǰͷ���ַ�,���м���,����ʵ��ɾ��������
//-----------------------------------------------------------------------------
INT	GUIRichEditBox::DelFrontText(INT nRowNum)
{
	INT nDelRowNum = 0;
	INT nTextSize = m_strText.size();

	INT i = 0;
	for(i=0; i<nTextSize;)
	{
		i = m_strText.find_first_of(0x0d, i);
		ASSERT( i >= 0 );

		nDelRowNum++;
		if( nDelRowNum == nRowNum )
			break;

		i += m_VecCharIntro[i].nByte;
	}

	//���û�е����Ļ��з�,���ǾͿ��԰�������з�Ҳɾ��
	if( i < nTextSize-2 )
		i += m_VecCharIntro[i].nByte;	
	//���û���ı���ɾ,ֱ�ӷ���
	if( i == 0 )
		return 0;

	//�ȵ���
	MoveCharIntro(0, -i);
	//����ã�˳������
	m_strText.erase(0, i);
	nTextSize = m_strText.size();

	m_nCursorPos -= i;
	m_nSelectStart -= i;
	m_nSelectEnd -= i;
	if( m_nCursorPos < 0 )
		m_nCursorPos = 0;
	if( m_nSelectStart < 0 )
		m_nSelectStart = 0;		
	if( m_nSelectEnd < 0 )
		m_nSelectEnd = 0;

	//��ȷ���ó�ʼ��ʾ����
	m_nStartDisplayLine -= nDelRowNum;
	if( m_nStartDisplayLine < 0 )
		m_nStartDisplayLine = 0;

	return nDelRowNum;			
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GUIRichEditBox::AdptiveCursor(bool bDrag, BOOL bLink)
{
	/*
	if( TRUE == bDrag )
	{
		m_pRender->SetCursor(_T("sizewe"));   //
	}
	else if( TRUE == bLink )
	{
		m_pRender->SetCursor(_T("hand"));
	}
	else
	{
		m_pRender->SetCursor(_T("ibeam"));   //		
	}*/
}



//--------------------------------------------------------------------------------
//������̰��µ���Ϣ(һ���ǹ��ܼ��İ���)
//--------------------------------------------------------------------------------
BOOL GUIRichEditBox::OnMsgForKeyDown(INT nKey)
{
	switch( nKey )
	{
	case VK_LEFT:	// ������������
		{
			m_bShowCursor = TRUE;
			if( GetKeyState(VK_SHIFT) < 0 )
			{ // shift��������

				if( FALSE == m_bCanSelect )
					break;

				INT nOriginPos = m_nCursorPos;
				CalCursorForLeftRight(TRUE);	
				if( m_nSelectStart != m_nSelectEnd )
					m_nSelectEnd = m_nCursorPos;
				else
				{
					m_nSelectEnd = m_nCursorPos;
					m_nSelectStart = nOriginPos;
				}
			}
			else
			{
				if( m_nSelectStart == m_nSelectEnd )
					CalCursorForLeftRight(TRUE);
				else
					m_nCursorPos = m_nSelectEnd < m_nSelectStart ? m_nSelectEnd : m_nSelectStart;
				m_nSelectStart = m_nSelectEnd = m_nCursorPos;
			}

			while( m_nCursorPos < GetRowHead(m_nStartDisplayLine) )
				m_nStartDisplayLine--;
			SendScrollEvent();
		}
		return TRUE;
	case VK_RIGHT:	// ��������ҷ����
		{
			m_bShowCursor = TRUE;
			if( GetKeyState(VK_SHIFT) < 0 )
			{ 
				// shift��������
				if( FALSE == m_bCanSelect )
					break;

				INT nOriginPos = m_nCursorPos;
				CalCursorForLeftRight(FALSE);	
				if( m_nSelectStart != m_nSelectEnd )
					m_nSelectEnd = m_nCursorPos;
				else
				{
					m_nSelectEnd = m_nCursorPos;
					m_nSelectStart = nOriginPos;
				}
			}
			else
			{
				if( m_nSelectStart == m_nSelectEnd )
					CalCursorForLeftRight(FALSE);
				else
					m_nCursorPos = m_nSelectEnd > m_nSelectStart ? m_nSelectEnd : m_nSelectStart;
				m_nSelectStart = m_nSelectEnd = m_nCursorPos;
			}

			while( m_nCursorPos > GetLastDisplayIndex() )
			{
				m_nStartDisplayLine++;
			}
			SendScrollEvent();
		}
		return TRUE;
	case VK_UP:// ��������Ϸ����
		{
			//�����µĹ��λ��
			INT nOriginPos = m_nCursorPos;
			CalCursonForUpDown(TRUE);
			while( m_nCursorPos < GetRowHead(m_nStartDisplayLine) )
				m_nStartDisplayLine--;
			SendScrollEvent();

			// ����ѡ��
			if( GetKeyState(VK_SHIFT) < 0 )
			{
				if( FALSE == m_bCanSelect )
					break;

				if( m_nSelectStart != m_nSelectEnd )
					m_nSelectEnd = m_nCursorPos;
				else
				{
					m_nSelectEnd = m_nCursorPos;
					m_nSelectStart = nOriginPos;
				}
			}
			else
				m_nSelectStart = m_nSelectEnd = m_nCursorPos;
		}
		return TRUE;
	case VK_DOWN:
		{
			//�����µĹ��λ��
			INT nOriginPos = m_nCursorPos;
			CalCursonForUpDown(FALSE);
			while( m_nCursorPos > GetLastDisplayIndex() )
				m_nStartDisplayLine++;
			SendScrollEvent();
			// ����ѡ��
			if( GetKeyState(VK_SHIFT) < 0 )
			{
				if( FALSE == m_bCanSelect )
					break;

				if( m_nSelectStart != m_nSelectEnd )
					m_nSelectEnd = m_nCursorPos;
				else
				{
					m_nSelectEnd = m_nCursorPos;
					m_nSelectStart = nOriginPos;
				}
			}
			else
				m_nSelectStart = m_nSelectEnd = m_nCursorPos;

		}
		return TRUE;
	case VK_HOME:
		{
			//�ƶ�������,�����ס��ctrl���ı��ײ�
			INT nOriginPos = m_nCursorPos;

			if( (GetKeyState(VK_LCONTROL)&0x80) || (GetKeyState(VK_RCONTROL)&0x80) )
			{
				//��ס��CTRL��,����ƶ����ı����ַ�
				if( FALSE == m_bCanSelect )
					break;

				m_nStartDisplayLine = 0;
				m_nCursorPos = 0;
				SendScrollEvent();
			}
			else 
			{//����ƶ�������
				if( nOriginPos > 0 )
				{
					INT nNewLineIndex = m_strText.find_last_of(0x0d, nOriginPos-1);
					if( nNewLineIndex < 0 )
						m_nCursorPos = 0;
					else
					{
						m_nCursorPos = nNewLineIndex + 2;
					}
				}
			}

			if( GetKeyState(VK_SHIFT) < 0 )
			{
				if( FALSE == m_bCanSelect )
					break;

				if( m_nSelectStart != m_nSelectEnd )
					m_nSelectEnd = m_nCursorPos;
				else
				{
					m_nSelectEnd = m_nCursorPos;
					m_nSelectStart = nOriginPos;
				}
			}
			else
				m_nSelectStart = m_nSelectEnd = m_nCursorPos;
		}
		return TRUE;

	case VK_END:
		{
			//�ƶ�����β,�����ס��ctrl�����ƶ����ı�ĩβ
			INT nOriginPos = m_nCursorPos;
			if( (GetKeyState(VK_LCONTROL)&0x80) || (GetKeyState(VK_RCONTROL)&0x80) )
			{
				if( FALSE == m_bCanSelect )
					break;

				m_nCursorPos = m_strText.size() - 2;
				while ( m_nCursorPos > GetLastDisplayIndex() )
					m_nStartDisplayLine++;
				SendScrollEvent();
			}
			else 
			{
				INT nNewLineIndex = m_strText.find_first_of(0x0d, nOriginPos);
				ASSERT( nNewLineIndex >= 0 );
				m_nCursorPos = nNewLineIndex;
			}

			if( GetKeyState(VK_SHIFT) < 0 )
			{
				if( FALSE == m_bCanSelect )
					break;

				if( m_nSelectStart != m_nSelectEnd )
					m_nSelectEnd = m_nCursorPos;
				else
				{
					m_nSelectEnd = m_nCursorPos;
					m_nSelectStart = nOriginPos;
				}
			}
			else
				m_nSelectStart = m_nSelectEnd = m_nCursorPos;
		}
		return TRUE;
	
	case VK_DELETE:	// ����delɾ��
		{
			if( m_bReadOnly )
				break;

			if( m_nSelectStart != m_nSelectEnd )
				this->DelSelected();
			else 
			{
				INT nStartPos = m_nCursorPos;
				DeleteText(nStartPos, nStartPos+m_VecCharIntro[nStartPos].nByte);	

				m_nCursorPos = nStartPos;
				//��֤���λ������Ұ��Χ��
				while( GT_INVALID == GetRowHead(m_nStartDisplayLine) )
					m_nStartDisplayLine--;

				while( m_nCursorPos < GetRowHead(m_nStartDisplayLine) )
					m_nStartDisplayLine -- ;
				SendScrollEvent();
			}
		}
		return TRUE;

	case VK_BACK:
		{
			if( TRUE == m_bReadOnly )
				break;

			if( (GetKeyState(VK_LCONTROL) & 0x80) 
				|| (GetKeyState(VK_RCONTROL) & 0x80) )
				break;

			if( m_nSelectStart != m_nSelectEnd ) // ��ѡ�У�ɾѡ��
			{
				this->DelSelected();
			}
			else if( m_nCursorPos > 0 )
			{
				INT nStartPos = m_nCursorPos - 1;
				while( nStartPos >= 0 )
				{
					if( m_VecCharIntro[nStartPos].nByte != -1 )
					{
						break;
					}
					nStartPos--;
				}
				DeleteText(nStartPos, m_nCursorPos);
				m_nCursorPos = nStartPos;

				//��֤���λ������Ұ��Χ��
				while( GT_INVALID == GetRowHead(m_nStartDisplayLine) )
					m_nStartDisplayLine--;

				while( m_nCursorPos < GetRowHead(m_nStartDisplayLine) )
					m_nStartDisplayLine -- ;
				SendScrollEvent();
			}
		}
		return TRUE;
	case VK_RETURN:	// ����س�
		{
			if( (GetKeyState(VK_LCONTROL) & 0x80) 
				|| (GetKeyState(VK_RCONTROL) & 0x80) )
			{
				//ctrl+enter
				if( TRUE == m_bReadOnly )
					break;
				if( FALSE == NewLine(m_nCursorPos) )
					break;

				m_nCursorPos += 2;

				while( m_nCursorPos > GetLastDisplayIndex() )
					m_nStartDisplayLine++;

				SendScrollEvent();
			}
			else
			{
				tagGUIEvent event(this, EGUIE_EditBoxEnter);
				SendEvent(&event);
			}
		}
		return TRUE;
	case VK_TAB:	// TAB�����ϲ㴦��
		{
			//�ݶ�4���ո�
			if( TRUE == m_bReadOnly )
				break;

			//�ֽ�������
			if( (INT)m_strText.size() + 4 >= m_nMaxEditTextCount )
				break;

			if( m_strText.size() + 4 >= m_dwMaxTextCount )
			{
				m_dwMaxTextCount += 5;
				m_VecCharIntro.resize(m_dwMaxTextCount+2);
			}

			InsertText(_T("    "), m_nCursorPos, m_dwTextColor, m_dwShadeColor);
			while( m_nCursorPos > GetLastDisplayIndex() )
				m_nStartDisplayLine++;
			SendScrollEvent();
		}
		return TRUE;
	}

	return FALSE;
}


//--------------------------------------------------------------------------------
//��ȡĳһ��ʵ����ʾʱ��ռ���(����-1��ʾ��Ч���к�)
//--------------------------------------------------------------------------------
INT GUIRichEditBox::GetRowRenderWidth(INT nRowIndex)
{
	//�õ����к����׵��ַ����к�
	INT nHeadIndex = GetRowHead(nRowIndex);

	if( nHeadIndex == GT_INVALID )
	{
		//δ�ҵ�
		return GT_INVALID;
	}

	INT nRowWidth = 0;

	INT nTextSize = m_strText.size();
	for(INT i=nHeadIndex; i<nTextSize;)
	{
		if( m_strText[i] == 0x0a || m_strText[i] == 0x0d )
			break;	 //���о�����

		nRowWidth += m_VecCharIntro[i].nCharSize;

		i += m_VecCharIntro[i].nByte;
	}

	return nRowWidth;
}


//--------------------------------------------------------------------------------
//���ַ������й���,���˵������ڲ���ʽ(������ɫ���õ�)
//--------------------------------------------------------------------------------
BOOL GUIRichEditBox::FilterString(tstring& str)
{
	BOOL bCheck = TRUE;
	tstring strKey;
	INT nFont = GT_INVALID;
	INT nTemp = 0;

	while( TRUE == bCheck )
	{
		bCheck = FALSE;

		//��������ؼ���
		m_mapKey.ResetIterator();
		while( m_mapKey.PeekNext(strKey, nFont) )
		{
			INT nPos = str.find(strKey, 0);
			if( str.npos != nPos )
			{
				str.erase(nPos, strKey.size());
				bCheck = TRUE;
			}			
		}

		//������������
		m_mapLink.ResetIterator();
		while( TRUE == m_mapLink.PeekNext(strKey, nFont) )
		{
			INT nPos = str.find(strKey, 0);
			if( str.npos != nPos )
			{
				str.erase(nPos, strKey.size());
				bCheck = TRUE;
			}			
		}

		//�������ת���ַ���

		m_mapFont.ResetIterator();
		while( TRUE == m_mapFont.PeekNext(strKey, nFont) )
		{
			INT nPos = 0;
			nTemp = 0;
			while( str.npos != (nPos=str.find(strKey, nPos)) )
			{
				nTemp = nPos + strKey.size();

				if( SET_FONT_SIZE == nFont )	
				{
					//���������С

					nTemp = str.find(_T(">"), nTemp);

					if( nTemp == str.npos )
						break;

					nTemp += 1;
					str.erase(nPos, nTemp-nPos);
					bCheck = TRUE;

				}
				else if( SET_FONT_STYLE == nFont )
				{
					//��������
					nTemp = str.find(_T(">"), nTemp);

					if( nTemp == str.npos )
						break;

					nTemp += 1;
					str.erase(nPos, nTemp-nPos);
					bCheck = TRUE;
				}
				else if( SET_FONT_COLOR == nFont )
				{
					nTemp = str.find(_T(">"), nTemp);

					if( nTemp == str.npos )
						break;

					nTemp += 1;
					str.erase(nPos, nTemp-nPos);
					bCheck = TRUE;
				}
				//ˢ���´ε�λ��
				nPos = nTemp;
			}
		}

	}

	return TRUE;
}

inline bool _IsLetterOrSymbol(TCHAR ch)
{
	return ch>32&&ch<127;
}

INT GUIRichEditBox::FindBreakLinePos(INT nPos)
{
	ASSERT(nPos>=0 && nPos<(int)m_strText.size());

	//��ǰ�ַ��Ƿ���Ӣ����ĸ/��㣿
	if(!(m_strText[nPos] > 32 && m_strText[nPos] < 127) )
		return nPos;

	//��ǰѰ�ҵ��ʵ���ʼλ��
	int i;
	for(i=nPos-1; i>=0; i--)
	{
		if(!(m_strText[i] > 32 && m_strText[i] < 127) )	// �ո���з���
			break;
	}

	//�����ַ���������ʼλ�ã�
	if( i < 0 )
		return nPos;

	//�������з���
	if( '\n' == m_strText[i] || '\r' == m_strText[i] )
		return nPos;

	return i+1;
}


}	// namespace vEngine {
