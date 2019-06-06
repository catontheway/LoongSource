//-----------------------------------------------------------------------------
//!\file session.cpp
//!\author Lyp
//!
//!\date 2004-07-07
//! last 2008-04-17
//!
//!\brief �������˻Ự��
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "session.h"

// ʹ����������
#include "transport.h"
#include "..\console\console_gui.h"

namespace vEngine {
//-----------------------------------------------------------------------------
//! construction
//-----------------------------------------------------------------------------
CompletionSession::CompletionSession():m_Trunk(this)
{
	// ��½�ص�����
	m_fnLogIn = NULL;
	m_fnLogOut = NULL;

	m_pTransport = new CompletionTransport;
	InitializeCriticalSectionAndSpinCount(&m_LockLog, DEFAULT_SPIN_COUNT);

	m_dwIDHold = 0;

	// unitͳ��
	m_dwGetFreeUnitCounter = 0;
	m_dwReturnUnitCounter = 0;
	m_dwSendUnitCounter = 0;
	m_dwRecvUnitCounter = 0;
}


//-----------------------------------------------------------------------------
//! destruction
//-----------------------------------------------------------------------------
CompletionSession::~CompletionSession()
{
	DeleteCriticalSection(&m_LockLog);
	SAFE_DEL(m_pTransport);
}


//-----------------------------------------------------------------------------
// ��ʼ����ɶ˿�����Ự��
//-----------------------------------------------------------------------------
BOOL CompletionSession::Init(tagServerInitParam* pInitParam)
{
	ASSERT(m_fnLogIn);	// �ⲿӦ���ȵ���SetLogCallBack����ע������ǳ��ص�����

	InterlockedExchange((LONG*)&m_bDestroy, FALSE);
	m_mapClient.Clear();

	// �Ǽǻص�����
	m_pTransport->SetLogCallback((LOGINCALLBACK)m_Trunk.sfp2(&vEngine::CompletionSession::LogInCallback),
		(LOGOUTCALLBACK)m_Trunk.sfp1(&vEngine::CompletionSession::LogOutCallback));

	// ��ʼ������ײ�
	BOOL bResult = m_pTransport->Init(pInitParam);
	if( FALSE == bResult)
		return FALSE;

	// ע�Ἰ�������õĿ���̨����
	TObjRef<Console>()->Register(_T("cn"), m_Trunk.sfp0(&vEngine::CompletionSession::GetClientNum), 
		_T("get number of client"), 0 );

	return TRUE;
}


//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
BOOL CompletionSession::Destroy()
{
	InterlockedExchange((LONG*)&m_bDestroy, TRUE);

	std::list<DWORD> list;
	m_mapClient.ExportAllKey(list);
	std::list<DWORD>::iterator it;
	for(it=list.begin(); it!=list.end(); ++it)
		this->Kick((*it));

	while( m_mapClient.Size() > 0 )	// �ȴ��������ӶϿ�
		Sleep(10);

	if( m_pTransport )
		m_pTransport->Destroy();

	m_fnLogIn = NULL;
	m_fnLogOut = NULL;

	return TRUE;
}


//-----------------------------------------------------------------------------
// SetLogCallBack
//-----------------------------------------------------------------------------
VOID CompletionSession::SetLogCallBack(LOGINCALLBACK fnLogIn, 
											LOGOUTCALLBACK fnLogOut)
{
	// ��½�ص�����
	m_fnLogIn = fnLogIn;
	m_fnLogOut = fnLogOut;
}



//-----------------------------------------------------------------------------
// ����Ϣ����ָ���ͻ���,���Է��ʹ���500�ֽڵ���Ϣ
//-----------------------------------------------------------------------------
BOOL CompletionSession::Send(DWORD dwID, LPVOID pMsg, DWORD dwSize, INT EncryptNum)
{
    ASSERT(dwSize < 1024*16);   // ����16K�ͱ���

	INT nCDIndex = m_mapClient.Peek(dwID);
	if( GT_INVALID == nCDIndex )
		return 0;

    INT nPackage = (dwSize + sizeof(DWORD)) / GT_MAX_PACKAGE_LEN;
    if( (DWORD)nPackage * GT_MAX_PACKAGE_LEN < dwSize + sizeof(DWORD) )
        nPackage += 1;
 
    LPBYTE pSrc = (LPBYTE)pMsg;
    DWORD dwUnsendSize = dwSize + sizeof(DWORD);
    tagUnitData* pUnit = NULL;
	BYTE byRand = (BYTE)EncryptNum;
 
    for(INT n=0; n<nPackage; n++)
    {
        pUnit = this->GetFreeUnit();
        pUnit->nCDIndex = nCDIndex;
		pUnit->byEncryptNum = byRand;
 
        // ����unit��С
        if( dwUnsendSize > GT_MAX_PACKAGE_LEN )
            pUnit->wbuf.len = GT_MAX_PACKAGE_LEN;
        else
            pUnit->wbuf.len = dwUnsendSize;
 
        if( 0 == n ) // ��һ������Ϊ�а�������Ϣ,������Ҫ���⴦��
        {
			pUnit->bFirstUnitOfPackage = true;
            *(DWORD*)&pUnit->wbuf.buf[0] = dwSize;  // ������
            LPBYTE pDest = ((LPBYTE)pUnit->wbuf.buf) + sizeof(DWORD);
            memcpy(pDest, pSrc, pUnit->wbuf.len-sizeof(DWORD));
            pSrc += pUnit->wbuf.len-sizeof(DWORD);
            
        }
        else
        {
			pUnit->bFirstUnitOfPackage = false;
            memcpy(pUnit->wbuf.buf, pSrc, pUnit->wbuf.len);
            pSrc += pUnit->wbuf.len;
        }
 
        dwUnsendSize -= pUnit->wbuf.len;
        this->Send(pUnit);
	}

	return TRUE;
}






//-----------------------------------------------------------------------------
// �߳�ָ��ID�����
//-----------------------------------------------------------------------------
BOOL CompletionSession::Kick(DWORD dwClientID)
{
	// ��Ҫ��ClientIDת����SerialID
	INT nCDIndex = m_mapClient.Peek(dwClientID);
	if( GT_INVALID != nCDIndex )
	{
		DWORD dwSerialID = m_pTransport->GetSerialID(nCDIndex) ;
		m_pTransport->Kick(dwSerialID);

		// ����һ��0�ֽ���Ϣ
		char mess[16] = {0};
		this->Send(dwClientID, mess, 0);
		return TRUE;
	}

	return FALSE;
}



//-----------------------------------------------------------------------------
// �õ�ָ��ID�ͻ��˵�sock name
//-----------------------------------------------------------------------------
BOOL CompletionSession::GetClientSockName(sockaddr_in& addr_in, DWORD dwID)
{
	INT nCDIndex = m_mapClient.Peek(dwID);
	if( FALSE == GT_VALID(nCDIndex) )
		return FALSE;

	return GetUnauthClientSockName(addr_in, nCDIndex);
}


//-----------------------------------------------------------------------------
// �õ�δ��֤�ͻ��˵�sock name
//-----------------------------------------------------------------------------
BOOL CompletionSession::GetUnauthClientSockName(sockaddr_in& addr_in, INT nCDIndex)
{
	DWORD dwSerialID = m_pTransport->GetSerialID(nCDIndex);
	if( FALSE == m_pTransport->GetClientSockName(addr_in, dwSerialID) )
		return FALSE;

	return TRUE;
}


//-----------------------------------------------------------------------------
// login callback function
//-----------------------------------------------------------------------------
UINT CompletionSession::LogInCallback(tagUnitData* pFirstUnit, tagLoginParam* param)
{
	DWORD dwID = GT_INVALID;
	if( m_bDestroy )
		return GT_INVALID;

	EnterCriticalSection(&m_LockLog);

	// �õ�һ�������������֤
	dwID = (*m_fnLogIn)(pFirstUnit, param);	// ���õ�½�ص�����
	if( dwID != GT_INVALID )
	{
		// ע��˴�д�����ⲿ�������send(ID,...)����ʱ��Ҫ�ȴ�
		// ��һ�����յ����������У���Ϊ�ڵ�һ�����յ�֮ǰ����m_mapClient
		// ��δ׼����
		m_mapClient.Add(dwID, pFirstUnit->nCDIndex);
	}

	LeaveCriticalSection(&m_LockLog);

	return dwID;
}

//-----------------------------------------------------------------------------
// logout callback function
//-----------------------------------------------------------------------------
UINT CompletionSession::LogOutCallback(DWORD dwID)
{
	EnterCriticalSection(&m_LockLog);

	if( m_mapClient.IsExist(dwID) )
	{
		if( m_fnLogOut )	// �����ȵ���ע���ص�����
			(*m_fnLogOut)(dwID);

		m_mapClient.Erase(dwID);
	}

	LeaveCriticalSection(&m_LockLog);
	return 0;
}




} // namespace wEngine {