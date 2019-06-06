//-----------------------------------------------------------------------------
//!\file mem_map.h
//!\author Lyp
//!
//!\date 2004-06-17
//! last 2009-03-05
//!
//!\brief �ڴ����
//-----------------------------------------------------------------------------
#pragma once

#ifdef _MEM_TRACE_

#undef new
#undef malloc
#undef calloc
#undef realloc
#undef free

namespace vEngine {
//-----------------------------------------------------------------------------
//!\brief �ڴ����
//! ��Ҫ�����ڴ�й¶���ö�ջʱ����������Alloc��������һ���ϵ㣬����������
//!	������������󣬽�m_dwBreakAddress�޸�Ϊ�ڴ�й¶��ַ��Ȼ��ȡ���ϵ㣬����
//! ���г��򡣵������趨��ַ���ڴ����ʱ��Alloc���Զ�������
//-----------------------------------------------------------------------------
class VENGINE_API MemMap
{
public:
	LPVOID Alloc(LPCSTR szFile, INT nLine, size_t uiSize, BOOL bArray);
	VOID Free(LPVOID pAddress, BOOL bArray);
	VOID WriteFile();

	MemMap();
	~MemMap();

private:
	struct tagMemNode
	{
		LPVOID	pAddress;		// �����ַ
		LPCSTR	szFile;			// �ļ���
		INT		nLine;			// �к�
		size_t	uiSize;			// �����С
		bool	bArray;			// �Ƿ���������
		bool	bArrayError;	// �����ͷŴ���
		bool	bBoundError;	// Խ�����
		bool	bReserved;		// Ϊ�˶��������
	};

	VOLATILE BOOL					m_bTerminate;
	TSafeMap<LPVOID, tagMemNode*>	m_mapMem;
	TSafeList<tagMemNode*>			m_listArrayErrorNode;
	TSafeList<tagMemNode*>			m_listBoundErrorNode;

	DWORD m_dwBreakAddress;
};

} // namespace vEngine {
VOID VENGINE_API WriteMem();

LPVOID VENGINE_API AllocateMemory(size_t uiSize, LPCSTR szFile, INT nLine, BOOL bArray, LPVOID pAddress=NULL); 
VOID VENGINE_API DeallocateMemory(LPVOID pAddress, BOOL bArray);

inline LPVOID operator new(size_t uiSize, LPCSTR szFile, INT nLine) { return AllocateMemory(uiSize, szFile, nLine, FALSE); }
inline LPVOID operator new[](size_t uiSize, LPCSTR szFile, INT nLine) { return AllocateMemory(uiSize, szFile, nLine, TRUE); }
inline VOID operator delete(LPVOID pAddress) { DeallocateMemory(pAddress, FALSE); }
inline VOID operator delete[](LPVOID pAddress) { DeallocateMemory(pAddress, TRUE); }

// These two routines should never get called, unless an error occures during the 
// allocation process.  These need to be defined to make Visual C++ happy.
inline VOID operator delete(LPVOID pAddress, LPCSTR szFile, INT nLine) { szFile;nLine;DeallocateMemory(pAddress, FALSE); }
inline VOID operator delete[](LPVOID pAddress, LPCSTR szFile, INT nLine) { szFile;nLine;DeallocateMemory(pAddress, TRUE); }

#define new							new(__FILE__, __LINE__)
#define malloc(uiSize)				AllocateMemory(uiSize, __FILE__, __LINE__,  FALSE)
#define calloc(uiNum, uiSize)		AllocateMemory(((uiSize)*(uiNum)), __FILE__, __LINE__, FALSE)
#define realloc(pAddress, uiSize)	AllocateMemory(uiSize, __FILE__, __LINE__, FALSE, pAddress)
#define free(pAddress)				DeallocateMemory(pAddress, FALSE)


#endif // #ifdef _MEM_TRACE_