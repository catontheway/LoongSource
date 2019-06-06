//------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: loongdb_role.cpp
// author: Sxg
// actor:
// data: 2009-05-04
// last:
// brief: ���ݿ����Ӧ�ò�ʵ�� -- ��ɫ��(roledata)�������
//------------------------------------------------------------------------------
#include "StdAfx.h"

#include "LoongDB.h"
//-------------------------------------------------------------------------------------------------------
// �޸Ľ�ɫ����
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::ChangeRoleName(DWORD dwAccountID, DWORD dwRoleID, LPCTSTR szNewRoleName)
{
	BOOL bRet = TRUE;

	// ��ȡ��������
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	// ��ʽ������
	pStream->SetUpdate("roledata");
	pStream->FillString("RoleName='").FillString(szNewRoleName, pCon);
	pStream->FillString("',RoleNameCrc=") << m_pUtil->Crc32(szNewRoleName);
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(" and AccountID=") << dwAccountID;

	// ִ��
	bRet = m_pDBLoong->Execute(pStream);

	// �ͷ���������
	m_pDBLoong->ReturnConnection(pCon);
	m_pDBLoong->ReturnStream(pStream);

	++m_dwWriteTimes;

	return bRet;
}

//-------------------------------------------------------------------------------------------------------
// �޸Ľ�ɫ����id
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::ChangeRoleGuild(DWORD dwRoleID, DWORD dwGuildID)
{
	return Update(dwRoleID, &dwGuildID, 1, 0, &CLoongDB::FormatUpdateChangeRoleGuild);
}

VOID CLoongDB::FormatUpdateChangeRoleGuild(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	pStream->SetUpdate("roledata");
	pStream->FillString("GuildID=") << *(DWORD*)pData;

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// �޸Ľ�ɫdressMdID
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::ChangeRoleDressMdID(DWORD dwRoleID, WORD wValue)
{
	return Update(dwRoleID, &wValue, 1, 0, &CLoongDB::FormatUpdateChangeRoleDressMdID);
}

VOID CLoongDB::FormatUpdateChangeRoleDressMdID(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	pStream->SetUpdate("roledata");
	pStream->FillString("DressModelID=") << *(WORD*)pData;

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// ���ý�ɫɾ����־λ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SetRoleRemoveFlag(DWORD dwRoleID, DWORD dwTime)
{
	return Update(dwRoleID, &dwTime, 1, 0, &CLoongDB::FormatUpdateSetRoleRemoveFlag);
}

VOID CLoongDB::FormatUpdateSetRoleRemoveFlag(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pTime)
{
	pStream->SetUpdate("roledata");
	pStream->FillString("RemoveFlag=") << 1;
	pStream->FillString(",RemoveTime=") << *(DWORD*)pTime;

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}