//--------------------------------------------------------------------------------------------------
// File: Field.h
// Desc: Database column value and attributes
// Time: 2008-05-04
// Auth: Aslan
//
// Copyright (c) 2008 Tengreng All rights reserved.
//---------------------------------------------------------------------------------------------------
#pragma once

#include "StdAfx.h"
#include "Config.h"

namespace Beton {

//----------------------------------------------------------------------------------------------------
/**	Filed�� - �ṩ���ݿ�ṹ���е�ǰĳһ��ĳһ�еĵ�ǰ����
	1. �����еĵ�ǰֵ����char*�洢�����ṩ���ض�������ȡ���ݵĹ��ܣ���BOLB�ֶ�����������
	2. �ṩ�ⲿ��ѯ�������ԣ������̶ȣ���ǰ�е�ǰ�еĳ��ȣ��Ƿ�ΪNULL���ֶ����͵ȵ�
	3. ��Unicode�ַ����ṩ֧�֣�ֻ�������ݱ��д洢utf8���͵��ַ���������m_szValue��char*��ʾ
*/
//----------------------------------------------------------------------------------------------------
class BETON_API Field
{
public:
	typedef enum						// ����ö��
	{
		EDBT_UNKNOWN	= 0,			// δ֪
		EDBT_INTEGER	= 1,			// ����
		EDBT_STRING		= 2,			// �ַ�����
		EDBT_FLOAT		= 3,			// ������
		EDBT_BLOB		= 4				// ����������
	} DataTypes;

public:
	Field();
	Field(char* szValue, char* szName, ulong uLen, ulong uMaxLen, DataTypes eType);
	Field(Field& field);
	
	~Field() {}

	BETON_INLINE VOID SetValue(char* szValue)	{ m_szValue = szValue; }
	BETON_INLINE VOID SetName(char* szName)		{ m_szName = szName; }	
	BETON_INLINE VOID SetType(DataTypes eType)		{ m_eType = eType; }
	BETON_INLINE VOID SetLen(ulong uLen)			{ m_uLen = uLen; }
	BETON_INLINE VOID SetMaxLen(ulong uMaxLen)		{ m_uMaxLen = uMaxLen; }
	BETON_INLINE VOID SetALL(char* szName, DataTypes eType, ulong uMaxLen)
	{
		SetName(szName); SetType(eType); SetMaxLen(uMaxLen);
	}

	BETON_INLINE BOOL IsNull() const { return m_szValue == NULL; }

	BETON_INLINE const char* GetName()	const	{ return m_szName; }
	BETON_INLINE DataTypes GetType() const		{ return m_eType; }
	BETON_INLINE ulong GetLen() const			{ return m_uLen; }
	BETON_INLINE ulong GetMaxLen() const		{ return m_uMaxLen; }

	BETON_INLINE const char* GetString() const 		{ return m_szValue; }
	BETON_INLINE BOOL GetBool()	const				{ return m_szValue ? atol(m_szValue) : 0; }
	BETON_INLINE INT GetInt() const					{ return m_szValue ? static_cast<INT>(atoi(m_szValue)) : 0; }
	BETON_INLINE DWORD GetDword() const				{ return m_szValue ? static_cast<DWORD>(_atoi64(m_szValue)) : 0; }
	BETON_INLINE INT64 GetLong() const				{ return m_szValue ? static_cast<LONG64>(_atoi64(m_szValue)) : 0; }
	BETON_INLINE BYTE GetByte() const				{ return m_szValue ? static_cast<BYTE>(_atoi64(m_szValue)) : 0; }
	BETON_INLINE SHORT GetShort() const				{ return m_szValue ? static_cast<SHORT>(_atoi64(m_szValue)) : 0; }
	BETON_INLINE FLOAT GetFloat() const				{ return m_szValue ? static_cast<FLOAT>(atof(m_szValue)) : 0.0f; }
	BETON_INLINE DOUBLE GetDouble() const			{ return m_szValue ? static_cast<DOUBLE>(atof(m_szValue)) : 0.0;}

	INT GetTCHAR(TCHAR* szValue, INT nLen, BOOL bCheckType = TRUE) const;
	BOOL GetBlob(VOID* buffer, ulong nLen) const;
	sstring GetUnicodeString() const;

private:
	char*		m_szValue;				// ���е�ֵ
	char*		m_szName;				// ���е�����
	DWORD		m_dwNameCrc;			// ���е�����CRC
	DataTypes	m_eType;				// �ֶ�����
	ulong		m_uLen;					// ��ǰĳ�и��еĳ��ȣ���ҪΪblog�ֶ�ʹ��
	ulong		m_uMaxLen;				// ���е����̶�
};


//------------------------------------------------------------------------------------------------------
// ���캯��
//------------------------------------------------------------------------------------------------------
inline Field::Field() 
: m_szValue(NULL), m_szName(NULL), m_eType(EDBT_UNKNOWN), m_uLen(0), m_uMaxLen(0)
{

}

inline Field::Field(Field& field) 
:m_szValue(field.m_szValue), m_szName(field.m_szName), m_eType(field.m_eType), m_uLen(field.m_uLen), m_uMaxLen(field.m_uMaxLen)
{

}

inline Field::Field(char* szValue, char* szName, ulong uLen, ulong uMaxLen, DataTypes eType)
: m_szValue(szValue), m_szName(szName), m_eType(eType), m_uLen(uLen), m_uMaxLen(uMaxLen)
{

}

} // namespace Beton

