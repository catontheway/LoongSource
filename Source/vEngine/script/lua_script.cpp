//-----------------------------------------------------------------------------
//!\file lua_scrip.cpp
//!\author Lyp
//!
//!\date 2008-01-07
//! last 2008-01-07
//!
//!\brief Lua�ű�����
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "lua_script.h"
#include "lua_manager.h"
#include "lua\lua.hpp"

#include "..\console\console_gui.h"
#include "..\util\util.h"
#include "..\vfs\vfs.h"

#include "..\gui\gui_static.h"
#include "..\gui\gui_editboxex.h"

namespace vEngine {
//-----------------------------------------------------------------------------
// CONSTRUCTOR
//-----------------------------------------------------------------------------
LuaScript::LuaScript(LuaManager* pMgr, GUIEditBoxEx* pWndCode)
{
	m_pManager = pMgr;
	m_eState = ELSS_NotLoaded;
	m_pWndCode = pWndCode;
	m_nActiveLine = GT_INVALID;
	m_threadState = NULL;
}


//-----------------------------------------------------------------------------
// DESTRUCTOR
//-----------------------------------------------------------------------------
LuaScript::~LuaScript()
{
	// ���ԭ���Ļ���
	if( m_threadState )
	{
		lua_pushlightuserdata(m_pManager->m_masterState, m_threadState);
		lua_pushnil(m_pManager->m_masterState);
		lua_settable(m_pManager->m_masterState, LUA_GLOBALSINDEX );
	}

	if( !m_strName.empty() )
	{
		lua_pushnil(m_pManager->m_masterState);
		lua_setfield(m_pManager->m_masterState, LUA_REGISTRYINDEX, m_pUtil->UnicodeToUnicode8(m_strName.c_str()));
	}
}


//-----------------------------------------------------------------------------
// load from a file and run
//-----------------------------------------------------------------------------
BOOL LuaScript::LoadFile(LPCSTR szVFS, LPCTSTR szFileName)
{
	TObjRef<VirtualFileSys> pVFS;	// Ĭ��vfs
	if( P_VALID(szVFS) )
		pVFS = szVFS;

	//--��ȡ�ļ�
	DWORD dwSize = pVFS->GetSize(szFileName);
	if( !P_VALID(dwSize) || dwSize < 3 )	// �ļ��������ٱ�����ڵ���3���ֽ�
	{
		IMSG(_T("Script file not find : %s\r\n"), szFileName);
		return FALSE;
	}

	CHAR* szScript = (CHAR*)malloc(dwSize+2);
	if( !szScript )
		return FALSE;

	pVFS->LoadFile(szScript, szFileName);
	szScript[dwSize] = szScript[dwSize+1] = 0;	// ����UNICODE 0������

	INT nOffset = 0;	// LUA�ű�ʵ����ʼƫ����
	if( (szScript[0] == -17) && (szScript[1] == -69) && (szScript[2] == -65) )	// UTF-8 BOM
		nOffset = 3;

	std::string strRealCode;
	if( m_pManager->IsDebug() )
	{
		m_pWndCode->AddText(m_pUtil->Unicode8ToUnicode(szScript+nOffset));
		tstring& str = m_pWndCode->GetText(TRUE);
		strRealCode = m_pUtil->UnicodeToUnicode8(str.c_str());
	}
	else
	{	
		strRealCode.assign(szScript+nOffset);
	}

	// ��ȡ������
	this->CreateThread(szFileName);	// ���´����߳������
	INT nResult = luaL_loadbuffer(m_threadState, strRealCode.c_str(), strRealCode.size(), 
		m_pUtil->UnicodeToUnicode8(m_strName.c_str()));
	if( nResult )
	{
		FormatError();
		OutputError(_T("Script error"));
		return FALSE;
	}

	this->Resume(0);

	//--�ͷ���ʱ�ڴ�
	free(szScript);
	return TRUE;
}


//-----------------------------------------------------------------------------
// Rebuild
//-----------------------------------------------------------------------------
VOID LuaScript::Rebuild()
{
	this->CreateThread(m_strName);	// ���´����߳������

	tstring& str = m_pWndCode->GetText(TRUE);
	std::string strRealCode = m_pUtil->UnicodeToUnicode8(str.c_str());

	// ��ȡ������
	INT nResult = luaL_loadbuffer(m_threadState, strRealCode.c_str(), strRealCode.size(), 
		m_pUtil->UnicodeToUnicode8(m_strName.c_str()));
	if( nResult )
	{
		FormatError();
		OutputError(_T("Script error"));
		return;
	}
	
	m_pManager->DebugMsg(_T("Rebuild %s ok\r\n"), m_strName.c_str());
	this->SetState(ELSS_Done);
}


//-----------------------------------------------------------------------------
// ����editbox ex�ĵ�ǰ ִ�� ����ǩ
//-----------------------------------------------------------------------------
VOID LuaScript::SetActiveLine(INT nLine)
{
	if( GT_VALID(m_nActiveLine) )
	{
		m_pWndCode->SetBookmark(m_nActiveLine, GT_INVALID);	// ȡ����ǰ��
		if( m_mapBreakPoint.IsExist(m_nActiveLine) )
			m_pWndCode->SetBookmark(m_nActiveLine, 0xffff7070);	// ������ضϵ����ɫ

	}
	
	m_nActiveLine = nLine;
	m_pWndCode->SetBookmark(m_nActiveLine, GT_INVALID);	// �����ǰ�Ĳ���
	m_pWndCode->SetBookmark(m_nActiveLine, 0xffffff70);
	m_pWndCode->FlipToTop();
}


//-----------------------------------------------------------------------------
// ���öϵ�
//-----------------------------------------------------------------------------
VOID LuaScript::SetBreakPoint(INT nLine)
{
	if( m_mapBreakPoint.IsExist(nLine) )
	{
		// �Ѿ����ھ�ȡ��
		m_pWndCode->SetBookmark(nLine, GT_INVALID);
		m_mapBreakPoint.Erase(nLine);

		if( m_nActiveLine == nLine )
			m_pWndCode->SetBookmark(m_nActiveLine, 0xffffff70);
	}
	else
	{
		m_pWndCode->SetBookmark(nLine, 0xffffffff);	// �����ǰ�Ĳ���
		m_pWndCode->SetBookmark(nLine, 0xffff7070);
		m_mapBreakPoint.Add(nLine, 0xffff7070);
	}
}


//-----------------------------------------------------------------------------
// do a string
//-----------------------------------------------------------------------------
BOOL LuaScript::RunString(LPCSTR szCommandString)
{
	m_pManager->DebugMsg(_T("Excute:%s\r\n"), m_pUtil->Unicode8ToUnicode(szCommandString));
	INT nStatus = luaL_loadbuffer(m_threadState, szCommandString, strlen(szCommandString), "Immediate");
    if( nStatus )
	{
		FormatError();
		OutputError(_T("Immediate script error"));
		return FALSE;
	}

	Resume(0.0f);
	return TRUE;
}


//-----------------------------------------------------------------------------
// Update
// The purpose of update is basically to check if any suspended scripts need
// to be turned back on again!
//
// Returns the next script in the list.  This assists in linked list traversal
// where this script may delete it's next sibling.
//-----------------------------------------------------------------------------
VOID LuaScript::Update(float fElapsedSec)
{
	m_fWaitTime -= fElapsedSec;
    switch( m_eState )
	{
	case ELSS_WaitTime:
		if( m_fWaitTime <= 0 )
			Resume(0.0f);
		break;
	default:
		break;
	}
}



//-----------------------------------------------------------------------------
// ResumeScript
// Runs or resumes a LUA script.  Executes until a yeild is encountered or
// the script terminates.
// The parameter to this function is pushed on the stack as the return value
// of the previous yield.
//-----------------------------------------------------------------------------
VOID LuaScript::Resume(float fParam)
{
	// we're about to run/resume the thread, so set the global
	this->SetState(ELSS_Running);

	// param is treated as a return value from the function that yielded
	lua_pushnumber(m_threadState, fParam);

	INT nStatus = lua_resume(m_threadState, 1);
	if( nStatus != 0 && nStatus != 1 )
	{
		FormatError();
		OutputError(_T("Runtime Error"));
	}

	if( m_eState != ELSS_WaitTime && m_eState != ELSS_Break)
		this->SetState(ELSS_Done);
}


//-----------------------------------------------------------------------------
// �����߳������
//-----------------------------------------------------------------------------
VOID LuaScript::CreateThread(tstring strName)
{
	// ���ԭ���Ļ���
	if( m_threadState )
	{
		lua_pushlightuserdata(m_pManager->m_masterState, m_threadState);
		lua_pushnil(m_pManager->m_masterState);
		lua_settable(m_pManager->m_masterState, LUA_GLOBALSINDEX );
	}

	if( !m_strName.empty() )
	{
		lua_pushnil(m_pManager->m_masterState);
		lua_setfield(m_pManager->m_masterState, LUA_REGISTRYINDEX, m_pUtil->UnicodeToUnicode8(m_strName.c_str()));
	}

	m_strName = strName;

	// create a thread/state for this object
	m_threadState = lua_newthread(m_pManager->m_masterState);
	// ���̼߳�¼��luaע���,1��ֹlua_newthread��ջռ�� 2��ֹ�������ռ���
	lua_setfield(m_pManager->m_masterState, LUA_REGISTRYINDEX, m_pUtil->UnicodeToUnicode8(m_strName.c_str()));

	// save a pointer to the thread manager object in the global table
	// using the new thread's vm pointer as a key
	lua_pushlightuserdata(m_pManager->m_masterState, m_threadState);
	lua_pushlightuserdata(m_pManager->m_masterState, this );
	lua_settable(m_pManager->m_masterState, LUA_GLOBALSINDEX );

	this->SetState(ELSS_NotLoaded);
}


//-----------------------------------------------------------------------------
// FormatError
// Grabs the last error message and stuffs it in lastErrorString.
//-----------------------------------------------------------------------------
VOID LuaScript::FormatError()
{
	const char* msg = lua_tostring(m_threadState, -1);
	if( !msg )
		msg = "(error with no message)";
	lua_pop(m_threadState, 1);
	strcpy(m_lastErrorString, msg);
}


//-----------------------------------------------------------------------------
// OUTPUT ERROR
//-----------------------------------------------------------------------------
VOID LuaScript::OutputError(LPCTSTR strType)
{
	m_pManager->DebugMsg(_T("%s:%s\r\n"), strType, m_pUtil->Unicode8ToUnicode(m_lastErrorString));
}


//-----------------------------------------------------------------------------
// ���ýű�״̬
//-----------------------------------------------------------------------------
VOID LuaScript::SetState(ELuaScriptState eState)
{
	ELuaScriptState eOrigin = m_eState;
	m_eState = eState;

	// ֪ͨmgr״̬�ı�
	// WaitTime �� running��״̬�л�����Ҫ֪ͨ
	if( !((eOrigin == ELSS_WaitTime && m_eState == ELSS_Running) 
		|| (eOrigin == ELSS_Running && m_eState == ELSS_WaitTime))  )
		m_pManager->OnScriptStateChange(this);
}




//-----------------------------------------------------------------------------
// table of function names and entry points that will be registered with Lua
//-----------------------------------------------------------------------------
static const luaL_reg scriptLib[] =
{
	{"Print",		LuaConsolePrint},
	{"WaitSec",		LuaWaitSec},
	{NULL, NULL}
};


//-----------------------------------------------------------------------------
// LUA OPEN SCRIPT LIB
// Registers all the basic script support functions with Lua.
//-----------------------------------------------------------------------------
VOID LuaOpenScriptLib(lua_State*  l)
{
	// register our library of routines under the table name Script
	luaL_openlib(l, "vEngine", scriptLib, 0);
}


//-----------------------------------------------------------------------------
// LuaConsolePrint
// Print a console string
// the stack.
//-----------------------------------------------------------------------------
int LuaConsolePrint(lua_State* l)
{
	LPCSTR szText = (char*)(lua_tostring(l, -1));	// UTF-8
	std::string str;
	if( !P_VALID(szText) )
	{
		str = luaL_typename(l, -1);
		str.append("[?]");
		szText = str.c_str();
	}
	
	TObjRef<Console>()->Print(TObjRef<Util>()->Unicode8ToUnicode(szText));
	return(0);
}


//-----------------------------------------------------------------------------
// LUA WAIT SEC
// Causes a script to wait for the given number of seconds.
//-----------------------------------------------------------------------------
int LuaWaitSec(lua_State* l)
{
	LuaScript* s = GetScriptObject(l);
	s->SetWaitTime((float)luaL_checknumber(l, 1));
	s->SetState(ELSS_WaitTime);

	return(lua_yield(l, 1));
}


//-----------------------------------------------------------------------------
// GET SCRIPT OBJECT
// Retrieves the address of the C++ object managing the given lua_State.
//-----------------------------------------------------------------------------
LuaScript* GetScriptObject(lua_State* l)
{
	lua_pushlightuserdata(l, l);
	lua_gettable(l, LUA_GLOBALSINDEX);
	return((LuaScript*)lua_touserdata(l, -1));
}


} // namespace vEngine{