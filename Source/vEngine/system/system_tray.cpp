// TrayIcon.cpp: implementation of the SystemTray class.
//
// NON-MFC VERSION
//
// This class is a light wrapper around the windows system tray stuff. It
// adds an icon to the system tray with the specified ToolTip text and 
// callback notification value, which is sent back to the Parent window.
//
// Updated: 21 Sep 2000 - Added GetDoWndAnimation - animation only occurs if the system
//                        settings allow it (Matthew Ellis). Updated the GetTrayWndRect
//                        function to include more fallback logic (Matthew Ellis)
//
// Updated: 4 Aug 2003 - Fixed bug that was stopping icon from being recreated when
//                       Explorer crashed
//                       Fixed resource leak in SetIcon
//						 Animate() now checks for empty icon list - Anton Treskunov
//						 Added the virutal CustomizeMenu() method - Anton Treskunov
//
// Written by Chris Maunder (cmaunder@mail.com)
// Copyright (c) 1999-2003.
//
/////////////////////////////////////////////////////////////////////////////
#include "..\stdafx.h"
#include "system_tray.h"

namespace vEngine {
#define TRAYICON_CLASS _T("TrayIconClass")

// The option here is to maintain a list of all TrayIcon windows,
// and iterate through them, instead of only allowing a single 
// TrayIcon per application
SystemTray* SystemTray::m_pThis = NULL;

const UINT SystemTray::m_nTimerID    = 4567;
UINT SystemTray::m_nMaxTooltipLength  = 64;     // This may change...
const UINT SystemTray::m_nTaskbarCreatedMsg = ::RegisterWindowMessage(_T("TaskbarCreated"));
HWND  SystemTray::m_hWndInvisible;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SystemTray::SystemTray()
{
	Initialise();
}

SystemTray::SystemTray(HINSTANCE hInst,			// Handle to application instance
						 HWND hParent,				// The window that will recieve tray notifications
						 UINT uCallbackMessage,     // the callback message to send to parent
						 LPCTSTR szToolTip,         // tray icon tooltip
						 HICON icon,                // Handle to icon
						 UINT uID,                  // Identifier of tray icon
						 BOOL bHidden /*=FALSE*/,   // Hidden on creation?                  
						 LPCTSTR szBalloonTip /*=NULL*/,    // Ballon tip (w2k only)
						 LPCTSTR szBalloonTitle /*=NULL*/,  // Balloon tip title (w2k)
						 DWORD dwBalloonIcon /*=NIIF_NONE*/,// Ballon tip icon (w2k)
						 UINT uBalloonTimeout /*=10*/)      // Balloon timeout (w2k)
{
	Initialise();
	Create(hInst, hParent, uCallbackMessage, szToolTip, icon, uID, bHidden,
		szBalloonTip, szBalloonTitle, dwBalloonIcon, uBalloonTimeout);
}

void SystemTray::Initialise()
{
	// If maintaining a list of all TrayIcon windows (instead of
	// only allowing a single TrayIcon per application) then add
	// this TrayIcon to the list
	m_pThis = this;

	memset(&m_tnd, 0, sizeof(m_tnd));
	m_bEnabled = FALSE;
	m_bHidden  = TRUE;
	m_bRemoved = TRUE;

	m_DefaultMenuItemID    = 0;
	m_DefaultMenuItemByPos = TRUE;

	m_bShowIconPending = FALSE;

	m_uIDTimer   = 0;
	m_hSavedIcon = NULL;

	m_hTargetWnd = NULL;
	m_uCreationFlags = 0;

	m_setGrayMenuItem.clear();

	OSVERSIONINFO os = { sizeof(os) };
	GetVersionEx(&os);
	m_bWin2K = ( VER_PLATFORM_WIN32_NT == os.dwPlatformId && os.dwMajorVersion >= 5 );
}

ATOM SystemTray::RegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= (WNDPROC)WindowProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= 0;
	wcex.hCursor		= 0;
	wcex.hbrBackground	= 0;
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= TRAYICON_CLASS;
	wcex.hIconSm		= 0;

	return RegisterClassEx(&wcex);
}

BOOL SystemTray::Create(HINSTANCE hInst, HWND hParent, UINT uCallbackMessage, 
						 LPCTSTR szToolTip, HICON icon, UINT uID, BOOL bHidden /*=FALSE*/,
						 LPCTSTR szBalloonTip /*=NULL*/, 
						 LPCTSTR szBalloonTitle /*=NULL*/,  
						 DWORD dwBalloonIcon /*=NIIF_NONE*/,
						 UINT uBalloonTimeout /*=10*/)
{
	m_bEnabled = TRUE;
	m_nMaxTooltipLength = _countof(m_tnd.szTip);

	// Make sure we avoid conflict with other messages
	ASSERT(uCallbackMessage >= WM_APP);

	// Tray only supports tooltip text up to m_nMaxTooltipLength) characters
	ASSERT(_tcslen(szToolTip) <= m_nMaxTooltipLength);

	m_hInstance = hInst;

	RegisterClass(hInst);

	// Create an invisible window
	m_hWnd = ::CreateWindow(TRAYICON_CLASS, _T(""), WS_POPUP, 
		CW_USEDEFAULT,CW_USEDEFAULT, 
		CW_USEDEFAULT,CW_USEDEFAULT, 
		NULL, 0,
		hInst, 0);

	// load up the NOTIFYICONDATA structure
	m_tnd.cbSize = sizeof(NOTIFYICONDATA);
	m_tnd.hWnd   = (hParent)? hParent : m_hWnd;
	m_tnd.uID    = uID;
	m_tnd.hIcon  = icon;
	m_tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	m_tnd.uCallbackMessage = uCallbackMessage;

	_tcsncpy(m_tnd.szTip, szToolTip, m_nMaxTooltipLength);

	if (m_bWin2K && szBalloonTip)
	{
		// The balloon tooltip text can be up to 255 chars long.
		ASSERT(lstrlen(szBalloonTip) < 256);

		// The balloon title text can be up to 63 chars long.
		if( szBalloonTitle )
		{
			ASSERT(lstrlen(szBalloonTitle) < 64);
		}

		// dwBalloonIcon must be valid.
		ASSERT(NIIF_NONE == dwBalloonIcon    || NIIF_INFO == dwBalloonIcon ||
			NIIF_WARNING == dwBalloonIcon || NIIF_ERROR == dwBalloonIcon);

		// The timeout must be between 10 and 30 seconds.
		ASSERT(uBalloonTimeout >= 10 && uBalloonTimeout <= 30);

		m_tnd.uFlags |= NIF_INFO;

		_tcsncpy(m_tnd.szInfo, szBalloonTip, 255);
		if (szBalloonTitle)
			_tcsncpy(m_tnd.szInfoTitle, szBalloonTitle, 63);
		else
			m_tnd.szInfoTitle[0] = _T('\0');
		m_tnd.uTimeout    = uBalloonTimeout * 1000; // convert time to ms
		m_tnd.dwInfoFlags = dwBalloonIcon;
	}

	m_bHidden = bHidden;
	m_hTargetWnd = m_tnd.hWnd;

	if (m_bWin2K && m_bHidden)
	{
		m_tnd.uFlags = NIF_STATE;
		m_tnd.dwState = NIS_HIDDEN;
		m_tnd.dwStateMask = NIS_HIDDEN;
	}

	m_uCreationFlags = m_tnd.uFlags;	// Store in case we need to recreate in OnTaskBarCreate

	BOOL bResult = TRUE;
	if (!m_bHidden || m_bWin2K)
	{
		bResult = Shell_NotifyIcon(NIM_ADD, &m_tnd);
		m_bShowIconPending = m_bHidden = m_bRemoved = !bResult;
	}

	if (m_bWin2K && szBalloonTip)
	{
		// Zero out the balloon text string so that later operations won't redisplay
		// the balloon.
		m_tnd.szInfo[0] = _T('\0');
	}

	return bResult;
}

SystemTray::~SystemTray()
{
	RemoveIcon();
	m_IconList.clear();
	if (m_hWnd)
		::DestroyWindow(m_hWnd);
}

/////////////////////////////////////////////////////////////////////////////
// SystemTray icon manipulation

void SystemTray::SetFocus()
{
	Shell_NotifyIcon ( NIM_SETFOCUS, &m_tnd );
}

BOOL SystemTray::MoveToRight()
{
	RemoveIcon();
	return AddIcon();
}

BOOL SystemTray::AddIcon()
{
	if (!m_bRemoved)
		RemoveIcon();

	if (m_bEnabled)
	{
		m_tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		if (!Shell_NotifyIcon(NIM_ADD, &m_tnd))
			m_bShowIconPending = TRUE;
		else
			m_bRemoved = m_bHidden = FALSE;
	}
	return (m_bRemoved == FALSE);
}

BOOL SystemTray::RemoveIcon()
{
	m_bShowIconPending = FALSE;

	if (!m_bEnabled || m_bRemoved)
		return TRUE;

	m_tnd.uFlags = 0;
	if (Shell_NotifyIcon(NIM_DELETE, &m_tnd))
		m_bRemoved = m_bHidden = TRUE;

	return (m_bRemoved == TRUE);
}

BOOL SystemTray::HideIcon()
{
	if (!m_bEnabled || m_bRemoved || m_bHidden)
		return TRUE;

	if (m_bWin2K)
	{
		m_tnd.uFlags = NIF_STATE;
		m_tnd.dwState = NIS_HIDDEN;
		m_tnd.dwStateMask = NIS_HIDDEN;

		m_bHidden = Shell_NotifyIcon( NIM_MODIFY, &m_tnd);
	}
	else
		RemoveIcon();

	return (m_bHidden == TRUE);
}

BOOL SystemTray::ShowIcon()
{
	if (m_bRemoved)
		return AddIcon();

	if (!m_bHidden)
		return TRUE;

	if (m_bWin2K)
	{
		m_tnd.uFlags = NIF_STATE;
		m_tnd.dwState = 0;
		m_tnd.dwStateMask = NIS_HIDDEN;
		Shell_NotifyIcon ( NIM_MODIFY, &m_tnd );
	}
	else
		AddIcon();

	return (m_bHidden == FALSE);
}

BOOL SystemTray::SetIcon(HICON hIcon)
{
	if (!m_bEnabled)
		return FALSE;

	m_tnd.uFlags = NIF_ICON;
	m_tnd.hIcon = hIcon;

	if (m_bHidden)
		return TRUE;
	else
		return Shell_NotifyIcon(NIM_MODIFY, &m_tnd);
}

BOOL SystemTray::SetIcon(LPCTSTR lpszIconName)
{
	HICON hIcon = (HICON) ::LoadImage(m_hInstance, 
		lpszIconName,
		IMAGE_ICON, 
		0, 0,
		LR_LOADFROMFILE);

	if (!hIcon)
		return FALSE;
	BOOL returnCode = SetIcon(hIcon);
	::DestroyIcon(hIcon);
	return returnCode;
}

BOOL SystemTray::SetIcon(UINT nIDResource)
{
	HICON hIcon = (HICON) ::LoadImage(m_hInstance, 
		MAKEINTRESOURCE(nIDResource),
		IMAGE_ICON, 
		0, 0,
		LR_DEFAULTCOLOR);

	BOOL returnCode = SetIcon(hIcon);
	::DestroyIcon(hIcon);
	return returnCode;
}

BOOL SystemTray::SetStandardIcon(LPCTSTR lpIconName)
{
	HICON hIcon = ::LoadIcon(NULL, lpIconName);

	return SetIcon(hIcon);
}

BOOL SystemTray::SetStandardIcon(UINT nIDResource)
{
	HICON hIcon = ::LoadIcon(NULL, MAKEINTRESOURCE(nIDResource));

	return SetIcon(hIcon);
}

HICON SystemTray::GetIcon() const
{
	return (m_bEnabled)? m_tnd.hIcon : NULL;
}

BOOL SystemTray::SetIconList(UINT uFirstIconID, UINT uLastIconID) 
{
	if (uFirstIconID > uLastIconID)
		return FALSE;

	UINT uIconArraySize = uLastIconID - uFirstIconID + 1;

	m_IconList.clear();
	try 
	{
		for (UINT i = uFirstIconID; i <= uLastIconID; i++)
			m_IconList.push_back(::LoadIcon(m_hInstance, MAKEINTRESOURCE(i)));
	}
	catch (...)
	{
		m_IconList.clear();
		return FALSE;
	}

	return TRUE;
}

BOOL SystemTray::SetIconList(HICON* pHIconList, UINT nNumIcons)
{
	m_IconList.clear();

	try {
		for (UINT i = 0; i <= nNumIcons; i++)
			m_IconList.push_back(pHIconList[i]);
	}
	catch (...)
	{
		m_IconList.clear();
		return FALSE;
	}

	return TRUE;
}

BOOL SystemTray::Animate(UINT nDelayMilliSeconds, int nNumSeconds /*=-1*/)
{
	if (m_IconList.empty())
		return FALSE;

	StopAnimation();

	m_nCurrentIcon = 0;
	time(&m_StartTime);
	m_nAnimationPeriod = nNumSeconds;
	m_hSavedIcon = GetIcon();

	// Setup a timer for the animation
	m_uIDTimer = ::SetTimer(m_hWnd, m_nTimerID, nDelayMilliSeconds, NULL);
	return (m_uIDTimer != 0);
}

BOOL SystemTray::StepAnimation()
{
	if (!m_IconList.size())
		return FALSE;

	m_nCurrentIcon++;
	if( m_nCurrentIcon >= (INT)m_IconList.size() )
		m_nCurrentIcon = 0;

	return SetIcon(m_IconList[m_nCurrentIcon]);
}

BOOL SystemTray::StopAnimation()
{
	BOOL bResult = FALSE;

	if (m_uIDTimer)
		bResult = ::KillTimer(m_hWnd, m_uIDTimer);
	m_uIDTimer = 0;

	if (m_hSavedIcon)
		SetIcon(m_hSavedIcon);
	m_hSavedIcon = NULL;

	return bResult;
}

/////////////////////////////////////////////////////////////////////////////
// SystemTray tooltip text manipulation

BOOL SystemTray::SetTooltipText(LPCTSTR pszTip)
{
	ASSERT(_tcslen(pszTip) < m_nMaxTooltipLength);

	if (!m_bEnabled)
		return FALSE;

	m_tnd.uFlags = NIF_TIP;
	_tcsncpy(m_tnd.szTip, pszTip, m_nMaxTooltipLength-1);

	if (m_bHidden)
		return TRUE;
	else
		return Shell_NotifyIcon(NIM_MODIFY, &m_tnd);
}

BOOL SystemTray::SetTooltipText(UINT nID)
{
	TCHAR strBuffer[1024];
	ASSERT(1024 >= m_nMaxTooltipLength);

	if (!LoadString(m_hInstance, nID, strBuffer, m_nMaxTooltipLength-1))
		return FALSE;

	return SetTooltipText(strBuffer);
}


//////////////////////////////////////////////////////////////////////////
//
// Function:    ShowBalloon
//
// Description:
//  Shows a balloon tooltip over the tray icon.
//
// Input:
//  szText: [in] Text for the balloon tooltip.
//  szTitle: [in] Title for the balloon.  This text is shown in bold above
//           the tooltip text (szText).  Pass "" if you don't want a title.
//  dwIcon: [in] Specifies an icon to appear in the balloon.  Legal values are:
//                 NIIF_NONE: No icon
//                 NIIF_INFO: Information
//                 NIIF_WARNING: Exclamation
//                 NIIF_ERROR: Critical error (red circle with X)
//  uTimeout: [in] Number of seconds for the balloon to remain visible.  Can
//            be between 10 and 30 inclusive.
//
// Returns:
//  TRUE if successful, FALSE if not.
//
//////////////////////////////////////////////////////////////////////////
// Added by Michael Dunn, November 1999
//////////////////////////////////////////////////////////////////////////

BOOL SystemTray::ShowBalloon(LPCTSTR szText,
							  LPCTSTR szTitle  /*=NULL*/,
							  DWORD   dwIcon   /*=NIIF_NONE*/,
							  UINT    uTimeout /*=10*/ )
{
	// Bail out if we're not on Win 2K.
	if (!m_bWin2K)
		return FALSE;

	// Verify input parameters.

	// The balloon tooltip text can be up to 255 chars long.
	ASSERT(lstrlen(szText) < 256);

	// The balloon title text can be up to 63 chars long.
	if(szTitle)
	{
		ASSERT(lstrlen(szTitle) < 64);
	}

	// dwBalloonIcon must be valid.
	ASSERT(NIIF_NONE == dwIcon    || NIIF_INFO == dwIcon ||
		NIIF_WARNING == dwIcon || NIIF_ERROR == dwIcon);

	// The timeout must be between 10 and 30 seconds.
	ASSERT(uTimeout >= 10 && uTimeout <= 30);


	m_tnd.uFlags = NIF_INFO;
	_tcsncpy(m_tnd.szInfo, szText, 256);
	if (szTitle)
		_tcsncpy(m_tnd.szInfoTitle, szTitle, 64);
	else
		m_tnd.szInfoTitle[0] = _T('\0');
	m_tnd.dwInfoFlags = dwIcon;
	m_tnd.uTimeout = uTimeout * 1000;   // convert time to ms

	BOOL bSuccess = Shell_NotifyIcon (NIM_MODIFY, &m_tnd);

	// Zero out the balloon text string so that later operations won't redisplay
	// the balloon.
	m_tnd.szInfo[0] = _T('\0');

	return bSuccess;
}

/////////////////////////////////////////////////////////////////////////////
// SystemTray notification window stuff

BOOL SystemTray::SetNotificationWnd(HWND hNotifyWnd)
{
	if (!m_bEnabled)
		return FALSE;

	// Make sure Notification window is valid
	if (!hNotifyWnd || !::IsWindow(hNotifyWnd))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	m_tnd.hWnd = hNotifyWnd;
	m_tnd.uFlags = 0;

	if (m_bHidden)
		return TRUE;
	else
		return Shell_NotifyIcon(NIM_MODIFY, &m_tnd);
}

HWND SystemTray::GetNotificationWnd() const
{
	return m_tnd.hWnd;
}

// Hatr added

// Change or retrive the window to send menu commands to
BOOL SystemTray::SetTargetWnd(HWND hTargetWnd)
{
	m_hTargetWnd = hTargetWnd;
	return TRUE;
} // SystemTray::SetTargetWnd()

HWND SystemTray::GetTargetWnd() const
{
	if (m_hTargetWnd)
		return m_hTargetWnd;
	else
		return m_tnd.hWnd;
} // SystemTray::GetTargetWnd()

/////////////////////////////////////////////////////////////////////////////
// SystemTray notification message stuff

BOOL SystemTray::SetCallbackMessage(UINT uCallbackMessage)
{
	if (!m_bEnabled)
		return FALSE;

	// Make sure we avoid conflict with other messages
	ASSERT(uCallbackMessage >= WM_APP);

	m_tnd.uCallbackMessage = uCallbackMessage;
	m_tnd.uFlags = NIF_MESSAGE;

	if (m_bHidden)
		return TRUE;
	else
		return Shell_NotifyIcon(NIM_MODIFY, &m_tnd);
}

UINT SystemTray::GetCallbackMessage() const
{
	return m_tnd.uCallbackMessage;
}

/////////////////////////////////////////////////////////////////////////////
// SystemTray menu manipulation

BOOL SystemTray::SetMenuDefaultItem(UINT uItem, BOOL bByPos)
{
	if ((m_DefaultMenuItemID == uItem) && (m_DefaultMenuItemByPos == bByPos)) 
		return TRUE;

	m_DefaultMenuItemID = uItem;
	m_DefaultMenuItemByPos = bByPos;   

	HMENU hMenu = ::LoadMenu(m_hInstance, MAKEINTRESOURCE(m_tnd.uID));
	if (!hMenu)
		return FALSE;

	HMENU hSubMenu = ::GetSubMenu(hMenu, 0);
	if (!hSubMenu)
	{
		::DestroyMenu(hMenu);
		return FALSE;
	}

	::SetMenuDefaultItem(hSubMenu, m_DefaultMenuItemID, m_DefaultMenuItemByPos);

	::DestroyMenu(hSubMenu);
	::DestroyMenu(hMenu);

	return TRUE;
}

void SystemTray::GetMenuDefaultItem(UINT& uItem, BOOL& bByPos)
{
	uItem = m_DefaultMenuItemID;
	bByPos = m_DefaultMenuItemByPos;
}

VOID SystemTray::EnableMenuItem(UINT uItem, BOOL bEnable)
{
	if( bEnable)
		m_setGrayMenuItem.erase(uItem);
	else
		m_setGrayMenuItem.insert(uItem);
}


/////////////////////////////////////////////////////////////////////////////
// SystemTray message handlers

/* If we were in MFC this is what we'd use...
BEGIN_MESSAGE_MAP(SystemTray, CWnd)
//{{AFX_MSG_MAP(SystemTray)
ON_WM_TIMER()
//}}AFX_MSG_MAP
#ifndef _WIN32_WCE
ON_WM_SETTINGCHANGE()
#endif
ON_REGISTERED_MESSAGE(WM_TASKBARCREATED, OnTaskbarCreated)
END_MESSAGE_MAP()
*/

LRESULT SystemTray::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent != m_uIDTimer)
	{
		ASSERT(FALSE);
		return 0L;
	}

	time_t CurrentTime;
	time(&CurrentTime);

	time_t period = CurrentTime - m_StartTime;
	if (m_nAnimationPeriod > 0 && m_nAnimationPeriod < period)
	{
		StopAnimation();
		return 0L;
	}

	StepAnimation();

	return 0L;
}

// This is called whenever the taskbar is created (eg after explorer crashes
// and restarts. Please note that the WM_TASKBARCREATED message is only passed
// to TOP LEVEL windows (like WM_QUERYNEWPALETTE)
LRESULT SystemTray::OnTaskbarCreated(WPARAM wParam, LPARAM lParam) 
{
	InstallIconPending();
	return 0L;
}


LRESULT SystemTray::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	if (uFlags == SPI_SETWORKAREA)
		InstallIconPending();
	return 0L;
}


LRESULT SystemTray::OnTrayNotification(UINT wParam, LONG lParam) 
{
	//Return quickly if its not for this tray icon
	if (wParam != m_tnd.uID)
		return 0L;

	HWND hTargetWnd = GetTargetWnd();
	if (!hTargetWnd)
		return 0L;

	// Clicking with right button brings up a context menu
	if (LOWORD(lParam) == WM_RBUTTONUP)
	{    
		HMENU hMenu = ::LoadMenu(m_hInstance, MAKEINTRESOURCE(m_tnd.uID));
		if (!hMenu)
			return 0;

		HMENU hSubMenu = ::GetSubMenu(hMenu, 0);
		if (!hSubMenu)
		{
			::DestroyMenu(hMenu);        //Be sure to Destroy Menu Before Returning
			return 0;
		}

		// Make chosen menu item the default (bold font)
		::SetMenuDefaultItem(hSubMenu, m_DefaultMenuItemID, m_DefaultMenuItemByPos);
		
		// ����disable�˵���
		std::set<UINT>::iterator it;
		for( it = m_setGrayMenuItem.begin(); it != m_setGrayMenuItem.end(); it++ )
			::EnableMenuItem(hSubMenu, (*it), MF_BYCOMMAND | MF_GRAYED);

		// Display and track the popup menu
		POINT pos;
		GetCursorPos(&pos);

		::SetForegroundWindow(m_tnd.hWnd);  
		::TrackPopupMenu(hSubMenu, 0, pos.x, pos.y, 0, hTargetWnd, NULL);


		// BUGFIX: See "PRB: Menus for Notification Icons Don't Work Correctly"
		::PostMessage(m_tnd.hWnd, WM_NULL, 0, 0);

		DestroyMenu(hMenu);
	} 
	else if (LOWORD(lParam) == WM_LBUTTONDBLCLK) 
	{
		// double click received, the default action is to execute default menu item
		::SetForegroundWindow(m_tnd.hWnd);  

		UINT uItem;
		if (m_DefaultMenuItemByPos)
		{
			HMENU hMenu = ::LoadMenu(m_hInstance, MAKEINTRESOURCE(m_tnd.uID));
			if (!hMenu)
				return 0;

			HMENU hSubMenu = ::GetSubMenu(hMenu, 0);
			if (!hSubMenu)
				return 0;
			uItem = ::GetMenuItemID(hSubMenu, m_DefaultMenuItemID);

			DestroyMenu(hMenu);
		}
		else
			uItem = m_DefaultMenuItemID;

		::PostMessage(hTargetWnd, WM_COMMAND, uItem, 0);
	}

	return 1;
}

// This is the global (static) callback function for all TrayIcon windows
LRESULT PASCAL SystemTray::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// The option here is to maintain a list of all TrayIcon windows,
	// and iterate through them. If you do this, remove these 3 lines.
	SystemTray* pTrayIcon = m_pThis;
	if (pTrayIcon->GetSafeHwnd() != hWnd)
		return ::DefWindowProc(hWnd, message, wParam, lParam);

	// If maintaining a list of TrayIcon windows, then the following...
	// pTrayIcon = GetFirstTrayIcon()
	// while (pTrayIcon != NULL)
	// {
	//    if (pTrayIcon->GetSafeHwnd() != hWnd) continue;

	// Taskbar has been recreated - all TrayIcons must process this.
	if (message == SystemTray::m_nTaskbarCreatedMsg)
		return pTrayIcon->OnTaskbarCreated(wParam, lParam);

	// Animation timer
	if (message == WM_TIMER && wParam == pTrayIcon->GetTimerID())
		return pTrayIcon->OnTimer(wParam);

	// Settings changed
	if (message == WM_SETTINGCHANGE && wParam == pTrayIcon->GetTimerID())
		return pTrayIcon->OnSettingChange(wParam, (LPCTSTR) lParam);

	// Is the message from the icon for this TrayIcon?
	if (message == pTrayIcon->GetCallbackMessage())
		return pTrayIcon->OnTrayNotification(wParam, lParam);

	//    pTrayIcon = GetNextTrayIcon();
	// }

	// Message has not been processed, so default.
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

void SystemTray::InstallIconPending()
{
	// Is the icon display pending, and it's not been set as "hidden"?
	if (!m_bShowIconPending || m_bHidden)
		return;

	// Reset the flags to what was used at creation
	m_tnd.uFlags = m_uCreationFlags;

	// Try and recreate the icon
	m_bHidden = !Shell_NotifyIcon(NIM_ADD, &m_tnd);

	// If it's STILL hidden, then have another go next time...
	m_bShowIconPending = !m_bHidden;

	ASSERT(m_bHidden == FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// For minimising/maximising from system tray

BOOL CALLBACK FindTrayWnd(HWND hwnd, LPARAM lParam)
{
	TCHAR szClassName[256];
	GetClassName(hwnd, szClassName, 255);

	// Did we find the Main System Tray? If so, then get its size and keep going
	if (_tcscmp(szClassName, _T("TrayNotifyWnd")) == 0)
	{
		LPRECT lpRect = (LPRECT) lParam;
		::GetWindowRect(hwnd, lpRect);
		return TRUE;
	}

	// Did we find the System Clock? If so, then adjust the size of the rectangle
	// we have and quit (clock will be found after the system tray)
	if (_tcscmp(szClassName, _T("TrayClockWClass")) == 0)
	{
		LPRECT lpRect = (LPRECT) lParam;
		RECT rectClock;
		::GetWindowRect(hwnd, &rectClock);
		// if clock is above system tray adjust accordingly
		if (rectClock.bottom < lpRect->bottom-5) // 10 = random fudge factor.
			lpRect->top = rectClock.bottom;
		else
			lpRect->right = rectClock.left;
		return FALSE;
	}

	return TRUE;
}


void SystemTray::GetTrayWndRect(LPRECT lprect)
{
#define DEFAULT_RECT_WIDTH 150
#define DEFAULT_RECT_HEIGHT 30

	HWND hShellTrayWnd = FindWindow(_T("Shell_TrayWnd"), NULL);
	if (hShellTrayWnd)
	{
		GetWindowRect(hShellTrayWnd, lprect);
		EnumChildWindows(hShellTrayWnd, FindTrayWnd, (LPARAM)lprect);
		return;
	}
	// OK, we failed to get the rect from the quick hack. Either explorer isn't
	// running or it's a new version of the shell with the window class names
	// changed (how dare Microsoft change these undocumented class names!) So, we
	// try to find out what side of the screen the taskbar is connected to. We
	// know that the system tray is either on the right or the bottom of the
	// taskbar, so we can make a good guess at where to minimize to
	APPBARDATA appBarData;
	appBarData.cbSize=sizeof(appBarData);
	if (SHAppBarMessage(ABM_GETTASKBARPOS,&appBarData))
	{
		// We know the edge the taskbar is connected to, so guess the rect of the
		// system tray. Use various fudge factor to make it look good
		switch(appBarData.uEdge)
		{
		case ABE_LEFT:
		case ABE_RIGHT:
			// We want to minimize to the bottom of the taskbar
			lprect->top    = appBarData.rc.bottom-100;
			lprect->bottom = appBarData.rc.bottom-16;
			lprect->left   = appBarData.rc.left;
			lprect->right  = appBarData.rc.right;
			break;

		case ABE_TOP:
		case ABE_BOTTOM:
			// We want to minimize to the right of the taskbar
			lprect->top    = appBarData.rc.top;
			lprect->bottom = appBarData.rc.bottom;
			lprect->left   = appBarData.rc.right-100;
			lprect->right  = appBarData.rc.right-16;
			break;
		}
		return;
	}

	// Blimey, we really aren't in luck. It's possible that a third party shell
	// is running instead of explorer. This shell might provide support for the
	// system tray, by providing a Shell_TrayWnd window (which receives the
	// messages for the icons) So, look for a Shell_TrayWnd window and work out
	// the rect from that. Remember that explorer's taskbar is the Shell_TrayWnd,
	// and stretches either the width or the height of the screen. We can't rely
	// on the 3rd party shell's Shell_TrayWnd doing the same, in fact, we can't
	// rely on it being any size. The best we can do is just blindly use the
	// window rect, perhaps limiting the width and height to, say 150 square.
	// Note that if the 3rd party shell supports the same configuraion as
	// explorer (the icons hosted in NotifyTrayWnd, which is a child window of
	// Shell_TrayWnd), we would already have caught it above
	if (hShellTrayWnd)
	{
		::GetWindowRect(hShellTrayWnd, lprect);
		if (lprect->right - lprect->left > DEFAULT_RECT_WIDTH)
			lprect->left = lprect->right - DEFAULT_RECT_WIDTH;
		if (lprect->bottom - lprect->top > DEFAULT_RECT_HEIGHT)
			lprect->top = lprect->bottom - DEFAULT_RECT_HEIGHT;

		return;
	}

	// OK. Haven't found a thing. Provide a default rect based on the current work
	// area
	SystemParametersInfo(SPI_GETWORKAREA,0,lprect, 0);
	lprect->left = lprect->right - DEFAULT_RECT_WIDTH;
	lprect->top  = lprect->bottom - DEFAULT_RECT_HEIGHT;
}

// Check to see if the animation has been disabled (Matthew Ellis <m.t.ellis@bigfoot.com>)
BOOL SystemTray::GetDoWndAnimation()
{
	ANIMATIONINFO ai;

	ai.cbSize=sizeof(ai);
	SystemParametersInfo(SPI_GETANIMATION,sizeof(ai),&ai,0);
	return ai.iMinAnimate?TRUE:FALSE;
}


BOOL SystemTray::RemoveTaskbarIcon(HWND hWnd)
{
	// Create static invisible window
	if (!::IsWindow(m_hWndInvisible))
	{
		m_hWndInvisible = CreateWindowEx(0, _T("Static"), _T(""), WS_POPUP,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			NULL, 0, NULL, 0);

		if (!m_hWndInvisible)
			return FALSE;
	}

	SetParent(hWnd, m_hWndInvisible);
	return TRUE;
}

void SystemTray::MinimiseToTray(HWND hWnd)
{
	if (GetDoWndAnimation())
	{
		RECT rectFrom, rectTo;

		GetWindowRect(hWnd, &rectFrom);
		GetTrayWndRect(&rectTo);

		DrawAnimatedRects(hWnd, IDANI_CAPTION, &rectFrom, &rectTo);
	}

	RemoveTaskbarIcon(hWnd);
	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) &~ WS_VISIBLE);
}

void SystemTray::MaximiseFromTray(HWND hWnd)
{
	if (GetDoWndAnimation())
	{
		RECT rectTo;
		::GetWindowRect(hWnd, &rectTo);

		RECT rectFrom;
		GetTrayWndRect(&rectFrom);

		::SetParent(hWnd, NULL);
		DrawAnimatedRects(hWnd, IDANI_CAPTION, &rectFrom, &rectTo);
	}
	else
		::SetParent(hWnd, NULL);

	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) | WS_VISIBLE);
	RedrawWindow(hWnd, NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN | RDW_FRAME |
		RDW_INVALIDATE | RDW_ERASE);

	// Move focus away and back again to ensure taskbar icon is recreated
	if (::IsWindow(m_hWndInvisible))
		SetActiveWindow(m_hWndInvisible);
	SetActiveWindow(hWnd);
	SetForegroundWindow(hWnd);
}

} // namespace vEngine {