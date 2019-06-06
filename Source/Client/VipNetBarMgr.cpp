#include "stdafx.h"
#include "VipNetBarMgr.h"
#include "..\WorldDefine\msg_vip_netbar.h"
#include "RoleMgr.h"


VipNetBarMgr::VipNetBarMgr() :
m_trunk( this )
{
}

VipNetBarMgr::~VipNetBarMgr()
{
}

void VipNetBarMgr::Init()
{
	m_pSession = TObjRef<NetSession>();
	m_pCmdMgr = TObjRef<NetCmdMgr>();

	//m_pCmdMgr->Register("NS_GetVipNetBarName",		(NETMSGPROC)m_trunk.sfp2(&VipNetBarMgr::OnNS_GetVipNetBarName),	_T("NS_GetVipNetBarName"));
	//m_pCmdMgr->Register("NS_VipNetBarGift",			(NETMSGPROC)m_trunk.sfp2(&VipNetBarMgr::OnNS_VipNetBarGift),	_T("NS_VipNetBarGift"));

	m_paReqRole.first = 0;
	m_paReqRole.second.clear();
}


void VipNetBarMgr::Destroy()
{
	//m_pCmdMgr->UnRegister("NS_GetVipNetBarName",	 (NETMSGPROC)m_trunk.sfp2(&VipNetBarMgr::OnNS_GetVipNetBarName));
	//m_pCmdMgr->UnRegister("NS_VipNetBarGift",		 (NETMSGPROC)m_trunk.sfp2(&VipNetBarMgr::OnNS_VipNetBarGift));

	m_mapNetBarName.clear();

	if( P_VALID(GetObj("VipNetBarGiftMsgBox")) )
		KillObj("VipNetBarGiftMsgBox");

	m_paReqRole.first = 0;
	m_paReqRole.second.clear();
}

DWORD VipNetBarMgr::OnNS_GetVipNetBarName( tagNS_GetVipNetBarName* pCmd, DWORD )
{
	for( INT i=0; i<pCmd->nNum; ++i )
	{
		m_mapNetBarName[pCmd->name[i].dwRoleID] = pCmd->name[i].szNetBarName;
	}

	return 0;
}

tstring VipNetBarMgr::GetVipNetBarName( DWORD dwRoleID ) 
{
	//std::map<DWORD,tstring>::const_iterator itr = m_mapNetBarName.find( dwRoleID );
	//if( itr != m_mapNetBarName.end() )
	//	return itr->second;
	//else
	//{
	//	//--��������б��в����ڣ������
	//	bool bFind=false;
	//	for(list<DWORD>::iterator iter=m_paReqRole.second.begin();
	//		iter!=m_paReqRole.second.end();++iter)
	//	{
	//		if(*iter==dwRoleID)
	//		{
	//			bFind=true;
	//			break;
	//		}
	//	}
	//	if(!bFind)
	//	{
	//		m_paReqRole.second.push_back(dwRoleID);
	//	}
	//}
	return _T("");
}

DWORD VipNetBarMgr::OnNS_VipNetBarGift( tagNS_VipNetBarGift* pCmd, DWORD )
{
	//if( P_VALID(GetObj("VipNetBarGiftMsgBox")) )
	//	KillObj("VipNetBarGiftMsgBox");

	//CreateObj("VipNetBarGiftMsgBox", "MsgBox");
	//TObjRef<MsgBox>("VipNetBarGiftMsgBox")->Init(_T(""), g_StrTable[_T("VipNetBarGift")], NULL, (MsgBoxFlag)(MBF_OK), FALSE);

	return 0;
}

void VipNetBarMgr::Update()
{
	//if( !m_paReqRole.second.empty() && 500 < Kernel::Inst()->GetAccumTimeDW() - m_paReqRole.first )
	//{
	//	tagNC_GetVipNetBarName NetCmdData;
	//	NetCmdData.nNum = m_paReqRole.second.size() > 50 ? 50 : m_paReqRole.second.size();
	//	// �ṹ�Ŀռ��ȥ����ָ���dwAllID[1]�Ŀռ��ټ���ʵ����SIZE����С��DWORD�ռ�����½ṹ�Ĵ�С
	//	int size = sizeof(tagNC_GetVipNetBarName) - sizeof(DWORD) + sizeof(DWORD) * NetCmdData.nNum;
	//	// ���ٸĴ�С������
	//	BYTE byAllocData[sizeof(tagNC_GetVipNetBarName) - sizeof(DWORD) + sizeof(DWORD) * 50];
	//	tagNC_GetVipNetBarName *pRGSCmd = (tagNC_GetVipNetBarName*)byAllocData;

	//	// ��������ָ��֮ǰ����������,��Ϊ��POD��,�ǰ�ȫ��.
	//	memcpy(pRGSCmd, &NetCmdData, (sizeof(NetCmdData)));
	//	// ID��ֵ��
	//	for (INT i = 0; i < NetCmdData.nNum; ++i)
	//	{
	//		pRGSCmd->dwAllID[i] = m_paReqRole.second.front();
	//		m_paReqRole.second.pop_front();
	//	}
	//	// �����µĴ�С
	//	pRGSCmd->dwSize = size;
	//	m_pSession->Send(pRGSCmd);
	//	m_paReqRole.first = Kernel::Inst()->GetAccumTimeDW();
	//}
}