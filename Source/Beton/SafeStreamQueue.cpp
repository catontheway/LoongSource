#include "StdAfx.h"
#include "Config.h"
#include "MysqlStream.h"
#include "SafeStreamQueue.h"

namespace Beton
{
//--------------------------------------------------------------------------------------------------------
// ��������
//--------------------------------------------------------------------------------------------------------
SafeStreamQueue::~SafeStreamQueue()
{
	// ASSERT( 0 == m_lNum );
	if( m_hEvent )
		::CloseHandle(m_hEvent);
}

} // namespace Beton