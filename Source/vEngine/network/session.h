//-----------------------------------------------------------------------------
//!\file session.h
//!\author Lyp
//!
//!\date 2004-07-07
//! last 2008-04-17
//!
//!\brief �������˻Ự��
//-----------------------------------------------------------------------------
#pragma once
#include "transport.h"

namespace vEngine {
//-----------------------------------------------------------------------------
//!\brief �������˻Ự��, ֧��3000��������(ͨ������)
//-----------------------------------------------------------------------------
class VENGINE_API CompletionSession
{
public:
	BOOL Init(tagServerInitParam* pInitParam);
	BOOL Destroy();

	//-----------------------------------------------------------------------------
	// �����������
	//-----------------------------------------------------------------------------
	//! ���õ�½�ص�����,�ⲿ��Ҫ��֤�����ص������̰߳�ȫ
	//! ע�⵱�û���¼ʱ,�Ὣ��һ��unit��Ϊ��������fnLogIn,fnLogIn���뷵��һ��ID
	//! ���û��ǳ�ʱ,�Ὣ�ϲ����ID��Ϊ��������fnLogOut
	VOID SetLogCallBack(LOGINCALLBACK fnLogIn, LOGOUTCALLBACK fnLogOut);

	//--------------------------------------------------
	// �Ż��շ�
	//--------------------------------------------------
	// ��ָ����Ϣ�����͵����(unit.nCDIndex����ָ��)
	// ��send֮ǰ����getfreeunit�õ����õ�unit
	// ���ش��û����ж��ٰ�δ����ȥ
	__forceinline INT Send(tagUnitData* pUnit);
	// ��������unit,�������Ӧ�õ���ReturnUnit����unit
	// ע��õ���unit.wbuf.bufǰ�ĸ��ֽڲ�����������(�������ʵ�ʳ���),������Ϣ�ӵ�5�ֽڿ�ʼ
	__forceinline tagUnitData* RecvUnitEx(DWORD& dwRecvedNum, INT nCDIndex);
	// �ⲿ�õ����е�unit
	tagUnitData* GetFreeUnit() { ++m_dwGetFreeUnitCounter; return m_pTransport->GetFreeUnit(); }
	// �ⲿ���������unit
	VOID ReturnUnit(tagUnitData* pUnit) { ++m_dwReturnUnitCounter; m_pTransport->ReturnUnit(pUnit); }


	//--------------------------------------------------
	// ��ͨ�շ�,Ϊ�˱�֤Ч��,Ӧ�����������һ���շ�����
	//--------------------------------------------------
	// ������ͨ��ʽ����,�ڲ���һ��memcpy�Լ���Ӧ��GetFreeUnit
	BOOL Send(DWORD dwID, LPVOID pMsg, DWORD dwSize, INT EncryptNum=0);
	// ��������unit,�������Ӧ�õ���ReturnUnit����unit
	// ע��õ���unit.wbuf.bufǰ�ĸ��ֽڲ�����������(�������ʵ�ʳ���),������Ϣ�ӵ�5�ֽڿ�ʼ
	__forceinline tagUnitData* RecvUnit(DWORD dwID);



	// �߳�ָ�����,����FALSE�����Ҳ���ָ�����
	BOOL Kick(DWORD dwClientID);

	//-----------------------------------------------------------------------------
	// ��Ϣͳ���Լ����ߺ���
	//-----------------------------------------------------------------------------
	// �õ�����wsasend����û���յ����֪ͨ�İ���Ŀ
	__forceinline INT GetSendCast(INT nCDIndex) { return m_pTransport->GetSendCast(nCDIndex); }

	// �õ���δ������ɵ�unit��Ŀ
	INT	GetUnsendUnitNum() { return m_pTransport->GetUnsendUnitNum(); } 

	// �õ������ϵĿͻ�����Ŀ
	INT	GetClientNum() { return m_mapClient.Size(); }  
	// ���ָ��ID�Ŀͻ��ǲ��Ǵ���
	BOOL IsClientExsit(DWORD dwID) { return m_mapClient.IsExist(dwID); }

	// �õ��û�ID��
	VOID GetClientList(std::list<DWORD>& list) { m_mapClient.ExportAllKey(list); }

	// �õ�ָ��ID�ͻ��˵�sock name
	BOOL GetClientSockName(sockaddr_in& addr_in, DWORD dwID);
	// �õ�δ��֤�ͻ��˵�sock name
	BOOL GetUnauthClientSockName(sockaddr_in& addr_in, INT nCDIndex);
	
	// �õ��������˿ں�
	INT GetPort() { return m_pTransport->GetPort(); }
	// �õ��ڲ�unit����Ϣ
	TCHAR* GetInfo() { return m_pTransport->GetInfo(); }

	// �õ�����ͳ������
	DWORD GetCounterOfGetFreeUnit()	{ return m_dwGetFreeUnitCounter; }
	DWORD GetCounterOfReturnUnit()	{ return m_dwReturnUnitCounter; }
	DWORD GetCounterOfSendUnit()	{ return m_dwSendUnitCounter; }
	DWORD GetCounterOfRecvUnit()	{ return m_dwRecvUnitCounter; }

	CompletionSession();
	~CompletionSession();

private:
	TSFPTrunk<CompletionSession>	m_Trunk;
	CompletionTransport*			m_pTransport;
	CRITICAL_SECTION				m_LockLog;
	LOGINCALLBACK					m_fnLogIn;	// ���ֻص�����
	LOGOUTCALLBACK					m_fnLogOut;
	
	VOLATILE BOOL					m_bDestroy;	// �Ƿ���Ҫ�ر�
	TSafeMap<DWORD, INT>			m_mapClient;// ��¼�û�nCDIndex��ID���
	DWORD							m_dwIDHold;	// ID����

	// unitͳ��
	DWORD							m_dwGetFreeUnitCounter;
	DWORD							m_dwReturnUnitCounter;
	DWORD							m_dwSendUnitCounter;
	DWORD							m_dwRecvUnitCounter;


	UINT	LogInCallback(tagUnitData* pFirstUnit, tagLoginParam* param);
	UINT	LogOutCallback(DWORD dwID);
};


//-----------------------------------------------------------------------------
// ��ָ����Ϣ�����͵�ָ��ID�����,
// ��send֮ǰ����getfreeunit�õ����õ�unit
// ���ش��û����ж��ٰ�δ����ȥ
//-----------------------------------------------------------------------------
INT CompletionSession::Send(tagUnitData* pUnit)
{
	ASSERT( pUnit );

	// ����һ����������Ϣ���зֳ�n������������������϶���assert
	//ASSERT( pUnit->wbuf.len == *(DWORD*)(&pUnit->wbuf.buf[0]) + sizeof(DWORD));
	++m_dwSendUnitCounter;
	return m_pTransport->SendUnit(pUnit);
}


//-----------------------------------------------------------------------------
// ��������unit,�������Ӧ�õ���returnunit����unit
//-----------------------------------------------------------------------------
tagUnitData* CompletionSession::RecvUnit(DWORD dwID)
{	
	DWORD dwTemp; 
	INT nCDIndex = m_mapClient.Peek(dwID);
	if( GT_INVALID == nCDIndex )
		return NULL;

	tagUnitData* pUnit = m_pTransport->RecvUnit(dwTemp, nCDIndex);
	if( pUnit )
		++m_dwRecvUnitCounter;

	return pUnit; 
}


//-----------------------------------------------------------------------------
// ��ȡ������Ϣ
//-----------------------------------------------------------------------------
tagUnitData* CompletionSession::RecvUnitEx(DWORD& dwRecvedNum, INT nCDIndex)
{ 
	tagUnitData* pUnit = m_pTransport->RecvUnit(dwRecvedNum, nCDIndex); 
	if( pUnit )
		++m_dwRecvUnitCounter;

	return pUnit;
}



} // namespace wEngine {