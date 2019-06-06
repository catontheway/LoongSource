#pragma once
#include "XMLReader_Dll.h"

class XMLREADER_API XMLReader
{
public:
	enum { MAX_STR_LEN = 256 };

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

	BOOL Load(IFS *pFS, LPCTSTR szFileName, LPCSTR szKeyName="name", std::list<tstring>* pFieldList = NULL);	// ��ȡ�ļ�
	BOOL Save(LPCTSTR szFileName,LPCSTR szID_KeyName="name");	// �����ļ�

	XMLReader();
	~XMLReader();
private:
	VOID	CutSpaceOfStringHeadAndTail(tstring& string);
	LPCSTR	UnicodeToUnicode8(LPCTSTR szSrc);
	LPCTSTR	Unicode8ToUnicode(LPCSTR szSrc);
private:
	std::map<tstring, tstring>	m_mapData;
	std::list<tstring>			m_listName;
	CHAR						m_pCharPool[MAX_STR_LEN];
	TCHAR						m_pTCharPool[MAX_STR_LEN];
};


