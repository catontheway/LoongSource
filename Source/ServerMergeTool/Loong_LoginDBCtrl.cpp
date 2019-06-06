//-----------------------------------------------------------------------------
//!\file db.cpp
//!\author Aslan
//!
//!\date 2008-11-30
//! last 2008-11-30
//!
//!\brief ���ݿ���
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "wx/wx.h"
#include "servermergetoolapp.h"
#include "Loong_OutputCtrl.h"
#include "Loong_LoginDBCtrl.h"
#include "../WorldDefine/time.h"


//-------------------------------------------------------------------------------
// ��ʼ������
//-------------------------------------------------------------------------------
BOOL CLoongLoginDBCtrl::Init(tagConfigDetail stConfig[EM_DB_TYPE_END])
{

	//��ʼ��LoginDB
	for(int iLoop=EM_DB_SRC_ONE;iLoop<EM_DB_TYPE_END;iLoop++)
	{
		BOOL bRet = m_LoginDB[iLoop].Init(stConfig[iLoop].strIp.c_str(),
			stConfig[iLoop].strUser.c_str(),
			stConfig[iLoop].strPsd.c_str(),
			stConfig[iLoop].strDBName.c_str(),
			stConfig[iLoop].dwPort);
		if(bRet == FALSE)
		{
			tstring strErr;
			strErr += stConfig[iLoop].strIp;
			strErr += _T("		");
			strErr += stConfig[iLoop].strUser;
			strErr += _T("		");
			strErr += stConfig[iLoop].strPsd;
			strErr += _T("		");
			strErr += stConfig[iLoop].strDBName;
			strErr += _T("		");
			strErr += stConfig[iLoop].dwPort;
			strErr += _T("��ʼ��ʧ�ܣ�");

			ILOG->Write(strErr.c_str());
			return FALSE;
		}
	}

	// ���ñ����ص�����
	for(int iLoop=EM_DB_SRC_ONE;iLoop<EM_DB_TYPE_END;iLoop++)
	{
		m_LoginDB[iLoop].SetWarningCallBack((WARNINGCALLBACK)LoginDBCallBack);
	}
	return TRUE;
}

//-------------------------------------------------------------------------------
// ���ٺ���
//-------------------------------------------------------------------------------
VOID CLoongLoginDBCtrl::Destroy()
{
	for(int iLoop=EM_DB_SRC_ONE;iLoop<EM_DB_TYPE_END;iLoop++)
	{
		m_LoginDB[iLoop].ShutDown();
	}
}
//-------------------------------------------------------------------------------
//�õ�account��������к�
//-------------------------------------------------------------------------------
DWORD CLoongLoginDBCtrl::GetAccountMaxNum(EDBType eDBType)
{
	MyStream* pStream = m_LoginDB[eDBType].GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	pStream->SetSelect("account","max(id)");
	// ��ѯ
	QueryResult* pResult = m_LoginDB[eDBType].Query(pStream);

	// ��ѯ���Ϊ��
	if( !P_VALID(pResult) || pResult->GetRowCount() <= 0) return GT_INVALID;

	return (*pResult)[0].GetDword();
}
//-------------------------------------------------------------------------
// ������ҷ�ͣ��¼
//-------------------------------------------------------------------------
BOOL CLoongLoginDBCtrl::LoginDB_Accout_Insert(std::vector <tagTWrap_Longin_WorldForbid*> &vecLoginWorldForbid,EDBType eDBType)
{
	MyStream* pStream = m_LoginDB[eDBType].GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	vector <tagTWrap_Longin_WorldForbid *>::iterator itLoop = vecLoginWorldForbid.begin();
	for(; itLoop != vecLoginWorldForbid.end(); ++itLoop)
	{
		pStream->SetInsert("world_forbid");
		pStream->FillString("accountid=")<<(*itLoop)->dwAccountID;
		pStream->FillString(",worldname_crc=")<<(*itLoop)->dwWorldNameCrc;

		// ��ѯ
		if(!m_LoginDB[eDBType].Execute(pStream))
		{
			return FALSE;
		}

		pStream->Clear();
	}
	// �ͷ���
	m_LoginDB[eDBType].ReturnStream(pStream);

	return TRUE;
}

//-------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------
BOOL CLoongLoginDBCtrl::LoginDB_Accout_Insert(std::vector <tagTWrap_Longin_BlackList*> &vecLoginBlackList,EDBType eDBType)
{
	MyStream* pStream = m_LoginDB[eDBType].GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	vector <tagTWrap_Longin_BlackList *>::iterator itLoop = vecLoginBlackList.begin();
	for(; itLoop != vecLoginBlackList.end(); ++itLoop)
	{
		pStream->SetInsert("black_list");
		pStream->FillString("ip='").FillString((*itLoop)->szIP).FillString("'");

		// ��ѯ
		if(!m_LoginDB[eDBType].Execute(pStream))
		{
			return FALSE;
		}

		pStream->Clear();
	}
	// �ͷ���
	m_LoginDB[eDBType].ReturnStream(pStream);

	return TRUE;
}
//-------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------
BOOL CLoongLoginDBCtrl::LoginDB_Accout_Insert(std::vector <tagTWrap_Longin_FatigueTime*> &vecLoginFatigueTime,EDBType eDBType)
{
	MyStream* pStream = m_LoginDB[eDBType].GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	vector <tagTWrap_Longin_FatigueTime *>::iterator itLoop = vecLoginFatigueTime.begin();
	for(; itLoop != vecLoginFatigueTime.end(); ++itLoop)
	{
		pStream->SetInsert("fatigue_time");
		pStream->FillString("accountname_crc=")<<(*itLoop)->dwAccountNameCrc;
		pStream->FillString(",acc_online_time=")<<(*itLoop)->dwOnlineTime;
		pStream->FillString(",acc_offline_time=")<<(*itLoop)->dwOfflineTime;

		// ��ѯ
		if(!m_LoginDB[eDBType].Execute(pStream))
		{
			return FALSE;
		}

		pStream->Clear();
	}
	// �ͷ���
	m_LoginDB[eDBType].ReturnStream(pStream);

	return TRUE;
}
//-------------------------------------------------------------------------
// ������Ҽ�¼
//-------------------------------------------------------------------------
BOOL CLoongLoginDBCtrl::LoginDB_Accout_Insert(std::vector <tagTWrap_Longin_Account*> &vecLoginAccount,EDBType eDBType)
{
	MyStream* pStream = m_LoginDB[eDBType].GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	// ��������
	Connection* pCon = m_LoginDB[eDBType].GetFreeConnection();

	vector <tagTWrap_Longin_Account *>::iterator itLoop = vecLoginAccount.begin();
	for(; itLoop != vecLoginAccount.end(); ++itLoop)
	{
		pStream->SetInsert("account");
		pStream->FillString("id=")<<(*itLoop)->stOthers.dwAccountID;
		pStream->FillString(",name='").FillString((*itLoop)->szName, pCon).FillString("'");
		pStream->FillString(",psd='").FillString((*itLoop)->szPsd, pCon).FillString("'");
		pStream->FillString(",privilege=") << (*itLoop)->stOthers.byPrivilege;
		pStream->FillString(",login_status=") << (*itLoop)->stOthers.eLoginStatus;
		pStream->FillString(",worldname_crc=") << (*itLoop)->stOthers.dwWorldNameCrc;
		pStream->FillString(",forbid_mask=") << (*itLoop)->stOthers.dwFrobidMask;
		pStream->FillString(",guard=") << (*itLoop)->stOthers.bGuard;

		pStream->FillString(",mibao='").FillString((*itLoop)->szMibao, pCon).FillString("'");

		// ��ѯ
		if(!m_LoginDB[eDBType].Execute(pStream))
		{
			return FALSE;
		}
		
		pStream->Clear();
	}
	// �ͷ�����
	m_LoginDB[eDBType].ReturnConnection(pCon);


	// �ͷ���
	m_LoginDB[eDBType].ReturnStream(pStream);

	return TRUE;
}
//-------------------------------------------------------------------------
// ��ѯ�����û���ͣ��¼
//-------------------------------------------------------------------------
BOOL CLoongLoginDBCtrl::LoginDB_Accout_Query(std::vector <tagTWrap_Longin_WorldForbid*> &vecLoginWorldForbid,EDBType eDBType)
{
	MyStream* pStream = m_LoginDB[eDBType].GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	pStream->SetSelect("world_forbid", "accountid,worldname_crc");

	QueryResult* pResult = m_LoginDB[eDBType].Query(pStream);

	// �ͷ���
	m_LoginDB[eDBType].ReturnStream(pStream);

	// ��ѯ���Ϊ��
	if( !P_VALID(pResult)) return FALSE;

	if(pResult->GetRowCount() == 0) return TRUE;

	do{
		tagTWrap_Longin_WorldForbid *pWorldForbid = new tagTWrap_Longin_WorldForbid;
		// ���ý��
		pWorldForbid->dwAccountID		=	(*pResult)[0].GetDword();
		pWorldForbid->dwWorldNameCrc	=	(*pResult)[1].GetDword();

		vecLoginWorldForbid.push_back(pWorldForbid);

	}while(pResult->NextRow());

	// �黹�����
	m_LoginDB[eDBType].FreeQueryResult(pResult);

	return TRUE;
}
//-------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------
BOOL CLoongLoginDBCtrl::LoginDB_Accout_Query(std::vector <tagTWrap_Longin_BlackList*> &vecLoginBlackList,EDBType eDBType)
{
	MyStream* pStream = m_LoginDB[eDBType].GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	pStream->SetSelect("black_list", "ip");

	QueryResult* pResult = m_LoginDB[eDBType].Query(pStream);

	// �ͷ���
	m_LoginDB[eDBType].ReturnStream(pStream);

	// ��ѯ���Ϊ��
	if( !P_VALID(pResult)) return FALSE;

	if(pResult->GetRowCount() == 0) return TRUE;

	do{
		tagTWrap_Longin_BlackList *pBlackList = new tagTWrap_Longin_BlackList;
		// ���ý��
		memcpy(pBlackList->szIP,(*pResult)[0].GetString(),(*pResult)[0].GetLen()+1);

		vecLoginBlackList.push_back(pBlackList);

	}while(pResult->NextRow());

	// �黹�����
	m_LoginDB[eDBType].FreeQueryResult(pResult);

	return TRUE;
}
//-------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------
BOOL CLoongLoginDBCtrl::LoginDB_Accout_Query(std::vector <tagTWrap_Longin_FatigueTime*> &vecLoginFatigueTime,EDBType eDBType)
{
	MyStream* pStream = m_LoginDB[eDBType].GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	pStream->SetSelect("fatigue_time", "accountname_crc,acc_online_time,acc_offline_time");

	QueryResult* pResult = m_LoginDB[eDBType].Query(pStream);

	// �ͷ���
	m_LoginDB[eDBType].ReturnStream(pStream);

	// ��ѯ���Ϊ��
	if( !P_VALID(pResult)) return FALSE;

	if(pResult->GetRowCount() == 0) return TRUE;

	do{
		tagTWrap_Longin_FatigueTime *pFatigueTime = new tagTWrap_Longin_FatigueTime;
		// ���ý��
		pFatigueTime->dwAccountNameCrc		=	(*pResult)[0].GetDword();
		pFatigueTime->dwOnlineTime		=	(*pResult)[0].GetDword();
		pFatigueTime->dwOfflineTime		=	(*pResult)[0].GetDword();

		vecLoginFatigueTime.push_back(pFatigueTime);

	}while(pResult->NextRow());

	// �黹�����
	m_LoginDB[eDBType].FreeQueryResult(pResult);

	return TRUE;
}
//-------------------------------------------------------------------------
// ��ѯ�����û�
//-------------------------------------------------------------------------
BOOL CLoongLoginDBCtrl::LoginDB_Accout_Query(std::vector <tagTWrap_Longin_Account*> &vecLoginAccount,EDBType eDBType)
{
	MyStream* pStream = m_LoginDB[eDBType].GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	pStream->SetSelect("account", "id,name,psd,privilege,login_status,worldname_crc,forbid_mask,guard,mibao");

	QueryResult* pResult = m_LoginDB[eDBType].Query(pStream);

	// �ͷ���
	m_LoginDB[eDBType].ReturnStream(pStream);

	// ��ѯ���Ϊ��
	if( !P_VALID(pResult)) return FALSE;

	if(pResult->GetRowCount() == 0) return TRUE;


	do{
		tagTWrap_Longin_Account *pAccount = new tagTWrap_Longin_Account;
		// ���ý��
		pAccount->stOthers.dwAccountID		=	(*pResult)[0].GetDword();
		
		strncpy(pAccount->szName,(*pResult)[1].GetString(),(*pResult)[1].GetLen()+1);
		strncpy(pAccount->szPsd,(*pResult)[2].GetString(),(*pResult)[2].GetLen()+1);
		pAccount->stOthers.byPrivilege		=	(*pResult)[3].GetByte();
		pAccount->stOthers.eLoginStatus		=	(EPlayerLoginStatus)(*pResult)[4].GetInt();
		pAccount->stOthers.dwWorldNameCrc	=	(*pResult)[5].GetDword();
		pAccount->stOthers.dwFrobidMask		=	(*pResult)[6].GetDword();
		pAccount->stOthers.bGuard			=	(*pResult)[7].GetBool();
		pAccount->stOthers.nGuardAccuTime	=	0;
		
		if((*pResult)[8].GetLen() > 0 )
		strncpy(pAccount->szMibao,(*pResult)[8].GetString(),(*pResult)[8].GetLen()+1);

		// ��¼�ܱ�
		CHAR szMibao[MIBAO_LEN] = {'\0'};
		BOOL bRet = (*pResult)[8].GetBlob(szMibao, MIBAO_LEN);
		if( '\0' == szMibao[0] )
		{
			pAccount->stOthers.bNeedMibao = FALSE;
		}
		else
		{
			pAccount->stOthers.bNeedMibao = TRUE;
		}


		vecLoginAccount.push_back(pAccount);

	}while(pResult->NextRow());

	// �黹�����
	m_LoginDB[eDBType].FreeQueryResult(pResult);

	return TRUE;
}

//------------------------------------------------------------------------------------
// ���ݿ�Ԥ���ص�����
//------------------------------------------------------------------------------------
VOID LoginDBCallBack(DataBase* pDB, INT nReason, INT nParam)
{
	TCHAR cTemp[200];

	if (nReason == DataBase::EDBE_System)
	{
		_stprintf(cTemp,_T("Beton Warning CallBack: Reason  %s , nParam = %u\r\n"), _T("EDBE_System"), nParam);
		g_clsOutput.Output(cTemp);
	}
	else if(nReason == DataBase::EDBE_QueueFull)
	{
		_stprintf(cTemp,_T("Beton Warning CallBack: Reason  %s , nParam = %u\r\n"), _T("EDBE_QueueFull"), nParam);
		g_clsOutput.Output(cTemp);
	}
	else if(nReason == DataBase::EDBE_PoolFull)
	{
		_stprintf(cTemp,_T("Beton Warning CallBack: Reason  %s , nParam = %u\r\n"), _T("EDBE_PoolFull"), nParam);
		g_clsOutput.Output(cTemp);
	}
	else
	{	
		_stprintf(cTemp,_T("Beton Warning CallBack: Reason Unknow,nReason = %u, nParam = %u\r\n"), nReason, nParam);
		g_clsOutput.Output(cTemp);
	}

}