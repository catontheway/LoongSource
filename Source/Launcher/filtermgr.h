//-----------------------------------------------------------------------------
// File: FilterMgr.h
// Desc: game tool filter manager 1.0
// Time: 2003-1-8
// Auth: CTCRST
//
// Copyright (c) 2003 Softstar Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#ifndef _GT_FILTER_MANAGER_
#define _GT_FILTER_MANAGER_
#pragma once


enum EFMFilter	// ����������
{
	FMF_Null,
	FMF_Changeless,
	FMF_MiniLzo1040,
	FMF_ZLib,
	FMF_End
};



struct tagFilterParam	// ��������������
{
	EFMFilter	eFilter;
	BOOL		bCompress;	// �����������
	LPVOID		lpMemIn;
	LPVOID		lpMemOut;
	DWORD		dwInBufferSize;
	DWORD		dwOutBufferSize;
	DWORD		dwInDataSize;
	DWORD		dwOutDataSize;	// һ���ɹ�������д
	BOOL		bSuccess;		// һ���ɹ�������д
};



//-----------------------------------------------------------------------------
// �򵥵Ĺ���������
//-----------------------------------------------------------------------------
class CFilterMgr
{
public:

	static BOOL InitFilter(EFMFilter eFilter);	// ��ʼ��������
	
	static INT GetFilterWorkSpace();	// �õ�����������Ҫ�Ĺ�������
	static INT GetFilterExtraSpace(INT nSrcSize);	

	// �����ֹ������㷨��Ҫ�Ķ�������ռ䣬����ָWork Space
	static BOOL CheckBufferSize(DWORD dwInSize, DWORD dwOutSize, EFMFilter eFilter);
	static VOID Filter(tagFilterParam* pParam);

	CFilterMgr();
	~CFilterMgr();

private:
	static BOOL m_bInit[FMF_End];
};


#endif	// _GT_FILTER_MANAGER_