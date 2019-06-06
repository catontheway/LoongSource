/********************************************************************
	created:	2008/04/29
	created:	29:4:2008   11:44
	filename: 	d:\loong_client\Client\GameScriptMgr.h
	file path:	d:\loong_client\Client
	file base:	GameScriptMgr
	file ext:	h
	author:		leili
	
	purpose:	��Ϸ�ű��������������
*********************************************************************/
#pragma once

#include "..\vEngine\script\lua\lua.hpp"
extern "C"
{
#include "..\vEngine\script\lua\lauxlib.h"
#include "..\vEngine\script\lua\lualib.h"
}

#ifdef _DEBUG
#pragma comment( lib, "..\\vsout\\lua\\Debug\\Lua.lib" )
#else
#pragma comment( lib, "..\\vsout\\lua\\Release\\Lua.lib" )
#endif

class GameScript;

class GameScriptMgr
{
public:
	GameScriptMgr(void);
	~GameScriptMgr(void);

	BOOL		Init(void);
	VOID		Update(float fElapsedSec);

	GameScript* CreateScript(LPCSTR szVFS, LPCTSTR szFileName, BOOL reCreated=TRUE);
	BOOL		DestroyScript(LPCTSTR szFileName);

	GameScript* GetScript(DWORD dwID)		{ return m_mapScript.Peek(dwID); }
	GameScript*	GetParseScript(void)		{ return m_pParseScript; }
	INT			GetNumScripts(void)			{ return m_mapScript.Size(); }
	LPCTSTR		GetMsg(DWORD msgID);
	LPCTSTR		GetQuestDesc(DWORD msgID);

	VOID		SetData(LPVOID pData, DWORD size);
	const BYTE*	GetData(void)		const	{ return m_pData; }
	VOID		AddParsePos(INT delta)		{ m_parsePos += delta; }
	DWORD		GetParsePos(void)	const	{ return m_parsePos; }
	DWORD		GetDataSize(void)	const	{ return m_dataSize; }

	void		RegFunc(LPCSTR szFunName,lua_CFunction pFunc);

public:
	lua_State					*m_masterState;

private:
	/** ����ID��STRING��Ӧ�ı�
	*/
	bool LoadIDRelevantMsg(void);

	/** ��������������
	*/
	bool LoadQuestDesc(void);

private:
	TObjRef<Util>				m_pUtil;
	TMap<DWORD, GameScript*>	m_mapScript;

	typedef std::map<DWORD, tstring*> IDRelevantMsgMap;
	IDRelevantMsgMap			m_mapIDRelevantMsg;
	IDRelevantMsgMap			m_mapQuestDesc;

	BYTE						*m_pData;		// �����������
	DWORD						m_dataSize;		// �������ݴ�С
	DWORD						m_parsePos;		// ��ǰ����λ��

	GameScript					*m_pParseScript;
};