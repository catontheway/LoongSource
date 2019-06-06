#include "StdAfx.h"
#include "wx/wx.h"
#include "Loong_MergeLogicCtrl.h"
#include "servermergetoolapp.h"
#include "Loong_OutputCtrl.h"
#include "../WorldDefine/time.h"
#include "../WorldDefine/base_define.h"

//����
BOOL Compare_Loong_GuildLess(tagTWrap_Loong_Guild* pFir,tagTWrap_Loong_Guild* pSec)
{
	if( pFir->stOthers.nReputation<pSec->stOthers.nReputation)
		return false;
	else if(pFir->stOthers.nReputation>pSec->stOthers.nReputation)
		return true;

	return false;
}
//����
BOOL Compare_Loong_RoleDataAcs(tagTWrap_Loong_RoleData* pFir,tagTWrap_Loong_RoleData* pSec)
{
	//����
	if( pFir->dwAccountID<pSec->dwAccountID)
		return true;
	else if(pFir->dwAccountID>pSec->dwAccountID)
		return false;

	//����
	if(pFir->stOthers.RoleDataSave.nLevel>pSec->stOthers.RoleDataSave.nLevel)
		return true;
	else if(pFir->stOthers.RoleDataSave.nLevel<pSec->stOthers.RoleDataSave.nLevel)
		return false;

	//����
	if(pFir->stOthers.RoleDataSave.nCurExp>pSec->stOthers.RoleDataSave.nCurExp)
		return true;
	else if(pFir->stOthers.RoleDataSave.nCurExp<pSec->stOthers.RoleDataSave.nCurExp)
		return false;

	return false;
}
//����
BOOL Compare_Loong_ItemAcs(tagTWrap_Loong_Item* pFir,tagTWrap_Loong_Item* pSec)
{
	//����
	if( pFir->stOthers.dwAccountID<pSec->stOthers.dwAccountID)
		return true;
	else if(pFir->stOthers.dwAccountID>pSec->stOthers.dwAccountID)
		return false;

	//���� Ϊ��ȷ������Դ1�ĵ���
	if( pFir->stOthers.dwOwnerID<pSec->stOthers.dwOwnerID)
		return true;
	else if(pFir->stOthers.dwOwnerID>pSec->stOthers.dwOwnerID)
		return false;

	//���������ͷ�һ�� ����
	if(pFir->stOthers.eConType<pSec->stOthers.eConType)
		return true;
	else if(pFir->stOthers.eConType>pSec->stOthers.eConType)
		return false;

	//��λ����Ϣ��һ��
	if( pFir->stOthers.n16Index<pSec->stOthers.n16Index)
		return true;
	else if(pFir->stOthers.n16Index>pSec->stOthers.n16Index)
		return false;


	return false;
}

CLoongMergeLogicCtrl::CLoongMergeLogicCtrl()
:m_eMergeType(EM_MERGE_NULL),m_strSrc2WorldName(_T("")),m_n64Src1ItemMaxSerialInternal(MIN_ITEM_SERIAL_INTERNAL+1)
,m_n64Src1ItemMinSerialOutter(MIN_ITEM_SERIAL_OUTTER),m_n64Src1ItemMinSerialBill(MAX_ITEM_SERIAL_BILL),m_dwSrc1VipStallMaxNum(0)
,m_dwSrc1RoleMaxNum(0),m_dwSrc1LoginMaxNum(0),m_dwSrc1WorldNameCrc(0),m_dwSrc1PetMaxNum(0)
{
	//����һЩ������Ʒ�Ͱٱ�����Ʒ
	m_vecOldSrc1AccountID.push_back(0xffffffff);
	m_mpOldSrc1VsSrc2.insert(make_pair(0xffffffff,0xffffffff));

	m_n64ItemCurSerialInternal = MIN_ITEM_SERIAL_INTERNAL;
	m_n64ItemCurSerialOutter = MAX_ITEM_SERIAL_OUTTER;
	m_n64ItemCurSerialBill = MAX_ITEM_SERIAL_BILL;

}

CLoongMergeLogicCtrl::~CLoongMergeLogicCtrl(void)
{
}
//�ɵ������м�����
VOID CLoongMergeLogicCtrl::ClearMemory()
{
	m_n64Src1ItemMaxSerialInternal = MIN_ITEM_SERIAL_INTERNAL+1;
	m_n64Src1ItemMinSerialOutter = MIN_ITEM_SERIAL_OUTTER;
	m_n64Src1ItemMinSerialBill = MAX_ITEM_SERIAL_BILL;

	m_n64ItemCurSerialInternal = MIN_ITEM_SERIAL_INTERNAL;
	m_n64ItemCurSerialOutter = MAX_ITEM_SERIAL_OUTTER;
	m_n64ItemCurSerialBill = MAX_ITEM_SERIAL_BILL;

	m_dwSrc1LoginMaxNum = 0;
	m_dwSrc1RoleMaxNum = 0;
	m_dwSrc1PetMaxNum = 0;
	m_dwSrc1VipStallMaxNum = 0;

	m_dwSrc1WorldNameCrc = 0;
	//m_mpOldItemNumvsNewItemNum.clear();
	m_mpOldGuildIDvsNewGuildID.clear();
	m_mpAccountIDvsWareSize.clear();
	m_mpRoleIDvsRemoveType.clear();
	m_vecOverItem.clear();
	m_setOldGuildID.clear();
	m_mpAccountIDvsRoleIDforCB.clear();
	m_vecOldSrc1AccountID.clear();
	m_mpOldSrc1VsSrc2.clear();
	m_vecOldSrc2OnlyAccountID.clear();
	m_vecRoleCompensation.clear();
	m_vecOverItem.clear();

}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//����roleid ��Դ2ʹ��
VOID CLoongMergeLogicCtrl::Update_Loong_RoleID(DWORD &dwRoleID)
{
	if(IS_PLAYER(dwRoleID))
		dwRoleID += m_dwSrc1RoleMaxNum;
	else if(IS_PET(dwRoleID))
		dwRoleID += (m_dwSrc1PetMaxNum - MIN_PET_ID);

}
//ת����Сд���crc32
DWORD CLoongMergeLogicCtrl::LowerCrc32(LPCTSTR str,DWORD strLen)
{
	TCHAR *szTemp = new TCHAR[strLen];

	_tcsncpy(szTemp,str,strLen);

	_tcslwr(szTemp);

	DWORD dwCrc = IUTIL->Crc32(szTemp);

	SAFE_DEL_ARRAY(szTemp);

	return dwCrc;
}

//����item 64λ���
VOID CLoongMergeLogicCtrl::Update_Loong_Item64SerialNum(INT64 &n64Serial)
{
	if(n64Serial != GT_INVALID)
	{
		if(n64Serial>MIN_ITEM_SERIAL_INTERNAL)
		{	
			n64Serial += (m_n64Src1ItemMaxSerialInternal - MIN_ITEM_SERIAL_INTERNAL);

			if(n64Serial > m_n64ItemCurSerialInternal)
			{
				m_n64ItemCurSerialInternal = n64Serial;
			}
		}
		else if(n64Serial>=MIN_ITEM_SERIAL_OUTTER && n64Serial<=MAX_ITEM_SERIAL_OUTTER)
		{
			n64Serial -= (MAX_ITEM_SERIAL_OUTTER - m_n64Src1ItemMinSerialOutter + 1);

			if(n64Serial < m_n64ItemCurSerialOutter)
			{
				m_n64ItemCurSerialOutter = n64Serial;
			}
		}
		else if(n64Serial<=MAX_ITEM_SERIAL_BILL)
		{
			n64Serial -= (MAX_ITEM_SERIAL_BILL - m_n64Src1ItemMinSerialBill);

			if(n64Serial < m_n64ItemCurSerialBill)
			{
				m_n64ItemCurSerialBill = n64Serial;
			}
		}
		else
		{
			ASSERT(0);
		}
	}

}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//login account��
BOOL CLoongMergeLogicCtrl::Loong_LoginMerge(std::vector <tagTWrap_Longin_Account*> &vecSrc1,
							  std::vector <tagTWrap_Longin_Account*> &vecSrc2,
							  std::vector <tagTWrap_Longin_Account*> &vecTar)
{
	//����� src1��accountid���� src2�Ĵ����� �����Ĳ�׷��
	std::vector <tagTWrap_Longin_Account*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Longin_Account*>::iterator itSrc2 = vecSrc2.begin();
	std::map<tagAccountIDKey,DWORD>::iterator itSrcFind;
	std::map<DWORD ,DWORD>::iterator itAccountFind;
	std::set<string>::iterator	itAccountName;
	std::set<string> setAccountName; //��������	

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	//Դ1
	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		setAccountName.insert((*itSrc1)->szName);
		vecTar.push_back(*itSrc1);
	}

	//��������ǿ����
	if(m_eMergeType != EM_MERGE_CROSS_SECTION)
	{
		return TRUE;
	}
	//��һ��Դ1��������crc 
	//��
	for(int iLoop = 0;iLoop<vecTar.size();iLoop++)
	{
		//����Դ1�ı����� ����account�Ƿ�����Դ1����
		itAccountFind = m_mpAccountIDvsWareSize.find(vecTar[iLoop]->stOthers.dwAccountID);
		if(itAccountFind != m_mpAccountIDvsWareSize.end())
		{
			//����ҵ��� ��crc����
			m_dwSrc1WorldNameCrc = vecTar[iLoop]->stOthers.dwWorldNameCrc;
			break;
		}
	}
	//Դ2
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		//��Դ2����
		itSrcFind = m_mpOldAccountVsNewAccount.find(tagAccountIDKey((*itSrc2)->stOthers.dwAccountID,EM_DB_SRC_TWO));
		//����Ҳ���
		if(itSrcFind == m_mpOldAccountVsNewAccount.end())
		{
			continue;
		}
		//������ҵ� ����id�Ƿ���src1��Χ�� 
		if( (itSrcFind->second) <=  m_dwSrc1LoginMaxNum )
		{
			continue;
		}
		//������ҵ�������
		itAccountName = setAccountName.find((*itSrc2)->szName);
		if(itAccountName != setAccountName.end())
		{
			continue;
		}
		(*itSrc2)->stOthers.dwWorldNameCrc = m_dwSrc1WorldNameCrc;
		(*itSrc2)->stOthers.dwAccountID = itSrcFind->second;
		vecTar.push_back(*itSrc2);
	}
	return TRUE;
}
//login worldforbid��
BOOL CLoongMergeLogicCtrl::Loong_LoginMerge(std::vector <tagTWrap_Longin_WorldForbid*> &vecSrc1,
											std::vector <tagTWrap_Longin_WorldForbid*> &vecSrc2,
											std::vector <tagTWrap_Longin_WorldForbid*> &vecTar)
{
	std::vector <tagTWrap_Longin_WorldForbid*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Longin_WorldForbid*>::iterator itSrc2 = vecSrc2.begin();
	std::map<tagAccountIDKey,DWORD>::iterator itSrcFind;
	//Դ1�Ĳ��� Դ2�ĸ���Դ2������ ���crc 
	DWORD dwSrc2WorldCrc = IUTIL->Crc32(m_strSrc2WorldName.c_str());

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());

	if(m_eMergeType != EM_MERGE_CROSS_SECTION)
	{
		return TRUE;
	}

	//��������
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		itSrcFind = m_mpOldAccountVsNewAccount.find(tagAccountIDKey((*itSrc2)->dwAccountID,EM_DB_SRC_TWO));
		//����ҵ�
		if(itSrcFind != m_mpOldAccountVsNewAccount.end())
		{
			//accountid
			(*itSrc2)->dwAccountID = (*itSrcFind).second;				
		}
		
		//����Ҳ���
		if(itSrcFind == m_mpOldAccountVsNewAccount.end())
		{
			continue;
		}	
		//������ҵ� ����id�Ƿ���src1��Χ�� 
		if( (itSrcFind->second) <=  m_dwSrc1LoginMaxNum )
		{
			continue;
		}		
		//��ȷ����ΪԴ2��Ϸ����ʱ ��crc�ĳ�Դ1��������crc
		if((*itSrc2)->dwWorldNameCrc == dwSrc2WorldCrc)
		{
			(*itSrc2)->dwWorldNameCrc = m_dwSrc1WorldNameCrc;
			vecTar.push_back(*itSrc2);	
		}

	}
	return true;
}
//login ������
BOOL CLoongMergeLogicCtrl::Loong_LoginMerge(std::vector <tagTWrap_Longin_BlackList*> &vecSrc1,
											std::vector <tagTWrap_Longin_BlackList*> &vecSrc2,
											std::vector <tagTWrap_Longin_BlackList*> &vecTar)
{
	std::vector <tagTWrap_Longin_BlackList*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Longin_BlackList*>::iterator itSrc2 = vecSrc2.begin();
	std::set<string>::iterator	itIP;
	std::set<string> setIP; //�����ظ�ip	
	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	//Դ1
	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		setIP.insert((*itSrc1)->szIP);
		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		//��Դ2����
		itIP = setIP.find((*itSrc2)->szIP);
		//����ҵ�
		if(itIP != setIP.end())
		{
			continue;
		}
		vecTar.push_back(*itSrc2);
	}
	return true;
}
//login ������
BOOL CLoongMergeLogicCtrl::Loong_LoginMerge(std::vector <tagTWrap_Longin_FatigueTime*> &vecSrc1,
											std::vector <tagTWrap_Longin_FatigueTime*> &vecSrc2,
											std::vector <tagTWrap_Longin_FatigueTime*> &vecTar)
{
	std::vector <tagTWrap_Longin_FatigueTime*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Longin_FatigueTime*>::iterator itSrc2 = vecSrc2.begin();

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());
	std::set<DWORD> setAccountNameCrc;
	std::set<DWORD>::iterator itFind;
	//Դ1
	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		setAccountNameCrc.insert((*itSrc1)->dwAccountNameCrc);
		vecTar.push_back(*itSrc1);
	}

	if(m_eMergeType != EM_MERGE_CROSS_SECTION)
	{
		return TRUE;
	}

	//��������
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		itFind = setAccountNameCrc.find((*itSrc2)->dwAccountNameCrc);
		if(itFind != setAccountNameCrc.end())
		{
			continue;
		}

		vecTar.push_back(*itSrc2);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//���˺������о���
string Loong_CheckAccountName(CHAR szAccountName[X_SHORT_NAME])
{
	ASSERT(szAccountName);

	string strTemp = szAccountName;

	//���ؽ��
	string strReturn = "";

	for(string::size_type iLoop = 0; iLoop != strTemp.size(); ++iLoop)
	{
		//����
		if(isdigit(strTemp[iLoop]) != 0)
		{
			strReturn += strTemp[iLoop];
			continue;
		}
		//��ĸ
		if(isalpha(strTemp[iLoop]) != 0)
		{
			strReturn += tolower(strTemp[iLoop]);
			continue;
		}
		if(strTemp[iLoop] == '-')
		{
			strReturn += strTemp[iLoop];
			continue;
		}
		if(strTemp[iLoop] == '+')
		{
			strReturn += strTemp[iLoop];
			continue;
		}
		if(strTemp[iLoop] == '_')
		{
			strReturn += strTemp[iLoop];
			continue;
		}
	}


	return strReturn;
}
//loong ��Ҫ�ϲ��ֿ��ڽ�Ǯ
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_AccountCommon*> &vecSrc1,
									std::vector <tagTWrap_Loong_AccountCommon*> &vecSrc2,
									std::vector <tagTWrap_Loong_AccountCommon*> &vecTar)
{

	std::vector <tagTWrap_Loong_AccountCommon*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_AccountCommon*>::iterator itSrc2 = vecSrc2.begin();

	std::map<string,tagTWrap_Loong_AccountCommon*>::iterator itSrcFind;
	std::map<string,tagTWrap_Loong_AccountCommon*> mpAccountNameVsAccountCom;//���ڴ�����ͬ��account

	//src1 login account�� id�����
	DWORD dwSrc1LoginMaxNum = m_dwSrc1LoginMaxNum + 1;
	//�洢�������˺���
	string strAccountName = "";

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());
	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		m_mpAccountIDvsWareSize[(*itSrc1)->dwAccountID] = (*itSrc1)->stOthers.n16WareSize;
		strAccountName = Loong_CheckAccountName((*itSrc1)->szAccountName);
		mpAccountNameVsAccountCom[ strAccountName ] = (*itSrc1);

		m_vecOldSrc1AccountID.push_back((*itSrc1)->dwAccountID);
		//�ƽ�ȥ
		vecTar.push_back(*itSrc1);	
	}

	//��m_mpOldAccountVsNewAccount���
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		strAccountName = Loong_CheckAccountName((*itSrc2)->szAccountName);
		//��Դ2��name�� Դ1����
		itSrcFind = mpAccountNameVsAccountCom.find(strAccountName);
		//����ҵ� ������ �Ѳֿ�ϲ�
		if(itSrcFind != mpAccountNameVsAccountCom.end())
		{
			//��һ��Դ1�ڲֿ�� �� �� Ԫ�� size��bag psd �Լ�����ʱ�� 
			(itSrcFind->second)->stOthers.nBaiBaoYuanBao += (*itSrc2)->stOthers.nBaiBaoYuanBao;
			(itSrcFind->second)->stOthers.n64WareSilver += (*itSrc2)->stOthers.n64WareSilver;
	
			//sizeȡ���
			if((itSrcFind->second)->stOthers.n16WareSize < (*itSrc2)->stOthers.n16WareSize)
			{
				//�޸�Ŀ������
				(itSrcFind->second)->stOthers.n16WareSize = (*itSrc2)->stOthers.n16WareSize;
				//�޸Ķ��ձ�
				m_mpAccountIDvsWareSize[(itSrcFind->second)->dwAccountID] = (*itSrc2)->stOthers.n16WareSize;
			}

			//�������ľͲ������ ֱ��continue
			m_mpOldAccountVsNewAccount[tagAccountIDKey((*itSrc2)->dwAccountID,EM_DB_SRC_TWO)] \
				= (itSrcFind->second)->dwAccountID; //����Ϊ��src1ͬ�����Ǹ�account id

			m_mpOldSrc1VsSrc2[(itSrcFind->second)->dwAccountID] = (*itSrc2)->dwAccountID;

			continue;
		}

		//���û�ҵ�������
		//��������
		if(m_eMergeType == EM_MERGE_CROSS_SECTION)
		{
			//�����ź�
			m_mpOldAccountVsNewAccount[tagAccountIDKey((*itSrc2)->dwAccountID,EM_DB_SRC_TWO)] \
				= dwSrc1LoginMaxNum;
			m_vecOldSrc2OnlyAccountID.push_back((*itSrc2)->dwAccountID);

			(*itSrc2)->dwAccountID = dwSrc1LoginMaxNum;
			//accountid�뱳���Ķ��ձ�
			m_mpAccountIDvsWareSize[dwSrc1LoginMaxNum] = (*itSrc2)->stOthers.n16WareSize;
			++dwSrc1LoginMaxNum;
		}
		//ͬһ�������µ����
		else
		{
			//m_mpOldAccountVsNewAccount[tagAccountIDKey((*itSrc2)->dwAccountID,EM_DB_SRC_TWO)] \
			//	= (*itSrc2)->dwAccountID;

			m_vecOldSrc2OnlyAccountID.push_back((*itSrc2)->dwAccountID);

			//�ݴ�
			std::map<DWORD ,DWORD>::iterator itCheck;
			itCheck = m_mpAccountIDvsWareSize.find((*itSrc2)->dwAccountID);
			if(itCheck != m_mpAccountIDvsWareSize.end())
			{
				continue;
			}
			//accountid�뱳���Ķ��ձ�
			m_mpAccountIDvsWareSize[(*itSrc2)->dwAccountID] = (*itSrc2)->stOthers.n16WareSize;
		}

		vecTar.push_back(*itSrc2);	
	}
	
	return TRUE;
	
}
//role data
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_RoleData*> &vecSrc1,
											std::vector <tagTWrap_Loong_RoleData*> &vecSrc2,
											std::vector <tagTWrap_Loong_RoleData*> &vecTar)
{
	if(vecSrc1.size() == 0 && vecSrc2.size() == 0)
		return TRUE;

	//������role name��  _������
	//һ��Account�û�����5��roleʱ��ȡ�ȼ���ߵ�5��role~������role��removeflagΪ1
	//guildid����
	//��������
	std::vector <tagTWrap_Loong_RoleData*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_RoleData*>::iterator itSrc2 = vecSrc2.begin();

	std::map<tagAccountIDKey,DWORD>::iterator itSrcFind;

	//����Ŀǰ���ݿ���ڴ���������ͬ��RoleName���ܻ��Ӧͬһ��RoleNameCrc
	//Ϊ��������⣬�ڴ�������ʱ��ͬʱҪ�ȶ����ֺ�crc
	std::set<tstring>::iterator itNameFind;
	std::set <tstring> setRoleName;
	std::set<DWORD> setRoleNameCrc;

	//�Ȱ�role������������ ���嵽vecTar��

	TCHAR	szRoleName[X_SHORT_NAME] = {0};
	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		//name���
		_tcsncpy(szRoleName, (*itSrc1)->stOthers.RoleDataConst.szRoleName, X_SHORT_NAME);
		szRoleName[X_SHORT_NAME-1] = _T('\0');
		_tcslwr(szRoleName);
		setRoleName.insert(szRoleName);
		
		//crc���
		setRoleNameCrc.insert((*itSrc1)->stOthers.RoleDataConst.dwRoleNameCrc);

		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		//��������ظ�����
		_tcsncpy(szRoleName, (*itSrc2)->stOthers.RoleDataConst.szRoleName, X_SHORT_NAME);
		szRoleName[X_SHORT_NAME-1] = _T('\0');
		_tcslwr(szRoleName);

		//�Ƿ����
		BOOL bChangeName = FALSE;

		itNameFind = setRoleName.find(szRoleName);
		//����ҵ������� �����ֺ���������� ���ڲ������ظ��Ϸ���������ִ����AddName�����
		if(itNameFind != setRoleName.end())
		{
			tstring AddName;
			AddName += _T("_");
			AddName += m_strSrc2WorldName;

			//role���ֳ�����
			StrCat((*itSrc2)->stOthers.RoleDataConst.szRoleName,AddName.c_str());

			//������crc
			(*itSrc2)->stOthers.RoleDataConst.dwRoleNameCrc = \
				LowerCrc32((*itSrc2)->stOthers.RoleDataConst.szRoleName,X_SHORT_NAME);
		
			bChangeName = TRUE;
		}
		
		//���crc�ظ�����
		while(setRoleNameCrc.end() !=  setRoleNameCrc.find((*itSrc2)->stOthers.RoleDataConst.dwRoleNameCrc))
		{
			//���ֺ���Ӹ��»������¼���
			tstring AddName;
			AddName += _T("_");
			StrCat((*itSrc2)->stOthers.RoleDataConst.szRoleName,AddName.c_str());

			//������crc
			DWORD dwSrc2 = LowerCrc32((*itSrc2)->stOthers.RoleDataConst.szRoleName,X_SHORT_NAME);
			(*itSrc2)->stOthers.RoleDataConst.dwRoleNameCrc = dwSrc2;
			
			bChangeName = TRUE;
		}
		setRoleNameCrc.insert((*itSrc2)->stOthers.RoleDataConst.dwRoleNameCrc);

		//ֱ�Ӽ���Դ1������� ��ɾ��role
		Update_Loong_RoleID((*itSrc2)->dwRoleID);

		//���°���id
		if((*itSrc2)->stOthers.RoleDataSave.dwGuildID != GT_INVALID)
		{
			(*itSrc2)->stOthers.RoleDataSave.dwGuildID = \
				m_mpOldGuildIDvsNewGuildID[tagGuildIDKey((*itSrc2)->stOthers.RoleDataSave.dwGuildID,EM_DB_SRC_TWO)];
		}
		//����ǿ������
		if(m_eMergeType == EM_MERGE_CROSS_SECTION)
		{
			itSrcFind = m_mpOldAccountVsNewAccount.find(tagAccountIDKey((*itSrc2)->dwAccountID,EM_DB_SRC_TWO));
			//����ҵ�
			if(itSrcFind != m_mpOldAccountVsNewAccount.end())
			{
				//accountid
				(*itSrc2)->dwAccountID = (*itSrcFind).second;				
			}
		}
		vecTar.push_back(*itSrc2);

		//���¸����
		if(bChangeName == TRUE)
		{
			m_vecRoleCompensation.push_back(tagRoleCompensation(ERC_CHANGE_ROLE_NAME,(*itSrc2)->dwAccountID,(*itSrc2)->dwRoleID));
		}
	}
	
	//��account id����  level����
	std::sort(vecTar.begin(),vecTar.end(),Compare_Loong_RoleDataAcs);
	//����һ�³���5��role�����
	std::map<DWORD ,DWORD>::iterator itAccountFinder;
	std::vector <tagTWrap_Loong_RoleData*>::iterator itTar = vecTar.begin();
	WORD wRoleNum = 0;
	DWORD dwAccountIDPre = (*itTar)->dwAccountID; //��ʼ��Ϊ��һ��account
	ETBRoleRemoveType eRemoveType = EM_R_NULL;

	for(;itTar != vecTar.end(); ++itTar)
	{	
		//Դ������ɾ��
		if((*itTar)->bRemoveFlag == TRUE)
		{
			eRemoveType = EM_R_ONESELF;
		}
		else
		{
			//���
			eRemoveType = EM_R_NULL;
		}

		if((*itTar)->dwAccountID == dwAccountIDPre)
		{
			if(wRoleNum >= ROLE_MAX_LIMIT)
			{
				//��ʾ��roleΪ�ϲ�ʱ�Զ�ɾ��
				if(eRemoveType != EM_R_ONESELF)
					eRemoveType = EM_R_MERGE;

				(*itTar)->bRemoveFlag = TRUE;
			}
			//�ۼ��˺�����
			if((*itTar)->bRemoveFlag == FALSE)
				++wRoleNum;
	
			//����һ���˺��С���δ��ɾ���ġ����ȼ���ߵġ�roleid���ڸİ�
			if((*itTar)->bRemoveFlag == FALSE)
			{
				itAccountFinder = m_mpAccountIDvsRoleIDforCB.find((*itTar)->dwAccountID);
				if(itAccountFinder == m_mpAccountIDvsRoleIDforCB.end())
				{
					m_mpAccountIDvsRoleIDforCB.insert(make_pair((*itTar)->dwAccountID,(*itTar)->dwRoleID));
				}
			}

		}
		else
		{	
			//���л�accountʱ�������һ����ɫû�б�ɾ�� rolenum����Ϊ1 �������role��m_mpAccountIDvsRoleIDforCB��
			if((*itTar)->bRemoveFlag == FALSE)
			{
				wRoleNum = 1;
				m_mpAccountIDvsRoleIDforCB.insert(make_pair((*itTar)->dwAccountID,(*itTar)->dwRoleID));
			}
			else
			{
				wRoleNum = 0;
			}

			dwAccountIDPre = (*itTar)->dwAccountID;
		}

		//�µ�roleid����removetype�Ķ��ձ�		
		m_mpRoleIDvsRemoveType[(*itTar)->dwRoleID] = eRemoveType;
	}
	return TRUE;
}
//itembaibao
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_Item*> &vecSrc1,
											std::vector <tagTWrap_Loong_Item*> &vecSrc2,
											std::vector <tagTWrap_Loong_Item*> &vecTar)
{
	return Loong_LoongMerge((std::vector <tagTWrap_Loong_Item*> &)vecSrc1,
							(std::vector <tagTWrap_Loong_Item*> &)vecSrc2,
							(std::vector <tagTWrap_Loong_Item*> &)vecTar,
							EM_TB_ITEM);
}
//itembaibao
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_ItemBaiBao*> &vecSrc1,
											std::vector <tagTWrap_Loong_ItemBaiBao*> &vecSrc2,
											std::vector <tagTWrap_Loong_ItemBaiBao*> &vecTar)
{
	return Loong_LoongMerge((std::vector <tagTWrap_Loong_Item*> &)vecSrc1,
							(std::vector <tagTWrap_Loong_Item*> &)vecSrc2,
							(std::vector <tagTWrap_Loong_Item*> &)vecTar,
							EM_TB_ITEM_BAIBAO);
}
//itemDel
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_ItemDel*> &vecSrc1,
											std::vector <tagTWrap_Loong_ItemDel*> &vecSrc2,
											std::vector <tagTWrap_Loong_ItemDel*> &vecTar)
{
	return Loong_LoongMerge((std::vector <tagTWrap_Loong_Item*> &)vecSrc1,
							(std::vector <tagTWrap_Loong_Item*> &)vecSrc2,
							(std::vector <tagTWrap_Loong_Item*> &)vecTar,
							EM_TB_ITEM_DEL);
}

//item
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_Item*> &vecSrc1,
											std::vector <tagTWrap_Loong_Item*> &vecSrc2,
											std::vector <tagTWrap_Loong_Item*> &vecTar,
											ETBItemType emTBType)
{
	//item��serial num����
	//���������ϲ� ��Ҫ����OwnerID(RoleID����Ʒ) AccountID

	if(vecSrc1.size() == 0 && vecSrc2.size() == 0)
		return TRUE;

	//ContainerTypeID����
	std::vector <tagTWrap_Loong_Item*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_Item*>::iterator itSrc2 = vecSrc2.begin();

	std::map<tagAccountIDKey,DWORD>::iterator itSrcFind;
	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	//ȫ�������� �����ձ�
	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());

	//Դ2 ����ownerid��accountid
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_Item64SerialNum((*itSrc2)->stOthers.n64Serial);

		//�����gm���� �� �������
		if((*itSrc2)->stOthers.eCreateMode == EICM_GM
			|| (*itSrc2)->stOthers.eCreateMode == EICM_Product)
			Update_Loong_RoleID((*itSrc2)->stOthers.dwCreateID);

		//
		if((*itSrc2)->stOthers.eCreateMode == EICM_Product)
			Update_Loong_RoleID((*itSrc2)->stOthers.dwCreatorID);

		//ownerid
		Update_Loong_RoleID((*itSrc2)->stOthers.dwOwnerID);

		//����ǰ��ɲֿ���Ʒ
		if((*itSrc2)->stOthers.eConType == EICT_GuildWare)
		{
			//���°���id
			(*itSrc2)->stOthers.dwAccountID = \
				m_mpOldGuildIDvsNewGuildID[tagGuildIDKey((*itSrc2)->stOthers.dwAccountID,EM_DB_SRC_TWO)];
		}

		if(m_eMergeType == EM_MERGE_CROSS_SECTION)
		{

			if((*itSrc2)->stOthers.eConType != EICT_GuildWare)
			{
				itSrcFind = m_mpOldAccountVsNewAccount.find(tagAccountIDKey((*itSrc2)->stOthers.dwAccountID,EM_DB_SRC_TWO));
				//����ҵ�
				if(itSrcFind != m_mpOldAccountVsNewAccount.end())
				{
					//accountid
					(*itSrc2)->stOthers.dwAccountID = (*itSrcFind).second;				
				}
			}	
		}
		vecTar.push_back(*itSrc2);
	}
	//�����item_baibao���m_vecOverItem��Ķ����ŵ�vectar�� һ��д��db
	if(emTBType == EM_TB_ITEM_BAIBAO)
	{
		std::vector <tagTWrap_Loong_Item>::iterator itOver = m_vecOverItem.begin();
		for(;itOver != m_vecOverItem.end();++itOver)
			vecTar.push_back(&(*itOver));
	}
	//////////////////////////////////////////////////////////////////////////
	//����EICT_RoleWare EICT_Baibao  �ֿ� �ٱ���Ʒ��������

	//�������item�� ���账����Ʒ��������
	if(emTBType != EM_TB_ITEM)
		return TRUE;

	//vecTar ��AccountID RoleID����
	std::sort(vecTar.begin(),vecTar.end(),Compare_Loong_ItemAcs);
	//��� EICT_RoleWare EICT_Baibao ���͵�item �������24�� item_baibao���
	std::vector <tagTWrap_Loong_Item*>::iterator itTar = vecTar.begin();
	std::map <DWORD, ETBRoleRemoveType>::iterator itRoleIDvsRemoveType = m_mpRoleIDvsRemoveType.begin();	
	std::map <DWORD, DWORD>::iterator itAccountIDvsWareSize;

	DWORD dwWareItem = 0;
	DWORD dwBaibaoItem = 0;

	DWORD dwWarePos = 0; //λ��
	DWORD dwBaibaoPos = 0;

	DWORD dwAccountPre = (*itTar)->stOthers.dwAccountID; //��ʼ��Ϊ��һ��account
	for(;itTar != vecTar.end();)
	{
		if((*itTar)->stOthers.dwAccountID == dwAccountPre)
		{
			if((*itTar)->stOthers.eConType != EICT_RoleWare
				&&(*itTar)->stOthers.eConType != EICT_Baibao)
			{
				++itTar;
				continue;
			}
			itRoleIDvsRemoveType = m_mpRoleIDvsRemoveType.find((*itTar)->stOthers.dwOwnerID);
			if(itRoleIDvsRemoveType != m_mpRoleIDvsRemoveType.end())
			{
				//�����role�Ǻϲ�ʱ���ɵ�
				if((*itRoleIDvsRemoveType).second == EM_R_MERGE)
				{
					//��Ҫ������Ʒ Ŀǰ����Ʒȫ����ɾ������
					//�ǰ󶨵ġ�����ֿ⹻λ�ͷŲֿ⡡�����ӵ�item_baibao��
					//�󶨵�	���Խ��иİ�
					if((*itTar)->stOthers.byBind == EBS_Bind)
					{
						std::map <DWORD,DWORD>::iterator itFinder = m_mpAccountIDvsRoleIDforCB.find((*itTar)->stOthers.dwAccountID);
						if(itFinder != m_mpAccountIDvsRoleIDforCB.end())
						{
							(*itTar)->stOthers.dwOwnerID = itFinder->second;
						}
						//�����Ҳ�����account�д���δɾ���˺�
						else
						{
							//itTar = vecTar.erase(itTar);
							//continue;
						}
					}
				}
				//���������ɾ��role ����ʣ�µ��δ�󶨵���Ʒ Ҳ��������
				//�󶨵��ߵĻ���ֱ��ɾ��
				else if((*itRoleIDvsRemoveType).second == EM_R_ONESELF)
				{
					if((*itTar)->stOthers.byBind == EBS_Bind)
					{
						//itTar = vecTar.erase(itTar);					
						//continue;
					}
				}
				//
				if((*itTar)->stOthers.eConType == EICT_RoleWare)
				{
					itAccountIDvsWareSize = m_mpAccountIDvsWareSize.find( (*itTar)->stOthers.dwAccountID);
					if(itAccountIDvsWareSize == m_mpAccountIDvsWareSize.end())
					{
						tstringstream tszErr;
						tszErr << _T("accountid:") << (*itTar)->stOthers.dwAccountID << _T("ӵ�вֿ���Ʒ����account_common�����Ҳ�����Ӧ��¼��");
						g_clsOutput.Output(tszErr.str().c_str());
						itTar = vecTar.erase(itTar);
						continue;
					}
					//����Ѿ��ﵽ�����
					if( dwWareItem >= (*itAccountIDvsWareSize).second )
					{
						//���Լ��ӵ�m_vecOverItem��
						m_vecOverItem.push_back(*(*itTar));
						//��tar����ɾ����item ���ڴ����ָ�� �ٶ�Ӱ�첻��
						itTar = vecTar.erase(itTar);					
						continue;
					}
					else
					{
						//�����itemһ��λ��
						(*itTar)->stOthers.n16Index = dwWarePos;
						++dwWarePos;
						++dwWareItem;
					}
				}
				if((*itTar)->stOthers.eConType == EICT_Baibao)
				{
					//����Ѿ��ﵽ�����
					if(dwBaibaoItem >= SPACE_BAIBAO_BAG)
					{
						//���Լ��ӵ�m_vecOverItem��
						m_vecOverItem.push_back(*(*itTar));
						//��tar����ɾ����item
						itTar = vecTar.erase(itTar);					
						continue;
					}
					//�����itemһ��λ��
					else
					{
						//�����itemһ��λ��
						(*itTar)->stOthers.n16Index = dwBaibaoPos;
						++dwBaibaoItem;
						++dwBaibaoPos;
					}
				}
			}
			//��itemû��ӵ����
			else
			{}
		}
		//�л���account
		else
		{	
			dwWarePos = 0; //λ��
			dwBaibaoPos = 0;
			dwWareItem = 0;	
			dwBaibaoItem = 0;
			dwAccountPre = (*itTar)->stOthers.dwAccountID;

			//��ؼ�����������
			if((*itTar)->stOthers.eConType == EICT_RoleWare)
			{	
				(*itTar)->stOthers.n16Index = dwWarePos;
				dwWareItem = 1;
				++dwWarePos;
			}	
			if((*itTar)->stOthers.eConType == EICT_Baibao)
			{
				(*itTar)->stOthers.n16Index = dwBaibaoPos;
				dwBaibaoItem = 1;
				++dwBaibaoPos;
			}
		}
		//��һ����Ʒ
		++itTar;
	}
	return TRUE;
}
//buff
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_Buff*> &vecSrc1,
											std::vector <tagTWrap_Loong_Buff*> &vecSrc2,
											std::vector <tagTWrap_Loong_Buff*> &vecTar)
{
	//roleid srcunitid ItemSerialID
	std::vector <tagTWrap_Loong_Buff*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_Buff*>::iterator itSrc2 = vecSrc2.begin();

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());
	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_Item64SerialNum((*itSrc2)->stOthers.n64Serial);
		Update_Loong_RoleID((*itSrc2)->dwRoleID);
		Update_Loong_RoleID((*itSrc2)->stOthers.dwSrcUnitID);

		vecTar.push_back(*itSrc2);
	}
	return true;
}
//����
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_Guild*> &vecSrc1,
											std::vector <tagTWrap_Loong_Guild*> &vecSrc2,
											std::vector <tagTWrap_Loong_Guild*> &vecTar)
{
	//����guild id ��������Сд��32λcrc
	//����������� ������������׺����
	//���´�ʼ��nameid ���ھ���roleid
	//��������
	std::vector <tagTWrap_Loong_Guild*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_Guild*>::iterator itSrc2 = vecSrc2.begin();

	//
	std::set <tstring> setGuildName;
	std::set <tstring>::iterator itFind;

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		//���ȥ
		setGuildName.insert((*itSrc1)->stOthers.szName);
		m_setOldGuildID.insert((*itSrc1)->stOthers.dwID);

		vecTar.push_back(*itSrc1);
	}
	DWORD dwNewGuildID;
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		itFind = setGuildName.find((*itSrc2)->stOthers.szName);

		m_setOldGuildID.insert((*itSrc2)->stOthers.dwID);
		//ԭ����id
		dwNewGuildID = (*itSrc2)->stOthers.dwID;
		//����ҵ������� �����ֺ����������
		if(itFind != setGuildName.end())
		{
			tstring AddName;
			AddName += _T("_");
			AddName += m_strSrc2WorldName;

			//role���ֳ�����
			StrCat((*itSrc2)->stOthers.szName,AddName.c_str());

			//������crc
			TCHAR szName[MAX_GUILD_NAME_LEN] = {'\0'};
			_tcsncpy(szName, (*itSrc2)->stOthers.szName, MAX_GUILD_NAME_LEN);
			szName[MAX_GUILD_NAME_LEN-1] = _T('\0');
			_tcslwr(szName);
			dwNewGuildID = IUTIL->Crc32(szName);

		}
		//������Դ2id Ŀǰ��ɾ��role
		Update_Loong_RoleID((*itSrc2)->stOthers.dwFounderNameID);
		Update_Loong_RoleID((*itSrc2)->stOthers.dwLeaderRoleID);
		//�¾�id���ձ�
		m_mpOldGuildIDvsNewGuildID[tagGuildIDKey((*itSrc2)->stOthers.dwID,EM_DB_SRC_TWO)] = dwNewGuildID;
		(*itSrc2)->stOthers.dwID = dwNewGuildID;


		vecTar.push_back(*itSrc2);
	}
	//��������
	std::sort(vecTar.begin(),vecTar.end(),Compare_Loong_GuildLess);

	std::vector<tagTWrap_Loong_Guild*>::iterator itTar = vecTar.begin();

	DWORD dwRank = 1;
	for(;itTar != vecTar.end();++itTar)
	{
		//��֮ǰ�Ѿ������� ȫ������������
		if((*itTar)->stOthers.n16Rank != 0)
		{
			(*itTar)->stOthers.n16Rank = dwRank;
			++dwRank;
		}
	}


	return TRUE;
}
//BlackList
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_BlackList*> &vecSrc1,
											std::vector <tagTWrap_Loong_BlackList*> &vecSrc2,
											std::vector <tagTWrap_Loong_BlackList*> &vecTar)
{
	//roleid blackid
	std::vector <tagTWrap_Loong_BlackList*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_BlackList*>::iterator itSrc2 = vecSrc2.begin();

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());
	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());
	
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);
		Update_Loong_RoleID((*itSrc2)->dwBlackID);

		vecTar.push_back(*itSrc2);
	}
	return true;
}
//enemy
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_Enemy*> &vecSrc1,
											std::vector <tagTWrap_Loong_Enemy*> &vecSrc2,
											std::vector <tagTWrap_Loong_Enemy*> &vecTar)
{
	//roleid blackid
	std::vector <tagTWrap_Loong_Enemy*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_Enemy*>::iterator itSrc2 = vecSrc2.begin();

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());
	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());
	//for(;itSrc1!= vecSrc1.end();++itSrc1)
	//{
	//	vecTar.push_back(*itSrc1);
	//}
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);
		Update_Loong_RoleID((*itSrc2)->dwEnemyID);

		vecTar.push_back(*itSrc2);
	}
	return true;
}
//equip
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_Equip*> &vecSrc1,
											std::vector <tagTWrap_Loong_Equip*> &vecSrc2,
											std::vector <tagTWrap_Loong_Equip*> &vecTar)
{
	//�������
	std::vector <tagTWrap_Loong_Equip*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_Equip*>::iterator itSrc2 = vecSrc2.begin();

	//std::map<tagItemNumKey ,INT64>::iterator itFind;

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());
	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_Item64SerialNum((*itSrc2)->n64SerialNum);

		vecTar.push_back(*itSrc2);
	}
	return true;
}
//EquipDel
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_EquipDel*> &vecSrc1,
											std::vector <tagTWrap_Loong_EquipDel*> &vecSrc2,
											std::vector <tagTWrap_Loong_EquipDel*> &vecTar)
{
	return Loong_LoongMerge((std::vector <tagTWrap_Loong_Equip*> &)vecSrc1,
							(std::vector <tagTWrap_Loong_Equip*> &)vecSrc2,
							(std::vector <tagTWrap_Loong_Equip*> &)vecTar);
}
//EquipBaiBao
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_EquipBaiBao*> &vecSrc1,
											std::vector <tagTWrap_Loong_EquipBaiBao*> &vecSrc2,
											std::vector <tagTWrap_Loong_EquipBaiBao*> &vecTar)
{
	return Loong_LoongMerge((std::vector <tagTWrap_Loong_Equip*> &)vecSrc1,
							(std::vector <tagTWrap_Loong_Equip*> &)vecSrc2,
							(std::vector <tagTWrap_Loong_Equip*> &)vecTar);
}
//����
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_ClanData*> &vecSrc1,
											std::vector <tagTWrap_Loong_ClanData*> &vecSrc2,
											std::vector <tagTWrap_Loong_ClanData*> &vecTar)
{
	//�������
	std::vector <tagTWrap_Loong_ClanData*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_ClanData*>::iterator itSrc2 = vecSrc2.begin();

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());
	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);
		vecTar.push_back(*itSrc2);
	}
	return true;
}

//�䱦����
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_ActClanTreasure*> &vecSrc1,
											std::vector <tagTWrap_Loong_ActClanTreasure*> &vecSrc2,
											std::vector <tagTWrap_Loong_ActClanTreasure*> &vecTar)
{

	//���¼�����id
	std::vector <tagTWrap_Loong_ActClanTreasure*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_ActClanTreasure*>::iterator itSrc2 = vecSrc2.begin();

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	std::set<tagActClanTreasureKey> setPrimaryKey;
	std::set<tagActClanTreasureKey>::iterator itFind;

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		//��������
		setPrimaryKey.insert(tagActClanTreasureKey((*itSrc1)->byClanType,(*itSrc1)->u16TreasureID));

		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		//Ҫ��֤����䱦��Դ1��û�б������
		itFind = setPrimaryKey.find(tagActClanTreasureKey((*itSrc1)->byClanType,(*itSrc1)->u16TreasureID));
		if(itFind != setPrimaryKey.end())
		{
			Update_Loong_RoleID((*itSrc2)->dwRoleID);
			Update_Loong_RoleID((*itSrc2)->dwActNameID);
			vecTar.push_back(*itSrc2);
		}
	}
	return true;
}
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_FameHallEnterSnap*> &vecSrc1,
											 std::vector <tagTWrap_Loong_FameHallEnterSnap*> &vecSrc2,
											 std::vector <tagTWrap_Loong_FameHallEnterSnap*> &vecTar)
{
	//������id
	std::vector <tagTWrap_Loong_FameHallEnterSnap*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_FameHallEnterSnap*>::iterator itSrc2 = vecSrc2.begin();

	std::set<DWORD> setTime;
	std::set<DWORD>::iterator itFind;

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		setTime.insert((*itSrc1)->dwEnterTime);
		vecTar.push_back(*itSrc1);
	}
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);
		Update_Loong_RoleID((*itSrc2)->dwEnterNameID);

		//������ͬʱ��ĳ�ͻ
		while (setTime.find((*itSrc2)->dwEnterTime) != setTime.end())
		{
			(*itSrc2)->dwEnterTime = IncreaseTime((*itSrc2)->dwEnterTime,1);
		}
		setTime.insert((*itSrc2)->dwEnterTime);

		vecTar.push_back(*itSrc2);
	}
	return true;
}
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_RepRstTime*> &vecSrc1,
											 std::vector <tagTWrap_Loong_RepRstTime*> &vecSrc2,
											 std::vector <tagTWrap_Loong_RepRstTime*> &vecTar)
{
	//ȡ�뵱ǰʱ�����������
	std::vector <tagTWrap_Loong_RepRstTime*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_RepRstTime*>::iterator itSrc2 = vecSrc2.begin();

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	//��ʱ��
	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		for(;itSrc2!= vecSrc2.end();++itSrc2)
		{
			
			DWORD dwCur = GetCurrentDWORDTime();
			if((*itSrc2)->byClanType == (*itSrc1)->byClanType)
			{
				if(   CalcTimeDiff(dwCur,(*itSrc1)->dwRepRstTimeStamp) 
					> CalcTimeDiff(dwCur,(*itSrc2)->dwRepRstTimeStamp) )
				{
						vecTar.push_back(*itSrc2);
				}
				else
				{
						vecTar.push_back(*itSrc1);
				}
			}
			

		}
	}

	return true;
}
//friend
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_Friend*> &vecSrc1,
											std::vector <tagTWrap_Loong_Friend*> &vecSrc2,
											std::vector <tagTWrap_Loong_Friend*> &vecTar)
{

	//�������
	std::vector <tagTWrap_Loong_Friend*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_Friend*>::iterator itSrc2 = vecSrc2.begin();

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());
	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);
		Update_Loong_RoleID((*itSrc2)->stOthers.dwFriendID);
		vecTar.push_back(*itSrc2);
	}
	return true;
}
//friendship
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_FriendshipSave*> &vecSrc1,
											std::vector <tagTWrap_Loong_FriendshipSave*> &vecSrc2,
											std::vector <tagTWrap_Loong_FriendshipSave*> &vecTar)
{

	//�������
	std::vector <tagTWrap_Loong_FriendshipSave*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_FriendshipSave*>::iterator itSrc2 = vecSrc2.begin();

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());
	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);
		Update_Loong_RoleID((*itSrc2)->stOthers.dwFriendID);
		vecTar.push_back(*itSrc2);
	}
	return true;
}
//GroupPurchase
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_GroupPurchase*> &vecSrc1,
											std::vector <tagTWrap_Loong_GroupPurchase*> &vecSrc2,
											std::vector <tagTWrap_Loong_GroupPurchase*> &vecTar)
{

	//�������
	std::vector <tagTWrap_Loong_GroupPurchase*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_GroupPurchase*>::iterator itSrc2 = vecSrc2.begin();

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());
	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		(*itSrc2)->stOthers.dwGuildID \
			= m_mpOldGuildIDvsNewGuildID[tagGuildIDKey((*itSrc2)->stOthers.dwGuildID,EM_DB_SRC_TWO)];
		Update_Loong_RoleID((*itSrc2)->stOthers.dwRoleID);
		//�����б�
		if((*itSrc2)->stOthers.nParticipatorNum > 0)
		{
			DWORD dwTemp;
			for(int iLoop = 0; iLoop<(*itSrc2)->stOthers.nParticipatorNum;iLoop++)
			{
				dwTemp = (DWORD)((*itSrc2)->pcBlob + sizeof(DWORD)*iLoop);
				Update_Loong_RoleID(dwTemp);
				memcpy((*itSrc2)->pcBlob + sizeof(DWORD)*iLoop,&dwTemp,sizeof(DWORD));
			}
		}	
		vecTar.push_back(*itSrc2);
	}
	return true;
}
//GuildMem
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_GuildMem*> &vecSrc1,
											 std::vector <tagTWrap_Loong_GuildMem*> &vecSrc2,
											 std::vector <tagTWrap_Loong_GuildMem*> &vecTar)
{

	//����roleid guildid
	std::vector <tagTWrap_Loong_GuildMem*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_GuildMem*>::iterator itSrc2 = vecSrc2.begin();

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());
	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{

		Update_Loong_RoleID((*itSrc2)->stOthers.sGuildMember.dwRoleID);
		(*itSrc2)->stOthers.dwGuildID = \
				m_mpOldGuildIDvsNewGuildID[tagGuildIDKey((*itSrc2)->stOthers.dwGuildID,EM_DB_SRC_TWO)];

		vecTar.push_back(*itSrc2);
	}
	return true;
}
//item_cdtime
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_ItemCDTime*> &vecSrc1,
											 std::vector <tagTWrap_Loong_ItemCDTime*> &vecSrc2,
											 std::vector <tagTWrap_Loong_ItemCDTime*> &vecTar)
{
	std::vector <tagTWrap_Loong_ItemCDTime*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_ItemCDTime*>::iterator itSrc2 = vecSrc2.begin();

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());
	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);
		vecTar.push_back(*itSrc2);
	}
	return true;
}
//item_needlog
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_ItemNeedLog*> &vecSrc1,
											 std::vector <tagTWrap_Loong_ItemNeedLog*> &vecSrc2,
											 std::vector <tagTWrap_Loong_ItemNeedLog*> &vecTar)
{

	//��Ҫȥ�ظ�
	std::vector <tagTWrap_Loong_ItemNeedLog*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_ItemNeedLog*>::iterator itSrc2 = vecSrc2.begin();

	std::set<DWORD> setTypeID;
	std::set<DWORD>::iterator itFind;

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		setTypeID.insert((*itSrc1)->dwTypeID);
		vecTar.push_back(*itSrc1);
	}
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		itFind = setTypeID.find((*itSrc2)->dwTypeID);
		//���û�ҵ�
		if(itFind == setTypeID.end())
		{
			vecTar.push_back(*itSrc2);
		}
	}
	return true;
}
//log_baibao
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_LogBaiBao*> &vecSrc1,
											 std::vector <tagTWrap_Loong_LogBaiBao*> &vecSrc2,
											 std::vector <tagTWrap_Loong_LogBaiBao*> &vecTar)
{
	//��Ҫȥ�ظ�
	std::vector <tagTWrap_Loong_LogBaiBao*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_LogBaiBao*>::iterator itSrc2 = vecSrc2.begin();
	std::map<tagAccountIDKey,DWORD>::iterator itSrcFind;

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());
	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->stOthers.dwRoleID);
		//��������
		if(m_eMergeType == EM_MERGE_CROSS_SECTION)
		{
			itSrcFind = m_mpOldAccountVsNewAccount.find(tagAccountIDKey((*itSrc2)->dwAccountID,EM_DB_SRC_TWO));
			//����ҵ�
			if(itSrcFind != m_mpOldAccountVsNewAccount.end())
			{
				//accountid
				(*itSrc2)->dwAccountID = (*itSrcFind).second;				
			}
		}

		vecTar.push_back(*itSrc2);	
	}
	return true;
}
//RoleDel
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_RoleDel*> &vecSrc1,
											 std::vector <tagTWrap_Loong_RoleDel*> &vecSrc2,
											 std::vector <tagTWrap_Loong_RoleDel*> &vecTar)
{
	//��Ҫȥ�ظ�
	std::vector <tagTWrap_Loong_RoleDel*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_RoleDel*>::iterator itSrc2 = vecSrc2.begin();
	std::map<tagAccountIDKey,DWORD>::iterator itSrcFind;

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());
	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);
		//��������
		if(m_eMergeType == EM_MERGE_CROSS_SECTION)
		{
			itSrcFind = m_mpOldAccountVsNewAccount.find(tagAccountIDKey((*itSrc2)->dwAccountID,EM_DB_SRC_TWO));
			//����ҵ�
			if(itSrcFind != m_mpOldAccountVsNewAccount.end())
			{
				//accountid
				(*itSrc2)->dwAccountID = (*itSrcFind).second;				
			}
		}

		vecTar.push_back(*itSrc2);	
	}
	return true;
}
//pet_data
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_PetData*> &vecSrc1,
											 std::vector <tagTWrap_Loong_PetData*> &vecSrc2,
											 std::vector <tagTWrap_Loong_PetData*> &vecTar)
{
	//��Ҫȥ�ظ�
	std::vector <tagTWrap_Loong_PetData*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_PetData*>::iterator itSrc2 = vecSrc2.begin();

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		(*itSrc2)->dwPetID += (m_dwSrc1PetMaxNum - MIN_PET_ID);
		Update_Loong_RoleID((*itSrc2)->dwMasterID);
		vecTar.push_back(*itSrc2);	
	}
	return true;
}
//pet skill
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_PetSkill*> &vecSrc1,
											 std::vector <tagTWrap_Loong_PetSkill*> &vecSrc2,
											 std::vector <tagTWrap_Loong_PetSkill*> &vecTar)
{
	std::vector <tagTWrap_Loong_PetSkill*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_PetSkill*>::iterator itSrc2 = vecSrc2.begin();

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());
	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());
	
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		(*itSrc2)->dwPetID += (m_dwSrc1PetMaxNum - MIN_PET_ID);
		vecTar.push_back(*itSrc2);	
	}
	return true;
}
//skill
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_Skill*> &vecSrc1,
											 std::vector <tagTWrap_Loong_Skill*> &vecSrc2,
											 std::vector <tagTWrap_Loong_Skill*> &vecTar)
{
	//��Ҫȥ�ظ�
	std::vector <tagTWrap_Loong_Skill*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_Skill*>::iterator itSrc2 = vecSrc2.begin();

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());
	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);
		vecTar.push_back(*itSrc2);	
	}
	return true;
}
//task
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_Task*> &vecSrc1,
											 std::vector <tagTWrap_Loong_Task*> &vecSrc2,
											 std::vector <tagTWrap_Loong_Task*> &vecTar)
{
	//��Ҫȥ�ظ�
	std::vector <tagTWrap_Loong_Task*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_Task*>::iterator itSrc2 = vecSrc2.begin();

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());
	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);
		vecTar.push_back(*itSrc2);	
	}
	return true;
}
//taskdone
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_TaskDone*> &vecSrc1,
											 std::vector <tagTWrap_Loong_TaskDone*> &vecSrc2,
											 std::vector <tagTWrap_Loong_TaskDone*> &vecTar)
{
	//��Ҫȥ�ظ�
	std::vector <tagTWrap_Loong_TaskDone*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_TaskDone*>::iterator itSrc2 = vecSrc2.begin();

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);
		vecTar.push_back(*itSrc2);	
	}
	return true;
}
//title
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_Title*> &vecSrc1,
											 std::vector <tagTWrap_Loong_Title*> &vecSrc2,
											 std::vector <tagTWrap_Loong_Title*> &vecTar)
{
	//��Ҫȥ�ظ�
	std::vector <tagTWrap_Loong_Title*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_Title*>::iterator itSrc2 = vecSrc2.begin();

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());
	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);
		vecTar.push_back(*itSrc2);	
	}
	return true;
}
//vip_stall
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_VipStall*> &vecSrc1,
											 std::vector <tagTWrap_Loong_VipStall*> &vecSrc2,
											 std::vector <tagTWrap_Loong_VipStall*> &vecTar)
{
	//��Ҫȥ�ظ�
	std::vector <tagTWrap_Loong_VipStall*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_VipStall*>::iterator itSrc2 = vecSrc2.begin();

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());
	
	//byStallIndex ��0��ʼ
	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{

		vecTar.push_back(*itSrc1);
		if((*itSrc1)->byStallIndex >= m_dwSrc1VipStallMaxNum)
		{
			//������Ǵ�0��ʼ���
			m_dwSrc1VipStallMaxNum = (*itSrc1)->byStallIndex;
			++m_dwSrc1VipStallMaxNum;
		}
	}
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwOwnerID);

		if((*itSrc2)->byStallIndex != GT_INVALID)
			(*itSrc2)->byStallIndex += m_dwSrc1VipStallMaxNum;

		vecTar.push_back(*itSrc2);	
	}
	return true;
}
//VisitingCard
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_VisitingCard*> &vecSrc1,
											 std::vector <tagTWrap_Loong_VisitingCard*> &vecSrc2,
											 std::vector <tagTWrap_Loong_VisitingCard*> &vecTar)
{
	std::vector <tagTWrap_Loong_VisitingCard*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_VisitingCard*>::iterator itSrc2 = vecSrc2.begin();
	
	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());
	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());
	//for(;itSrc1!= vecSrc1.end();++itSrc1)
	//{
	//	vecTar.push_back(*itSrc1);
	//}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->stOthers.dwRoleID);
		//��ż
		if((*itSrc2)->stOthers.dwMateRoleID != GT_INVALID)
			Update_Loong_RoleID((*itSrc2)->stOthers.dwMateRoleID);
		//����
		if((*itSrc2)->stOthers.dwFactionID != GT_INVALID)
			(*itSrc2)->stOthers.dwFactionID = \
				m_mpOldGuildIDvsNewGuildID[tagGuildIDKey((*itSrc2)->stOthers.dwFactionID,EM_DB_SRC_TWO)];		

		vecTar.push_back(*itSrc2);	
	}
	return true;
}
//ybaccount
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_YBAccount*> &vecSrc1,
											 std::vector <tagTWrap_Loong_YBAccount*> &vecSrc2,
											 std::vector <tagTWrap_Loong_YBAccount*> &vecTar)
{
	std::vector <tagTWrap_Loong_YBAccount*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_YBAccount*>::iterator itSrc2 = vecSrc2.begin();

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());
	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());
	
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->dwRoleID);
		vecTar.push_back(*itSrc2);	
	}
	return true;
}
//yuanbaoorder
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_YuanBaoOrder*> &vecSrc1,
											 std::vector <tagTWrap_Loong_YuanBaoOrder*> &vecSrc2,
											 std::vector <tagTWrap_Loong_YuanBaoOrder*> &vecTar)
{
	
	std::vector <tagTWrap_Loong_YuanBaoOrder*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_YuanBaoOrder*>::iterator itSrc2 = vecSrc2.begin();
	
	DWORD dwMaxID = 0;


	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		vecTar.push_back(*itSrc1);

		if((*itSrc1)->dwID > dwMaxID)
		{
			dwMaxID = (*itSrc1)->dwID;
		}
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		++dwMaxID;
		//����
		(*itSrc2)->dwID = dwMaxID;
		//roleid
		Update_Loong_RoleID((*itSrc2)->dwRoleID);
		vecTar.push_back(*itSrc2);	
	}
	
	return true;
}
//bill item
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_BillItem*> &vecSrc1,
											 std::vector <tagTWrap_Loong_BillItem*> &vecSrc2,
											 std::vector <tagTWrap_Loong_BillItem*> &vecTar)
{
	
	std::vector <tagTWrap_Loong_BillItem*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_BillItem*>::iterator itSrc2 = vecSrc2.begin();
	std::map<tagAccountIDKey,DWORD>::iterator itSrcFind;

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());
	//for(;itSrc1!= vecSrc1.end();++itSrc1)
	//{
	//	vecTar.push_back(*itSrc1);
	//}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		//��������
		if(m_eMergeType == EM_MERGE_CROSS_SECTION)
		{
			itSrcFind = m_mpOldAccountVsNewAccount.find(tagAccountIDKey((*itSrc2)->dwAccountID,EM_DB_SRC_TWO));
			//����ҵ�
			if(itSrcFind != m_mpOldAccountVsNewAccount.end())
			{
				//accountid
				(*itSrc2)->dwAccountID = (*itSrcFind).second;				
			}
		}

		vecTar.push_back(*itSrc2);	
	}
	
	return true;
}
//bill yuanbao log
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_BillYuanBaoLog*> &vecSrc1,
											 std::vector <tagTWrap_Loong_BillYuanBaoLog*> &vecSrc2,
											 std::vector <tagTWrap_Loong_BillYuanBaoLog*> &vecTar)
{

	std::vector <tagTWrap_Loong_BillYuanBaoLog*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_BillYuanBaoLog*>::iterator itSrc2 = vecSrc2.begin();
	std::map<tagAccountIDKey,DWORD>::iterator itSrcFind;

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());
	//for(;itSrc1!= vecSrc1.end();++itSrc1)
	//{
	//	vecTar.push_back(*itSrc1);
	//}
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		//��������
		if(m_eMergeType == EM_MERGE_CROSS_SECTION)
		{
			itSrcFind = m_mpOldAccountVsNewAccount.find(tagAccountIDKey((*itSrc2)->dwAccountID,EM_DB_SRC_TWO));
			//����ҵ�
			if(itSrcFind != m_mpOldAccountVsNewAccount.end())
			{
				//accountid
				(*itSrc2)->dwAccountID = (*itSrcFind).second;				
			}
		}

		vecTar.push_back(*itSrc2);	
	}

	return true;
}
//bill yuanbao
BOOL  CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_BillYuanBao*> &vecSrc1,
											 std::vector <tagTWrap_Loong_BillYuanBao*> &vecSrc2,
											 std::vector <tagTWrap_Loong_BillYuanBao*> &vecTar)
{
	
	std::vector <tagTWrap_Loong_BillYuanBao*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_BillYuanBao*>::iterator itSrc2 = vecSrc2.begin();
	std::map<tagAccountIDKey,DWORD>::iterator itSrcFind;

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());
	//for(;itSrc1!= vecSrc1.end();++itSrc1)
	//{
	//	vecTar.push_back(*itSrc1);
	//}
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		//��������
		if(m_eMergeType == EM_MERGE_CROSS_SECTION)
		{
			itSrcFind = m_mpOldAccountVsNewAccount.find(tagAccountIDKey((*itSrc2)->dwAccountID,EM_DB_SRC_TWO));
			//����ҵ�
			if(itSrcFind != m_mpOldAccountVsNewAccount.end())
			{
				//accountid
				(*itSrc2)->dwAccountID = (*itSrcFind).second;				
			}
		}

		vecTar.push_back(*itSrc2);	
	}
	
	return true;
}
//guild_commodity
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_GuildCommodity*> &vecSrc1,
					  std::vector <tagTWrap_Loong_GuildCommodity*> &vecSrc2,
					  std::vector <tagTWrap_Loong_GuildCommodity*> &vecTar)
{
	std::vector <tagTWrap_Loong_GuildCommodity*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_GuildCommodity*>::iterator itSrc2 = vecSrc2.begin();
	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->stOthers.dwRoleID);
		(*itSrc2)->dwGuildID = m_mpOldGuildIDvsNewGuildID[tagGuildIDKey((*itSrc2)->dwGuildID,EM_DB_SRC_TWO)];

		vecTar.push_back(*itSrc2);	
	}

	return true;
}
//guild_skill
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_GuildSkill*> &vecSrc1,
					  std::vector <tagTWrap_Loong_GuildSkill*> &vecSrc2,
					  std::vector <tagTWrap_Loong_GuildSkill*> &vecTar)
{
	std::vector <tagTWrap_Loong_GuildSkill*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_GuildSkill*>::iterator itSrc2 = vecSrc2.begin();
	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		(*itSrc2)->dwGuildID = m_mpOldGuildIDvsNewGuildID[tagGuildIDKey((*itSrc2)->dwGuildID,EM_DB_SRC_TWO)];

		vecTar.push_back(*itSrc2);	
	}

	return true;

}
//vip_netbar
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_VipNetBar*> &vecSrc1,
					  std::vector <tagTWrap_Loong_VipNetBar*> &vecSrc2,
					  std::vector <tagTWrap_Loong_VipNetBar*> &vecTar)
{
	std::vector <tagTWrap_Loong_VipNetBar*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_VipNetBar*>::iterator itSrc2 = vecSrc2.begin();
	std::map<tagAccountIDKey,DWORD>::iterator itSrcFind;

	std::set<DWORD> setAccountID;
	std::set<DWORD>::iterator itAccountFind;

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		setAccountID.insert((*itSrc1)->dwAccountID);
		vecTar.push_back(*itSrc1);	
	}
	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		//����ǿ������
		if(m_eMergeType == EM_MERGE_CROSS_SECTION)
		{
			itSrcFind = m_mpOldAccountVsNewAccount.find(tagAccountIDKey((*itSrc2)->dwAccountID,EM_DB_SRC_TWO));
			//����ҵ�
			if(itSrcFind != m_mpOldAccountVsNewAccount.end())
			{		
				//����һ��id
				(*itSrc2)->dwAccountID = itSrcFind->second;			
			}
		}

		//����ҵ��ظ��ĺ�
		itAccountFind = setAccountID.find((*itSrc2)->dwAccountID);
		if(itAccountFind != setAccountID.end())
		{
			continue;
		}


		vecTar.push_back(*itSrc2);	
	}

	return true;
}
//guild_upgrade
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_GuildFacilities*> &vecSrc1,
					  std::vector <tagTWrap_Loong_GuildFacilities*> &vecSrc2,
					  std::vector <tagTWrap_Loong_GuildFacilities*> &vecTar)
{
	std::vector <tagTWrap_Loong_GuildFacilities*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_GuildFacilities*>::iterator itSrc2 = vecSrc2.begin();

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		(*itSrc2)->dwGuildID = m_mpOldGuildIDvsNewGuildID[tagGuildIDKey((*itSrc2)->dwGuildID,EM_DB_SRC_TWO)];
		vecTar.push_back(*itSrc2);	
	}

	return true;
}
//commerce_rank
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_CommerceRank*> &vecSrc1,
					  std::vector <tagTWrap_Loong_CommerceRank*> &vecSrc2,
					  std::vector <tagTWrap_Loong_CommerceRank*> &vecTar)
{
	std::vector <tagTWrap_Loong_CommerceRank*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_CommerceRank*>::iterator itSrc2 = vecSrc2.begin();
	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	vecTar.insert(vecTar.end(),vecSrc1.begin(),vecSrc1.end());

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		Update_Loong_RoleID((*itSrc2)->stOthers.dwRoleID);
		(*itSrc2)->dwGuildID = m_mpOldGuildIDvsNewGuildID[tagGuildIDKey((*itSrc2)->dwGuildID,EM_DB_SRC_TWO)];
		vecTar.push_back(*itSrc2);	
	}

	return true;
}
//activity
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_ActivityData*> &vecSrc1,
					  std::vector <tagTWrap_Loong_ActivityData*> &vecSrc2,
					  std::vector <tagTWrap_Loong_ActivityData*> &vecTar)
{
	std::vector <tagTWrap_Loong_ActivityData*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_ActivityData*>::iterator itSrc2 = vecSrc2.begin();
	std::set <DWORD> setScriptID;
	std::set <DWORD>::iterator itFinder;
	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	//���ݽű�idȥ�ظ�


	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		setScriptID.insert((*itSrc1)->dwID);
		vecTar.push_back(*itSrc1);	
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		itFinder = setScriptID.find((*itSrc2)->dwID);
		if(itFinder != setScriptID.end())
		{
			continue;
		}
		vecTar.push_back(*itSrc2);	
	}

	return true;
}

//left msg
BOOL CLoongMergeLogicCtrl::Loong_LoongMerge(std::vector <tagTWrap_Loong_LeftMsg*> &vecSrc1,
											std::vector <tagTWrap_Loong_LeftMsg*> &vecSrc2,
											std::vector <tagTWrap_Loong_LeftMsg*> &vecTar)
{
	std::vector <tagTWrap_Loong_LeftMsg*>::iterator itSrc1 = vecSrc1.begin();
	std::vector <tagTWrap_Loong_LeftMsg*>::iterator itSrc2 = vecSrc2.begin();

	std::set<DWORD> setTime;
	std::set<DWORD>::iterator itFind;

	//Ԥ���ռ�
	vecTar.reserve(vecSrc1.size() + vecSrc2.size());

	for(;itSrc1!= vecSrc1.end();++itSrc1)
	{
		setTime.insert((*itSrc1)->dwMsgID);
		vecTar.push_back(*itSrc1);
	}

	for(;itSrc2!= vecSrc2.end();++itSrc2)
	{
		//������ͬʱ��ĳ�ͻ
		while (setTime.find((*itSrc2)->dwMsgID) != setTime.end())
		{
			(*itSrc2)->dwMsgID = IncreaseTime((*itSrc2)->dwMsgID,1);
		}
		setTime.insert((*itSrc2)->dwMsgID);

		Update_Loong_RoleID((*itSrc2)->dwRoleID);
		vecTar.push_back(*itSrc2);	
	}

	return true;
}