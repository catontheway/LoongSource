//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: guild_pos.cpp
// author: Sxg
// actor:
// data: 2009-4-20
// last:
// brief: ����ְλ����
//-------------------------------------------------------------------------------
#include "StdAfx.h"

#include "guild_pos.h"
#include "role_mgr.h"
#include "../ServerDefine/role_data_define.h"
//-------------------------------------------------------------------------------
// ����&����
//-------------------------------------------------------------------------------
GuildPos::GuildPos()
{
	m_n8CurNumBangZhu	 = 0;
	m_n8CurNumFuBangZhu	 = 0;
	m_n8CurNumHongHuFa	 = 0;
	m_n8CurNumZiHuFa	 = 0;
	m_n8CurNumJingYing	 = 0;

	m_n8MaxNumJingYing	 = 0;

	m_n16CurNumMember	 = 0;
	m_n16MaxNumMember	 = 0;
}

GuildPos::~GuildPos()
{
}

//-------------------------------------------------------------------------------
// ��ʼ��
//-------------------------------------------------------------------------------
VOID GuildPos::Init(BYTE byGuildLevel)
{
	for(INT i=0; i<X_GUILD_TANG_NUM; ++i)
	{
		m_Tang[i].Init(byGuildLevel);
	}

	m_n8MaxNumJingYing	= MGuildPosJingZhongNum(byGuildLevel);
	m_n16MaxNumMember	= MGuildMaxMember(byGuildLevel);
}

//-------------------------------------------------------------------------------
// ָ��ְλ����Ա
//-------------------------------------------------------------------------------
BOOL GuildPos::IsPosFull(EGuildMemberPos eGuildPos)
{
	ASSERT(!IsMemberFull());
	ASSERT(::IsGuildPosValid(eGuildPos));
	
	// ����ֱ��ְλ�ж�
	switch(eGuildPos)
	{
	case EGMP_BangZhong:
		return FALSE;
	case EGMP_JingYing:
		return IsJingYingFull();
	case EGMP_FuBangZhu:
		return IsFuBangZhuFull();
	case EGMP_HongHuFa:
		return IsHongHuFaFull();
	case EGMP_ZiHuFa:
		return IsZiHuFaFull();
	case EGMP_BangZhu:
		return IsBangZhuFull();
	}

	// ����ְλ�ж�
	if(::IsInTang(eGuildPos))
	{
		EGuildTangType eTang;
		ETangMemberPos eTangPos;
		::GetTangType(eGuildPos, eTang, eTangPos);

		return m_Tang[eTang].IsPosFull(eTangPos);
	}

	// û�н����������̷��أ�˵��ְλ������
	return TRUE;
}

//-------------------------------------------------------------------------------
// ��ӳ�Ա
//-------------------------------------------------------------------------------
DWORD GuildPos::AddMember(DWORD dwRoleID, EGuildMemberPos eGuildPos)
{
	ASSERT(!IsPosFull(eGuildPos));

	DWORD dwErrorCode = GT_INVALID;
	BOOL  bRunDefault = FALSE;

	// ����ֱ��ְλ�ж�
	switch(eGuildPos)
	{
	case EGMP_BangZhong:
		dwErrorCode = AddPuTong(dwRoleID);
		break;
	case EGMP_JingYing:
		dwErrorCode = AddJingYing(dwRoleID);
		break;
	case EGMP_FuBangZhu:
		dwErrorCode = AddFuBangZhu(dwRoleID);
		break;
	case EGMP_HongHuFa:
		dwErrorCode = AddHongHuFa(dwRoleID);
		break;
	case EGMP_ZiHuFa:
		dwErrorCode = AddZiHuFa(dwRoleID);
		break;
	case EGMP_BangZhu:
		dwErrorCode = AddBangZhu(dwRoleID);
		break;
	default:
		bRunDefault = TRUE;
	}

	// ����ְλ�ж�
	if(::IsInTang(eGuildPos))
	{
		EGuildTangType eTang;
		ETangMemberPos eTangPos;
		::GetTangType(eGuildPos, eTang, eTangPos);

		dwErrorCode = m_Tang[eTang].AddMember(dwRoleID, eTangPos);
	}
	else if(bRunDefault)
	{
		// ִ�е��˴������øú���ǰ���߼�������
		ASSERT(0);
	}

	if(E_Success == dwErrorCode)
	{
		++m_n16CurNumMember;
	}

	return dwErrorCode;
}

DWORD GuildPos::AddBangZhu(DWORD dwRoleID)
{
	ASSERT(!IsBangZhuFull());
	++m_n8CurNumBangZhu;
	return E_Success;
}

DWORD GuildPos::AddFuBangZhu(DWORD dwRoleID)
{
	ASSERT(!IsFuBangZhuFull());
	++m_n8CurNumFuBangZhu;
	return E_Success;
}

DWORD GuildPos::AddHongHuFa(DWORD dwRoleID)
{
	ASSERT(!IsHongHuFaFull());
	++m_n8CurNumHongHuFa;
	return E_Success;
}

DWORD GuildPos::AddZiHuFa(DWORD dwRoleID)
{
	ASSERT(!IsZiHuFaFull());
	++m_n8CurNumZiHuFa;
	return E_Success;
}

DWORD GuildPos::AddJingYing(DWORD dwRoleID)
{
	ASSERT(!IsJingYingFull());
	++m_n8CurNumJingYing;
	return E_Success;
}

DWORD GuildPos::AddPuTong(DWORD dwRoleID)
{
	ASSERT(!IsMemberFull());
	return E_Success;
}

//-------------------------------------------------------------------------------
// ɾ��
//-------------------------------------------------------------------------------
VOID GuildPos::RemoveMember(DWORD dwRoleID, EGuildMemberPos eGuildPos)
{
	ASSERT(m_n16CurNumMember >= 1);

	BOOL  bRunDefault = FALSE;
	
	// ����ֱ��ְλ�ж�
	switch(eGuildPos)
	{
	case EGMP_BangZhong:
		RemovePuTong(dwRoleID);
		break;
	case EGMP_JingYing:
		RemoveJingYing(dwRoleID);
		break;
	case EGMP_FuBangZhu:
		RemoveFuBangZhu(dwRoleID);
		break;
	case EGMP_HongHuFa:
		RemoveHongHuFa(dwRoleID);
		break;
	case EGMP_ZiHuFa:
		RemoveZiHuFa(dwRoleID);
		break;
	case EGMP_BangZhu:
		RemoveBangZhu(dwRoleID);
		break;
	default:
		bRunDefault = TRUE;
	}

	// ����ְλ�ж�
	if(::IsInTang(eGuildPos))
	{
		EGuildTangType eTang;
		ETangMemberPos eTangPos;
		::GetTangType(eGuildPos, eTang, eTangPos);
		m_Tang[eTang].RemoveMember(dwRoleID, eTangPos);
	}
	else if(bRunDefault)
	{
		// ִ�е��˴������øú���ǰ���߼�������
		ASSERT(0);
		return;
	}

	--m_n16CurNumMember;
}

VOID GuildPos::RemoveBangZhu(DWORD dwRoleID)
{
	--m_n8CurNumBangZhu;
	ASSERT(m_n8CurNumBangZhu >= 0);
}

VOID GuildPos::RemoveFuBangZhu(DWORD dwRoleID)
{
	--m_n8CurNumFuBangZhu;
	ASSERT(m_n8CurNumFuBangZhu >= 0);
}

VOID GuildPos::RemoveHongHuFa(DWORD dwRoleID)
{
	--m_n8CurNumHongHuFa;
	ASSERT(m_n8CurNumHongHuFa >= 0);
}

VOID GuildPos::RemoveZiHuFa(DWORD dwRoleID)
{
	--m_n8CurNumZiHuFa;
	ASSERT(m_n8CurNumZiHuFa >= 0);
}

VOID GuildPos::RemoveJingYing(DWORD dwRoleID)
{
	--m_n8CurNumJingYing;
	ASSERT(m_n8CurNumJingYing >= 0);
}

VOID GuildPos::RemovePuTong(DWORD dwRoleID)
{
	ASSERT(m_n16CurNumMember >= 1);
}

//-------------------------------------------------------------------------------
// ְλ����Ҫ��
//-------------------------------------------------------------------------------
DWORD GuildPos::SpecGuildPosLimit(DWORD dwRoleID, EGuildMemberPos eGuildPos)
{
	// ���»�������������ֻ��Ů�Խ�ɫ���Ե���
	if(EGMP_HongHuFa == eGuildPos || EGMP_ZiHuFa == eGuildPos)
	{
		tagRoleInfo *pRoleInfo = g_roleMgr.GetRoleInfo(dwRoleID);
		if(!P_VALID(pRoleInfo))
		{
			// ִ�е��ˣ�˵��ɾ����ɫ���߼���������
			ASSERT(0);
			return E_Role_Not_ExistIn_World;
		}

		if(pRoleInfo->bySex != (BYTE)ES_Woman)
		{
			return E_Guild_Appoint_SexLimit;
		}
	}

	return E_Success;
}