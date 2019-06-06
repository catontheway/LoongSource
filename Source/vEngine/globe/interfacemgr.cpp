//-----------------------------------------------------------------------------
// File: interfacemgr
// Auth: Lyp
// Date: 2003/12/11
// Last: 2004/03/15
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "interfacemgr.h"

#undef new
#undef delete
#undef malloc
#undef calloc
#undef realloc
#undef free

namespace vEngine{
//-----------------------------------------------------------------------------
// construction
//-----------------------------------------------------------------------------
ObjMgr::ObjMgr()
{
	InitializeCriticalSection(&m_cs);
}


//-----------------------------------------------------------------------------
// �������������ж���
//-----------------------------------------------------------------------------
ObjMgr::~ObjMgr()
{
	std::map<DWORD, tagObj>::iterator it;

	EnterCriticalSection(&m_cs);

	// ����ע����෴˳����������
	while(!m_listObj.empty())
	{
		std::string str = m_listObj.back();
		Kill(str.c_str());
	}


	// ����ʣ�����
	for( it=m_mapObj.begin(); it!=m_mapObj.end(); ++it )
	{
		if( !P_VALID(it->second.pObj) )
			continue;
		m_AF.Destroy(it->second.pObj, it->second.dwClassID);
		it->second.pObj = NULL;
	}

	m_mapObj.clear();

	LeaveCriticalSection(&m_cs);
	DeleteCriticalSection(&m_cs);
}


//-----------------------------------------------------------------------------
// ����szName��ö���
// ����singleton����: �������û�д������򴴽�֮, �̰߳�ȫ(��ֻ֤��һ��ʵ��)
//-----------------------------------------------------------------------------
LPVOID ObjMgr::Get(LPCSTR szName)
{
	ASSERT(szName);
	DWORD dwID = Crc32(szName);
	LPVOID pObj = NULL;
	std::map<DWORD, tagObj>::iterator it;

	EnterCriticalSection(&m_cs);

	it = m_mapObj.find(dwID);	// �Ӷ�������
	if( it == m_mapObj.end() )
	{
		LeaveCriticalSection(&m_cs);
		return NULL;
	}

	if( it->second.pObj )
		pObj = it->second.pObj;
	else
	{
		// ����û�д�����׼������
		it->second.pObj = m_AF.Create(dwID);
		pObj = it->second.pObj;
	}

	LeaveCriticalSection(&m_cs);
	return pObj;
}


LPVOID ObjMgr::GetByTypeName(LPCSTR szName)
{
	DWORD dwTypeID = Crc32(szName);
	std::map<DWORD, DWORD>::iterator it0;

	it0 = m_mapTypeObj.find(dwTypeID);	// �Ӷ�������
	if( it0 == m_mapTypeObj.end() )
		return  NULL;

	LPVOID pObj = NULL;
	DWORD dwID = it0->second;
	std::map<DWORD, tagObj>::iterator it;

	EnterCriticalSection(&m_cs);

	it = m_mapObj.find(dwID);	// �Ӷ�������
	if( it == m_mapObj.end() )
	{
		LeaveCriticalSection(&m_cs);
		return NULL;
	}

	if( it->second.pObj )
		pObj = it->second.pObj;
	else
	{
		// ����û�д�����׼������
		it->second.pObj = m_AF.Create(dwID);
		pObj = it->second.pObj;
	}

	LeaveCriticalSection(&m_cs);
	return pObj;


}




//-----------------------------------------------------------------------------
// ����ע����Ķ��󴴽���һ������
//-----------------------------------------------------------------------------
BOOL ObjMgr::Create(LPCSTR szDestObjName, LPCSTR szSrcObjName)
{
	ASSERT(szSrcObjName);
	ASSERT(szDestObjName);
	ASSERT(CheckName(szDestObjName));	// ��Ҫ����Ƿ����ظ�����

	EnterCriticalSection(&m_cs);

	tagObj objDesc;
	DWORD dwClassID = Crc32(szSrcObjName);
	objDesc.pObj = m_AF.Create(dwClassID);	// ��������
	if( !P_VALID(objDesc.pObj) )
	{
		LeaveCriticalSection(&m_cs);
		return FALSE;
	}
	objDesc.dwClassID = dwClassID;
	objDesc.strObjName.assign(szDestObjName);
	m_mapObj.insert( std::pair<DWORD, tagObj>(Crc32(szDestObjName), objDesc) );
	m_listObj.push_back(szDestObjName);

	LeaveCriticalSection(&m_cs);
	return TRUE;
}


//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
BOOL ObjMgr::Kill(LPCSTR szObjName)
{
	ASSERT(szObjName);
	DWORD dwID = Crc32(szObjName);
	std::map<DWORD, tagObj>::iterator it;
	std::list<std::string>::iterator itList;

	EnterCriticalSection(&m_cs);
	it = m_mapObj.find(dwID);	// �Ӷ�������
	if( it == m_mapObj.end() )
	{
		ASSERT(0);
		LeaveCriticalSection(&m_cs);
		return FALSE;	// �����Ҳ���
	}

	LPVOID pObj = it->second.pObj;
	DWORD dwClassID = it->second.dwClassID;
	
	// �ȴ�����б���ɾ��������Destory�������ִ�m_mapObj���ҵ��Լ�
	m_mapObj.erase( it );
	// ͬʱ��lis��ɾ��
	for(itList = m_listObj.begin(); itList!=m_listObj.end(); itList++)
	{
		if((*itList) == szObjName)
		{
			m_listObj.erase(itList);
			break;
		}
	}
	
	if( pObj )
		m_AF.Destroy(pObj, dwClassID);
		
	LeaveCriticalSection(&m_cs);
	return TRUE;
}


//-----------------------------------------------------------------------------
// ��������ֵĺϷ���
//-----------------------------------------------------------------------------
BOOL ObjMgr::CheckName(LPCSTR szName)
{
	ASSERT( strlen(szName) < 32 ); // name too long
	DWORD dwID = Crc32(szName);
	std::map<DWORD, tagObj>::iterator it;
	
	EnterCriticalSection(&m_cs);
	it = m_mapObj.find(dwID);
	ASSERT( it == m_mapObj.end() );	// repeated ID
	LeaveCriticalSection(&m_cs);
	
	return TRUE;
}


//-----------------------------------------------------------------------------
// ��Ϊ���Ǳ�����Interface�࣬����Ϊ�˲����CUtil�������Լ�ʵ����һ��CRC32
// ����32λCRC,��һ���㷨���ٺܶ�,
//-----------------------------------------------------------------------------
DWORD ObjMgr::Crc32(LPCSTR lpData)
{
	UINT unResult;

	//��ASC����127���ַ��� ������unsigned char, ��������
	LPBYTE pData = (LPBYTE)lpData;
	if( 0 == pData[0] ) 
		return (DWORD)GT_INVALID;

	unResult = *pData++ << 24;
	if( *pData )
	{
		unResult |= *pData++ << 16;
		if( *pData )
		{
			unResult |= *pData++ << 8;
			if( *pData ) unResult |= *pData++;
		}
	}
	unResult = ~ unResult;

	while( *pData )
	{
		unResult = (unResult << 8 | *pData) ^ globe_Crc32Table[unResult >> 24];
		pData++;
	}

	return ~unResult;
}

}