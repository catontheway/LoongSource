//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: variable_len.h
// author: 
// actor:
// data: 2009-04-10
// last:
// brief: ��ͬ���԰汾�ַ�(TCHAR)�����ṹ����
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
struct tagVariableLen
{
	// ��ɫ����
	INT		nRoleNameMax;
	INT		nRoleNameMin;
	
	// �������
	INT		nGuildNameMax;
	INT		nGuildNameMin;
	INT		nGuildTenet;

	// ��̯
	INT		nStallTitleMax;

	tagVariableLen() { ZeroMemory(this, sizeof(*this)); }

	BOOL CheckValid() const
	{
		return (IsRoleNameValid()
				&& IsGuildNameValid()
				&& IsStallTitleValid());
	}

private:
	BOOL IsRoleNameValid() const
	{
		return !(nRoleNameMin < 0 || nRoleNameMax < nRoleNameMin || X_SHORT_NAME < nRoleNameMax);
	}

	BOOL IsGuildNameValid() const
	{
		return !(nGuildNameMin < 0 || nGuildNameMin > nGuildNameMax || nGuildTenet < 0);
	}

	BOOL IsStallTitleValid() const
	{
		return nStallTitleMax > 0;
	}
};