//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: guild_affair.h
// author: Sunnee
// actor:
// data: 2009-07-03
// last:
// brief: �������񷢲�
//-----------------------------------------------------------------------------
#pragma once

class Guild;

class GuildAffair
{
public:
	GuildAffair();
	~GuildAffair();

	VOID	Init(Guild* pGuild);
	BOOL	IsInitOK()	{ return m_bInit; }

	// ��������
	DWORD	SpreadGuildAffair(Role* pRole, DWORD dwBuffID);

	// ���ð��񷢲�����
	VOID	ResetAffairCounter();

private:
	BOOL			m_bInit;
	Guild*			m_pGuild;

	BYTE			m_byCounter;		// ���񷢲�����(���浽���ݿ�?)
};