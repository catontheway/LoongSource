#include "stdafx.h"
#include "OwnerImeMgr.h"
#include "OwnerImeEvent.h"
#include "OwnerImeFrame.h"

OwnerImeMgr::OwnerImeMgr( void ):m_Trunk(this)
{
	m_lpCandList	= NULL;
}

OwnerImeMgr::~OwnerImeMgr( void )
{

}

OwnerImeMgr g_OwnerImeMgr;
OwnerImeMgr* OwnerImeMgr::Inst()
{
	return &g_OwnerImeMgr;
}

void OwnerImeMgr::Init()
{
	m_pUtil			= TObjRef<Util>();
	m_pCmdMgr		= TObjRef<NetCmdMgr>();
	m_pSession		= TObjRef<NetSession>();
	m_pFrameMgr		= TObjRef<GameFrameMgr>();
	
	DisableOwnerIme();
}

void OwnerImeMgr::Destroy()
{
	DisableOwnerIme();
	if( m_lpCandList ){
		GlobalFree( (HANDLE)m_lpCandList );
		m_lpCandList = NULL;
	}
}

void OwnerImeMgr::EnableOwnerIme()
{
	m_bOwnerIme = true;
}

void OwnerImeMgr::DisableOwnerIme()
{
	while( ImmIsIME( GetKeyboardLayout( 0 )) && m_bOwnerIme)
		ActivateKeyboardLayout(( HKL )HKL_NEXT, 0 );//���ime��ͨ��ѭ���л�����һ���ر�
	m_bOwnerIme = false;
	m_szImeName[0] = 0;
	m_szCompStr[0] = 0;
	m_szCompReadStr[0] = 0;
	m_szCandList[0] = 0;
	m_nImeCursor = 0;
}

void OwnerImeMgr::NextOwnerIme()
{
	if( m_bOwnerIme )
		ActivateKeyboardLayout(( HKL )HKL_NEXT, 0 );
}

void OwnerImeMgr::SharpOwnerIme( HWND hWnd )
{
	ImmSimulateHotKey( hWnd, IME_CHOTKEY_SHAPE_TOGGLE );
}

void OwnerImeMgr::SymbolOwnerIme( HWND hWnd )
{
	ImmSimulateHotKey( hWnd, IME_CHOTKEY_SYMBOL_TOGGLE );
}

bool OwnerImeMgr::OnWM_INPUTLANGCHANGEREQUEST()
{
	return !m_bOwnerIme;								//�����ֹime�򷵻�false����ʱ���ں���Ӧ����0������DefWindowProc������뷨
}

bool OwnerImeMgr::OnWM_INPUTLANGCHANGE( HWND hWnd )
{
	if(!m_bOwnerIme)
		return false;
	//ime�ı�
	HKL hKL = GetKeyboardLayout( 0 );
	if( ImmIsIME( hKL ))
	{
		HIMC hIMC = ImmGetContext( hWnd );
		ImmEscape( hKL, hIMC, IME_ESC_IME_NAME, m_szImeName );						//ȡ�������뷨����
		DWORD dwConversion, dwSentence;
		ImmGetConversionStatus( hIMC, &dwConversion, &dwSentence );
		m_bImeSharp = ( dwConversion & IME_CMODE_FULLSHAPE )? true : false;			//ȡ��ȫ�Ǳ�־
		m_bImeSymbol = ( dwConversion & IME_CMODE_SYMBOL )? true : false;			//ȡ�����ı���־
		ImmReleaseContext( hWnd, hIMC );
		tagOwnerImeShow evt;
		evt.bShow = TRUE;
		m_pFrameMgr->SendEvent(&evt);
	}
	else		//Ӣ������
	{
		m_szImeName[ 0 ] = 0;
		tagOwnerImeShow evt;
		evt.bShow = FALSE;
		m_pFrameMgr->SendEvent(&evt);
	}
	return false;								//���Ƿ���false����Ϊ��Ҫ���ں�������DefWindowProc��������
}

bool OwnerImeMgr::OnWM_IME_NOTIFY( HWND hWnd, WPARAM wParam )
{
	if(!m_bOwnerIme)
		return false;

	HIMC hIMC;
	DWORD dwSize;
	DWORD dwConversion, dwSentence;
	switch( wParam ){
		case IMN_SETCONVERSIONMODE:		//ȫ��/��ǣ���/Ӣ�ı��ı�
			{
				hIMC = ImmGetContext( hWnd );
				ImmGetConversionStatus( hIMC, &dwConversion, &dwSentence );
				m_bImeSharp = ( dwConversion & IME_CMODE_FULLSHAPE )? true : false;
				m_bImeSymbol = ( dwConversion & IME_CMODE_SYMBOL )? true : false;
				ImmReleaseContext( hWnd, hIMC );
			}
			break;
		case IMN_OPENCANDIDATE:			//����ѡ��״̬
		case IMN_CHANGECANDIDATE:		//ѡ�ֱ�ҳ
			{
				hIMC = ImmGetContext( hWnd );
				if( m_lpCandList )
				{
					GlobalFree( (HANDLE)m_lpCandList );
					m_lpCandList = NULL;
				}
				//�ͷ���ǰ��ѡ�ֱ�
				if( dwSize = ImmGetCandidateList( hIMC, 0, NULL, 0 ))
				{
					m_lpCandList = (LPCANDIDATELIST)GlobalAlloc( GPTR, dwSize );
					if( m_lpCandList )
						ImmGetCandidateList( hIMC, 0, m_lpCandList, dwSize );
				}
				
				//�õ��µ�ѡ�ֱ�
				ImmReleaseContext( hWnd, hIMC );
				if( m_lpCandList )
					ConvertCandList( m_lpCandList, m_szCandList );				//ѡ�ֱ�����ɴ�
				
				tagOwnerImeWordEvent evt;
				evt.strText = m_szCandList;
				m_pFrameMgr->SendEvent(&evt);
			}
			break;
		case IMN_CLOSECANDIDATE:		//�ر�ѡ�ֱ�
			{
				if( m_lpCandList )
				{
					GlobalFree( (HANDLE)m_lpCandList );
					m_lpCandList = NULL;
				}
				m_szCandList[0] = 0;
			}
			break;
	}
	return true;						//���Ƿ���true����ֹime���ڴ�
}

bool OwnerImeMgr::OnWM_IME_COMPOSITION( HWND hWnd, LPARAM lParam )
{
	if(!m_bOwnerIme)
		return false;

	HIMC hIMC;
	DWORD dwSize;
	hIMC = ImmGetContext( hWnd );
	if( lParam & GCS_COMPSTR )
	{
		dwSize = ImmGetCompositionString( hIMC, GCS_COMPSTR, (LPVOID)m_szCompStr, sizeof( m_szCompStr ));
		dwSize /= sizeof(TCHAR);
		m_szCompStr[ dwSize ] = 0;
	}
	
	//ȡ��szCompStr
	if( lParam & GCS_COMPREADSTR )
	{
		dwSize = ImmGetCompositionString( hIMC, GCS_COMPREADSTR, (LPVOID)m_szCompReadStr, sizeof( m_szCompReadStr ));
		dwSize /= sizeof(TCHAR);
		m_szCompReadStr[ dwSize ] = 0;
	}
	
	//ȡ��szCompReadStr
	if( lParam & GCS_CURSORPOS )
	{
		m_nImeCursor = 0xffff & ImmGetCompositionString( hIMC, GCS_CURSORPOS, NULL, 0 );
	}

	//�����Զ������뷨�������
	tagOwnerInputWordEvent evt;
	evt.strText = m_szCompStr;
	m_pFrameMgr->SendEvent(&evt);
	
	//ȡ��nImeCursor
	if( lParam & GCS_RESULTSTR )
	{
		TBYTE str[ X_LONG_NAME ];
		dwSize = ImmGetCompositionString( hIMC, GCS_RESULTSTR, (LPVOID)str, sizeof( str ));//ȡ�ú������봮
		dwSize /= sizeof(TCHAR);
		str[ dwSize ] = 0;
		TBYTE *p = str;
		while( *p )
			PostMessage( hWnd, WM_CHAR, (WPARAM)(*p++), 1 );//ת��WM_CHAR��Ϣ

		//����Զ������뷨�������
		tagOwnerInputWordClear evt;
		m_pFrameMgr->SendEvent(&evt);
	}
	ImmReleaseContext( hWnd, hIMC );
	return true;							//���Ƿ���true����ֹime���ڴ�
}

TCHAR* OwnerImeMgr::GetOwnerImeName()
{
	return m_szImeName[0]? m_szImeName : NULL;
}

//�Ƿ�ȫ��
bool OwnerImeMgr::IfOwnerImeSharp()
{
	return m_bImeSharp;
}

//�Ƿ����ı��
bool OwnerImeMgr::IfOwnerImeSymbol()
{
	return m_bImeSymbol;
}

void OwnerImeMgr::GetOwnerImeInput( TCHAR **pszCompStr, TCHAR **pszCompReadStr, TCHAR *pnImeCursor, TCHAR **pszCandList )
{
	if( pszCompStr )
		*pszCompStr = m_szCompStr;
	if( pszCompReadStr )
		*pszCompReadStr = m_szCompReadStr;
	if( pnImeCursor )
		*pnImeCursor = m_nImeCursor;
	if( pszCandList )
		*pszCandList = m_szCandList;
}

void OwnerImeMgr::ConvertCandList( CANDIDATELIST* pCandList, TCHAR* pszCandList )
{
	unsigned int i;
	if( pCandList->dwCount < pCandList->dwSelection )
	{
		pszCandList[ 0 ] = 0;
		return;
	}
	char* a;
	TCHAR* b;

	for( i = 0; ( i < pCandList->dwCount - pCandList->dwSelection )&&( i < pCandList->dwPageSize ); i++ )
	{
		*pszCandList++ = ( i % 10 != 9 )? i % 10 + _T('1') : _T('0');				//ÿ���Ӧ�����ּ�
		*pszCandList++ = _T(' ');													//��'.'�ָ�
		a = (char*)pCandList + pCandList->dwOffset[ pCandList->dwSelection + i ];	//����ƫ���ҵ��ַ����ĵ�ַ
		b = (TCHAR*)a;																//��ָ��ת���ɿ���
		_tcscpy( pszCandList, b );													//ÿ��ʵ�ʵ�����
		pszCandList += _tcslen( pszCandList );
		*pszCandList++ = _T('\t');	
	}
	*( pszCandList )= _T('\0');	
}

void OwnerImeMgr::OnWindowsMsg( DWORD dwMsg, DWORD dwParam1, DWORD dwParam2 )
{
	if( !m_bOwnerIme )
		return;

	switch(dwMsg)
	{
	case WM_IME_SETCONTEXT:
		{
			OnWM_IME_SETCONTEXT();
		}
		break;
	case WM_INPUTLANGCHANGEREQUEST:
		{
			OnWM_INPUTLANGCHANGEREQUEST();
		}
		break;
	case WM_INPUTLANGCHANGE:
		{
			HWND hWnd = (HWND)TObjRef<VarContainer>()->GetDword(_T("HWND"));
			OnWM_INPUTLANGCHANGE( hWnd );
		}
		break;
	case WM_IME_STARTCOMPOSITION:
		{
			OnWM_IME_STARTCOMPOSITION();
		}
		break;
	case WM_IME_ENDCOMPOSITION:
		{
			OnWM_IME_ENDCOMPOSITION();
		}
		break;
	case WM_IME_NOTIFY:
		{
			HWND hWnd = (HWND)TObjRef<VarContainer>()->GetDword(_T("HWND"));
			OnWM_IME_NOTIFY( hWnd, dwParam1 );
		}
		break;
	case WM_IME_COMPOSITION:
		{
			HWND hWnd = (HWND)TObjRef<VarContainer>()->GetDword(_T("HWND"));
			OnWM_IME_COMPOSITION( hWnd, dwParam2 );
		}
		break;
	}
}

