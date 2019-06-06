//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: guild_tang.h
// author: Sxg
// actor:
// data: 2009-4-7
// last:
// brief: �� -- �����ά����Աid��ְ�����ⲿ����,
//				�����ɾ��ʱ���ϲ㱣֤��id�Ϸ���
//-------------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/guild_define.h"
//-------------------------------------------------------------------------------
class Tang
{
public:
	Tang();
	~Tang();

	VOID	Init(BYTE byGuildLevel);
	
	BOOL	IsPosFull(ETangMemberPos eTangPos);
	DWORD	AddMember(DWORD dwRoleID, ETangMemberPos eTangPos);
	VOID	RemoveMember(DWORD dwRoleID, ETangMemberPos eTangPos);

private:
	BOOL	IsTangZhuFull() const;
	BOOL	IsXiangZhuFull() const;
	BOOL	IsTangZhongFull();

	DWORD	AddTangZhu(DWORD dwRoleID);
	DWORD	AddXiangZhu(DWORD dwRoleID);
	DWORD	AddTangZhong(DWORD dwRoleID);

	VOID	RemoveTangZhu(DWORD dwRoleID);
	VOID	RemoveXiangZhu(DWORD dwRoleID);
	VOID	RemoveTangZhong(DWORD dwRoleID);

private:
	// ����
	DWORD				m_dwTangZhuID[MAX_GUILDPOS_TANGZHU];
	DWORD				m_dwXiangZhuID[MAX_GUILDPOS_XIANGZHU];
	TList<DWORD>		m_lstTangZhong;

	// ��������
	INT8				m_n8CurNumTangZhu;
	INT8				m_n8CurNumXiangZhu;

	INT8				m_n8MaxNumTangZhong;
};


/*************************** ����ʵ�� *******************************/

inline BOOL Tang::IsTangZhuFull() const 
{
	return (m_n8CurNumTangZhu >= MAX_GUILDPOS_TANGZHU);
}

inline BOOL Tang::IsXiangZhuFull() const
{
	return (m_n8CurNumXiangZhu >= MAX_GUILDPOS_XIANGZHU);
}

inline BOOL Tang::IsTangZhongFull()
{
	return (m_lstTangZhong.Size() >= m_n8MaxNumTangZhong);
}