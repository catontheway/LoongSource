#include "..\StdAfx.h"
#include ".\gwin32app.h"
#include "GInput.h"
#include "..\..\Cool3D\Util\DebugMisc.h"
#include "..\..\Cool3D\Util\Exception.h"
using namespace Cool3D;

GWin32App*	g_win32AppInst=NULL;

LRESULT CALLBACK GWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(g_win32AppInst==NULL)
		return DefWindowProc(hWnd, message, wParam, lParam);
	else
		return g_win32AppInst->OnWin32Msg(hWnd,message,wParam,lParam);
}

GWin32App::GWin32App(void)
{
	m_bRun=true;
	m_hWnd=NULL;

	ASSERT(g_win32AppInst==NULL);//ֻ����һ��ʵ��
	g_win32AppInst=this;
	m_bFullScreen=false;
}

GWin32App::~GWin32App(void)
{
}

GWin32App* GWin32App::Inst()
{
	return g_win32AppInst;
}

void GWin32App::Create(UINT windowW,UINT windowH,const TCHAR* szTitle,bool bFullScreenWndStyle,HICON hIcon)
{
	ASSERT(m_hWnd==NULL);//�����ظ�����

	m_bFullScreen=bFullScreenWndStyle;
	HINSTANCE hInstance=GetModuleHandle(NULL);
	
	//--ע�ᴰ��class 
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.style			= CS_HREDRAW | CS_VREDRAW |CS_DBLCLKS;//֧��˫��
	wc.lpfnWndProc		= (WNDPROC)GWndProc;
	wc.hInstance		= hInstance;
	wc.hIcon			= hIcon;
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszClassName	= g_szWindowClass;

	if( 0 == RegisterClass(&wc) )
		THROW_NULL(0, _T("error register window class"));

	//--����һ������Ļ�Ͼ��еĴ���
	DWORD style = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	DWORD styleEx = 0;
	if( bFullScreenWndStyle )
	{
		styleEx = WS_EX_TOPMOST;
	}
	else
	{
		UINT scrW = ::GetSystemMetrics(SM_CXSCREEN);
		UINT scrH = ::GetSystemMetrics(SM_CYSCREEN);

		if(scrW > windowW && scrH > windowH)
			style = WS_OVERLAPPEDWINDOW;	// ����С����Ļ��С�����ڲŴ��߿�
	}

	m_hWnd=CreateWindowEx(styleEx,g_szWindowClass,szTitle,style, 0,0,0,0,
		NULL,NULL,hInstance, NULL);
	THROW_NULL(m_hWnd, _T("error creating window"));

	//------------------------------------------------------
	// ���ڴ��ڿͻ���ΪҪ��Ĵ�С
	RECT rcInt;   // rcInt�൱��rcClient
	RECT rcExt;   // rcExt�൱��rcWindow

	// ����߿�Ĵ�С
	::GetWindowRect(m_hWnd, &rcExt);
	::GetClientRect(m_hWnd, &rcInt);

	INT x = (rcExt.right-rcExt.left) - (rcInt.right-rcInt.left);
	INT y = (rcExt.bottom-rcExt.top) - (rcInt.bottom-rcInt.top);

	RECT rc;
	rc.left = 0; rc.top = 0; rc.bottom = windowH; rc.right = windowW;

	if( !bFullScreenWndStyle )
	{
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);	// �õ��������Ĵ�С(����������������)

		if( (LONG)windowW > rc.right - rc.left )
			windowW = rc.right - rc.left;
		if( (LONG)windowH > rc.bottom - rc.top )
			windowH = rc.bottom - rc.top;
	}

	// �µĴ��ڴ�С
	SetWindowPos(m_hWnd, HWND_TOP, rc.left, rc.top, windowW+x, windowH+y, SWP_SHOWWINDOW);


	//-----------------------------------------------------------------------

	//--��ʼ����������
	GInput::Inst()->CreateInput();

	//--����������ӦCreate�¼��Ļ���
	OnCreate();

	ShowWindow(m_hWnd, SW_HIDE);
	UpdateWindow(m_hWnd);

	m_bRun=true;
}

void GWin32App::PostMessage(DWORD msg,DWORD wParam,DWORD lParam)
{
	tagMsg tmp={msg,wParam,lParam};
	m_msgList.push_back(tmp);
}

void GWin32App::MainLoop()
{
	MSG msg;
	while(m_bRun)
	{
		//--��Ϣ����
		if (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
		{
			if( !::GetMessage( &msg, NULL, 0, 0 ) )
				break;

			::TranslateMessage(&msg); 
			::DispatchMessage(&msg);
		}
		else//ֻ�а�����messageȡ�꣬�Ž�����Ϸ����һ֡���������ȴ�����Ϣ��ʹ�����������
		{
			//--����Ӧ�ó�����Ϣ
			DoMessages();
			//--��Ϸ�������
			OnIdle();
		}
	}

	DoMessages();
	Destroy();
}

void GWin32App::Destroy()
{
	//--�ȸ�������Destory�Ļ���
	OnDestroy();
	//--��Destroy�ڲ�����
	if(m_hWnd)
	{
		DestroyWindow(m_hWnd);
		m_hWnd=NULL;
	}
}

void GWin32App::ScreenToClient(POINT *pPt)
{
	::ScreenToClient(m_hWnd,pPt);
}

void GWin32App::DoMessages()
{
	
	for(list<tagMsg>::iterator iter=m_msgList.begin();
			iter!=m_msgList.end();iter++)
	{
		switch(iter->msg)
		{
		case GAPP_QUIT:
			m_bRun=false;
			ShowWindow(m_hWnd,SW_HIDE);
			break;
		}
		DoMessage(iter->msg,iter->wParam,iter->lParam);
	}
	m_msgList.clear();
}

DWORD GWin32App::OnWin32Msg(HWND hWnd,UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_ACTIVATEAPP:
		if(m_bFullScreen)
		{
			if(wParam==WA_INACTIVE)
				ShowWindow(m_hWnd,SW_MINIMIZE);
			else
				ShowWindow(m_hWnd,SW_SHOWNORMAL);
		}
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool GWin32App::IfPointInClient(const POINT& pt)
{
	RECT rc;
	GetWindowRect(m_hWnd,&rc);
	return PtInRect(&rc,pt)!=0;//����ģʽ�ı�������ô��??
}
