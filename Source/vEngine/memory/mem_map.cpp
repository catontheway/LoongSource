//-----------------------------------------------------------------------------
//!\file mem_map.cpp
//!\author Lyp
//!
//!\date 2004-06-17
//! last 2008-02-15
//!
//!\brief �ڴ����
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "mem_map.h"

#ifdef _MEM_TRACE_


#undef new
#undef malloc
#undef calloc
#undef realloc
#undef free

vEngine::MemMap g_MemMap;	// ȫ�ֶ����Ա��ڳ������������
//-----------------------------------------------------------------------------
//! AllocateMemory
//-----------------------------------------------------------------------------
LPVOID AllocateMemory(size_t uiSize, LPCSTR szFile, INT nLine, BOOL bArray, LPVOID pAddress)
{
	if( pAddress )
		DeallocateMemory(pAddress, FALSE);

	return g_MemMap.Alloc(szFile, nLine, uiSize, bArray);
}

//-----------------------------------------------------------------------------
//! DeallocateMemory
//-----------------------------------------------------------------------------
VOID DeallocateMemory(LPVOID pAddress, BOOL bArray)
{
	if( 0 == pAddress )
		return;  // ANSI states that delete will allow NULL pointers.

	g_MemMap.Free(pAddress, bArray);
}

VOID WriteMem()
{
	g_MemMap.WriteFile();

}

namespace vEngine {
//-----------------------------------------------------------------------------
//! construction
//-----------------------------------------------------------------------------
MemMap::MemMap()
{
	m_dwBreakAddress = GT_INVALID;
	m_bTerminate = FALSE;
}

//-----------------------------------------------------------------------------
//! destruction
//-----------------------------------------------------------------------------
MemMap::~MemMap()
{
	m_bTerminate = TRUE;
	WriteFile();	// �������ʱд��log
}


//-----------------------------------------------------------------------------
// InsertMemNode
//-----------------------------------------------------------------------------
LPVOID MemMap::Alloc(LPCSTR szFile, INT nLine, size_t uiSize, BOOL bArray)
{
	LPVOID pAddress = NULL;
	size_t uiNewSize = uiSize + sizeof(DWORD) * 2;	// ͷβ�������־
	
	while( true )	// ���Է����ڴ�
	{
		pAddress = malloc(uiNewSize);
		if( pAddress )	// ����ɹ�
		{
			if( m_bTerminate )
				return pAddress;
			break;
		}

		// ���䲻�ɹ����ҳ���ǰ��������
		new_handler fGlobalHandler = set_new_handler(0);
		set_new_handler(fGlobalHandler);

		if( fGlobalHandler )
			(*fGlobalHandler)();	// ���ó�����
		else 
			throw std::bad_alloc();
	}

	// ����ɹ���д����β���������
	*(DWORD*)pAddress = 0xDeadBeef;
	*(DWORD*)((BYTE*)pAddress + uiSize + sizeof(DWORD)) = 0xDeadBeef;

	// ��ָ����Ƶ��ⲿ���õĵ�ַ
	pAddress = (BYTE*)pAddress + sizeof(DWORD);
	
	tagMemNode* pMemNode = (tagMemNode*)malloc(sizeof(tagMemNode));
	pMemNode->pAddress = pAddress;
	pMemNode->szFile = szFile;
	pMemNode->nLine = nLine;
	pMemNode->uiSize = uiSize;
	pMemNode->bArray = (bArray == TRUE);
	pMemNode->bArrayError = false;
	pMemNode->bBoundError = false;

	ASSERT( !m_mapMem.IsExist(pAddress) );
	m_mapMem.Add(pAddress, pMemNode);


	if( GT_VALID(m_dwBreakAddress) && (DWORD)pAddress == m_dwBreakAddress )
	{
		__asm int 3;	// �ϵ�
	}

	return pAddress;
}


//-----------------------------------------------------------------------------
// RemoveMemNode
//-----------------------------------------------------------------------------
VOID MemMap::Free(LPVOID pAddress, BOOL bArray)
{
	if( m_bTerminate )	// ��ֹ���������󱻼�������
		return;

	tagMemNode* pNode = m_mapMem.Peek(pAddress);
	if( P_VALID(pNode) )
	{
		m_mapMem.Erase(pAddress);
		pAddress = (BYTE*)pAddress - sizeof(DWORD);	// �õ�ʵ�ʵ�ַ

		BOOL bStartChanged = (*(DWORD*)pAddress != 0xDeadBeef);
		BOOL bEndChanged = (*(DWORD*)((BYTE*)pAddress + pNode->uiSize + sizeof(DWORD)) != 0xDeadBeef);

		if( bStartChanged || bEndChanged )	// ������
		{
			pNode->bBoundError = true;
			m_listBoundErrorNode.PushBack(pNode);
		}
		else if( pNode->bArray != (bArray==TRUE) ) // �����ͷż��
		{
			pNode->bArrayError = true;
			m_listArrayErrorNode.PushBack(pNode);
		}
		else	// �޴�
		{
			free(pNode);	// �ͷŽڵ�����
		}
	}

	free(pAddress);	// �ͷ�ʵ���ڴ�
}


//-----------------------------------------------------------------------------
// Refresh
//-----------------------------------------------------------------------------
VOID MemMap::WriteFile()
{
	// ���ɺ��ʵ��ļ���
	CHAR szFileName[MAX_PATH], szTime[MAX_PATH], szTemp[MAX_PATH];
	GetModuleFileNameA(NULL, szTemp, MAX_PATH);

	// ��ȥ·��
	CHAR *pResult = strrchr(szTemp, '\\');
	pResult = pResult ?	pResult+1 :	pResult = (CHAR *)szTemp;
	sprintf(szFileName, "log\\%s", pResult);
	// ��ȥ�ļ���չ��
	pResult = strrchr(szFileName, '.');
	if( pResult )
		*pResult = '\0';
	// ����ʱ��
	FILETIME CurrentTime;
	GetSystemTimeAsFileTime(&CurrentTime);
	szTime[0] = '\0';
	WORD wDate, wTime;
	if (FileTimeToLocalFileTime(&CurrentTime, &CurrentTime) &&
		FileTimeToDosDateTime(&CurrentTime, &wDate, &wTime))
	{
		// �������Լ�ʱ����ͽ���id
		sprintf(szTime, "_Mem[%d-%d-%d %02d%02d%02d %05d].log",
			(wDate / 32) & 15, wDate & 31, (wDate / 512) + 1980,
			(wTime >> 11), (wTime >> 5) & 0x3F, (wTime & 0x1F) * 2, 
			GetCurrentProcessId());
		strcat(szFileName, szTime);
	}

	FILE *fp = fopen(szFileName, "wb"); // ���ԭ�ȵ�����
	if( !fp )
		return;

	std::list<tagMemNode*> listNode;
	std::list<tagMemNode*>::iterator it;
	
	// �ڴ�й¶
	fprintf( fp, "-----------------  Memory Leaks  -----------------\r\n");
	UINT uiTotalSize = 0;
	m_mapMem.ExportAllValue(listNode);
	for(it= listNode.begin(); it!=listNode.end(); ++it)
	{
		fprintf(fp, "%s, line:%d, size:%d, address:0x%X\r\n", (*it)->szFile, (*it)->nLine, (*it)->uiSize, (*it)->pAddress);
		uiTotalSize += (*it)->uiSize;
	}
	fprintf( fp, "Total leak size:%d\r\n\r\n", uiTotalSize);


	// �ͷ����ʹ���
	listNode.clear();
	fprintf( fp, "-----------------  Free Array Error  -----------------\r\n");
	m_listArrayErrorNode.Export(listNode);
	for(it= listNode.begin(); it!=listNode.end(); ++it)
	{
		fprintf( fp, "%s, line:%d, size:%d\r\n", (*it)->szFile, (*it)->nLine, (*it)->uiSize);
	}

	// Խ�����
	listNode.clear();
	fprintf( fp, "-----------------  Bound Error  -----------------\r\n");
	m_listBoundErrorNode.Export(listNode);
	for(it= listNode.begin(); it!=listNode.end(); ++it)
	{
		fprintf( fp, "%s, line:%d, size:%d\r\n", (*it)->szFile, (*it)->nLine, (*it)->uiSize);
	}

	fclose( fp );
}


} // namespace vEngine {
#endif // #ifdef _MEM_TRACE_