#include "StdAfx.h"
#include "gm_policy.h"
#include "world.h"
#include "role_mgr.h"

#include "../ServerDefine/gm_tool_define.h"
#include "../ServerDefine/msg_rt_s.h"
#include "../ServerDefine/base_define.h"

#include "../WorldDefine/chat.h"

GMPolicy g_GMPolicy;

//-----------------------------------------------------------------------------
// ˫������
//-----------------------------------------------------------------------------
GMDoublePolicy::GMDoublePolicy()
{
	for (INT i=0; i<NUM_EDT; ++i)
	{
		m_fCurDouble[i] = 1.0f;
		m_bDouble[i]	= FALSE;
	}
}

VOID GMDoublePolicy::SetRate( EDoubleType eDoubleType, DWORD dwRate, DWORD dwStart, DWORD dwLast )
{
	if (!EDT_VALID(eDoubleType))
		return;
	m_bDouble[eDoubleType] = TRUE;

	// ���ñ���
	m_fCurDouble[eDoubleType] = dwRate / 100.0f;

	// ��¼�ر�ʱ��
	m_dwEndTime[eDoubleType] = IncreaseTime(dwStart, dwLast);
}

VOID GMDoublePolicy::Update()
{
	for (INT i=0; i<NUM_EDT; ++i)
	{
		if (m_bDouble[i] == TRUE)
		{
			// ���Ŀǰ�����˶౶�ʣ���ô����Ƿ�ʱ
			tagDWORDTime dwNow = GetCurrentDWORDTime();

			if(dwNow >= m_dwEndTime[i])
			{
				// �౶��ʱ���ѵ�
				m_fCurDouble[i] = 1.0f;
				m_bDouble[i] = FALSE;
			}
		}
	}
}
