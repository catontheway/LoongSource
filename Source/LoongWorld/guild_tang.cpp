//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: guild_tang.cpp
// author: Sxg
// actor:
// data: 2009-4-7
// last:
// brief: ��
//-------------------------------------------------------------------------------
#include "StdAfx.h"

#include "guild_tang.h"
//-------------------------------------------------------------------------------
// ����&����
//-------------------------------------------------------------------------------
Tang::Tang()
{
	// ����
	for(INT i=0; i<MAX_GUILDPOS_TANGZHU; ++i)
	{
		m_dwTangZhuID[i] = GT_INVALID;
	}

	for(INT i=0; i<MAX_GUILDPOS_XIANGZHU; ++i)
	{
		m_dwXiangZhuID[i] = GT_INVALID;
	}

	// ��������
	m_n8CurNumTangZhu		= 0;
	m_n8CurNumXiangZhu		= 0;
	m_n8MaxNumTangZhong		= 0;
}

Tang::~Tang()
{
}

//-------------------------------------------------------------------------------
// ��ʼ��
//-------------------------------------------------------------------------------
VOID Tang::Init(BYTE byGuildLevel)
{
	m_n8MaxNumTangZhong = MGuildPosTangZhongNum(byGuildLevel);
}

//-------------------------------------------------------------------------------
// ��ӳ�Ա
//-------------------------------------------------------------------------------
DWORD Tang::AddMember(DWORD dwRoleID, ETangMemberPos eTangPos)
{
	switch(eTangPos)
	{
	case ETMP_TangZhu:		// ����
		return AddTangZhu(dwRoleID);
		break;
	case ETMP_XiangZhu:		// ����
		return AddXiangZhu(dwRoleID);
		break;
	case ETMP_BangZhong:	// ����
		return AddTangZhong(dwRoleID);
		break;
	default:				// ���� -- Ӧ���ϲ��жϣ��ų����ִ���
		ASSERT(0);
		return GT_INVALID;
		break;
	}

	return E_Success;
}

DWORD Tang::AddTangZhu(DWORD dwRoleID)
{
	// ����
	//if(m_n8CurNumTangZhu >= MAX_GUILDPOS_TANGZHU)
	//{
	//	return E_Guild_Member_Full;
	//}
	ASSERT(!IsTangZhuFull());

	// ����
	for(INT i=0; i<MAX_GUILDPOS_TANGZHU; ++i)
	{
		if(GT_INVALID == m_dwTangZhuID[i])
		{
			m_dwTangZhuID[i] = dwRoleID;
			
			// �����Լ�
			++m_n8CurNumTangZhu;
			return E_Success;
		}
	}

	// ִ�е��˴�˵��������bug
	ASSERT(0);
	return GT_INVALID;
}

DWORD Tang::AddXiangZhu(DWORD dwRoleID)
{
	// ����
	//if(m_n8CurNumXiangZhu >= MAX_GUILDPOS_XIANGZHU)
	//{
	//	return E_Guild_Member_Full;
	//}
	ASSERT(!IsXiangZhuFull());

	// ����
	for(INT i=0; i<MAX_GUILDPOS_XIANGZHU; ++i)
	{
		if(GT_INVALID == m_dwXiangZhuID[i])
		{
			m_dwXiangZhuID[i] = dwRoleID;

			// �����Լ�
			++m_n8CurNumXiangZhu;
			return E_Success;
		}
	}

	// ִ�е��˴�˵��������bug
	ASSERT(0);
	return GT_INVALID;
}

DWORD Tang::AddTangZhong(DWORD dwRoleID)
{
	// ����
	//if(m_lstTangZhong.Size() >= m_n8MaxNumBangZhong)
	//{
	//	return E_Guild_Member_Full;
	//}
	ASSERT(!IsTangZhongFull());

	// ����
	m_lstTangZhong.PushBack(dwRoleID);

	return E_Success;
}

//-------------------------------------------------------------------------------
// ɾ����Ա
//-------------------------------------------------------------------------------
VOID Tang::RemoveMember(DWORD dwRoleID, ETangMemberPos eTangPos)
{
	switch(eTangPos)
	{
	case ETMP_TangZhu:		// ����
		return RemoveTangZhu(dwRoleID);
		break;
	case ETMP_XiangZhu:		// ����
		return RemoveXiangZhu(dwRoleID);
		break;
	case ETMP_BangZhong:	// ����
		return RemoveTangZhong(dwRoleID);
		break;
	default:				// ���� -- Ӧ���ϲ��жϣ��ų����ִ���
		ASSERT(0);
		break;
	}
}

VOID Tang::RemoveTangZhu(DWORD dwRoleID)
{
	if(m_n8CurNumTangZhu <= 0)
	{
		ASSERT(0);
		return;
	}

	for(INT i=0; i<MAX_GUILDPOS_TANGZHU; ++i)
	{
		if(dwRoleID == m_dwTangZhuID[i])
		{
			m_dwTangZhuID[i] = GT_INVALID;

			--m_n8CurNumTangZhu;
			return;
		}
	}

	ASSERT(0);
	return;
}

VOID Tang::RemoveXiangZhu(DWORD dwRoleID)
{
	if(m_n8CurNumXiangZhu <= 0)
	{
		ASSERT(0);
		return;
	}

	for(INT i=0; i<MAX_GUILDPOS_XIANGZHU; ++i)
	{
		if(dwRoleID == m_dwXiangZhuID[i])
		{
			m_dwXiangZhuID[i] = GT_INVALID;

			--m_n8CurNumXiangZhu;
			return;
		}
	}

	ASSERT(0);
	return;
}

VOID Tang::RemoveTangZhong(DWORD dwRoleID)
{
	if(m_lstTangZhong.Size() <= 0)
	{
		ASSERT(0);
		return;
	}

	if(!m_lstTangZhong.Erase(dwRoleID))
	{
		ASSERT(0);
		return;
	}
}

//-------------------------------------------------------------------------------
// �Ƿ���Լ���
//-------------------------------------------------------------------------------
BOOL Tang::IsPosFull(ETangMemberPos eTangPos)
{
	switch(eTangPos)
	{
	case ETMP_TangZhu:		// ����
		return IsTangZhuFull();
		break;
	case ETMP_XiangZhu:		// ����
		return IsXiangZhuFull();
		break;
	case ETMP_BangZhong:	// ����
		return IsTangZhongFull();
		break;
	default:				// ���� -- Ӧ���ϲ��жϣ��ų����ִ���
		ASSERT(0);
		return TRUE;
		break;
	}

	return TRUE;
}