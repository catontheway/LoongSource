//-----------------------------------------------------------------------------
// File: MemMgr
// Desc: game tool tread safe mem pool manager 1.0
// Auth: Lyp
// Date: 2003-1-8	
// Last: 2003-12-11
//
// Copyright (c) 2003 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

CONST INT GT_MAX_TS_CACHE_PER_TYPE = 32;

enum ETSMM_CACHE_TYPE	// ���ö��
{
	TSMMCT_1k=0,
	TSMMCT_64k,
	TSMMCT_256k,
	TSMMCT_1024k,
	TSMMCT_4096k,
	TSMMCT_8192k,
	TSMMCT_End,
};

//-----------------------------------------------------------------------------
// �ṩ�̰߳�ȫ���ڴ�ع���ӿ�
//-----------------------------------------------------------------------------
class CTSMemMgr
{
public:

	VOID* Alloc(DWORD dwBytes);
	VOID  Free(VOID* pMem);

	CTSMemMgr();~CTSMemMgr();


private:

	// ÿ�黺�������
	struct tagCache
	{
		PVOID pMem;
		DWORD dwSize;		// �����С
		DWORD dwUsage;		// ʹ���˶���
		DWORD dwMaxUsage;	// ���ʹ�ÿռ�
		DWORD dwUseTime;	// �ܹ�ʹ�ô���
		
		BOOL bBeUsed;		// ���ڱ�ʹ��,ע��������Ϊ��д��BOOL
		BOOL bAllocated;	// �ڴ�ָ����Ч��
		BOOL bEverBeenUsed;	// ����ʹ�ù�
	};

	tagCache			m_Pool[TSMMCT_End][GT_MAX_TS_CACHE_PER_TYPE];
	CRITICAL_SECTION	m_PoolLock;

};
