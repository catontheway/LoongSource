//-----------------------------------------------------------------------------
// File: globe_function
// Auth: Lyp
// Date: 2004/3/14
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "globe_function.h"

// ʹ����������
#include "interfacemgr.h"
#include "..\memory\new_protect.h"
#include "..\util\util.h"
#include "..\util\fastcode.h"
#include "..\debug\log.h"
#include "..\debug\debug.h"
#include "..\debug\mini_dump.h"
#include "..\render\render.h"
#include "..\render\d3d_interface.h"
#include "..\render\gdi_interface.h"
#include "..\memory\mem_map.h"
#include "..\system\disciomgr.h"
#include "..\system\sysinfo.h"
#include "..\system\thread.h"
#include "..\system\stdio_redir.h"
#include "..\filter\filtermgr.h"
#include "..\audio\audio.h"
#include "..\vfs\vfs.h"
#include "..\input\input_dx8.h"
#include "..\input\input_map.h"
#include "..\network\session.h"
#include "..\network\stream_server.h"
#include "..\network\stream_transport.h"
#include "..\network\x_server.h"
#include "..\network\x_client.h"
#include "..\network\ping.h"
#include "..\network\broadcast.h"
#include "..\network\net_cmd_mgr.h"

#include "..\image\image_obj.h"
#include "..\variable\ini_obj.h"
#include "..\variable\var_container.h"
#include "..\console\console_command.h"
#include "..\console\console_gui.h"
#include "..\system\window_wrap.h"
#include "..\gui\gui_system.h"
#include "..\gui\gui_editor.h"
#include "..\script\lua_manager.h"
#include "..\system\system_tray.h"


#undef new
#undef delete
#undef malloc
#undef calloc
#undef realloc
#undef free


namespace vEngine{
//-----------------------------------------------------------------------------
//! �����ʼ��
//-----------------------------------------------------------------------------
BOOL VENGINE_API InitEngine()
{
	if( NULL != g_pInterfaceMgr )	// ���治��Ҫ�ظ���ʼ��
		return TRUE;

	// ���ó���Ĭ��·��
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	TCHAR *pResult = _tcsrchr(szPath, _T('\\'));	// ��ȥ·��
	if(pResult)
		*pResult = 0;
	pResult = _tcsrchr(szPath, _T('\\'));
	if(pResult)
		*pResult = 0;
	SetCurrentDirectory(szPath);

	SetErrorMode(SEM_NOGPFAULTERRORBOX); // ���ⵯ��ϵͳ�����
	_set_new_handler( NewHandle );	// new ����
	_set_new_mode(1);	// malloc ����

	g_pInterfaceMgr = new ObjMgr;
	if( NULL == g_pInterfaceMgr )
		return FALSE;

	// ע������ӿ�, ��Ϊ����������ʱ���ᰴ��ע��ķ���˳��������
	// ɾ����ע�������, ����������ѻ����Ľӿڷ���ǰ��ע��
	g_pInterfaceMgr->Register(TWrap<Util>(),				"Util");
	g_pInterfaceMgr->Register(TWrap<VarContainer>(),		"VarContainer");
	g_pInterfaceMgr->Register(TWrap<D3DRender>(),			"D3DRender");
	g_pInterfaceMgr->Register(TWrap<GDIRender>(),			"GDIRender");
	g_pInterfaceMgr->Register(TWrap<InputDX8>(),			"InputDX8");
	g_pInterfaceMgr->Register(TWrap<ConsoleCommand>(),		"ConsoleCommand");
	g_pInterfaceMgr->Register(TWrap<Console>(),				"Console");
	g_pInterfaceMgr->Register(TWrap<Debug>(),				"Debug");
	g_pInterfaceMgr->Register(TWrap<DiscIOMgr>(),			"DiscIOMgr");
	g_pInterfaceMgr->Register(TWrap<Log>(),					"Log");
	g_pInterfaceMgr->Register(TWrap<ExceptionLog>(),		"ExceptionLog");
	g_pInterfaceMgr->Register(TWrap<FastCode>(),			"FastCode");
	g_pInterfaceMgr->Register(TWrap<SysInfo>(),				"SysInfo");
	g_pInterfaceMgr->Register(TWrap<VirtualFileSys>(),		"VirtualFileSys");
	g_pInterfaceMgr->Register(TWrap<FilterMgr>(),			"FilterMgr");
	g_pInterfaceMgr->Register(TWrap<Audio>(),				"Audio");
	g_pInterfaceMgr->Register(TWrap<ImageObj>(),			"ImageObj");
	g_pInterfaceMgr->Register(TWrap<INIObj>(),				"IniObj");
	g_pInterfaceMgr->Register(TWrap<CompletionSession>(),	"CompletionSession");
	g_pInterfaceMgr->Register(TWrap<StreamServer>(),		"StreamServer");
	g_pInterfaceMgr->Register(TWrap<StreamTransport>(),		"StreamTransport");
	g_pInterfaceMgr->Register(TWrap<XServer>(),				"XServer");
	g_pInterfaceMgr->Register(TWrap<XClient>(),				"XClient");
	g_pInterfaceMgr->Register(TWrap<Broadcast>(),			"Broadcast");
	g_pInterfaceMgr->Register(TWrap<Ping>(),				"Ping");
	g_pInterfaceMgr->Register(TWrap<NetCmdMgr>(),			"NetCmdMgr");
	g_pInterfaceMgr->Register(TWrap<WindowWrap>(),			"WindowWrap");
	g_pInterfaceMgr->Register(TWrap<Thread>(),				"Thread");
	g_pInterfaceMgr->Register(TWrap<GUISystem>(),			"GUISystem");
	g_pInterfaceMgr->Register(TWrap<GUIEditor>(),			"GUIEditor");
	g_pInterfaceMgr->Register(TWrap<LuaManager>(),			"LuaManager");
	g_pInterfaceMgr->Register(TWrap<InputMap>(),			"InputMap");
	g_pInterfaceMgr->Register(TWrap<StdioRedir>(),			"StdioRedir");
	g_pInterfaceMgr->Register(TWrap<SystemTray>(),			"SystemTray");

	
	TObjRef<VarContainer>()->Add(szPath, _T("DIRECTORY"));
	SetDefaultDir();

	TObjRef<VarContainer>()->Load(NULL, _T("config\\local.xml"));
	g_strLocalPath = TObjRef<VarContainer>()->GetString(_T("local_path"));
	return TRUE;
}

//-----------------------------------------------------------------------------
//! ��������
//-----------------------------------------------------------------------------
VOID VENGINE_API DestroyEngine()
{	
	SAFE_DEL(g_pInterfaceMgr);
}

//-----------------------------------------------------------------------------
// ����Ĭ��·��
//-----------------------------------------------------------------------------
VOID VENGINE_API SetDefaultDir()
{
	SetCurrentDirectory(TObjRef<VarContainer>()->GetString(_T("DIRECTORY")));
}


//-----------------------------------------------------------------------------
// ��ʼ�������
//-----------------------------------------------------------------------------
BOOL VENGINE_API InitNetwork()
{
	WORD wVersionRequested;
	WSADATA wsaData;

	// �ȳ���winsocket2.2
	wVersionRequested = MAKEWORD( 2, 2 );
	INT nErr = WSAStartup( wVersionRequested, &wsaData );
	if( 0 != nErr ) 
	{
		// ����winsocket2.1
		wVersionRequested = MAKEWORD( 2, 1 );
		nErr = WSAStartup( wVersionRequested, &wsaData );
		if( 0 != nErr ) 
		{
			// ����winsocket1.1
			wVersionRequested = MAKEWORD( 1, 1 );
			nErr = WSAStartup( wVersionRequested, &wsaData );
			if( 0 != nErr ) 
			{
				IMSG(_T("WSAStartup failed\r\n"));
				return FALSE;
			}
		}
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// ���������
//-----------------------------------------------------------------------------
VOID VENGINE_API DestroyNetwork()
{
	::WSACleanup();
}


//-----------------------------------------------------------------------------
// �õ���ָ�������ָ��
//-----------------------------------------------------------------------------
LPVOID VENGINE_API GetObj(LPCSTR szName) 
{ 
	// �Ƚ�ָ������תΪLPVOID,������ؼ̳�ʱ���������к���ECX�Ĵ���ƫ��
	return g_pInterfaceMgr->Get(szName); 
}


//-----------------------------------------------------------------------------
// ����ע����Ķ��󴴽�����
//-----------------------------------------------------------------------------
LPCSTR VENGINE_API CreateObj(LPCSTR szDestObjName, LPCSTR szSrcObjName ) 
{ 
	ASSERT( g_pInterfaceMgr );	// ����û�г�ʼ��

	// ���ݽӿڿ�¡����,��¡�ɹ��ʹ��ؿ�¡���Ķ��������
	if( TRUE == g_pInterfaceMgr->Create(szDestObjName, szSrcObjName) )
		return szDestObjName;
	else
		return NULL;
}


//-----------------------------------------------------------------------------
//!\return �����Ƿ�ɹ�
//-----------------------------------------------------------------------------
BOOL VENGINE_API KillObj(LPCSTR szObjName)
{
	ASSERT( g_pInterfaceMgr );	// ����û�г�ʼ��
	return g_pInterfaceMgr->Kill(szObjName);
}


//-----------------------------------------------------------------------------
// �ṹ���쳣ת��
//-----------------------------------------------------------------------------
VENGINE_API VOID SEH_Translation(UINT unCode, EXCEPTION_POINTERS* pExp)
{
	throw Exception(unCode, pExp);
}


//-----------------------------------------------------------------------------
// ���::SetUnhandledExceptionFilterʹ�õ��쳣������
// ���޷�ʹ��try...catch��ʱ����ʹ��
//-----------------------------------------------------------------------------
VENGINE_API LONG WINAPI SEH_ExceptionFilter(EXCEPTION_POINTERS* pExp)
{
	Exception(0, pExp);
	return EXCEPTION_CONTINUE_SEARCH;
}


//-----------------------------------------------------------------------------
// �����Ƿ���Log��������ʾ,Ĭ�ϴ���������û���ʾ��
//-----------------------------------------------------------------------------
VENGINE_API VOID InitDump(LPCTSTR szIP, LPCTSTR szUser, LPCTSTR szPassword, LPCTSTR szVersion)
{
	Exception::Init(szIP, szUser, szPassword, szVersion);
}



} // namespace vEngine{

