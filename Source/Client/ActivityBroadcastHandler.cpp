#include "stdafx.h"
#include "ActivityBroadcastHandler.h"
#include "CreatureData.h"
#include "MapLogicData.h"
#include "MapMgr.h"
#include "PlayerNameTab.h"

ActivityBroadcastHandler::ActivityBroadcastHandler()
	: m_bHandlingNow(false)
	, m_msgID(-1)
	, m_pSession(NULL)
	, m_trunk(this)
	, m_pNetCmdMgr(NULL)
	, m_pGameFrameMgr(NULL)
{

}

ActivityBroadcastHandler* ActivityBroadcastHandler::Inst()
{
	static ActivityBroadcastHandler inst;
	return &inst;
}

void ActivityBroadcastHandler::Init()
{
	InitMsgID2BroadcastImplMap();
	m_pNetCmdMgr->Register("NS_MsgBlob", 
		reinterpret_cast<NETMSGPROC>(m_trunk.sfp2(&ActivityBroadcastHandler::OnNS_MsgBlob)), 
		_T("NS_MsgBlob"));

	m_pGameFrameMgr->RegisterEventHandle(_T("GetSomeNameOK"), 
		reinterpret_cast<FRAMEEVENTPROC>(m_trunk.sfp1(&ActivityBroadcastHandler::OnGetSomeNameOKEvent)));
	
}

void ActivityBroadcastHandler::Destroy()
{
	m_pNetCmdMgr->UnRegister("NS_MsgBlob", 
		reinterpret_cast<NETMSGPROC>(m_trunk.sfp2(&ActivityBroadcastHandler::OnNS_MsgBlob)));

	m_pGameFrameMgr->UnRegisterEventHandler(_T("GetSomeNameOK"), 
		reinterpret_cast<FRAMEEVENTPROC>(m_trunk.sfp1(&ActivityBroadcastHandler::OnGetSomeNameOKEvent)));
}




ActivityBroadcastHandler::~ActivityBroadcastHandler()
{
}

DWORD ActivityBroadcastHandler::OnNS_MsgBlob(tagNS_MsgBlob *pCmd, DWORD)
{
	// �����һ����Ϣ��û�д������,���������������
	if (m_bHandlingNow)
	{
		BYTE *pAllocData = new BYTE[pCmd->dwSize];
		memcpy(pAllocData, pCmd, pCmd->dwSize);
		tagNS_MsgBlob *pNetCmd = reinterpret_cast<tagNS_MsgBlob*>(pAllocData);
		m_MsgList.push_back(pNetCmd);
	}
	else
	{
		HandleNetMsg(pCmd);
	}

	return 0;
}

void ActivityBroadcastHandler::HandleNetMsg(tagNS_MsgBlob *pCmd)
{
	m_bHandlingNow = true;	// ���Ϊ���ڴ���״̬

	DWORD dwSize = pCmd->dwSize - sizeof(tagNS_MsgBlob) + sizeof(BYTE);
	MsgInfo::ChangeBlob2MsgInfo(pCmd->byData, dwSize, m_vecMsgUnit);

	std::vector<MsgUnit*>::iterator iter = m_vecMsgUnit.begin();
	std::vector<MsgUnit*>::iterator end = m_vecMsgUnit.end();
	// ���ͷ����EMUT_MsgID�Ļ�..
	if (iter != end)
	{
		if ((*iter)->GetMsgUnitType() == EMUT_MsgID)
		{
			m_msgID = *(static_cast<int*>((*iter)->GetData()));
		}
		else
		{
			m_bHandlingNow = false;
			return;
		}
	}
	else
	{
		m_bHandlingNow = false;
		return;
	}

	for (; iter != end; ++iter)
	{
		if ((*iter)->GetMsgUnitType() == EMUT_RoleID)
		{
			DWORD dwRoleID = *(static_cast<DWORD*>((*iter)->GetData()));
			m_ReqIDList.push_back(dwRoleID);
		}
	}
	// ���������б�Ϊ��
	if (!m_ReqIDList.empty())
	{
		std::vector<DWORD> cantfind;
		// �����ܲ��ܴ�playernametab���ҵ����е�����
		bool bRet = PlayerNameTab::Inst()->CanGetBundleOfNames(m_ReqIDList, cantfind);
		// ������ҵ�
		if (bRet)
		{
			StartBroadcast();
		}
		// �������ȫ���ҵ�
		else
		{
			m_ReqIDList.clear();
			// �����б��Ϊ����ȡ�����ֵ��б�
			m_ReqIDList.assign(cantfind.begin(), cantfind.end());
		}
	}
	// ����Ҫ��������,ֱ�ӹ㲥
	else
	{
		StartBroadcast();
	}
}

void ActivityBroadcastHandler::StartBroadcast()
{
	LPCTSTR szMsg = m_pGSMgr->GetMsg(m_msgID);
	if( szMsg == NULL || _tcslen(szMsg)==0 )
	{
		return;
	}
	const TCHAR *szBroadcastContent = g_StrTable[szMsg];
	std::map<int, BroadcastImplPtr>::iterator iter = m_mapMsgID2BroadcastImpl.find(m_msgID);
	if (iter != m_mapMsgID2BroadcastImpl.end())
	{
		(this->*(iter->second))(szBroadcastContent);
	}
	
	// �㲥֮�����һЩ����
	ReleaseMsgUnit();
	m_msgID = -1;
	m_bHandlingNow = false;		// ���Ϊ�������״̬.
	m_ReqIDList.clear();
}

tstring ActivityBroadcastHandler::GetRoleName(DWORD dwRoleID) const
{
	return PlayerNameTab::Inst()->FindNameByID(dwRoleID);
}

void ActivityBroadcastHandler::BroadcastImpl1(const TCHAR *szBroadcastContent)
{
	tstring strNames(_T(""));
	tstring strCreatureName(_T(""));
	std::vector<MsgUnit*>::iterator xiter = m_vecMsgUnit.begin();
	std::vector<MsgUnit*>::iterator xend = m_vecMsgUnit.end();
	for (; xiter != xend; ++xiter)
	{
		if ((*xiter)->GetMsgUnitType() == EMUT_RoleID)
		{
			DWORD dwRoleID = *(static_cast<DWORD*>((*xiter)->GetData()));
			tstring strName = GetRoleName(dwRoleID);
			strNames += strName;
			strNames += _T("��");
		}
		if ((*xiter)->GetMsgUnitType() == EMUT_NPCTypeID)
		{
			DWORD dwNpcID = *(static_cast<DWORD*>((*xiter)->GetData()));
			const tagCreatureProto *pCP = CreatureData::Inst()->FindNpcAtt(dwNpcID);
			if (pCP != NULL)
			{
				strCreatureName = pCP->szName;
			}
		}
	}

	//�Ƴ����һ���ٺ�
	tstring strNewNames = strNames.substr(0, strNames.size() - 1);
	TCHAR szBuffer[100];
	_sntprintf(szBuffer, 100, szBroadcastContent, strNewNames.c_str(), strCreatureName.c_str());

	NotifyBroadcast(szBuffer);
}

void ActivityBroadcastHandler::BroadcastImpl2(const TCHAR *szBroadcastContent)
{
	NotifyBroadcast(szBroadcastContent);	
}


void ActivityBroadcastHandler::BroadcastImpl3(const TCHAR *szBroadcastContent)
{
	tstring strMapShowName(_T(""));
	DWORD dwMapCrc = *(static_cast<DWORD*>(m_vecMsgUnit[1]->GetData()));
	const tagMapLogicData *pMapLogicData = NULL;
	pMapLogicData = MapLogicData::Inst()->FindMapLogicData(dwMapCrc);
	if (pMapLogicData)
		strMapShowName = pMapLogicData->szShowName;

	TCHAR szBuffer[100];
	_sntprintf(szBuffer, 100, szBroadcastContent, strMapShowName.c_str());
	NotifyBroadcast(szBuffer);
}

void ActivityBroadcastHandler::BroadcastImpl4(const TCHAR *szBroadcastContent)
{
	int pos1 = *(static_cast<int*>((m_vecMsgUnit[1])->GetData()));
	int pos2 = *(static_cast<int*>((m_vecMsgUnit[2])->GetData()));
	TCHAR szBuffer[100];
	_sntprintf(szBuffer, 100, szBroadcastContent, pos1, pos2);
	NotifyBroadcast(szBuffer);
}

void ActivityBroadcastHandler::BroadcastImpl5(const TCHAR *szBroadcastContent)
{
	NotifyBroadcast(szBroadcastContent);
}

void ActivityBroadcastHandler::BroadcastImpl10(const TCHAR *szBroadcastContent)
{
	int pos1 = *(static_cast<int*>((m_vecMsgUnit[1])->GetData()));
	int pos2 = *(static_cast<int*>((m_vecMsgUnit[2])->GetData()));
	TCHAR szBuffer[100];
	_sntprintf(szBuffer, 100, szBroadcastContent, pos1, pos2);
	NotifyBroadcast(szBuffer); 
}

void ActivityBroadcastHandler::BroadcastImpl11(const TCHAR *szBroadcastContent)
{
	tstring strNames(_T(""));
	std::vector<MsgUnit*>::iterator xiter = m_vecMsgUnit.begin();
	std::vector<MsgUnit*>::iterator xend = m_vecMsgUnit.end();
	for (; xiter != xend; ++xiter)
	{
		if ((*xiter)->GetMsgUnitType() == EMUT_RoleID)
		{
			DWORD dwRoleID = *(static_cast<DWORD*>((*xiter)->GetData()));
			tstring strName = GetRoleName(dwRoleID);
			strNames += strName;
			strNames += _T("��");
		}
	}

	//�Ƴ����һ���ٺ�
	tstring strNewNames = strNames.substr(0, strNames.size() - 1);
	TCHAR szBuffer[100];
	_sntprintf(szBuffer, 100, szBroadcastContent, strNewNames.c_str());

	NotifyBroadcast(szBuffer);
}
void ActivityBroadcastHandler::BroadcastImpl12(const TCHAR *szBroadcastContent)
{
	int nNum = *(static_cast<int*>((m_vecMsgUnit[1])->GetData()));
	TCHAR szBuffer[100];
	_sntprintf(szBuffer, 100, szBroadcastContent, nNum);
	NotifyBroadcast(szBuffer); 
}
void ActivityBroadcastHandler::BroadcastImpl13(const TCHAR *szBroadcastContent)
{
	NotifyBroadcast(szBroadcastContent);
}
void ActivityBroadcastHandler::BroadcastImpl14(const TCHAR *szBroadcastContent)
{
	NotifyBroadcast(szBroadcastContent);
}
void ActivityBroadcastHandler::BroadcastImpl15(const TCHAR *szBroadcastContent)
{
	NotifyBroadcast(szBroadcastContent);
}
void ActivityBroadcastHandler::BroadcastImpl16(const TCHAR *szBroadcastContent)
{
	int pos1 = *(static_cast<int*>((m_vecMsgUnit[1])->GetData()));
	int pos2 = *(static_cast<int*>((m_vecMsgUnit[2])->GetData()));
	TCHAR szBuffer[100];
	_sntprintf(szBuffer, 100, szBroadcastContent, pos1, pos2);
	NotifyBroadcast(szBuffer); 
}
void ActivityBroadcastHandler::BroadcastImpl17(const TCHAR *szBroadcastContent)
{	
	DWORD dwTriggerID1 = *(static_cast<DWORD*>((m_vecMsgUnit[1])->GetData()));
	DWORD dwTriggerID2 = *(static_cast<DWORD*>((m_vecMsgUnit[2])->GetData()));
	DWORD dwTriggerID3 = *(static_cast<DWORD*>((m_vecMsgUnit[3])->GetData()));

	Vector3 pos1, pos2, pos3;
	BOOL bRet = MapMgr::Inst()->GetTriggerPos(dwTriggerID1, pos1);
	ASSERT(bRet);
	bRet = MapMgr::Inst()->GetTriggerPos(dwTriggerID2, pos2);
	ASSERT(bRet);
	bRet = MapMgr::Inst()->GetTriggerPos(dwTriggerID3, pos3);
	ASSERT(bRet);

	TCHAR szBuffer[100];
	_sntprintf(szBuffer, 100, szBroadcastContent, (int)(pos1.x / 50), (int)(pos1.z / 50), (int)(pos2.x / 50), (int)(pos2.z / 50), (int)(pos3.x / 50), (int)(pos3.z / 50));
	NotifyBroadcast(szBuffer); 
}

void ActivityBroadcastHandler::BroadcastImpl30(const TCHAR *szBroadcastContent)
{
	NotifyBroadcast(szBroadcastContent);
}

void ActivityBroadcastHandler::BroadcastImpl31(const TCHAR *szBroadcastContent)
{
	tstring strNames(_T(""));
	tstring strCreatureName(_T(""));
	std::vector<MsgUnit*>::iterator xiter = m_vecMsgUnit.begin();
	std::vector<MsgUnit*>::iterator xend = m_vecMsgUnit.end();
	for (; xiter != xend; ++xiter)
	{
		if ((*xiter)->GetMsgUnitType() == EMUT_RoleID)
		{
			DWORD dwRoleID = *(static_cast<DWORD*>((*xiter)->GetData()));
			tstring strName = GetRoleName(dwRoleID);
			strNames += strName;
			strNames += _T("��");
		}
		if ((*xiter)->GetMsgUnitType() == EMUT_NPCTypeID)
		{
			DWORD dwNpcID = *(static_cast<DWORD*>((*xiter)->GetData()));
			const tagCreatureProto *pCP = CreatureData::Inst()->FindNpcAtt(dwNpcID);
			if (pCP != NULL)
			{
				strCreatureName = pCP->szName;
			}
		}
	}

	//�Ƴ����һ���ٺ�
	tstring strNewNames = strNames.substr(0, strNames.size() - 1);
	TCHAR szBuffer[100];
	_sntprintf(szBuffer, 100, szBroadcastContent, strNewNames.c_str(), strCreatureName.c_str());

	NotifyBroadcast(szBuffer);
}

void ActivityBroadcastHandler::BroadcastImpl35(const TCHAR *szBroadcastContent)
{
	NotifyBroadcast(szBroadcastContent);
}


void ActivityBroadcastHandler::ReleaseMsgUnit()
{
	MsgInfo::ReleaseMsgInfo(m_vecMsgUnit);
}

DWORD ActivityBroadcastHandler::OnGetSomeNameOKEvent(tagRoleGetSomeNameEvent *pEvent)
{
	// �жϻ�õ������Ƿ��������������
	if (pEvent->IDs == m_ReqIDList)
	{
		StartBroadcast();
		// ������治Ϊ�գ�ȡ����һ����ʼ����..
		if (!m_MsgList.empty())
		{
			tagNS_MsgBlob *pCacheCmd = m_MsgList.front();
			m_MsgList.pop_front();
			HandleNetMsg(pCacheCmd);
			SAFE_DELETE (pCacheCmd);
		}
	}
	return 0;
}

void ActivityBroadcastHandler::InitMsgID2BroadcastImplMap()
{
	m_mapMsgID2BroadcastImpl.insert(make_pair(1, &ActivityBroadcastHandler::BroadcastImpl1));
	m_mapMsgID2BroadcastImpl.insert(make_pair(2, &ActivityBroadcastHandler::BroadcastImpl2));
	m_mapMsgID2BroadcastImpl.insert(make_pair(3, &ActivityBroadcastHandler::BroadcastImpl3));
	m_mapMsgID2BroadcastImpl.insert(make_pair(4, &ActivityBroadcastHandler::BroadcastImpl4));
	m_mapMsgID2BroadcastImpl.insert(make_pair(5, &ActivityBroadcastHandler::BroadcastImpl5));
	m_mapMsgID2BroadcastImpl.insert(make_pair(10, &ActivityBroadcastHandler::BroadcastImpl10));
	m_mapMsgID2BroadcastImpl.insert(make_pair(11, &ActivityBroadcastHandler::BroadcastImpl11));
	m_mapMsgID2BroadcastImpl.insert(make_pair(12, &ActivityBroadcastHandler::BroadcastImpl12));
	m_mapMsgID2BroadcastImpl.insert(make_pair(13, &ActivityBroadcastHandler::BroadcastImpl13));
	m_mapMsgID2BroadcastImpl.insert(make_pair(14, &ActivityBroadcastHandler::BroadcastImpl14));
	m_mapMsgID2BroadcastImpl.insert(make_pair(15, &ActivityBroadcastHandler::BroadcastImpl15));
	m_mapMsgID2BroadcastImpl.insert(make_pair(16, &ActivityBroadcastHandler::BroadcastImpl16));
	m_mapMsgID2BroadcastImpl.insert(make_pair(17, &ActivityBroadcastHandler::BroadcastImpl17));
	m_mapMsgID2BroadcastImpl.insert(make_pair(30, &ActivityBroadcastHandler::BroadcastImpl30));
	m_mapMsgID2BroadcastImpl.insert(make_pair(31, &ActivityBroadcastHandler::BroadcastImpl31));
	m_mapMsgID2BroadcastImpl.insert(make_pair(35, &ActivityBroadcastHandler::BroadcastImpl35));
}

