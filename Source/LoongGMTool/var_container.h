//-----------------------------------------------------------------------------
// File: var_container
// Auth: Lyp
// Date: 2004/04/02
// Last: 2004/11/17
//
// Desc: ��������
// Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

class Util;
//-----------------------------------------------------------------------------
// �̰߳�ȫ�ı�������
//-----------------------------------------------------------------------------
class VarContainer
{
public:
	// ���Ԫ��
	BOOL	Add(LPCTSTR pStr, LPCTSTR szName);
	BOOL	Add(DWORD dwData, LPCTSTR szName);
	BOOL	Add(FLOAT fData, LPCTSTR szName);


	// ��ȡԪ��
	LPCTSTR	GetString(LPCTSTR szName);
	DWORD	GetDword(LPCTSTR szName);
	FLOAT	GetFloat(LPCTSTR szName);

	LPCTSTR	GetString(LPCTSTR szName, LPCTSTR szField);
	DWORD	GetDword(LPCTSTR szName, LPCTSTR szField);
	FLOAT	GetFloat(LPCTSTR szName, LPCTSTR szField);

	LPCTSTR	GetString(LPCTSTR szName, LPCTSTR szField, LPCTSTR szDefault);
	DWORD	GetDword(LPCTSTR szName, LPCTSTR szField, DWORD dwDefault);
	FLOAT	GetFloat(LPCTSTR szName, LPCTSTR szField, FLOAT fDefault);

	// ����Ԫ�أ����û�о���Ӳ�����FALSE
	BOOL	Set(LPCTSTR szValue, LPCTSTR szName);
	BOOL	Set(DWORD dwValue, LPCTSTR szName);
	BOOL	Set(FLOAT fValue, LPCTSTR szName);


	// ɾ��Ԫ��
	BOOL	Erase(LPCTSTR szName);
	BOOL	Erase(LPCTSTR szName, LPCTSTR szField);
	VOID	Clear() { m_mapData.clear(); }


	// ������������
	DWORD	GetVarNum() { return m_mapData.size(); }	// �õ�Ԫ����Ŀ
	std::list<tstring>&	GetVarName();	// �õ��������м���

	BOOL Load(LPCSTR szVFS, LPCTSTR szFileName, LPCSTR szKeyName="name", std::list<tstring>* pFieldList = NULL);	// ��ȡ�ļ�
	BOOL Save(LPCTSTR szFileName);	// �����ļ�


	VarContainer();
	~VarContainer();

private:
	Util*						m_pUtil;
	std::map<tstring, tstring>	m_mapData;
	std::list<tstring>			m_listName;
	CRITICAL_SECTION			m_Lock;
};


