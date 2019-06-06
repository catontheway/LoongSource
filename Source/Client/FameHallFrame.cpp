#include "StdAfx.h"
#include "FameHallFrame.h"

FameHallFrame::FameHallFrame(void) : m_Trunk(this)
{
}

FameHallFrame::~FameHallFrame(void)
{
	/* ��������������� */
	SafeDeleteRankData();

	/* ��������������� */
	SafeDeleteFameData();
}

BOOL FameHallFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);

	/* ע�Ὺ�ش����¼��ص����� */
	m_pFrameMgr->RegisterEventHandle(_T("SetFamousOpen"), (FRAMEEVENTPROC)m_Trunk.sfp1(&FameHallFrame::OnOpenFamousEvent));
	m_pFrameMgr->RegisterEventHandle(_T("SetFamousClose"), (FRAMEEVENTPROC)m_Trunk.sfp1(&FameHallFrame::OnCloseFamousEvent));
	m_pFrameMgr->RegisterEventHandle(_T("ClanTreaOpen"), (FRAMEEVENTPROC)m_Trunk.sfp1(&FameHallFrame::OnClanTreaOpen));
	m_pFrameMgr->RegisterEventHandle(_T("ClanTreaClose"), (FRAMEEVENTPROC)m_Trunk.sfp1(&FameHallFrame::OnClanTreaClose));

	m_pFrameMgr->RegisterEventHandle(_T("tagRoleGetNameEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&FameHallFrame::OnGetRankName));
	m_pFrameMgr->RegisterEventHandle(_T("GetNameByNameID"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&FameHallFrame::OnGetNameByNameID));

	/* ע�������¼��ص����� */
	m_pCmdMgr->Register("NS_GetReputeTop",		(NETMSGPROC)m_Trunk.sfp2(&FameHallFrame::OnNS_GetReputeTop), _T("NS_GetReputeTop"));
	m_pCmdMgr->Register("NS_GetFameHallRoles",  (NETMSGPROC)m_Trunk.sfp2(&FameHallFrame::OnNS_GetFameHallRoles), _T("NS_GetFameHallRoles"));

	return bRet;
}

BOOL FameHallFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	/* ��������������� */
	SafeDeleteRankData();

	/* ��������������� */
	SafeDeleteFameData();

	/* ע�����ش����¼��ص����� */
	m_pFrameMgr->UnRegisterEventHandler(_T("SetFamousOpen"), (FRAMEEVENTPROC)m_Trunk.sfp1(&FameHallFrame::OnOpenFamousEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("SetFamousClose"), (FRAMEEVENTPROC)m_Trunk.sfp1(&FameHallFrame::OnCloseFamousEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("ClanTreaOpen"), (FRAMEEVENTPROC)m_Trunk.sfp1(&FameHallFrame::OnClanTreaOpen));
	m_pFrameMgr->UnRegisterEventHandler(_T("ClanTreaClose"), (FRAMEEVENTPROC)m_Trunk.sfp1(&FameHallFrame::OnClanTreaClose));

	m_pFrameMgr->UnRegisterEventHandler(_T("tagRoleGetNameEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&FameHallFrame::OnGetRankName));
	m_pFrameMgr->UnRegisterEventHandler(_T("GetNameByNameID"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&FameHallFrame::OnGetNameByNameID));

	/* ע�������¼��ص����� */

	m_pCmdMgr->UnRegister("NS_GetReputeTop", (NETMSGPROC)m_Trunk.sfp2(&FameHallFrame::OnNS_GetReputeTop));
	m_pCmdMgr->UnRegister("NS_GetFameHallRoles",  (NETMSGPROC)m_Trunk.sfp2(&FameHallFrame::OnNS_GetFameHallRoles));

    /* ��������ô��� */
    if (P_VALID(m_pWnd))
    {
        m_pGUI->UnRegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&FameHallFrame::EventHandler));
        m_pGUI->AddToDestroyList(m_pWnd);
        m_pWnd = NULL;
    }

	return bRet;
}

BOOL FameHallFrame::ReloadUI()
{
	char	cbuff[128];
	TCHAR	tbuff[128];

	/* ����GUI���� */
	XmlElement ele;
	tstring strPath = g_strLocalPath + _T("\\ui\\famous.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());
	
	/* ���Ŀؼ� */
	for (INT i = 1; i <= MAX_RANK_PLACE; ++i)
	{
		_snprintf(cbuff, sizeof(cbuff)/sizeof(char), "famousbackpic\\rankingbackpic\\rank%d\\propic\\pro", i);
		m_pGUI->ChangeXml(&ele, cbuff, "ClassName", "ProgressEx");
	}

	/* ���� */
	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);

	/* �����ҳ */
	m_pBtClans[ECLT_XuanYuan]	=	(GUIPushButton*)m_pWnd->GetChild(_T("famousbackpic\\famousfra\\xuanyuan"));
	m_pBtClans[ECLT_ShenNong]	=	(GUIPushButton*)m_pWnd->GetChild(_T("famousbackpic\\famousfra\\shennong"));
	m_pBtClans[ECLT_FuXi]		=	(GUIPushButton*)m_pWnd->GetChild(_T("famousbackpic\\famousfra\\fuxi"));
	m_pBtClans[ECLT_SanMiao]	=	(GUIPushButton*)m_pWnd->GetChild(_T("famousbackpic\\famousfra\\sanmiao"));
	m_pBtClans[ECLT_JiuLi]		=	(GUIPushButton*)m_pWnd->GetChild(_T("famousbackpic\\famousfra\\jiuli"));
	m_pBtClans[ECLT_YueZhi]		=	(GUIPushButton*)m_pWnd->GetChild(_T("famousbackpic\\famousfra\\yuezhi"));
	m_pBtClans[ECLT_NvWa]		=	(GUIPushButton*)m_pWnd->GetChild(_T("famousbackpic\\famousfra\\nvwa"));
	m_pBtClans[ECLT_GongGong]	=	(GUIPushButton*)m_pWnd->GetChild(_T("famousbackpic\\famousfra\\gonggong"));

	/* �������� */
	for(INT i = 1; i <= MAX_RANK_PLACE; ++i)
	{
		INT index = i - 1;
		/* ��������������� */
		_sntprintf(tbuff, sizeof(tbuff)/sizeof(TCHAR), _T("famousbackpic\\rankingbackpic\\rank%d"), i);
		m_pStcRoleName[index]	=	(GUIStatic*)m_pWnd->GetChild(tbuff);
		/* �������� */
		_sntprintf(tbuff, sizeof(tbuff)/sizeof(TCHAR), _T("famousbackpic\\rankingbackpic\\rank%d\\propic\\word"), i);
		m_pStcReputeLevel[index] =	(GUIStatic*)m_pWnd->GetChild(tbuff);
		/* ����ֵ */
		_sntprintf(tbuff, sizeof(tbuff)/sizeof(TCHAR), _T("famousbackpic\\rankingbackpic\\rank%d\\propic\\pro"), i);
		m_pPrgrss[index]		=	(ProgressEx*)m_pWnd->GetChild(tbuff);

        m_pPrgrss[index]->SetSpeed(100000);
	}
	/* ҳ�� */
	m_pStcRankPage				=	(GUIStatic*)m_pWnd->GetChild(_T("famousbackpic\\rankingbackpic\\pagepic\\new"));
	/* ��һҳ */
	m_pBtRankPrevPage			=	(GUIButton*)m_pWnd->GetChild(_T("famousbackpic\\rankingbackpic\\pagepic\\backbutt"));
	/* ��һҳ */
	m_pBtRankNextPage			=	(GUIButton*)m_pWnd->GetChild(_T("famousbackpic\\rankingbackpic\\pagepic\\nextbutt"));

	/* ������ҳ�б� */
	m_pStcFameTopPage			=	(GUIStatic*)m_pWnd->GetChild(_T("famousbackpic\\famousshowpic\\top"));
	for (INT i = 1; i <= MAX_FAMOUS_TOP_PLACE; ++i)
	{
		INT index = i - 1;
		/* ��ҳ�������� */
		_sntprintf(tbuff, sizeof(tbuff)/sizeof(TCHAR), _T("famousbackpic\\famousshowpic\\top\\name%d"), i);
		m_pStcFamousTop[index]	=	(GUIStatic*)m_pWnd->GetChild(tbuff);
	}
	/* ��������ҳ�б� */
	m_pStcFameCommonPage		=	(GUIStatic*)m_pWnd->GetChild(_T("famousbackpic\\famousshowpic\\common"));
	for (INT i = 0; i < MAX_FAMOUS_COMMON_PLACE; ++i)
	{
		/* ����ҳ�������� */
		_sntprintf(tbuff, sizeof(tbuff)/sizeof(TCHAR), _T("famousbackpic\\famousshowpic\\common\\name_%c"), _T('a') + i);
		m_pStcFamousCommon[i]	=	(GUIStatic*)m_pWnd->GetChild(tbuff);
	}
	/* ҳ�� */
	m_pStcFamousPage			=	(GUIStatic*)m_pWnd->GetChild(_T("famousbackpic\\famousshowpic\\new"));
	/* ��һҳ */
	m_pBtFamousPrevPage			=	(GUIButton*)m_pWnd->GetChild(_T("famousbackpic\\famousshowpic\\backbutt"));
	/* ��һҳ */
	m_pBtFamousNextPage			=	(GUIButton*)m_pWnd->GetChild(_T("famousbackpic\\famousshowpic\\nextbutt"));

	/* �����䱦 */
	m_pBtEnterClanTreasure		=	(GUIButton*)m_pWnd->GetChild(_T("famousbackpic\\rankingbackpic\\weaponshowbutt"));

	/* �ر� */
	m_pBtClose					=	(GUIButton*)m_pWnd->GetChild(_T("famousbackpic\\closebutt"));

	/* ע���¼������� */
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&FameHallFrame::EventHandler));

	/* Ĭ��ѡ��ԯ��ҳ */
	m_pBtClans[ECLT_XuanYuan]->SetState(EGUIBS_PushDown);
	SetClan(ECLT_XuanYuan);
	for (INT i = 0; i < ECLT_NUM; ++i)
	{
		m_nRankPageCount[i]		=	1;
		m_nRankCurrPage[i]		=	1;
		m_nFamePageCount[i]		=	1;
		m_nFameCurrPage[i]		=	1;

		m_dwRankUpdateTime[i]	=	0;
		m_dwFameUpdateTime[i]	=	0;
	}

	/* Ĭ�����ش��� */
	ShowWnd(false);

	/*! ���������䱦Frame */
	m_pClanTreasure = (ClanTreasureFrame*)m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("ClanTreasure"), _T("ClanTreasureFrame"), 0);
	if (!P_VALID(m_pClanTreasure))
		IMSG(_T("Create ClanTreasureFrame failed"));

	m_bisClanTreasureOpen		=	false;

	return TRUE;
}

VOID FameHallFrame::Update()
{

}

VOID FameHallFrame::UpdateFameHall()
{
    RefreshRankToUI(m_eSelectedClan);
    BeginGetReputRank(m_eSelectedClan);
    RefreshFameToUI(m_eSelectedClan);
    BeginGetFame(m_eSelectedClan);
}


DWORD FameHallFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		if (pWnd == m_pBtClose)
		{
			/* �رմ��� */
			ShowWnd(false);


		}
		else if (pWnd == m_pBtEnterClanTreasure)
		{
			/* ���������䱦���� */
			if (m_bisClanTreasureOpen)
				m_pFrameMgr->SendEvent(&tagGameEvent(_T("SetClanTreasureClose"), this));
			else
				m_pFrameMgr->SendEvent(&tagOpenClanTreasure(_T("SetClanTreasureOpen"), m_eSelectedClan, this));
		}
        else if (pWnd == m_pBtFamousNextPage)
        {
            /* ��һҳ */
            NextFamePage();
        }
        else if (pWnd == m_pBtFamousPrevPage)
        {
            /* ��һҳ */
            PrevFamePage();
        }
		else if (pWnd == m_pBtRankNextPage)
		{
            /* ��һҳ */
            NextRankPage();
		}
		else if (pWnd == m_pBtRankPrevPage)
		{
            /* ��һҳ */
            PrevRankPage();
		}
		break;
	case EGUIE_PushButtonDown:
		if (pEvent->dwParam1 != 1)	/* ���ǰ��²����� */
			return 0;

		for (int i = 0; i < ECLT_NUM; ++i)
		{
			if (pWnd == m_pBtClans[i])
			{
				/* ���������ҳ���� */
				SetClan((ECLanType)i);
                /* �ر������䱦���� */
                if (m_bisClanTreasureOpen)
                    m_pFrameMgr->SendEvent(&tagGameEvent(_T("SetClanTreasureClose"), this));
				break;
			}
		}
		break;
	}

	return 0;
}

DWORD FameHallFrame::OnOpenFamousEvent( tagGameEvent* pGameEvent )
{
	ShowWnd(true);

	return 0;
}

VOID FameHallFrame::ShowWnd( bool bVal )
{
	/* ���ô���ɼ��� */
	m_pWnd->SetInvisible(!bVal);

	if (bVal)
	{
		/* �ö� */
		m_pWnd->FlipToTop();

		/* ˢ�´��� */
        m_pWnd->SetRefresh(TRUE);

		/* ���������巢�ʹ��¼� */
		m_pFrameMgr->SendEvent(&tagGameEvent(_T("FamousUIOpen"), this));

        /* ˢ������ */
        UpdateFameHall();
	}
	else
	{
		/* ͬʱ�ر������䱦���� */
		if (m_bisClanTreasureOpen)
			m_pFrameMgr->SendEvent(&tagGameEvent(_T("SetClanTreasureClose"), this));

		/* ���������巢�͹ر��¼� */
		m_pFrameMgr->SendEvent(&tagGameEvent(_T("FamousUIClose"), this));
	}
}

DWORD FameHallFrame::OnCloseFamousEvent( tagGameEvent* pGameEvent )
{
	ShowWnd(false);

	return 0;
}

DWORD FameHallFrame::OnClanTreaOpen( tagGameEvent* pGameEvent )
{
	m_bisClanTreasureOpen = true;

	return 0;
}

DWORD FameHallFrame::OnClanTreaClose( tagGameEvent* pGameEvent )
{
	m_bisClanTreasureOpen = false;

	return 0;
}

VOID FameHallFrame::SetClan( ECLanType eVal )
{
	m_eSelectedClan = eVal;
    UpdateFameHall();
}

VOID FameHallFrame::BeginGetReputRank( ECLanType eVal )
{
	/* ���ͻ�����������б����� */
	tagNC_GetReputeTop e;
	e.byClanType	=	(BYTE)eVal;
	e.dwUpdateTime	=	m_dwRankUpdateTime[eVal];
	if (m_pSession->IsConnect())
		m_pSession->Send(&e);
}

VOID FameHallFrame::BeginGetFame( ECLanType eVal )
{
	/* ���ͻ�������б����� */
	tagNC_GetFameHallRoles e;
	e.byClanType	=	(BYTE)eVal;
	e.dwUpdateTime	=	m_dwFameUpdateTime[eVal];
	if (m_pSession->IsConnect())
		m_pSession->Send(&e);
}

DWORD FameHallFrame::OnNS_GetReputeTop( tagNS_GetReputeTop* pMsg, DWORD param )
{
	ECLanType eClan = (ECLanType)pMsg->byClanType;
	if (!JDG_VALID(ECLT, eClan))
		return 0;

	/* ����ޱ仯���򷵻� */
	if (pMsg->byErrCode == E_FrameHall_RepOrderUnchanged)
		return 0;
    else
        /* �б仯���򱣴�ʱ��� */
        m_dwRankUpdateTime[eClan] = pMsg->dwUpdateTime;

	/* ������� */
	ClearRankData(eClan);

	for (INT i = 0, offset = 0; i < (INT)pMsg->byRoleNum; ++i, ++offset)
	{
		/* ����ռ� */
		tagRepRankData rank;

		/* �����䳤�б� */
		memcpy(&rank, pMsg->byData + sizeof(tagRepRankData) * offset, sizeof(tagRepRankData));

		/* ������Ϣ�������� */
		tagRankData* pRankData = new tagRankData();
		pRankData->dwRoleID		=	rank.dwRoleID;
		pRankData->nReputeValue	=	(INT)rank.dwReputValue;
		pRankData->strRoleName	=	PlayerNameTab::Inst()->FindNameByID(rank.dwRoleID);
		/* ����ӻ�����鲻������,�������ұ���,���õ����ַ����¼����� */
		if (pRankData->strRoleName.empty())
			m_mapRoleIDtoRank[eClan].insert(make_pair(rank.dwRoleID, pRankData));
		m_vecRankArray[eClan].push_back(pRankData);

		/* �������������������� */
		FillReputRank(eClan);

		/* ˢ��UI */
		if (eClan == m_eSelectedClan)
			RefreshRankToUI(m_eSelectedClan);
	}
	return 0;
}

DWORD FameHallFrame::OnNS_GetFameHallRoles( tagNS_GetFameHallRoles* pMsg, DWORD param )
{
	ECLanType eClan = (ECLanType)pMsg->eClanType;
	if (!JDG_VALID(ECLT, eClan))
		return 0;

	/* ����ޱ仯���򷵻� */
	if (pMsg->byErrCode == E_FrameHall_MemberOrderUnchanged)
		return 0;
	else
		/* �б仯���򱣴�ʱ��� */
		m_dwFameUpdateTime[eClan] = pMsg->dwUpdateTime;

    /* ������� */
    ClearFameData(eClan);

	for (INT i = 0, offset = 0; i < (INT)pMsg->byRoleNum; ++i, ++offset)
	{
		/* ����ռ� */
		DWORD dwFameNameID;

		/* �����䳤�б� */
		memcpy(&dwFameNameID, pMsg->byData + sizeof(DWORD) * offset, sizeof(DWORD));

		/* ������Ϣ�������� */
		tagFameData* pFameData = new tagFameData();
		pFameData->dwNameID		=	dwFameNameID;
		pFameData->strName		=	PlayerNameTab::Inst()->FindNameByNameID(dwFameNameID);
		/* ����ӻ�����鲻������,�������ұ���,���õ����ַ����¼����� */
		if (pFameData->strName.empty())
			m_mapNameIDtoFame[eClan].insert(make_pair(dwFameNameID, pFameData));
		m_vecFameNameIDs[eClan].push_back(pFameData);

		/* �������������������� */
		FillFameData(eClan);

		/* ˢ��UI */
		if (eClan == m_eSelectedClan)
			RefreshFameToUI(m_eSelectedClan);
	}
	return 0;
}

VOID FameHallFrame::FillReputRank( ECLanType eVal )
{
	/* ����ҳ���� */
	INT rankCount = m_vecRankArray[eVal].size();
	if (0 == rankCount)
	{
		m_nRankPageCount[eVal]	=	1;
		m_nRankCurrPage[eVal]	=	1;
		return;
	}
	/* ������ҳ����ҳ */
	if (rankCount % MAX_RANK_PLACE == 0)
		m_nRankPageCount[eVal]	=	rankCount / MAX_RANK_PLACE;
	else
		m_nRankPageCount[eVal]	=	rankCount / MAX_RANK_PLACE + 1;
	
	/* ѡ�е�һҳ */
	m_nRankCurrPage[eVal]		=	1;
}

VOID FameHallFrame::FillFameData( ECLanType eVal )
{
	/* ����ҳ���� */
	INT fameCount = m_vecFameNameIDs[eVal].size();

	/* ������ҳ */
	if (fameCount <= MAX_FAMOUS_TOP_PLACE)
	{
		m_nFamePageCount[eVal]	=	1;
		m_nFameCurrPage[eVal]	=	1;
		return;
	}

	/* ������ҳ����ҳ */
	INT commonCount = fameCount - MAX_FAMOUS_TOP_PLACE;

	/* ����ҳ��ʱ�����˵�һҳ */
	if (commonCount % MAX_FAMOUS_COMMON_PLACE == 0)
		m_nFamePageCount[eVal]	=	m_vecFameNameIDs[eVal].size() / MAX_FAMOUS_COMMON_PLACE + 1;
	else
		m_nFamePageCount[eVal]	=	m_vecFameNameIDs[eVal].size() / MAX_FAMOUS_COMMON_PLACE + 2;
	
	/* ѡ�е�һҳ */
	m_nFameCurrPage[eVal]		=	1;
}

DWORD FameHallFrame::OnGetRankName( tagRoleGetNameEvent* pGameEvent )
{
	if (!pGameEvent->bResult)
		return 0;

	for (INT i = 0; i < ECLT_NUM; ++i)
	{
		/* �����Ƿ��������ѯ������ */
		map<DWORD, tagRankData*>::iterator it =
			m_mapRoleIDtoRank[(ECLanType)i].find(pGameEvent->dwRoleID);
		if (it != m_mapRoleIDtoRank[(ECLanType)i].end())
		{
			/* �����ִ������� */
			(it->second)->strRoleName = pGameEvent->strRoleName;

			/* ɾ�����������еĶ��� */
			m_mapRoleIDtoRank[(ECLanType)i].erase(it);

			/* ˢ�½��� */
			if ((ECLanType)i == m_eSelectedClan)
				RefreshRankToUI(m_eSelectedClan);
		}
	}
	return 0;
}

DWORD FameHallFrame::OnGetNameByNameID( tagGetNameByNameID* pGameEvent )
{
	if (!pGameEvent->bResult)
		return 0;

	for (INT i = 0; i < ECLT_NUM; ++i)
	{
		/* �����Ƿ��������ѯ������ */
		map<DWORD, tagFameData*>::iterator it =
			m_mapNameIDtoFame[(ECLanType)i].find(pGameEvent->dwNameID);
		if (it != m_mapNameIDtoFame[(ECLanType)i].end())
		{
			/* �����ִ������� */
			(it->second)->strName = pGameEvent->strName;

			/* ɾ�������������� */
			m_mapNameIDtoFame[(ECLanType)i].erase(it);

			/* ˢ�½��� */
			if ((ECLanType)i == m_eSelectedClan)
				RefreshFameToUI(m_eSelectedClan);
		}
	}
	return 0;
}

VOID FameHallFrame::RefreshRankToUI( ECLanType eVal )
{
	/* ��ѡ�������򷵻� */
	if (m_eSelectedClan != eVal)
		return;
	
	/* ��ս��� */
    for (INT i = 0; i < MAX_RANK_PLACE; ++i)
	{
		m_pStcRoleName[i]->SetText(_T(""));
        m_pStcRoleName[i]->SetRefresh(TRUE);
		m_pPrgrss[i]->SetValue(0);
		m_pStcReputeLevel[i]->SetText(_T(""));
        m_pStcReputeLevel[i]->SetRefresh(TRUE);
	}

	/* ˢ��ҳ�� */
	TCHAR tcbuff[16];
	_itot(m_nRankCurrPage[m_eSelectedClan], tcbuff, 10);
	m_pStcRankPage->SetText(tcbuff);

	/* ���б��򷵻� */
	INT rankCount = m_vecRankArray[eVal].size();
	if (0 == rankCount)
		return;

	/* ���ݵ�ǰҳ�����б���ʾ��Χ */
    INT nBegin  = (m_nRankCurrPage[eVal] - 1) * MAX_RANK_PLACE;
    INT nEnd    = nBegin + MAX_RANK_PLACE;
    if (nEnd > rankCount)
        nEnd = rankCount;

	vector<tagRankData*>::iterator itBegin	=   m_vecRankArray[eVal].begin() + nBegin;
    vector<tagRankData*>::iterator itEnd    =   m_vecRankArray[eVal].begin() + nEnd;

	/* ��ʾ���ؼ� */
	INT index = 0;
	for (vector<tagRankData*>::iterator it = itBegin; it != itEnd; ++it)
	{
		/* ������������ */
		tagReputeLevel rlevel;
		ReputeMgr::Inst()->UlityCalReputeLevel(rlevel, (*it)->nReputeValue);
		/* ���ý�ɫ�� */
		m_pStcRoleName[index]->SetText((*it)->strRoleName.c_str());
        m_pStcRoleName[index]->SetRefresh(TRUE);
		/* ���ý�������ɫ */
		ReputeMgr::Inst()->UlitySetPrgrssColor(m_pPrgrss[index], rlevel.eColor);
		/* ���ý��������� */
		m_pPrgrss[index]->SetMaxValue((FLOAT)rlevel.nDiffValue);
		/* ���ý�������ǰֵ */
		m_pPrgrss[index]->SetValue((FLOAT)((*it)->nReputeValue - rlevel.nFrom));
		/* ���������������� */
		m_pStcReputeLevel[index]->SetText(rlevel.szLevel);
        m_pStcReputeLevel[index]->SetRefresh(TRUE);
		/* ��һ���ؼ� */
		++index;
	}
}

VOID FameHallFrame::RefreshFameToUI( ECLanType eVal )
{
	/* ��ѡ�������򷵻� */
	if (m_eSelectedClan != eVal)
		return;

	/* ��ս��� */
	for (INT i = 0; i < MAX_FAMOUS_TOP_PLACE; ++i)
    {
		m_pStcFamousTop[i]->SetText(_T(""));
        m_pStcFamousTop[i]->SetRefresh(TRUE);
    }

	for (INT i = 0; i < MAX_FAMOUS_COMMON_PLACE; ++i)
    {
		m_pStcFamousCommon[i]->SetText(_T(""));
        m_pStcFamousCommon[i]->SetRefresh(TRUE);
    }

	/* ˢ��ҳ�� */
	TCHAR tcbuff[16];
	_itot(m_nFameCurrPage[m_eSelectedClan], tcbuff, 10);
	m_pStcFamousPage->SetText(tcbuff);
    m_pStcFamousPage->SetRefresh(TRUE);

	/* ���б��򷵻� */
	INT fameCount = m_vecFameNameIDs[eVal].size();
	if (0 == fameCount)
		return;

	/* ���ݵ�ǰҳ�����б���ʾ��Χ */
    INT nBegin;
    INT nEnd;
	vector<tagFameData*>::iterator itBegin;
	vector<tagFameData*>::iterator itEnd;

	if (1 == m_nFameCurrPage[eVal])
	{
        nBegin  =   0;
        nEnd    =   MAX_FAMOUS_TOP_PLACE;
	}
	else
	{
        nBegin  =   MAX_FAMOUS_TOP_PLACE + (m_nFameCurrPage[eVal] - 2) * MAX_FAMOUS_COMMON_PLACE;
        nEnd    =   nBegin + MAX_FAMOUS_COMMON_PLACE;
	}

    if (nEnd > fameCount)
        nEnd    =   fameCount;

    itBegin     =   m_vecFameNameIDs[eVal].begin() + nBegin;
    itEnd       =   m_vecFameNameIDs[eVal].begin() + nEnd;

	/* ��ʾ���ؼ� */
	if (1 == m_nFameCurrPage[eVal])
	{
		INT index = 0;
		for (vector<tagFameData*>::iterator it = itBegin; it != itEnd; ++it)
        {
			m_pStcFamousTop[index]->SetText((*it)->strName.c_str());
            m_pStcFamousTop[index]->SetRefresh(TRUE);
            ++index;
        }

        m_pStcFameTopPage->SetInvisible(FALSE);
        m_pStcFameCommonPage->SetInvisible(TRUE);
	}
	else
	{
		INT index = 0;
		for (vector<tagFameData*>::iterator it = itBegin; it != itEnd; ++it)
        {
			m_pStcFamousCommon[index]->SetText((*it)->strName.c_str());
            m_pStcFamousCommon[index]->SetRefresh(TRUE);
            ++index;
        }

        m_pStcFameTopPage->SetInvisible(TRUE);
        m_pStcFameCommonPage->SetInvisible(FALSE);
	}
}

VOID FameHallFrame::PrevRankPage()
{
	/* ҳ���һ */
	if (m_nRankCurrPage[m_eSelectedClan] > 1)
    {
		--m_nRankCurrPage[m_eSelectedClan];

	    /* ˢ�½��� */
	    RefreshRankToUI(m_eSelectedClan);
    }
}

VOID FameHallFrame::PrevFamePage()
{
	/* ҳ���һ */
	if (m_nFameCurrPage[m_eSelectedClan] > 1)
    {
		--m_nFameCurrPage[m_eSelectedClan];

	    /* ˢ�½��� */
	    RefreshFameToUI(m_eSelectedClan);
    }
}

VOID FameHallFrame::NextRankPage()
{
	/* ҳ���һ */
	if (m_nRankCurrPage[m_eSelectedClan] < m_nRankPageCount[m_eSelectedClan])
    {
		++m_nRankCurrPage[m_eSelectedClan];

	    /* ˢ�½��� */
	    RefreshRankToUI(m_eSelectedClan);
    }
}

VOID FameHallFrame::NextFamePage()
{
	/* ҳ���һ */
	if (m_nFameCurrPage[m_eSelectedClan] < m_nFameCurrPage[m_eSelectedClan])
    {
		++m_nFameCurrPage[m_eSelectedClan];

	    /* ˢ�½��� */
	    RefreshFameToUI(m_eSelectedClan);
    }
}

VOID FameHallFrame::ClearRankData( ECLanType eVal )
{
	/* �ͷſռ� */
	for (vector<tagRankData*>::iterator it = m_vecRankArray[eVal].begin();
		it != m_vecRankArray[eVal].end(); ++it)
	{
		delete (*it);
		(*it) = NULL;
	}

	/* ������� */
	m_vecRankArray[eVal].clear();
	m_mapRoleIDtoRank[eVal].clear();
}

VOID FameHallFrame::ClearFameData( ECLanType eVal )
{
	/* �ͷſռ� */
	for (vector<tagFameData*>::iterator it = m_vecFameNameIDs[eVal].begin();
		it != m_vecFameNameIDs[eVal].end(); ++it)
	{
		delete (*it);
		(*it) = NULL;
	}

	/* ������� */
	m_vecFameNameIDs[eVal].clear();
	m_mapNameIDtoFame[eVal].clear();
}

VOID FameHallFrame::SafeDeleteRankData()
{
	for (INT i = 0; i < ECLT_NUM; ++i)
		ClearRankData((ECLanType)i);
}

VOID FameHallFrame::SafeDeleteFameData()
{
	for (INT i = 0; i < ECLT_NUM; ++i)
		ClearFameData((ECLanType)i);
}
