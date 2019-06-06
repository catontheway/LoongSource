//-----------------------------------------------------------------------------
// File: var_container
// Auth: Lyp
// Date: 2004/04/02
// Last: 2004/11/17
//
// Desc: ��������
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine {


//-----------------------------------------------------------------------------
// �̰߳�ȫ�ı�������
//-----------------------------------------------------------------------------
class VENGINE_API VarContainer
{
public:
	// ���Ԫ��
	__forceinline  BOOL	Add(tstring& strStr, tstring& strName);

	__forceinline BOOL	Add(LPCTSTR pStr, LPCTSTR szName);
	__forceinline BOOL	Add(DWORD dwData, LPCTSTR szName);
	__forceinline BOOL	Add(FLOAT fData, LPCTSTR szName);

	// ��ȡԪ��
	__forceinline LPCTSTR GetString(LPCTSTR szName);
	__forceinline DWORD	GetDword(LPCTSTR szName);
	__forceinline FLOAT	GetFloat(LPCTSTR szName);
	__forceinline INT32	GetInt(LPCTSTR szName);

	__forceinline LPCTSTR GetString(LPCTSTR szName, LPCTSTR szField);
	__forceinline DWORD	GetDword(LPCTSTR szName, LPCTSTR szField);
	__forceinline FLOAT	GetFloat(LPCTSTR szName, LPCTSTR szField);
	__forceinline INT32	GetInt(LPCTSTR szName, LPCTSTR szField);

	__forceinline LPCTSTR GetString(LPCTSTR szName, LPCTSTR szField, LPCTSTR szDefault);
	__forceinline DWORD	GetDword(LPCTSTR szName, LPCTSTR szField, DWORD dwDefault);
	__forceinline FLOAT	GetFloat(LPCTSTR szName, LPCTSTR szField, FLOAT fDefault);
	__forceinline INT32	GetInt(LPCTSTR szName, LPCTSTR szField, INT32 nDefault);

	__forceinline LPCTSTR GetString(LPCTSTR szName, LPCTSTR szNamePostfix, LPCTSTR szField, LPCTSTR szDefault);
	__forceinline DWORD	GetDword(LPCTSTR szName, LPCTSTR szNamePostfix, LPCTSTR szField, DWORD dwDefault);
	__forceinline FLOAT	GetFloat(LPCTSTR szName, LPCTSTR szNamePostfix, LPCTSTR szField, FLOAT fDefault);
	__forceinline INT32	GetInt(LPCTSTR szName, LPCTSTR szNamePostfix, LPCTSTR szField, INT32 nDefault);

	// ����Ԫ�أ����û�о���Ӳ�����FALSE
	BOOL	Set(LPCTSTR szValue, LPCTSTR szName);
	BOOL	Set(DWORD dwValue, LPCTSTR szName);
	BOOL	Set(FLOAT fValue, LPCTSTR szName);

	// ɾ��Ԫ��
	BOOL	Erase(LPCTSTR szName);
	BOOL	Erase(LPCTSTR szName, LPCTSTR szField);
	VOID	Clear() { m_mapData.clear(); m_mapName.clear(); }

	// ������������
	DWORD	GetVarNum() { return m_mapData.size(); }	// �õ�Ԫ����Ŀ
	std::list<tstring>&	GetVarName();	// �õ��������м���

	BOOL Load(LPCSTR szVFS, LPCTSTR szFileName, LPCSTR szID_KeyName="name", std::list<tstring>* pFieldList = NULL);	// ��ȡ�ļ�
	BOOL Save(LPCTSTR szFileName);	// �����ļ�

	// �����Ƿ�֧�ִ���
	VOID SetSaveSupport(BOOL bSupportSave) { m_bSupportSave = bSupportSave;	}

	// Ĭ�ϲ�֧�ִ���
	VarContainer():m_bSupportSave(FALSE){}

private:
	TObjRef<Util>				m_pUtil;
	std::map<DWORD, tstring>	m_mapData;

	BOOL						m_bSupportSave;
	std::map<DWORD, tstring>	m_mapName;
	std::list<tstring>			m_listName;
};

//-----------------------------------------------------------------------------
// ����ַ���Ԫ��
//-----------------------------------------------------------------------------
BOOL VarContainer::Add(tstring& strStr, tstring& strName)
{
	DWORD dwID = m_pUtil->Crc32(strName.c_str());
	if( m_mapData.insert(std::pair<DWORD, tstring>(dwID, strStr)).second == NULL )	
		ERR(_T("Couldn't add <%s> twice to var_container"), strName.c_str());	// ID �ظ�

	if( m_bSupportSave )
		m_mapName.insert(std::pair<DWORD, tstring>(dwID, strName));
	return TRUE;
}

//-----------------------------------------------------------------------------
// ����ַ���Ԫ��
//-----------------------------------------------------------------------------
BOOL VarContainer::Add(LPCTSTR pStr, LPCTSTR szName)
{
	DWORD dwID = m_pUtil->Crc32(szName);
	tstring str = pStr;

	if( NULL == m_mapData.insert(std::pair<DWORD, tstring>(dwID, str)).second )	
		ERR(_T("Couldn't add <%s> twice to var_container"), szName);	// ID �ظ�

	if( m_bSupportSave )
	{
		tstring strName = szName;
		m_mapName.insert(std::pair<DWORD, tstring>(dwID, strName));
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// ���DWORDԪ��
//-----------------------------------------------------------------------------
BOOL VarContainer::Add(DWORD dwData, LPCTSTR szName)
{
	TCHAR mess[X_LONG_STRING];
	_itot(dwData, mess, 10);
	return this->Add(mess, szName);
}


//-----------------------------------------------------------------------------
// ���FLOATԪ��
//-----------------------------------------------------------------------------
BOOL VarContainer::Add(FLOAT fData, LPCTSTR szName)
{
	CHAR mess[X_LONG_STRING];
	_gcvt(fData, 6, mess);
	return this->Add(m_pUtil->Unicode8ToUnicode(mess), szName);
}



//-----------------------------------------------------------------------------
// ��ȡ�ַ���Ԫ��
//-----------------------------------------------------------------------------
LPCTSTR VarContainer::GetString(LPCTSTR szName)
{
	std::map<DWORD, tstring>::iterator it;
	DWORD dwID = m_pUtil->Crc32(szName);

	it = m_mapData.find(dwID);
	if( it == m_mapData.end() )
	{
		ERR(_T("Couldn't find <%s> in var_container"), szName);
		return NULL;	// �Ҳ���
	}

	LPCTSTR pReturn = it->second.c_str();
	return pReturn;
}


//-----------------------------------------------------------------------------
// ��ȡ����Ԫ��
//-----------------------------------------------------------------------------
DWORD VarContainer::GetDword(LPCTSTR szName)
{
	LPCTSTR szValue = this->GetString(szName);
	return _tcstol(szValue, NULL, 10);
}


//-----------------------------------------------------------------------------
// ��ȡ��������Ԫ��
//-----------------------------------------------------------------------------
FLOAT VarContainer::GetFloat(LPCTSTR szName)
{
	LPCTSTR szValue = this->GetString(szName);
	return (FLOAT)_tstof(szValue);
}

//-----------------------------------------------------------------------------
// ��ȡ��������Ԫ��
//-----------------------------------------------------------------------------
INT32 VarContainer::GetInt(LPCTSTR szName)
{
	LPCTSTR szValue = this->GetString(szName);
	return (INT32)_tcstol(szValue, NULL, 10);
}


//-----------------------------------------------------------------------------
// ��ȡ�ַ���Ԫ��
//-----------------------------------------------------------------------------
LPCTSTR VarContainer::GetString(LPCTSTR szName, LPCTSTR szField)
{
	tstring str = szName;
	str.append(_T(" "));
	str.append(szField);
	return GetString(str.c_str());
}

//-----------------------------------------------------------------------------
// ��ȡ����Ԫ��
//-----------------------------------------------------------------------------
DWORD VarContainer::GetDword(LPCTSTR szName, LPCTSTR szField)
{
	tstring str = szName;
	str.append(_T(" "));
	str.append(szField);
	return GetDword(str.c_str());
}


//-----------------------------------------------------------------------------
// ��ȡ��������Ԫ��
//-----------------------------------------------------------------------------
FLOAT VarContainer::GetFloat(LPCTSTR szName, LPCTSTR szField)
{
	tstring str = szName;
	str.append(_T(" "));
	str.append(szField);
	return GetFloat(str.c_str());
}

//-----------------------------------------------------------------------------
// ��ȡ��������Ԫ��
//-----------------------------------------------------------------------------
INT32 VarContainer::GetInt(LPCTSTR szName, LPCTSTR szField)
{
	tstring str = szName;
	str.append(_T(" "));
	str.append(szField);
	return GetInt(str.c_str());
}


//-----------------------------------------------------------------------------
// ��ȡ�ַ���Ԫ��
//-----------------------------------------------------------------------------
LPCTSTR VarContainer::GetString(LPCTSTR szName, LPCTSTR szField, LPCTSTR szDefault)
{
	std::map<DWORD, tstring>::iterator it;
	TCHAR mess[X_LONG_STRING];

	_tcscpy(mess, szName);
	if( P_VALID(szField) )
	{
		_tcscat(mess, _T(" "));
		_tcscat(mess, szField);
	}

	DWORD dwID = m_pUtil->Crc32(mess);
	it = m_mapData.find(dwID);
	if( it == m_mapData.end() )
		return szDefault;	// �Ҳ���

	return it->second.c_str();
}


//-----------------------------------------------------------------------------
// ��ȡ����Ԫ��
//-----------------------------------------------------------------------------
DWORD VarContainer::GetDword(LPCTSTR szName, LPCTSTR szField, DWORD dwDefault)
{
	LPCTSTR szReturn = this->GetString(szName, szField, NULL);
	if( NULL == szReturn )
		return dwDefault;

	return _tcstol(szReturn, NULL, 10);
}


//-----------------------------------------------------------------------------
// ��ȡ��������Ԫ��
//-----------------------------------------------------------------------------
FLOAT VarContainer::GetFloat(LPCTSTR szName, LPCTSTR szField, FLOAT fDefault)
{
	LPCTSTR szReturn = this->GetString(szName, szField, NULL);
	if( NULL == szReturn )
		return fDefault;

	return (FLOAT)_tstof(szReturn);
}

//-----------------------------------------------------------------------------
// ��ȡ��������Ԫ��
//-----------------------------------------------------------------------------
INT32 VarContainer::GetInt(LPCTSTR szName, LPCTSTR szField, INT32 nDefault)
{
	LPCTSTR szReturn = this->GetString(szName, szField, NULL);
	if( NULL == szReturn )
		return nDefault;

	return (INT32)_tcstol(szReturn, NULL, 10);
}

//-----------------------------------------------------------------------------
// ��ȡ�ַ���Ԫ��
//-----------------------------------------------------------------------------
LPCTSTR	VarContainer::GetString(LPCTSTR szName, LPCTSTR szNamePostfix, LPCTSTR szField, LPCTSTR szDefault)
{
	std::map<DWORD, tstring>::iterator it;
	tstring title = szName;
	title += szNamePostfix;
	if( P_VALID(szField) )
	{
		title.append(_T(" "));
		title.append(szField);
	}

	DWORD dwID = m_pUtil->Crc32(title.c_str());
	it = m_mapData.find(dwID);
	if( it == m_mapData.end() )
		return szDefault;	// �Ҳ���

	return it->second.c_str();
}

//-----------------------------------------------------------------------------
// ��ȡ����Ԫ��
//-----------------------------------------------------------------------------
DWORD VarContainer::GetDword(LPCTSTR szName, LPCTSTR szNamePostfix, LPCTSTR szField, DWORD dwDefault)
{
	LPCTSTR szReturn = this->GetString(szName, szNamePostfix, szField, NULL);
	if( NULL == szReturn )
		return dwDefault;

	return _tcstol(szReturn, NULL, 10);
}


//-----------------------------------------------------------------------------
// ��ȡ��������Ԫ��
//-----------------------------------------------------------------------------
FLOAT VarContainer::GetFloat(LPCTSTR szName, LPCTSTR szNamePostfix, LPCTSTR szField, FLOAT fDefault)
{
	LPCTSTR szReturn = this->GetString(szName, szNamePostfix, szField, NULL);
	if( NULL == szReturn )
		return fDefault;

	return (FLOAT)_tstof(szReturn);
}

//-----------------------------------------------------------------------------
// ��ȡ��������Ԫ��
//-----------------------------------------------------------------------------
INT32 VarContainer::GetInt(LPCTSTR szName, LPCTSTR szNamePostfix, LPCTSTR szField, INT32 nDefault)
{
	LPCTSTR szReturn = this->GetString(szName, szNamePostfix, szField, NULL);
	if( NULL == szReturn )
		return nDefault;

	return (INT32)_tcstol(szReturn, NULL, 10);
}



} // namespace vEngine {
