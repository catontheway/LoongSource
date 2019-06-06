#include "stdafx.h"
#include "PlayerNameTab.h"
#include "RoleMgr.h"
#include "Role.h"
PlayerNameTab::PlayerNameTab(void):m_Trunk(this)
{
}

PlayerNameTab::~PlayerNameTab(void)
{
}

PlayerNameTab g_playerNameTab;
PlayerNameTab* PlayerNameTab::Inst()
{
	return &g_playerNameTab;
}

void PlayerNameTab::Init()
{
	m_pUtil = TObjRef<Util>();
	m_pSession = TObjRef<NetSession>();
	m_pFrameMgr = TObjRef<GameFrameMgr>();
	m_pCmdMgr = TObjRef<NetCmdMgr>();

	m_pCmdMgr->Register("NS_GetNamebyNameID",		   (NETMSGPROC)m_Trunk.sfp2(&PlayerNameTab::OnNS_GetNamebyNameID),			_T("NS_GetNamebyNameID"));
	m_pCmdMgr->Register("NS_RoleGetSomeName",		   (NETMSGPROC)m_Trunk.sfp2(&PlayerNameTab::OnNS_RoleGetSomeName),			_T("NS_RoleGetSomeName"));
    m_pCmdMgr->Register("NS_GetRoleHeadPicURL",		   (NETMSGPROC)m_Trunk.sfp2(&PlayerNameTab::OnNS_GetRoleHeadPicURL),		_T("NS_GetRoleHeadPicURL"));

	m_paReqRole.first = 0;
	m_paReqRole.second.clear();
}

void PlayerNameTab::Destroy()
{
    m_pCmdMgr->UnRegister("NS_GetNamebyNameID",		     (NETMSGPROC)m_Trunk.sfp2(&PlayerNameTab::OnNS_GetNamebyNameID));
	m_pCmdMgr->UnRegister("NS_RoleGetSomeName",		     (NETMSGPROC)m_Trunk.sfp2(&PlayerNameTab::OnNS_RoleGetSomeName));
    m_pCmdMgr->UnRegister("NS_GetRoleHeadPicURL",		 (NETMSGPROC)m_Trunk.sfp2(&PlayerNameTab::OnNS_GetRoleHeadPicURL));
    m_id2name.clear();
    m_name2id.clear();

	//--��������б�
	m_paReqRole.first = 0;
	m_paReqRole.second.clear();
}

void PlayerNameTab::AddRecord(DWORD id,const TCHAR* szName)
{
	tstring strTmp;
	if(szName==NULL || _tcslen(szName)<=0)
		strTmp.assign( _T("GFC_NULL") );
	else
		strTmp.assign(szName);

	m_id2name.insert(std::pair<DWORD,tstring>(id,strTmp));
	m_name2id.insert(std::pair<tstring,DWORD>(strTmp,id));
}


// ÿ֡����
void PlayerNameTab::Update()
{
	// �������б�Ϊ�գ��Ҵ���ʱ�����500����ʱ������������ͽ�ɫ��������
	if( !m_paReqRole.second.empty() && 500 < Kernel::Inst()->GetAccumTimeDW() - m_paReqRole.first )
	{
		tagNC_RoleGetSomeName NetCmdData;
		NetCmdData.nNum = m_paReqRole.second.size() > 50 ? 50 : m_paReqRole.second.size();
		NetCmdData.nUserData = 0;
		// �ṹ�Ŀռ��ȥ����ָ���dwAllID[1]�Ŀռ��ټ���ʵ����SIZE����С��DWORD�ռ�����½ṹ�Ĵ�С
		int size = sizeof(tagNC_RoleGetSomeName) - sizeof(DWORD) + sizeof(DWORD) * NetCmdData.nNum;
		// ���ٸĴ�С������
		BYTE byAllocData[sizeof(tagNC_RoleGetSomeName) - sizeof(DWORD) + sizeof(DWORD) * 50];
		tagNC_RoleGetSomeName *pRGSCmd = (tagNC_RoleGetSomeName*)byAllocData;

		// ��������ָ��֮ǰ����������,��Ϊ��POD��,�ǰ�ȫ��.
		memcpy(pRGSCmd, &NetCmdData, (sizeof(NetCmdData)));
		// ID��ֵ��
		for (INT i = 0; i < NetCmdData.nNum; ++i)
		{
			pRGSCmd->dwAllID[i] = m_paReqRole.second.front();
			m_paReqRole.second.pop_front();
		}
		// �����µĴ�С
		pRGSCmd->dwSize = size;
		m_pSession->Send(pRGSCmd);
		m_paReqRole.first = Kernel::Inst()->GetAccumTimeDW();
	}
}


DWORD PlayerNameTab::FindIDByName(const TCHAR* szName)
{
	ASSERT(szName!=NULL && GT_VALID(szName));

	map<tstring,DWORD>::iterator it=m_name2id.find((tstring)(szName));
	if(it!=m_name2id.end() && (*it).second!=GT_INVALID )
	{
		return (*it).second;
	}
	else
	{
		TCHAR szBuff[X_SHORT_NAME] = {0};
		_tcsncpy(szBuff, szName, X_SHORT_NAME-1);
		_tcslwr(szBuff);
		DWORD dwCRC = m_pUtil->Crc32(szBuff);
		///�Ѿ������������
		if(m_mapName2IDReq.find(dwCRC)!=m_mapName2IDReq.end())
			return GT_INVALID;

		//���PlayerNameTab��û�б����ӦName��ID		
		if(m_pSession->IsConnect())
		{
			tagNC_RoleGetID m;
			m.dwRoleNameCrc = dwCRC;
			m_pSession->Send(&m);

			//�������󻺴�
			m_mapName2IDReq[dwCRC]=szName;
		}
		else
			return GT_INVALID;
	}

	return GT_INVALID;
}

const TCHAR* PlayerNameTab::FindNameByID(DWORD id)
{
	if( !IS_PLAYER(id) )
		return _T("");

	map<DWORD,tstring>::iterator it=m_id2name.find(id);
	if( it!=m_id2name.end() && (*it).second!=_T("GFC_NULL") )
	{
		return (*it).second.c_str();
	}
	else
	{
		//���PlayerNameTab��û�б����ӦID��name
		//--��������б��в����ڣ������
		bool bFind=false;
		for(list<DWORD>::iterator iter=m_paReqRole.second.begin();
			iter!=m_paReqRole.second.end();++iter)
		{
			if(*iter==id)
			{
				bFind=true;
				break;
			}
		}
		if(!bFind)
		{
			m_paReqRole.second.push_back(id);
		}
	}

	return _T("");
}

bool PlayerNameTab::CanGetBundleOfNames(const std::vector<DWORD>& IDs, std::vector<DWORD>& Cantfinds)
{
	// �����в����ڣ���Ҫ��������������ֵ�ID�б�
	std::vector<DWORD> reqlist;
	reqlist.clear();
	Cantfinds.clear();

	std::vector<DWORD>::const_iterator iter = IDs.begin();
	std::vector<DWORD>::const_iterator end = IDs.end();
	
	bool bResult = true;

	for (; iter != end; ++iter)
	{
		DWORD dwReqID = *iter;
		map<DWORD, tstring>::iterator finder = m_id2name.find(dwReqID);
		// ���û�ҵ���������GFC_NULL
		if (finder == m_id2name.end() || finder->second == (_T("GFC_NULL")))
		{
			// ��һ������û�в鵽�������ѯʧ��
			bResult = false;
			// !!!ֱ�Ӽ��������б����ﲻ�������󻺴棬��Ϊ���뵱�յ���������getsomename��Ϣ��
			// !!!�յ��������������������
			reqlist.push_back(dwReqID);
			// ���벻���ҵ����б�
			Cantfinds.push_back(dwReqID);
		}
	}
	// ����������б�Ϊ��
	if (!reqlist.empty())
	{
		tagNC_RoleGetSomeName NetCmdData;
		NetCmdData.nNum = static_cast<INT>(reqlist.size());
		// �ṹ�Ŀռ��ȥ����ָ���dwAllID[1]�Ŀռ��ټ���ʵ����SIZE����С��DWORD�ռ�����½ṹ�Ĵ�С
		int size = sizeof(tagNC_RoleGetSomeName) - sizeof(DWORD) + sizeof(DWORD) * reqlist.size();
		// ���ٸĴ�С������
		BYTE *pAllocData = new BYTE[size];
		tagNC_RoleGetSomeName *pRGSCmd = reinterpret_cast<tagNC_RoleGetSomeName*>(pAllocData);
		ZeroMemory(pRGSCmd, size);
		// ��������ָ��֮ǰ����������,��Ϊ��POD��,�ǰ�ȫ��.
		memcpy(pRGSCmd, &NetCmdData, (sizeof(NetCmdData) - sizeof(DWORD)));
		// ID��ֵ��
		size_t sizeReq = reqlist.size();
		for (size_t i = 0; i < sizeReq; ++i)
		{
			pRGSCmd->dwAllID[i] = reqlist[i];
		}
		// �����µĴ�С
		pRGSCmd->dwSize = size;
		m_pSession->Send(pRGSCmd);
		delete [] pAllocData;
	}
	
	return bResult;
}



const TCHAR* PlayerNameTab::FindNameByNameID( DWORD dwNameID )
{
	map<DWORD, tstring>::const_iterator cit = m_NameID2Name.find(dwNameID);
	if (cit != m_NameID2Name.end())
		return cit->second.c_str();
	else
	{
		// ��������б������򷵻�
		if (m_setNameID2NameReq.find(dwNameID) != m_setNameID2NameReq.end())
			return _T("");

		// ��������б���û����������
		if (m_pSession->IsConnect())
		{
			tagNC_GetNamebyNameID msg;
			msg.dwNameID = dwNameID;
			m_pSession->Send(&msg);

			//���浽�����б���
			m_setNameID2NameReq.insert(dwNameID);
		}
	}
	return _T("");
}


LPCTSTR PlayerNameTab::FindHeadPicURLbyID( DWORD dwRoleID )
{
    map<DWORD, tstring>::const_iterator cit = m_RoleID2HeadPicUrl.find(dwRoleID);
    if (cit != m_RoleID2HeadPicUrl.end())
        return cit->second.c_str();
    else
    {
        // ��������б������򷵻�
        if (m_setRoleID2HeadPic.find(dwRoleID) != m_setRoleID2HeadPic.end())
            return _T("");

        // ��������б���û����������
        if (m_pSession->IsConnect())
        {
            tagNC_GetRoleHeadPicURL msg;
            msg.dwRoleID = dwRoleID;
            m_pSession->Send(&msg);

            // ���浽�����б���
            m_setRoleID2HeadPic.insert(dwRoleID);
        }
    }

    return _T("");
}

void PlayerNameTab::OnNS_RoleGetSomeName(tagNS_RoleGetSomeName *pCmd, DWORD)
{
	tagRoleGetSomeNameEvent event(_T("GetSomeNameOK"), NULL);
	// ���뵽��¼.
	for (int i = 0; i < pCmd->nNum; ++i)
	{
		if( pCmd->IdName[i].szName[0] )
			AddRecord(pCmd->IdName[i].dwID, pCmd->IdName[i].szName);

		if( pCmd->nUserData == 1)
		{
			if( pCmd->IdName[i].szName[0] )
				event.IDs.push_back(pCmd->IdName[i].dwID);
		}
		else
		{
			tagRoleGetNameEvent getName(_T("tagRoleGetNameEvent"), NULL);
			getName.dwRoleID = pCmd->IdName[i].dwID;
			getName.strRoleName = pCmd->IdName[i].szName;
			getName.dwParam = 0;
			getName.bResult = !getName.strRoleName.empty();
			m_pFrameMgr->SendEvent(&getName);

			if(getName.bResult)
			{
				Role* pRole = RoleMgr::Inst()->FindRole(pCmd->IdName[i].dwID);
				if(P_VALID(pRole))
				{
					pRole->SetRoleName(pCmd->IdName[i].szName);
				}
			}
		}
	}
	
	// ������Ϣ֪ͨ��Ϣע���ߣ���ȡ���ֳɹ�.
	if( pCmd->nUserData == 1 )
		m_pFrameMgr->SendEvent(&event);
}

void PlayerNameTab::OnNS_GetNamebyNameID( tagNS_GetNamebyNameID* pNetCmd, DWORD dwParam )
{
	if (pNetCmd->bResult)
	{
		// �����¼
		tstring strTmp;
		if(pNetCmd->szName == NULL || _tcslen(pNetCmd->szName) <= 0)
			strTmp = _T("");
		else
			strTmp = pNetCmd->szName;

		m_NameID2Name.insert(make_pair(pNetCmd->dwNameID, strTmp));

		// ������Ϸ�¼�
		tagGetNameByNameID e(_T("GetNameByNameID"), NULL);
		e.bResult	=	pNetCmd->bResult;
		e.dwNameID	=	pNetCmd->dwNameID;
		e.strName	=	strTmp;

		m_pFrameMgr->SendEvent(&e);
	}

	// �������б���ɾ��
	m_setNameID2NameReq.erase(pNetCmd->dwNameID);
}

void PlayerNameTab::OnNS_GetRoleHeadPicURL( tagNS_GetRoleHeadPicURL* pNetCmd, DWORD dwParam )
{
    // �����¼
    TCHAR szImageURL[200] = {_T('\0')};
    memcpy(szImageURL, pNetCmd->byData, (INT)pNetCmd->byHeadUrlSize);
    tstring strImageURL = szImageURL;

    if (!strImageURL.empty())
    {
        m_RoleID2HeadPicUrl.insert(make_pair(pNetCmd->dwRoleID, strImageURL));

    }

    // �������б���ɾ��
    m_setRoleID2HeadPic.erase(pNetCmd->dwRoleID);

    // ������Ϸ�¼�
    tagGetHeadpicByID e(_T("tagGetHeadpicByID"), NULL);
    e.dwRoleID      =   pNetCmd->dwRoleID;
    e.strImageURL   =   strImageURL;

    m_pFrameMgr->SendEvent(&e);
}

void PlayerNameTab::OnNetRoleGetID( tagNS_RoleGetID* pNetCmd, DWORD dwParam )
{
	if( GT_VALID(pNetCmd->dwRoleID) )
	{
		map<DWORD,tstring>::const_iterator it= m_mapName2IDReq.find(pNetCmd->dwRoleNameCrc);
		if(it!=m_mapName2IDReq.end())
		{
			AddRecord(pNetCmd->dwRoleID, pNetCmd->szName);
            m_mapName2IDReq.erase(pNetCmd->dwRoleNameCrc);
			tagRoleGetIDEvent getID(_T("tagRoleGetIDEvent"), NULL);
			getID.dwRoleNameCrc = pNetCmd->dwRoleNameCrc;
			getID.dwRoleID = pNetCmd->dwRoleID;
			getID.dwParam = dwParam;
			m_pFrameMgr->SendEvent(&getID);
		}
	}
	else
	{
		tagGameEvent event(_T("Event_RoleNotExist"), NULL);
		m_pFrameMgr->SendEvent(&event);
	}
	
}
