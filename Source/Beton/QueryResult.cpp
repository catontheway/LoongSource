//--------------------------------------------------------------------------------------------------
// File: QueryResult.cpp
// Desc: Database record set values and attributes
// Time: 2008-05-05
// Auth: Aslan
//
// Copyright (c) 2008 Tengreng All rights reserved.
//---------------------------------------------------------------------------------------------------
#pragma once

#include "StdAfx.h"
#include "QueryResult.h"
#include "Field.h"

namespace Beton {

//---------------------------------------------------------------------------------------------------
// ���캯��
//---------------------------------------------------------------------------------------------------
QueryResult::QueryResult(MYSQL_RES* result, INT nRows, INT nCols)
: m_Result(result), m_nRows(nRows), m_nCols(nCols)
{
	ASSERT(m_Result != NULL && m_nRows >= 0 && m_nCols > 0);

	m_CurrentRow = new Field[m_nCols];
	ASSERT( m_CurrentRow != NULL );

	MYSQL_FIELD* field = mysql_fetch_fields(m_Result);
	for(INT n = 0; n < m_nCols; n++)
	{
		m_CurrentRow[n].SetALL(field[n].name, ConvertType(field[n].type), field[n].max_length);
	}
}

//-----------------------------------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------------------------------
QueryResult::~QueryResult()
{
	delete[] m_CurrentRow;
	if( m_Result ) mysql_free_result(m_Result);
}

//-----------------------------------------------------------------------------------------------------
// �õ���һ��
// ע�⣺������QueryResultʱ���ϲ���Ҫ���̵���һ��NextRow���Ա㶨λ��������ĵ�һ��
//-----------------------------------------------------------------------------------------------------
BOOL QueryResult::NextRow()
{
	MYSQL_ROW row = mysql_fetch_row(m_Result);
	if( NULL == row )
		return FALSE;

	// ͬʱ������ֵ���еĳ���
	ulong* uLen = mysql_fetch_lengths(m_Result);
	for(INT n = 0; n < m_nCols; n++)
	{
		m_CurrentRow[n].SetValue(row[n]);
		m_CurrentRow[n].SetLen(uLen[n]);
	}

	return TRUE;
}

//-----------------------------------------------------------------------------------------------------
// ת��mysql����������ΪFieldԤ�����ö������
//-----------------------------------------------------------------------------------------------------
Field::DataTypes QueryResult::ConvertType(enum_field_types mysqlType) const
{
	switch (mysqlType)
	{
	case FIELD_TYPE_TIMESTAMP:
	case FIELD_TYPE_DATE:
	case FIELD_TYPE_TIME:
	case FIELD_TYPE_DATETIME:
	case FIELD_TYPE_YEAR:
	case FIELD_TYPE_STRING:
	case FIELD_TYPE_VAR_STRING:
	case FIELD_TYPE_SET:
	case FIELD_TYPE_NULL:
		return Field::EDBT_STRING;

	case FIELD_TYPE_TINY:
	case FIELD_TYPE_SHORT:
	case FIELD_TYPE_LONG:
	case FIELD_TYPE_INT24:
	case FIELD_TYPE_LONGLONG:
	case FIELD_TYPE_ENUM:
		return Field::EDBT_INTEGER;

	case FIELD_TYPE_DECIMAL:
	case FIELD_TYPE_FLOAT:
	case FIELD_TYPE_DOUBLE:
		return Field::EDBT_FLOAT;
		
	case FIELD_TYPE_BLOB:
		return Field::EDBT_BLOB;

	default:
		return Field::EDBT_UNKNOWN;
	}
}

} // namespace Beton