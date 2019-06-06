#include "StdAfx.h"
#include "TreasureMgr.h"

TreasureMgr::TreasureMgr(void) : m_Trunk(this)
{
}

TreasureMgr::~TreasureMgr(void)
{
    ClearData();
}

VOID TreasureMgr::Init()
{
    m_pCmdMgr = TObjRef<NetCmdMgr>();
    m_pSession = TObjRef<NetSession>();
    m_pFrameMgr = TObjRef<GameFrameMgr>();
    m_pUtil    = TObjRef<Util>();

    // ע��������Ϣ
    m_pCmdMgr->Register("NS_GetActClanTreasure",    (NETMSGPROC)m_Trunk.sfp2(&TreasureMgr::OnNS_GetActClanTreasure),    _T("NS_GetActClanTreasure"));
    m_pCmdMgr->Register("NS_NewActivedTreasure",    (NETMSGPROC)m_Trunk.sfp2(&TreasureMgr::OnNS_NewActivedTreasure),    _T("NS_NewActivedTreasure"));
    m_pCmdMgr->Register("NS_ActiveTreasure",        (NETMSGPROC)m_Trunk.sfp2(&TreasureMgr::OnNS_ActiveTreasure),        _T("NS_ActiveTreasure"));

    // ע����Ϸ�¼�
    m_pFrameMgr->RegisterEventHandle(_T("GetNameByNameID"),    (FRAMEEVENTPROC)m_Trunk.sfp1(&TreasureMgr::OnGetNameByNameID));

    // ��ʼ��
    ClearData();
    LoadData();

}

VOID TreasureMgr::Destroy()
{
    // ע��������Ϣ
    m_pCmdMgr->UnRegister("NS_GetActClanTreasure",    (NETMSGPROC)m_Trunk.sfp2(&TreasureMgr::OnNS_GetActClanTreasure));
    m_pCmdMgr->UnRegister("NS_NewActivedTreasure",    (NETMSGPROC)m_Trunk.sfp2(&TreasureMgr::OnNS_NewActivedTreasure));
    m_pCmdMgr->UnRegister("NS_ActiveTreasure",        (NETMSGPROC)m_Trunk.sfp2(&TreasureMgr::OnNS_ActiveTreasure));

    // ע����Ϸ�¼�
    m_pFrameMgr->UnRegisterEventHandler(_T("GetNameByNameID"), (FRAMEEVENTPROC)m_Trunk.sfp1(&TreasureMgr::OnGetNameByNameID));

    // �����Ϣ
    ClearData();
}

TreasureMgr g_TreasureMgr;
TreasureMgr* TreasureMgr::Inst()
{
    return &g_TreasureMgr;
}

VOID TreasureMgr::ClearData()
{
    // �ͷſռ�
    for (_MapTreasure::iterator it = m_mapTreasurebyTrID.begin();
        it != m_mapTreasurebyTrID.end(); ++it)
        SAFE_DELETE(it->second);

    // �������
    m_mapTreasurebyTrID.clear();
    m_mapTreasurebyItemID.clear();
    for (INT i = ECLT_BEGIN; i < ECLT_NUM; ++i)
    {
        m_mapTreasurebyTrIDA[i].clear();
        m_mapTreasurebyItemIDA[i].clear();
        m_mapActivedbyTrIDA[i].clear();
        m_mapUnActivedbyTrIDA[i].clear();
    }

    m_mulmapNameIDReq.clear();
}

VOID TreasureMgr::LoadData()
{
    const map<DWORD, tagTreasureProto>* mapProto =
        ItemProtoData::Inst()->GetTreasureProto();

    map<DWORD, tagTreasureProto>::const_iterator cit;

    for (cit = mapProto->begin(); cit != mapProto->end(); ++cit)
    {
        // ����ռ�
        tagTreasureMgrData* pData = new tagTreasureMgrData();
        pData->dwTreasureID     =   cit->second.dwTreasureID;
        pData->dwItemID         =   cit->second.dwItemID;
        pData->dwShopID         =   cit->second.dwShopID;
        pData->eClanType        =   cit->second.eCtype;
        pData->nConNeed         =   cit->second.nConNeed;
        pData->bActived         =   false;
        pData->strNamePrefix    =   g_StrTable[_T("TreasureDefault")];    // Ĭ��Ϊ"����"

        const tagItemDisplayInfo* itemDis = ItemProtoData::Inst()->FindItemDisplay(cit->second.dwItemID);
        //ASSERT(P_VALID(itemDis));
        if (P_VALID(itemDis))
            pData->strNameSuffix=    itemDis->szName;

        // ��������
        if (!m_mapTreasurebyTrID.insert(make_pair(pData->dwTreasureID, pData)).second) SAFE_DELETE(pData);
        m_mapTreasurebyItemID.insert(make_pair(pData->dwItemID, pData));
        if (!m_mapTreasurebyTrIDA[pData->eClanType].insert(make_pair(pData->dwTreasureID, pData)).second) SAFE_DELETE(pData);
        m_mapTreasurebyItemIDA[pData->eClanType].insert(make_pair(pData->dwItemID, pData));
        if (!m_mapUnActivedbyTrIDA[pData->eClanType].insert(make_pair(pData->dwTreasureID, pData)).second) SAFE_DELETE(pData);
    }
}

DWORD TreasureMgr::OnGetNameByNameID( tagGetNameByNameID* msg )
{
    if (!msg->bResult)
        return 0;

    // ���������б��Ƿ���,�еĻ���������ڸ�������ǰ׺
    _MultiMapTrea::_Paircc range = m_mulmapNameIDReq.equal_range(msg->dwNameID);
    for (_MultiMapTrea::const_iterator cit = range.first;
        cit != range.second; ++cit)
        cit->second->strNamePrefix      =   msg->strName;

    // ɾ�������������
    m_mulmapNameIDReq.erase(msg->dwNameID);
	
	return 0;
}

DWORD TreasureMgr::OnNS_GetActClanTreasure( tagNS_GetActClanTreasure* pNetCmd, DWORD dwParam )
{
    // ���������ص��Ѽ����䱦�б�
    tagTreasureData treaData;

    for (INT i = 0, offset = 0; i < pNetCmd->n16ActTreasureNum; ++i, ++offset)
    {
        memcpy(&treaData, pNetCmd->byData + offset * sizeof(tagTreasureData), sizeof(tagTreasureData));

        // ���뱾�������䱦�б�
        _MapTreasure::iterator it = m_mapTreasurebyTrID.find((DWORD)treaData.n16TreasureID);
        if (it != m_mapTreasurebyTrID.end())
        {
            it->second->bActived        =   true;
            // ��nameid����������,�Ҳ������������б��ȴ��¼�����
            it->second->strNamePrefix   =   PlayerNameTab::Inst()->FindNameByNameID(treaData.dwNamePrefixID);
            if (it->second->strNamePrefix.empty())
                m_mulmapNameIDReq.insert(make_pair(treaData.dwNamePrefixID, it->second));

            // ���ļ����б�
            m_mapActivedbyTrIDA[it->second->eClanType].insert(make_pair(it->second->dwTreasureID, it->second));
            m_mapUnActivedbyTrIDA[it->second->eClanType].erase(it->second->dwTreasureID);
        }
    }
	return 0;
}

DWORD TreasureMgr::OnNS_NewActivedTreasure( tagNS_NewActivedTreasure* pNetCmd, DWORD dwParam )
{
    // �õ��µļ�����䱦
    _MapTreasure::const_iterator cit = m_mapTreasurebyTrID.find((DWORD)pNetCmd->n16TreasureID);
    if (cit != m_mapTreasurebyTrID.end())
    {
        cit->second->bActived       =   true;
        cit->second->dwNameID       =   pNetCmd->dwNameID;
        // ��nameid����������,�Ҳ������������б��ȴ��¼�����
        cit->second->strNamePrefix  =   PlayerNameTab::Inst()->FindNameByNameID(pNetCmd->dwNameID);
        if (cit->second->strNamePrefix.empty())
            m_mulmapNameIDReq.insert(make_pair(pNetCmd->dwNameID, cit->second));

        // ���ļ����б�
        m_mapActivedbyTrIDA[cit->second->eClanType].insert(make_pair(cit->second->dwTreasureID, cit->second));
        m_mapUnActivedbyTrIDA[cit->second->eClanType].erase(cit->second->dwTreasureID);
    }

    // ������Ϸ�¼�
    tagNewActivedTreasure e(_T("tagNewActivedTreasure"), NULL);
    e.dwTreasureid      =   (DWORD)pNetCmd->n16TreasureID;
    m_pFrameMgr->SendEvent(&e);
	return 0;
}

DWORD TreasureMgr::OnNS_ActiveTreasure( tagNS_ActiveTreasure* pNetCmd, DWORD dwParam )
{
    // ���ͳ��Լ����䱦��ķ�����Ϣ
    tagActTreasureRezult e(_T("tagActTreasureRezult"), NULL);
    e.dwTreasureid      =   (DWORD)pNetCmd->u16TreasureID;
    e.dwErrCode         =   pNetCmd->dwErrCode;
    m_pFrameMgr->SendEvent(&e);
	return 0;
}

VOID TreasureMgr::ActiveTreasure( DWORD dwTreasureID )
{
    // ���ͼ����䱦����
    tagNC_ActiveTreasure e;
    e.u16TreasureID     =   (UINT16)dwTreasureID;
    if (m_pSession->IsConnect())
        m_pSession->Send(&e);
}

DWORD TreasureMgr::GetItemIDbyTreaID( DWORD dwTreaID )
{
    _MapTreasure::const_iterator cit;
    if ((cit = m_mapTreasurebyTrID.find(dwTreaID)) != m_mapTreasurebyTrID.end())
        return cit->second->dwItemID;
    else
        return GT_INVALID;
}

DWORD TreasureMgr::GetTreaIDbyItemID( DWORD dwItemID, ECLanType eType )
{
    _MultiMapTrea::_Paircc          range;
    _MultiMapTrea::const_iterator   cit;

    range = m_mapTreasurebyItemID.equal_range(dwItemID);

    for (cit = range.first; cit != range.second; ++cit)
    {
        if (cit->second->eClanType == eType)
        {
            return cit->second->dwTreasureID;
        }
    }

    return GT_INVALID;
}

bool TreasureMgr::IsTreaActivedbyItemID( DWORD dwItemID, ECLanType eType )
{
    _MultiMapTrea::_Paircc          range;
    _MultiMapTrea::const_iterator   cit;

    range = m_mapTreasurebyItemID.equal_range(dwItemID);

    for (cit = range.first; cit != range.second; ++cit)
    {
        if (cit->second->eClanType == eType)
        {
            return cit->second->bActived;
        }
    }

    return false;
}

tstring TreasureMgr::GetTreaNamebyItemID( DWORD dwItemID, ECLanType eType )
{
    DWORD treaID = GetTreaIDbyItemID(dwItemID, eType);

    if (treaID != GT_INVALID)
    {
        return GetTreasureName(treaID);
    }
    else
    {
        return _T("");
    }
}

tstring TreasureMgr::GetTreasureName( DWORD dwTreaID )
{
    _MapTreasure::const_iterator cit;
    if ((cit = m_mapTreasurebyTrID.find(dwTreaID)) != m_mapTreasurebyTrID.end())
        return cit->second->strNamePrefix + cit->second->strNameSuffix;
    else
        return _T("");
}

INT TreasureMgr::GetContributNeed( DWORD dwTreaID )
{
    _MapTreasure::const_iterator cit;
    if ((cit = m_mapTreasurebyTrID.find(dwTreaID)) != m_mapTreasurebyTrID.end())
        return cit->second->nConNeed;
    else
        return GT_INVALID;
}
