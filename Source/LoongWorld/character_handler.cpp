//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: character_handler.cpp
// author: Sxg
// actor:
// data: 2008-07-21
// last:
// brief: �ͻ��˺ͷ������˼���Ϣ���� -- ѡ�˽������
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#include "../WorldDefine/selectrole.h"
#include "../ServerDefine/role_data.h"
#include "../ServerDefine/msg_common.h"
#include "../WorldDefine/msg_time.h"
#include "../WorldDefine/filter.h"

#include "player_session.h"
#include "db_session.h"
#include "role.h"
#include "role_mgr.h"
#include "map_creator.h"
#include "map.h"
#include "world_session.h"
#include "guild_mgr.h"
//--------------------------------------------------------------------------
// ������Ϸ
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleJoinGame(tagNetCmd* pCmd)
{
	// ���ͷ�����Ϣ
	tagNS_JoinGame send;
	send.dwErrorCode = 0;

	SendMessage(&send, send.dwSize);

	return 0;
}

//--------------------------------------------------------------------------
// �����ɫ�б�
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleEnum(tagNetCmd* pCmd)
{
	tagNC_EnumRole* pRecv = (tagNC_EnumRole*)pCmd;

	DWORD dwErrorCode = E_Success;

	if( m_bRoleEnumDone )
	{
		dwErrorCode = E_EnumRole_RoleEnumed;
	}
	else if( m_bRoleEnuming )
	{
		dwErrorCode = E_EnumRole_RoleEnuming;
	}
	else if( m_bRoleInWorld || m_pRole )
	{
		dwErrorCode = E_EnumRole_InWorld;
	}

	// �����鲻�Ϸ����򷵻ش�����
	if( E_Success != dwErrorCode )
	{
		tagNS_EnumRole send;
		send.dwErrorCode = dwErrorCode;
		SendMessage(&send, send.dwSize);

		return GT_INVALID;
	}
	else
	{
		// ͬ�����ͻ��˷�����ʱ��
		tagNS_SynchronismTime sendClient;
		sendClient.dwTime = GetCurrentDWORDTime();
		SendMessage(&sendClient, sendClient.dwSize);

		tagNDBC_LoadSimRole send;
		send.dwAccountID = m_dwAccountID;
		IFASTCODE->MemCpy(send.szAccount, GetAccount(), sizeof(send.szAccount));;
		g_dbSession.Send(&send, send.dwSize);

		m_bRoleEnuming = true;

		return 0;
	}

}

//--------------------------------------------------------------------------
// ������ɫ
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleCreate(tagNetCmd* pCmd)
{
	tagNC_CreateRole* pRecv = (tagNC_CreateRole*)pCmd;

	DWORD dwErrorCode = E_Success;

	// �����ҵ�ǰ��״̬
	if( !m_bRoleEnumDone )
	{
		dwErrorCode = E_CreateRole_RoleEnumNotDone;
	}
	else if( m_bRoleCreating )
	{
		dwErrorCode = E_CreateRole_RoleCreating;
	}
	else if (!m_bRoleEnumSuccess)
	{
		dwErrorCode = E_CreateRole_RoleEnumNotSuccess;
	}
	else if( m_bRoleDeleting )
	{
		dwErrorCode = E_CreateRole_RoleDeleting;
	}
	else if( m_bRoleLoading )
	{
		dwErrorCode = E_CreateRole_RoleLoading;
	}
	else if( m_n8RoleNum >= MAX_ROLENUM_ONEACCOUNT )
	{
		dwErrorCode = E_CreateRole_RoleNumFull;
	}
	else if( m_bRoleInWorld || m_pRole )
	{
		dwErrorCode = E_CreateRole_InWorld;
	}
	else
	{
		// Todo��Ҫ��鰲ȫ��ĺϷ��ԣ�ͬʱҪ��rolename
		// �ǲ��ǻ�Ҫ���һ��Avatar�еĸ����ֶ�ֵ
		//������Ҫ������RoleName�Ƿ��Ѿ�����
		//��RoleNameת��Сд����CRC32
		TCHAR	buf[X_SHORT_NAME] = {0};
		_tcsncpy(buf, pRecv->szRoleName, X_SHORT_NAME);
		buf[X_SHORT_NAME-1] = _T('\0');
		_tcslwr(buf);
		DWORD dwNameCrc = IUTIL->Crc32(buf);
		if (g_roleMgr.GetRoleIDByNameCrc(dwNameCrc) != (DWORD)GT_INVALID)
		{
			//RoleName�Ѿ�����
			dwErrorCode = E_CreateRole_NameExist;
		}
		else
		{
			// ������ֳ���,�Ϸ����ɿͻ��˱�֤
			dwErrorCode = Filter::CheckName(buf, g_attRes.GetVariableLen().nRoleNameMax, 
							g_attRes.GetVariableLen().nRoleNameMin, g_attRes.GetNameFilterWords());
		}	
	}

	if( E_Success != dwErrorCode )
	{
		tagNS_CreateRole send;
		send.dwErrorCode = dwErrorCode;

		SendMessage(&send, send.dwSize);

		return GT_INVALID;
	}
	else
	{
		tagNDBC_CreateRole send;
		send.dwAccountID = m_dwAccountID;

		// Todo: �õ��ʺ�����
		
		IFASTCODE->MemCpy(&send.RoleInfo.Avatar, &pRecv->AvatarAtt, sizeof(tagAvatarAtt));
		IFASTCODE->MemCpy(send.RoleInfo.szRoleName, pRecv->szRoleName, sizeof(pRecv->szRoleName));

		send.RoleInfo.byLevel = 1;
		send.RoleInfo.dwMapID = g_mapCreator.GetBornMapID();
		
		send.RoleInfo.CreateTime = g_world.GetWorldTime();

		// ��ʱ��д�������ڻ�Ӹõ�ͼ�ĵ�������Ѱ�ҵ�һ����������
		send.RoleInfo.fX = 0.0f;
		send.RoleInfo.fY = 0.0f;
		send.RoleInfo.fZ = 0.0f;
		send.RoleInfo.fFaceX = 0.0f;
		send.RoleInfo.fFaceY = 0.0f;
		send.RoleInfo.fFaceZ = 0.0f;

		g_dbSession.Send(&send, send.dwSize);

		m_bRoleCreating = true;

		return 0;
	}
}

//--------------------------------------------------------------------------
// ɾ����ɫ
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleDelete(tagNetCmd* pCmd)
{
	tagNC_DeleteRole* pRecv = (tagNC_DeleteRole*)pCmd;

	DWORD dwErrorCode = E_Success;
	// ������ﵱǰ״̬
	if( !m_bRoleEnumDone )
	{
		dwErrorCode = E_DelRole_RoleEnumNotDone;
	}
	else if( m_bRoleDeleting )
	{
		dwErrorCode = E_DelRole_RoleDeleting;
	}
	else if( m_bRoleCreating )
	{
		dwErrorCode = E_DelRole_RoleCreating;
	}
	else if( m_bRoleLoading )
	{
		dwErrorCode = E_DelRole_RoleLoading;
	}
	else if( m_bRoleInWorld || m_pRole )
	{
		dwErrorCode = E_DelRole_InWorld;
	}
	else if( FALSE == IsRoleExist(pRecv->dwRoleID) )
	{
		dwErrorCode = E_DelRole_RoleNotExist;
	}
	else if(pRecv->dwSafeCodeCrc != m_sAccountCommon.sSafeCode.dwSafeCodeCrc)
	{
		dwErrorCode = E_DelRole_SafeCodeIncorrect;
	}
	else	// ���������
	{
		tagRoleInfo* pRole = g_roleMgr.GetRoleInfo(pRecv->dwRoleID);
		if(P_VALID(pRole))
		{
			if(pRole->dwLoverID != GT_INVALID)
			{
				dwErrorCode = E_DelRole_BreakMarriage1st;
				goto EXIT;
			}
		}
	
		// ���ɴ���
		dwErrorCode = g_guildMgr.ClearRoleRemove(pRecv->dwRoleID);
		if(dwErrorCode != E_Success)
		{
			dwErrorCode = E_DelRole_LeaveGuild1st;
			goto EXIT;
		}
	}

EXIT:
	if( E_Success != dwErrorCode )
	{
		tagNS_DeleteRole send;
		send.dwErrorCode	= dwErrorCode;
		send.dwRoleID		= pRecv->dwRoleID;

		SendMessage(&send, send.dwSize);

		return GT_INVALID;
	}
	else
	{
		// ����ɾ����־λ
		tagNDBC_DelRole send;
		send.dwAccountID = m_dwAccountID;
		send.dwRoleID = pRecv->dwRoleID;
		send.dwTime = g_world.GetWorldTime();

		CHAR szIP[X_IP_LEN] = {0};
		strncpy(szIP, inet_ntoa((*(in_addr*)&m_dwIP)), X_IP_LEN);
		IFASTCODE->MemCpy(send.szIP, szIP, sizeof(szIP));

		g_roleMgr.GetRoleNameByID(pRecv->dwRoleID, send.szRoleName);

		g_dbSession.Send(&send, send.dwSize);

		m_bRoleDeleting = true;

		return 0;
	}
}

//------------------------------------------------------------------------------------
// ѡ���ɫ
//------------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSelect(tagNetCmd* pCmd)
{
	tagNC_SelectRole* pRecv = (tagNC_SelectRole*)pCmd;

	DWORD dwErrorCode = E_Success;

	if( !m_bRoleEnumDone )
	{
		dwErrorCode = E_SelectRole_RoleEnumNotDone;
	}
	else if( m_bRoleCreating )
	{
		dwErrorCode = E_SelectRole_RoleCreating;
	}
	else if( m_bRoleDeleting )
	{
		dwErrorCode = E_SelectRole_RoleDeleting;
	}
	else if( m_bRoleLoading )
	{
		dwErrorCode = E_SelectRole_RoleLoading;
	}
	else if( m_bRoleInWorld || P_VALID(m_pRole) )
	{
		dwErrorCode = E_SelectRole_InWorld;
	}
	else if( FALSE == IsRoleExist(pRecv->dwRoleID) )
	{
		dwErrorCode = E_SelectRole_RoleNotExist;
	}

	if( E_Success != dwErrorCode )
	{
		tagNS_SelectRole send;
		send.dwErrorCode = dwErrorCode;

		SendMessage(&send, send.dwSize);
	}
	else
	{
		tagNDBC_LoadRole send;
		send.dwAccountID = m_dwAccountID;
		send.dwRoleID = pRecv->dwRoleID;

		g_dbSession.Send(&send, send.dwSize);

		m_bRoleLoading = true;		
	}

	return 0;
}

//------------------------------------------------------------------------------------
// ���ð�ȫ��
//------------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSetSafeCode(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SafeCode);

	if(GT_INVALID == pRecv->dwSafeCode)
	{
		return GT_INVALID;
	}

	if(!CanSetSafeCode())
	{
		return GT_INVALID;
	}

	// ����
	m_sAccountCommon.sSafeCode.dwSafeCodeCrc = pRecv->dwSafeCode;
	m_sAccountCommon.sSafeCode.dwTimeReset = GT_INVALID;

	tagNDBC_SafeCodeSet send2DB;
	send2DB.dwAccountID = GetSessionID();
	send2DB.dwSafeCodeCrc = pRecv->dwSafeCode;
	g_dbSession.Send(&send2DB, send2DB.dwSize);

	tagNS_SafeCode send;
	send.dwErrorCode = E_Success;
	SendMessage(&send, send.dwSize);

	return E_Success;
}

//------------------------------------------------------------------------------------
// ���ð�ȫ��
//------------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleResetSafeCode(tagNetCmd* pCmd)
{
	//MGET_MSG(pRecv, pCmd, NC_ResetSafeCode);

	if(!CanResetSafeCode())
	{
		return GT_INVALID;
	}

	// ����
	m_sAccountCommon.sSafeCode.dwTimeReset = g_world.GetWorldTime();

	tagNDBC_SafeCodeReset send2DB;
	send2DB.dwAccountID = GetSessionID();
	send2DB.dwTimeReset = m_sAccountCommon.sSafeCode.dwTimeReset;
	g_dbSession.Send(&send2DB, send2DB.dwSize);

	tagNS_ResetSafeCode send;
	send.dwTimeReset = m_sAccountCommon.sSafeCode.dwTimeReset;
	SendMessage(&send, send.dwSize);

	return E_Success;
}

//------------------------------------------------------------------------------------
// ȡ����ȫ������
//------------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleCancelSafeCodeReset(tagNetCmd* pCmd)
{
	//MGET_MSG(pRecv, pCmd, NC_CancelSafeCodeReset);

	if(!CanCancelSafeCodeReset())
	{
		return GT_INVALID;
	}

	// ȡ��
	m_sAccountCommon.sSafeCode.dwTimeReset = GT_INVALID;

	tagNDBC_SafeCodeResetCancel send2DB;
	send2DB.dwAccountID = GetSessionID();
	g_dbSession.Send(&send2DB, send2DB.dwSize);

	tagNS_CancelSafeCodeReset send;
	send.dwErrorCode = E_Success;
	SendMessage(&send, send.dwSize);

	return E_Success;
}

////-------------------------------------------------------------------------
//// ���ԣ�������Ϸ
////-------------------------------------------------------------------------
//DWORD PlayerSession::HandleRoleEnterWorld(tagNetCmd* pCmd)
//{
//	tagNC_EnterWorld* pRecv = (tagNC_EnterWorld*)pCmd;
//
//	Role* pRole = new Role(this, pRecv->dwMapID, pRecv->curPos, pRecv->faceTo);
//	ASSERT( P_VALID(pRole) );
//
//	FullLogin(pRole);
//
//	return 0;
//}
