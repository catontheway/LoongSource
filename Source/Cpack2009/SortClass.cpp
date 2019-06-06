//-------------------------------------------------------------------------------------------------
// Copyright (c) 2006 TENGWU Entertainment All rights reserved.
// filename: main.h
// author: ��������
// actor:
// data: 2006-08-10
// last:
// brief: clistview�ؼ�item������
//-------------------------------------------------------------------------------------------------
#include "stdafx.h"
#include ".\sortclass.h"


//-------------------------------------------------------------------------------------------------
CSortClass::CSortItem::CSortItem(const CString & _txt, BOOL bFile )
{
	txt = _txt;
	this->bFile = bFile;
}

CSortClass::CSortItem::~CSortItem()
{
}

CSortClass::CSortItemInt::CSortItemInt(const CString & _txt)
{
	lValue = atol(_txt);
}

//-------------------------------------------------------------------------------------------------
//constructor
//-------------------------------------------------------------------------------------------------
CSortClass::CSortClass(CListCtrl * pListCtrl, const int nColIndex, const INT nSortMode)
{
	m_nColIndex = nColIndex;
	m_pListCtrl = pListCtrl;
	m_nSortMode = nSortMode;

	ASSERT(m_pListCtrl);

	INT nMaxItemNum = m_pListCtrl->GetItemCount();
	CString txt, strFile;

	switch( (ESortMode)nSortMode )
	{
	case ESM_String:
		{
			for (int t = 0; t < nMaxItemNum; t++)
			{
				txt = m_pListCtrl->GetItemText(t, m_nColIndex);
				strFile = m_pListCtrl->GetItemText(t, 1 );
				if( strFile == "Ŀ¼" )
					m_pListCtrl->SetItemData(t, (DWORD) new CSortItem(txt, FALSE) );
				else
					m_pListCtrl->SetItemData(t, (DWORD) new CSortItem(txt) );
			}
		}
		break;
	case ESM_Value:
		{
			for (int t = 0; t < nMaxItemNum; t++)
			{
				txt = m_pListCtrl->GetItemText(t, m_nColIndex);
				m_pListCtrl->SetItemData(t, (DWORD) new CSortItemInt(txt));
			}
		}
		break;
	case ESM_Percent:
		{
			for (int t = 0; t < nMaxItemNum; t++)
			{
				txt = m_pListCtrl->GetItemText(t, m_nColIndex);

				if( txt.GetLength() > 0 )
				{
					txt.Delete(txt.GetLength()-1, 1);
				}

				m_pListCtrl->SetItemData(t, (DWORD) new CSortItemInt(txt));
			}	
		}
		break;
	default:
		{
			ASSERT( FALSE && "δʶ�������ʽ" );
		}
		break;
	}
}
//-------------------------------------------------------------------------------------------------
//destructor
//-------------------------------------------------------------------------------------------------
CSortClass::~CSortClass()
{
	ASSERT(m_pListCtrl);
	int max = m_pListCtrl->GetItemCount();

	if( m_nSortMode == ESM_String )
	{
		CSortItem * pItem;
		for (int t = 0; t < max; t++)
		{
			pItem = (CSortItem*) m_pListCtrl->GetItemData(t);
			ASSERT(pItem);
			delete pItem;
		}
	}
	else
	{
		CSortItemInt * pItem;
		for (int t = 0; t < max; t++)
		{
			pItem = (CSortItemInt *) m_pListCtrl->GetItemData(t);
			ASSERT(pItem);
			delete pItem;
		}
	}

}
//-------------------------------------------------------------------------------------------------
//������ú���
//-------------------------------------------------------------------------------------------------
void CSortClass::Sort(const BOOL bAsc)
{
	switch( (ESortMode)m_nSortMode )
	{
	case ESM_String:
		m_pListCtrl->SortItems(CompareString, (LONG)(bAsc));
		break;
	case ESM_Value:
	case ESM_Percent:
		m_pListCtrl->SortItems(CompareValue, (LONG)(bAsc));
		break;
	default:
		ASSERT( FALSE && "δʶ�������ʽ" );
		break;
	}

	return;
}

//-------------------------------------------------------------------------------------------------
//�ַ����ȽϺ���
//-------------------------------------------------------------------------------------------------
INT CALLBACK CSortClass::CompareString(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	BOOL bAsc = (BOOL)lParamSort;

	CSortItem * i1 = (CSortItem *) lParam1;
	CSortItem * i2 = (CSortItem *) lParam2;
	ASSERT(i1 && i2);

	//��Ŀ¼�������⴦��,
	if( i1->bFile == FALSE && i2->bFile != FALSE )
	{
		if( TRUE == bAsc )
			return 1;
		else
			return -1;
	}

	if( i1->bFile != FALSE && i2->bFile == FALSE )
	{
		if( TRUE == bAsc )
			return -1;
		else
			return 1;
	}

	//��ʼ�Ƚ��ַ�����С
	if( TRUE == bAsc )
		return i1->txt.CompareNoCase(i2->txt);
	else
		return i2->txt.CompareNoCase(i1->txt);
}
//-------------------------------------------------------------------------------------------------
//��ֵ�ȽϺ���
//-------------------------------------------------------------------------------------------------
INT CALLBACK CSortClass::CompareValue(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	BOOL bAsc = (BOOL)lParamSort;

	CSortItemInt * i1 = (CSortItemInt *) lParam1;
	CSortItemInt * i2 = (CSortItemInt *) lParam2;

	ASSERT(i1 && i2);

	if (i1->lValue == i2->lValue)   //���
		return 0;

	if( TRUE == bAsc )
		return i1->lValue > i2->lValue ? 1 : -1;
	else
		return i1->lValue < i2->lValue ? 1 : -1;

}
//-------------------------------------------------------------------------------------------------
