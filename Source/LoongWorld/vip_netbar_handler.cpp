#include "stdafx.h"
#include "player_session.h"

#include "../WorldDefine/msg_vip_netbar.h"

#include "role_mgr.h"
#include "role.h"

//player session
DWORD PlayerSession::HandleGetVipNetBarName(tagNetCmd* pCmd)
{
	//	��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	MGET_MSG(pRecv, pCmd, NC_GetVipNetBarName);
	INT		nNum = pRecv->nNum;

	if(nNum <= 0)
		return 0;

	if(nNum > 50)
		return 0;

	DWORD	dwSize = sizeof(tagNS_GetVipNetBarName) + (nNum - 1) * sizeof(tagNetBarIDName);
	MCREATE_MSG(pSend, dwSize, NS_GetVipNetBarName);
	pSend->nNum = nNum;

	for(INT n = 0; n < nNum; ++n)
	{
		Role* pRemoteRole = g_roleMgr.GetRolePtrByID(pRecv->dwAllID[n]);
		if (P_VALID(pRemoteRole))
		{
			LPCTSTR szVNBName = pRemoteRole->GetVNBName();
			// ������ǽ��������û����򲻷��ʹ������Ϣ
			if (!P_VALID(szVNBName))
			{
				continue;
			}
			else
			{
				DWORD dwNameLen = 0;
				dwNameLen = _tcslen(szVNBName);
				pSend->name[n].dwRoleID = pRecv->dwAllID[n];
				_tcsncpy(pSend->name[n].szNetBarName, szVNBName, dwNameLen);
			}
		}
		
	}

	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);

	return 0;
}
