//-----------------------------------------------------------------------------
//!\file db.h
//!\author Aslan
//!
//!\date 2008-11-30
//! last 2008-11-30
//!
//!\brief ���ݿ��� ������ϲ�ʱLogin���ݿ����
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#include "Singleton.h"
#include "Loong_CommonDef.h"

//-----------------------------------------------------------------------------
// ���ݿ���
//-----------------------------------------------------------------------------
class CLoongLoginDBCtrl : public Singleton<CLoongLoginDBCtrl>
{
public:
    //-------------------------------------------------------------------------
	// CONSTURCT
	//-------------------------------------------------------------------------
	CLoongLoginDBCtrl() { }
	~CLoongLoginDBCtrl() { Destroy(); }

    //-------------------------------------------------------------------------
	// ��ʼ��������
	//-------------------------------------------------------------------------
    BOOL	Init(tagConfigDetail stConfig[EM_DB_TYPE_END]);
    VOID	Destroy();
   
	//-------------------------------------------------------------------------
	// �������ݿ��������
	//-------------------------------------------------------------------------	
	BOOL LoginDB_Accout_Insert(std::vector <tagTWrap_Longin_Account*> &vecLoginAccount,EDBType eDBType);
	BOOL LoginDB_Accout_Insert(std::vector <tagTWrap_Longin_WorldForbid*> &vecLoginWorldForbid,EDBType eDBType);
	BOOL LoginDB_Accout_Insert(std::vector <tagTWrap_Longin_BlackList*> &vecLoginWorldForbid,EDBType eDBType);
	BOOL LoginDB_Accout_Insert(std::vector <tagTWrap_Longin_FatigueTime*> &vecLoginWorldForbid,EDBType eDBType);

	BOOL LoginDB_Accout_Query(std::vector <tagTWrap_Longin_Account*> &vecLoginAccount,EDBType eDBType);
	BOOL LoginDB_Accout_Query(std::vector <tagTWrap_Longin_WorldForbid*> &vecLoginAccount,EDBType eDBType);
	BOOL LoginDB_Accout_Query(std::vector <tagTWrap_Longin_BlackList*> &vecLoginAccount,EDBType eDBType);
	BOOL LoginDB_Accout_Query(std::vector <tagTWrap_Longin_FatigueTime*> &vecLoginAccount,EDBType eDBType);
public:
	//�õ�account��������к�
	DWORD GetAccountMaxNum(EDBType eDBType);

private:

	DataBase		m_LoginDB[EM_DB_TYPE_END];	// Beton���ݿ�

};

//Beton�ص�����
VOID LoginDBCallBack(DataBase* pDB, INT nReason, INT nParam);

#define sLoginBeton CLoongLoginDBCtrl::getSingleton()







