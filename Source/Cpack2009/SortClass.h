//-------------------------------------------------------------------------------------------------
// Copyright (c) 2006 TENGWU Entertainment All rights reserved.
// filename: main.h
// author: ��������
// actor:
// data: 2006-08-10
// last:
// brief: clistview�ؼ�item������
//-------------------------------------------------------------------------------------------------
#pragma once

//-------------------------------------------------------------------------------------------------
//����ʽ
//-------------------------------------------------------------------------------------------------
enum ESortMode
{
	ESM_Start = 0,

	ESM_String,	 //�ַ�������
	ESM_Value,	 //��ֵ
	ESM_Percent, //�ٷ���

	ESM_End = 20,
};

//-------------------------------------------------------------------------------------------------
class CSortClass
{
public:
	CSortClass(CListCtrl * pListCtrl, const int _iCol, const INT nSortMode);
	virtual ~CSortClass();

	void Sort(const BOOL bAsc);

	static INT CALLBACK CompareString(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static INT CALLBACK CompareValue(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

private:
	INT			m_nColIndex;    //�е����к�
	CListCtrl	*m_pListCtrl;	//����ؼ�
	INT			m_nSortMode;	//����ģʽ

public:
	class CSortItem
	{
	public:
		virtual  ~CSortItem();
		CSortItem(const CString &_txt, BOOL bFile = TRUE);
		CString txt;
		BOOL  bFile;
	};
	class CSortItemInt
	{
	public:
		CSortItemInt(const CString &_txt);
		long lValue ;
	};
};

