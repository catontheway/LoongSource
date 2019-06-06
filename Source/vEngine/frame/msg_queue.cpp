//-----------------------------------------------------------------------------
// File: msg_queue
// Desc: �ṩ�򵥵��Ƚ��ȳ���Ϣ���й���
// Auth: Lyp
// Date: 2003-11-30
// Last: 2008-06-12
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "msg_queue.h"

#include "..\memory\mem_pool.h"
namespace vEngine {
//-----------------------------------------------------------------------------
// MsgQueue construction / destruction
//-----------------------------------------------------------------------------
MsgQueue::MsgQueue()
{
	m_nMsg = 0;
	m_pQueueBegin = NULL;
	m_pQueueEnd = NULL;
	m_pCurrentItem = NULL;

	m_pMemPool = new MemPool(16*1024);	// Ĭ�� 16KB ����
}

MsgQueue::~MsgQueue()
{
	Clear();
	SAFE_DEL(m_pMemPool);
}

}