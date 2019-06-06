/********************************************************************
	created:	2008/04/29
	created:	29:4:2008   14:10
	filename: 	d:\loong_client\Client\GameScript.cpp
	file path:	d:\loong_client\Client
	file base:	GameScript
	file ext:	cpp
	author:		leili
	
	purpose:	�ű�����ʵ��
*********************************************************************/
#include "StdAfx.h"
#include "GameScript.h"

//-----------------------------------------------------------------------------
// ���캯��
//-----------------------------------------------------------------------------
GameScript::GameScript( void )
: m_eState(EGSS_NotLoad)
, m_threadState(0)
{
}

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
GameScript::~GameScript( void )
{
	Clear();
}

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
VOID GameScript::Update( float fElapsedSec )
{
	m_fWaitTime -= fElapsedSec;
	switch( m_eState )
	{
	case EGSS_WaitTime:
		if( m_fWaitTime <= 0.0f )
			Resume(0.0f);
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// �����ļ���������
//-----------------------------------------------------------------------------
BOOL GameScript::LoadFile( LPCSTR szVFS, LPCTSTR szFileName )
{
	TObjRef<VirtualFileSys> pVFS;
	if( P_VALID(szVFS) )
		pVFS = szVFS;

	//--��ȡ�ļ�
	DWORD dwSize = pVFS->GetSize(szFileName);
	if( !P_VALID(dwSize) || dwSize < 3 )	// �ļ����ȱ�����ڵ���3���ֽ�
	{
		//ERR(_T("Script file not find : %s\r\n"), szFileName);
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
	strRealCode.assign(szScript+nOffset);

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

	this->SetState(EGSS_Loaded);
	this->Resume(0.0f);

	//--�ͷ���ʱ�ڴ�
	free(szScript);
	return TRUE;
}

//-----------------------------------------------------------------------------
// ResumeScript
// Runs or resumes a LUA script.  Executes until a yield is encountered or
// the script terminates.
// The parameter to this function is pushed on the stack as the return value
// of the previous yield.
//-----------------------------------------------------------------------------
VOID GameScript::Resume( float fParam )
{
	// param is treated as a return value from the function that yielded
	lua_pushnumber(m_threadState, fParam);

	INT nStatus = lua_resume(m_threadState, 1);
	if( nStatus != 0 && nStatus != LUA_YIELD )
	{
		FormatError();
		OutputError(_T("Runtime Error"));
	}

	if( m_eState != EGSS_WaitTime && m_eState != EGSS_Break)
		this->SetState(EGSS_Done);
}

//-----------------------------------------------------------------------------
// ִ��һ���ַ���
//-----------------------------------------------------------------------------
BOOL GameScript::RunString( LPCSTR szCommandString )
{
	INT nStatus = luaL_loadbuffer(m_threadState, szCommandString, strlen(szCommandString), "Immediate");
	if( nStatus )
	{
		FormatError();
		OutputError(_T("Immediate script error"));
		return FALSE;
	}
	this->SetState(EGSS_Running);
	Resume(0.0f);

	return TRUE;
}

//-----------------------------------------------------------------------------
// FormatError
// Grabs the last error message and stuffs it in lastErrorString.
//-----------------------------------------------------------------------------
VOID GameScript::FormatError()
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
VOID GameScript::OutputError( LPCTSTR strType )
{
	IMSG(_T("%s:%s\r\n"), strType, m_pUtil->Unicode8ToUnicode(m_lastErrorString));
}

//-----------------------------------------------------------------------------
// �����߳������
//-----------------------------------------------------------------------------
VOID GameScript::CreateThread( tstring strName )
{
	Clear();

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
}

//-----------------------------------------------------------------------------
// ���ԭ���Ļ���
//-----------------------------------------------------------------------------
VOID GameScript::Clear()
{
	if( m_threadState != NULL )
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

LPCSTR GameScript::GetObjString(void* obj, void* proto)
{
	lua_getglobal(m_threadState, "GetObjString");
	lua_pushlightuserdata(m_threadState, obj);
	lua_pushlightuserdata(m_threadState, proto);

	INT nStatus = lua_pcall(m_threadState, 2, 1, 0);
	if( nStatus )
	{
		FormatError();
		OutputError(_T("Immediate script error"));
		return "";
	}

	nStatus = lua_isstring(m_threadState, -1);
	if( 0 == nStatus )
	{
		FormatError();
		OutputError(_T("Immediate script return error"));
		return "";
	}

	LPCSTR szTmp = lua_tostring(m_threadState, -1);
	lua_pop(m_threadState, 1);

	this->SetState(EGSS_Running);
	Resume(0.0f);
	return szTmp;
}

LPCSTR GameScript::GetObjString( void* proto, DOUBLE quality, DOUBLE nameid )
{
	lua_getglobal(m_threadState, "GetObjString");
	lua_pushlightuserdata(m_threadState, proto);
	lua_pushnumber(m_threadState, quality);
	lua_pushnumber(m_threadState, nameid);

	INT nStatus = lua_pcall(m_threadState, 3, 1, 0);
	if( nStatus )
	{
		FormatError();
		OutputError(_T("Immediate script error"));
		return "";
	}

	nStatus = lua_isstring(m_threadState, -1);
	if( 0 == nStatus )
	{
		FormatError();
		OutputError(_T("Immediate script return error"));
		return "";
	}

	LPCSTR szTmp = lua_tostring(m_threadState, -1);
	lua_pop(m_threadState, 1);

	this->SetState(EGSS_Running);
	Resume(0.0f);
	return szTmp;
}