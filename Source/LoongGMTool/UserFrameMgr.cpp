#include "stdafx.h"
#include "UserFrameMgr.h"

UserFrameMgr::UserFrameMgr()
{
	m_mapHandle.clear();
}

UserFrameMgr::~UserFrameMgr()
{
	Destroy();
}


//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL UserFrameMgr::Init()
{
	return TRUE;
}

//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
BOOL UserFrameMgr::Destroy()
{
	std::map<DWORD, tagEventHandle*>::iterator it=m_mapHandle.begin();
	for( ; it!=m_mapHandle.end(); )
	{
		SAFE_DEL(it->second);
		it = m_mapHandle.erase(it);
	}
	return TRUE;
}

//-----------------------------------------------------------------------------
// ע��Frame�¼�������
//-----------------------------------------------------------------------------
BOOL UserFrameMgr::RegisterEventHandle(LPCTSTR szEvent, FRAMEEVENTPROC fp)
{
	DWORD dwID = Crc32(szEvent);
	std::map<DWORD, tagEventHandle*>::iterator it = m_mapHandle.find(dwID);
	if( it!= m_mapHandle.end() )
	{
		return FALSE;
	}

	tagEventHandle* pHandle = new tagEventHandle;
	pHandle->strName = szEvent;
	pHandle->fp = fp;

	m_mapHandle.insert(std::make_pair(dwID, pHandle));
	return TRUE;
}


//-----------------------------------------------------------------------------
// ע���¼�������
//-----------------------------------------------------------------------------
BOOL UserFrameMgr::UnRegisterEventHandler(LPCTSTR szEvent, FRAMEEVENTPROC fp)
{
	DWORD dwID = Crc32(szEvent);
	std::map<DWORD, tagEventHandle*>::iterator it = m_mapHandle.find(dwID);
	if( it==m_mapHandle.end() )
		return FALSE;

	m_mapHandle.erase(it);
	return TRUE;
}


//-----------------------------------------------------------------------------
// �����¼�������
//-----------------------------------------------------------------------------
VOID UserFrameMgr::SendEvent( tagUserEvent* pEvent )
{
	std::map<DWORD, tagEventHandle*>::iterator it = m_mapHandle.find(pEvent->dwNameCrc);
	if( it==m_mapHandle.end() )
		return;

	it->second->fp(pEvent);
}