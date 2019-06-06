//-----------------------------------------------------------------------------
// File: MemMgr.h
// Desc: game tool mem manager 1.0
// Time: 2003-1-8
// Auth: CTCRST
//
// Copyright (c) 2003 Softstar Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#ifndef _GT_MEM_MANAGER_
#define _GT_MEM_MANAGER_
#pragma once

#define HEAP_INIT_SIZE	4096		// ÿ��Heap����ʱ�ĳ�ʼ����

enum EMMAllocMode	// �ڴ���ʽ
{
	MMAM_Null,
	MMAM_Common,
	MMAM_Pool,
	MMAM_Heap,
	MMAM_Virtual,
	MMAM_End
};


//-----------------------------------------------------------------------------
// �ṩ�ڴ����ļ򵥽ӿ�
//-----------------------------------------------------------------------------
class CMemMgr
{
public:

	static LPVOID Alloc(DWORD dwBytes);
	static LPVOID ReSize(LPVOID pMem, DWORD dwBytes);
	static BOOL Free(LPVOID pMem);
	
	CMemMgr(BOOL bRaiseException);
	~CMemMgr();


private:
	static BOOL   m_bRaiseException;
	static HANDLE m_hMainHeap;
	static DWORD  m_dwMainHeapSize;

};












#endif	// _GT_MEM_MANAGER_