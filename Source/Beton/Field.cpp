//--------------------------------------------------------------------------------------------------
// File: Field.cpp
// Desc: Database column value and attributes
// Time: 2008-05-04
// Auth: Aslan
//
// Copyright (c) 2008 Tengreng All rights reserved.
//---------------------------------------------------------------------------------------------------
#pragma once

#include "StdAfx.h"
#include "Config.h"
#include "Field.h"

namespace Beton {

//----------------------------------------------------------------------------------------------------
// �õ�TCHAR�ַ���
// ��������ݴ�����nLen��ֵ��������Ӧ���������nLenΪ0����ת���ɹ����򷵻�ֵΪ��Ҫ�ⲿ�ṩ�Ĵ洢�ռ�
// ���ַ��������nLen��Ϊ0�����������ַ�ת�������ת���ɹ����򷵻�ת���ɹ����ַ��������򷵻�0
//----------------------------------------------------------------------------------------------------
INT Field::GetTCHAR(TCHAR* szValue, INT nLen, BOOL bCheckType) const
{
	if( bCheckType && ( NULL == szValue || NULL == m_szValue || EDBT_BLOB == m_eType || EDBT_UNKNOWN == m_eType ))
		return 0;

#if defined _UNICODE

	return MultiByteToWideChar(CP_UTF8, 0, m_szValue, -1, szValue, nLen);

#else

	if( nLen < m_uLen + 1 ) return 0;
	memcpy(szValue, m_szValue, m_uLen+1);
	return m_uLen+1;

#endif
}

//------------------------------------------------------------------------------------------------------
// �õ�sstring�ַ���
// �����ǰ����UNICODE�������ڲ�����ת������ת�����ַ���д��sstring�У�����ֱ�ӿ���
//------------------------------------------------------------------------------------------------------
sstring Field::GetUnicodeString() const
{
	if( NULL == m_szValue || EDBT_BLOB == m_eType || EDBT_UNKNOWN == m_eType )
		return sstring(_T(""));

#if defined _UNICODE

	INT nDesiredLen = MultiByteToWideChar(CP_UTF8, 0, m_szValue, -1, NULL, 0);
	if( nDesiredLen == 0 ) return sstring(_T(""));

	TCHAR* szResultValue = new TCHAR[nDesiredLen];
	if( 0 == MultiByteToWideChar(CP_UTF8, 0, m_szValue, -1, szResultValue, nDesiredLen) )
	{
		delete []szResultValue;
		return sstring(_T(""));
	}

	sstring str(szResultValue);
	delete[] szResultValue;
	return str;

#else

	return sstring(m_szValue);

#endif

}

//--------------------------------------------------------------------------------------------------------
// �õ�blog�ֶε�ֵ�����ؿ������ֽ�����������Ȳ������򷵻�0
//--------------------------------------------------------------------------------------------------------
BOOL Field::GetBlob(VOID* buffer, ulong uLen) const
{
	if( uLen < m_uLen ) return FALSE;

	memcpy(buffer, m_szValue, m_uLen);
	return TRUE;
}

} // namespace Beton