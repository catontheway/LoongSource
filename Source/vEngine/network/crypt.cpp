//-----------------------------------------------------------------------------
// File: crypt.cpp
// Desc: �ṩ�̰߳�ȫ�ļ��ܽ���
// Auth: Lyp
// Date: 2005-11-17
// Last: 2005-11-17
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "crypt.h"

namespace vEngine{
#define SECT_KIND	256
#define SECT_LEN	GT_MAX_PACKAGE_LEN
//-----------------------------------------------------------------------------
// consturction / destruction
//-----------------------------------------------------------------------------
SectCrypt::SectCrypt():m_Trunk(this)
{
	m_pSect = new BYTE[SECT_KIND*SECT_LEN];
	CreateSect();
}

SectCrypt::~SectCrypt()
{
	SAFE_DEL_ARRAY(m_pSect);
}



//-----------------------------------------------------------------------------
// encrypt
//-----------------------------------------------------------------------------
UINT SectCrypt::Encrypt(tagUnitData* pUnit)
{
	INT nSect = pUnit->byEncryptNum % SECT_KIND;
	PBYTE p1 = (PBYTE)&pUnit->wbuf.buf[0];
	PBYTE p2 = &m_pSect[nSect*SECT_LEN];
	DWORD dwHeader = 0;

	if( pUnit->bFirstUnitOfPackage )	// ��ͷ�����⴦��
	{
		pUnit->wbuf.buf[sizeof(DWORD)-1] = pUnit->byEncryptNum;	// ��һ��DWORD�����λ
		dwHeader = *(DWORD*)&pUnit->wbuf.buf[0];
	}

	INT nLen = pUnit->wbuf.len / 8;
	if( nLen > 0 )
	{
		_asm
		{      
			pxor	mm0, mm0 
			mov		ecx, nLen
			mov		edi, dword ptr [p1] 
			mov		esi, dword ptr [p2] 
			ALIGN	4
_next:        
			movq	mm0, dword ptr [edi] 
			movq	mm1, dword ptr [esi]
			add		esi, 8
			pxor	mm0, mm1
			movq	dword ptr [edi], mm0
			add		edi, 8
			loop	_next
			emms
		}
	}

	if( pUnit->bFirstUnitOfPackage )	// ��ͷ�����⴦��
		*(DWORD*)&pUnit->wbuf.buf[0] = dwHeader;

	return 0;
}





//-----------------------------------------------------------------------------
// decrypt
//-----------------------------------------------------------------------------
UINT SectCrypt::Decrypt(tagUnitData* pUnit)
{
	INT nSect = pUnit->byEncryptNum % SECT_KIND;
	PBYTE p1 = (PBYTE)&pUnit->wbuf.buf[0];
	PBYTE p2 = &m_pSect[nSect*SECT_LEN];
	INT nLen = 0;

	if( pUnit->bFirstUnitOfPackage )	// ��ͷ�����⴦��
		nLen = (pUnit->wbuf.len + sizeof(DWORD)) / 8;
	else
		nLen = pUnit->wbuf.len / 8;
	
	if( nLen > 0 )
	{
		_asm
		{      
			pxor	mm0, mm0 
			mov		ecx, nLen
			mov		edi, dword ptr [p1] 
			mov		esi, dword ptr [p2] 
			ALIGN	4
_next:        
			movq	mm0, dword ptr [edi] 
			movq	mm1, dword ptr [esi]
			add		esi, 8
			pxor	mm0, mm1
			movq	dword ptr [edi], mm0
			add		edi, 8

			loop	_next
			emms
		}
	}

	return 0;

}



//-----------------------------------------------------------------------------
// �������Դ
//-----------------------------------------------------------------------------
VOID SectCrypt::CreateSect()
{
	INT nHolder = 0xDeadBeef;
	for(INT n=0; n<SECT_KIND*SECT_LEN; n++)
	{
		INT r = ((nHolder = nHolder * 214013L + 2531011L) >> 16) & 0x7fff;
		m_pSect[n] = (BYTE)r;
	}
}


}