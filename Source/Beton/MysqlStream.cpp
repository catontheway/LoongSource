//---------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: MysqlStream.cpp
// author: Aslan
// data: 2008-05-10
// last:
// brief: �����ƻ��ַ������ݲ�����
//---------------------------------------------------------------------------------

#include "StdAfx.h"
#include "Config.h"
#include "MysqlStream.h"

namespace Beton
{
//---------------------------------------------------------------------------------
// ���캯������ʼ�����ڴ�
//---------------------------------------------------------------------------------
MyStream::MyStream(INT nDefaultSize)
	: m_nBufLen(nDefaultSize), m_nDefaultSize(nDefaultSize), m_nPos(0)
{
	m_pBuf = new char[m_nDefaultSize];
	if( NULL == m_pBuf ) { abort(); }
	m_pBuf[0] = 0;
}

//----------------------------------------------------------------------------------
// ��������
//----------------------------------------------------------------------------------
MyStream::~MyStream()
{
	SAFE_DEL_ARRAY(m_pBuf);
}

//----------------------------------------------------------------------------
// �ַ����α��ƶ���ָ��λ��
//----------------------------------------------------------------------------
BOOL MyStream::Seek(INT nPos)
{
	if( nPos <= m_nBufLen - 1 )
	{
		m_nPos = nPos;
		End();
		return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------------------------
// ���ַ�����������
//-----------------------------------------------------------------------------
VOID MyStream::Grow(INT nSize)
{
	if( nSize <= 0 ) nSize = m_nDefaultSize;

	if( m_nPos + nSize <= m_nBufLen - 1 )
		return;

	INT nTmpLen = m_nBufLen  + nSize;
	char* pTmp = new char[nTmpLen];

	// ����ԭ��������
	if(m_pBuf)
	{
		memcpy(pTmp, m_pBuf, m_nBufLen);
		SAFE_DEL_ARRAY(m_pBuf);
	}

	m_pBuf = pTmp;
	m_nBufLen = nTmpLen;
}

//-------------------------------------------------------------------------------
// ����������غ���
//-------------------------------------------------------------------------------
MyStream& MyStream::operator << (const INT p)
{
	char szBuf[64] = {0};
	_itoa_s(p, szBuf, 10);

	INT nLen = strlen(szBuf);
	Grow(nLen);
	memcpy(m_pBuf + m_nPos, szBuf, nLen);
	m_nPos += nLen;
	End();
	return *this;
}

MyStream& MyStream::operator << (const DWORD p)
{
	char szBuf[64] = {0};
	_ultoa_s(p, szBuf, 10);

	INT nLen = strlen(szBuf);
	Grow(nLen);
	memcpy(m_pBuf + m_nPos, szBuf, nLen);
	m_nPos += nLen;
	End();
	return *this;

}

MyStream& MyStream::operator << (const INT64 p)
{
	char szBuf[96] = {0};
	_i64toa_s(p, szBuf, 96, 10);

	INT nLen = strlen(szBuf);
	Grow(nLen);
	memcpy(m_pBuf + m_nPos, szBuf, nLen);
	m_nPos += nLen;
	End();
	return *this;

}

MyStream& MyStream::operator << (const DOUBLE p)
{
	char szBuf[_CVTBUFSIZE] = {0};
	_gcvt_s(szBuf, p, 5);

	INT nLen = strlen(szBuf);
	Grow(nLen);
	memcpy(m_pBuf + m_nPos, szBuf, nLen);
	m_nPos += nLen;
	End();
	return *this;
}

//----------------------------------------------------------------------------
// ����Unicode�ַ���
//----------------------------------------------------------------------------
MyStream& MyStream::FillString(const char* p, Connection* con/* = NULL */)
{
	if( NULL == p || '\0' == p[0] ) return (*this);

	// ת���ַ���
	if( NULL != con )
	{
		INT nStrlen = strlen(p);
		INT nGrowSize = nStrlen*2 + 1;
		Grow(nGrowSize);
		char* pRet = m_pBuf + m_nPos;
		memset(pRet, 0, nGrowSize);
		INT nLen = mysql_real_escape_string(con->m_Mysql, pRet, p, nStrlen);	
		m_nPos += nLen;
		End();
	}
	// ��ת���ַ���
	else
	{
		INT nLen = strlen(p);
		Grow(nLen);
		char* pRet = m_pBuf + m_nPos;
		strcat(pRet, p);
		m_nPos += nLen;
		End();
	}

	return *this;
}

//------------------------------------------------------------------------------
// ���Unicode�ַ���
//------------------------------------------------------------------------------
MyStream& MyStream::FillString(const WCHAR* p, Connection* con/* =NULL */)
{
	if( NULL == p || 0 == p[0] ) return (*this);

	// �ַ���ת��
	INT nDesiredLen = WideCharToMultiByte(CP_UTF8, 0, p, -1, NULL, 0, NULL, NULL);
	if( 0 == nDesiredLen ) return (*this);

	char* szTemp = new char[nDesiredLen];
	if( 0 == WideCharToMultiByte(CP_UTF8, 0, p, -1, szTemp, nDesiredLen, NULL, NULL) )
	{
		SAFE_DEL_ARRAY(szTemp);
		return (*this);
	}

	// ���г����ַ�������
	FillString(szTemp, con);

	// ɾ����ʱ�ַ���
	SAFE_DEL_ARRAY(szTemp);

	return (*this);
}


//------------------------------------------------------------------------------
// ���BLOB�ֶ�
//------------------------------------------------------------------------------
MyStream& MyStream::FillBlob(const VOID* p, INT nSize, Connection* con/* =NULL */)
{
	if( NULL == p || nSize <= 0 )	return (*this);

	if( NULL == con ) return (*this);

	// ת��
	INT nGrowSize = nSize*2 + 1;
	Grow(nGrowSize);
	char* pRet = m_pBuf + m_nPos;
	memset(pRet, 0, nGrowSize);
	INT nLen = mysql_real_escape_string(con->m_Mysql, pRet, (char*)p, nSize);
	m_nPos += nLen;
	End();

	return *this;
}

//------------------------------------------------------------------------------
// ���BLOB�ֶ�
//------------------------------------------------------------------------------
MyStream& MyStream::FillBlob(VOID* p, INT nSize)
{
	Grow(nSize);
	memcpy(m_pBuf + m_nPos, p, nSize);
	m_nPos += nSize;
	End();

	return *this;
}

} // namespace Beton

