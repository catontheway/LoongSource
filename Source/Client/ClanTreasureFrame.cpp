#include "StdAfx.h"
#include "ClanTreasureFrame.h"
#include "CombatSysUtil.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "ItemProtoData.h"

ClanTreasureFrame::ClanTreasureFrame(void) : m_Trunk(this), m_nSelected(GT_INVALID), m_nCurrPage(1)
{
}

ClanTreasureFrame::~ClanTreasureFrame(void)
{
}

BOOL ClanTreasureFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
    BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);

    m_pFrameMgr->RegisterEventHandle(_T("SetClanTreasureOpen"),     (FRAMEEVENTPROC)m_Trunk.sfp1(&ClanTreasureFrame::OnOpenClanTreasure));
    m_pFrameMgr->RegisterEventHandle(_T("SetClanTreasureClose"),    (FRAMEEVENTPROC)m_Trunk.sfp1(&ClanTreasureFrame::OnCloseClanTreasure));
    m_pFrameMgr->RegisterEventHandle(_T("tagNewActivedTreasure"),   (FRAMEEVENTPROC)m_Trunk.sfp1(&ClanTreasureFrame::OnNewActivedTreasure));
    m_pFrameMgr->RegisterEventHandle(_T("tagActTreasureRezult"),    (FRAMEEVENTPROC)m_Trunk.sfp1(&ClanTreasureFrame::OnActiveTreasureRet));

    return bRet;
}

BOOL ClanTreasureFrame::Destroy()
{
    BOOL bRet = GameFrame::Destroy();

    m_pFrameMgr->UnRegisterEventHandler(_T("SetClanTreasureOpen"),  (FRAMEEVENTPROC)m_Trunk.sfp1(&ClanTreasureFrame::OnOpenClanTreasure));
    m_pFrameMgr->UnRegisterEventHandler(_T("SetClanTreasureClose"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ClanTreasureFrame::OnCloseClanTreasure));
    m_pFrameMgr->UnRegisterEventHandler(_T("tagNewActivedTreasure"),(FRAMEEVENTPROC)m_Trunk.sfp1(&ClanTreasureFrame::OnNewActivedTreasure));
    m_pFrameMgr->UnRegisterEventHandler(_T("tagActTreasureRezult"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ClanTreasureFrame::OnActiveTreasureRet));

    /* ��������ô��� */
    if (P_VALID(m_pWnd))
    {
        m_pGUI->UnRegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&ClanTreasureFrame::EventHandler));
        m_pGUI->AddToDestroyList(m_pWnd);
        m_pWnd = NULL;
    }

    return bRet;
}

BOOL ClanTreasureFrame::ReloadUI()
{
    // ͼ���ļ�
    strSelectIcon   =   _T("data\\ui\\Common\\L_icon-l2.bmp");
    strItemIcon     =   _T("data\\ui\\sys\\l_diban-a.bmp");

    char    cbuff[128];
    TCHAR   tbuff[128];

    // ���ؽ���
    XmlElement ele;
    tstring strPath = g_strLocalPath + _T("\\ui\\racetreasure.xml");
    m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());

    // ���Ŀؼ�
    for (INT i = 1; i <= MAX_TREASURE_PLACE; ++i)
    {
        _snprintf(cbuff, sizeof(cbuff)/sizeof(char), "rtreasure_win\\rtreasure_backpic\\rtreasure_back\\weaponback%d", i);
        m_pGUI->ChangeXml(&ele, cbuff, "ClassName", "StaticEx");
        _snprintf(cbuff, sizeof(cbuff)/sizeof(char), "rtreasure_win\\rtreasure_backpic\\rtreasure_back\\weaponback%d\\weaponback", i);
        m_pGUI->ChangeXml(&ele, cbuff, "ClassName", "StaticEx");
        _snprintf(cbuff, sizeof(cbuff)/sizeof(char), "rtreasure_win\\rtreasure_backpic\\rtreasure_back\\weaponback%d\\weaponback\\new", i);
        m_pGUI->ChangeXml(&ele, cbuff, "ClassName", "ItemButton");
    }

    // ��������
    m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);

    for (INT index = 0; index < MAX_TREASURE_PLACE; ++index)
    {
        INT i = index + 1;

        // ͼ��װ�
        _sntprintf(tbuff, sizeof(tbuff)/sizeof(TCHAR), _T("rtreasure_backpic\\rtreasure_back\\weaponback%d"), i);
        m_pStcItemPad[index]    =   (StaticEx*)m_pWnd->GetChild(tbuff);

        // �䱦��
        _sntprintf(tbuff, sizeof(tbuff)/sizeof(TCHAR), _T("rtreasure_backpic\\rtreasure_back\\weaponback%d\\wname"), i);
        m_pStcName[index]       =   (GUIStatic*)m_pWnd->GetChild(tbuff);

        // ���Ĺ���ֵ
        _sntprintf(tbuff, sizeof(tbuff)/sizeof(TCHAR), _T("rtreasure_backpic\\rtreasure_back\\weaponback%d\\state"), i);
        m_pStcStus[index]       =   (GUIStatic*)m_pWnd->GetChild(tbuff);

        // �䱦ͼ��
        _sntprintf(tbuff, sizeof(tbuff)/sizeof(TCHAR), _T("rtreasure_backpic\\rtreasure_back\\weaponback%d\\weaponback"), i);
        m_pStcIcon[index]       =   (StaticEx*)m_pWnd->GetChild(tbuff);

        // �䱦��ť
        _sntprintf(tbuff, sizeof(tbuff)/sizeof(TCHAR), _T("rtreasure_backpic\\rtreasure_back\\weaponback%d\\weaponback\\new"), i);
        m_pIbtTrea[index]       =   (ItemButton*)m_pWnd->GetChild(tbuff);
    }

    // �����䱦
    m_pBtActiveTreasure         =   (GUIButton*)m_pWnd->GetChild(_T("rtreasure_backpic\\activebutt"));

    // �䱦ʣ�༤�����
    m_pStcRestnum               =   (GUIStatic*)m_pWnd->GetChild(_T("rtreasure_backpic\\times"));

    // ҳ��
    m_pStcPage                  =   (GUIStatic*)m_pWnd->GetChild(_T("rtreasure_backpic\\new00"));
    // ǰһҳ
    m_pBtPrevPage               =   (GUIButton*)m_pWnd->GetChild(_T("rtreasure_backpic\\new"));
    // ��һҳ
    m_pBtNextPage               =   (GUIButton*)m_pWnd->GetChild(_T("rtreasure_backpic\\new0"));

    // �رհ�ť
    m_pBtClose                  =   (GUIButton*)m_pWnd->GetChild(_T("rtreasure_backpic\\closebutt"));



    // ע���¼�������
    m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&ClanTreasureFrame::EventHandler));

    // Ĭ�Ϲرս���
    m_pWnd->SetInvisible(TRUE);

    // Ĭ�ϼ��ť������
    m_pBtActiveTreasure->SetEnable(false);

    // Ĭ�Ͽɼ������Ϊ0
    _sntprintf(tbuff, sizeof(tbuff)/sizeof(TCHAR), _T("%s0"), g_StrTable[_T("ClanTreaActCount")]);
    m_pStcRestnum->SetText(tbuff);

    return TRUE;
}

VOID ClanTreasureFrame::Update()
{

}

DWORD ClanTreasureFrame::OnGameEvent( tagGameEvent* pGameEvent )
{
	return 0;
}

DWORD ClanTreasureFrame::EventHandler( tagGUIEvent* pEvent )
{
    GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

    switch (pEvent->eEvent)
    {
    case EGUIE_Click:
        if (pWnd == m_pBtClose)
        {
            // �رս���
            ShowWnd(false);
        }
        else if (pWnd == m_pBtNextPage)
        {
            // ��һҳ
        }
        else if (pWnd == m_pBtPrevPage)
        {
            // ��һҳ
        }
        else if (pWnd == m_pBtActiveTreasure)
        {
            // �����䱦
            BeginActive();
        }
    case EGUIE_Drag:
        // ItemButton �������
        for (INT i = 0; i < MAX_TREASURE_PLACE; ++i)
            if (pWnd == m_pIbtTrea[i])
                SelectItemPad(i);
        break;
    case EGUIE_MoveIntoCtrl:
        // ��ʾtips
        for (INT i = 0; i < MAX_TREASURE_PLACE; ++i)
            if (pWnd == m_pIbtTrea[i])
            {
                const tagTreasureMgrData* pTrea = GetTreaByPos(m_nCurrPage, i);
                if (P_VALID(pTrea))
                {
                    const tagItemProto* pItem = ItemProtoData::Inst()->FindItemProto(pTrea->dwItemID);
                    BYTE quality = P_VALID(pItem) ? pItem->byQuality : 0;

                    if (pTrea->bActived)
                    {
                        ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pTrea->dwItemID, quality, pTrea->dwNameID).c_str());
                    }
                    else
                    {
                        ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pTrea->dwItemID, quality).c_str());
                    }
                }
            }
        break;
    case EGUIE_MoveOutOfCtrl:
        // �ر�tips
        ShowTip(pWnd, NULL);
        break;
    }

	return 0;
}

DWORD ClanTreasureFrame::OnOpenClanTreasure( tagOpenClanTreasure* pGameEvent )
{
    m_eClan = pGameEvent->eClanTypeValue;
    ShowWnd(true);

	return 0;
}

DWORD ClanTreasureFrame::OnCloseClanTreasure( tagGameEvent* pGameEvent )
{
    ShowWnd(false);

	return 0;
}

VOID ClanTreasureFrame::ShowWnd( bool bval )
{
    m_pWnd->SetInvisible(!bval);

    if (bval)
    {
        m_pWnd->FlipToTop();

        m_pFrameMgr->SendEvent(&tagGameEvent(_T("ClanTreaOpen"), this));

        // ����
        UpdateClanTreasure();
    }
    else
    {
        m_pFrameMgr->SendEvent(&tagGameEvent(_T("ClanTreaClose"), this));
    }
}

VOID ClanTreasureFrame::SelectItemPad( INT nIndex )
{
    if (nIndex >= 0 && nIndex < MAX_TREASURE_PLACE)
    {
        if (m_nSelected >= 0 && m_nSelected < MAX_TREASURE_PLACE)
        {
            // �����ǰѡ���
            m_pStcItemPad[m_nSelected]->SetPicFile(strItemIcon);
        }
        // ������ѡ���
        m_pStcItemPad[nIndex]->SetPicFile(strSelectIcon);
        m_nSelected = nIndex;

        // ���ü��ť
        const tagTreasureMgrData* pTrea = GetTreaByPos(m_nCurrPage, m_nSelected);
        if (P_VALID(pTrea) && !pTrea->bActived)
            m_pBtActiveTreasure->SetEnable(true);
        else
            m_pBtActiveTreasure->SetEnable(false);
    }
    else if (nIndex == GT_INVALID)
    {
        //m_pStcItemPad[m_nSelected]->SetPicFile(strItemIcon);
        m_nSelected =   GT_INVALID;
    }
}

DWORD ClanTreasureFrame::OnNewActivedTreasure( tagNewActivedTreasure* pGameEvent )
{
    UpdateClanTreasure();

	return 0;
}

DWORD ClanTreasureFrame::OnActiveTreasureRet( tagActTreasureRezult* pGameEvent )
{
    // ���������䱦��Ĵ�����
    switch (pGameEvent->dwErrCode)
    {
    case E_FrameHall_Success:
        // �ɹ�
        CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("ClanTreaTip0")]);
    	break;
    case E_FrameHall_ItemIsNotTreasure:
        // ��Ʒ�����䱦
        CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("ClanTreaTip1")]);
        break;
    case E_FrameHall_TreasureAlreadyAct:
        // �䱦�Ѽ���
        CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("ClanTreaTip2")]);
        break;
    case E_FrameHall_ActCountNotEnough:
        // �����������
        CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("ClanTreaTip3")]);
        break;
    case E_FrameHall_ClanColNotEnough:
        // ���幱�ײ���
        CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("ClanTreaTip4")]);
        break;
    case E_FrameHall_UnknownError:
        // δ֪����
        CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("ClanTreaTip5")]);
        break;
    }

	return 0;
}

VOID ClanTreasureFrame::UpdateClanTreasure()
{
    InitTreasure();
    RefreshToUI();
}

VOID ClanTreasureFrame::InitTreasure()
{
    typedef TreasureMgr::_MapTreasure::const_iterator CIT;
    const TreasureMgr::_MapTreasure actived = TreasureMgr::Inst()->GetAllActived(m_eClan);
    const TreasureMgr::_MapTreasure unactived = TreasureMgr::Inst()->GetAllUnActived(m_eClan);

    // ��ȫ������ת�뱾��m_vecTreasure
    m_vecTreasure.clear();

    // �Ѽ�����䱦����
    for (CIT cit = actived.begin(); cit != actived.end(); ++cit)
        m_vecTreasure.push_back(cit->second);

    // δ�����䱦�ں�
    for (CIT cit = unactived.begin(); cit != unactived.end(); ++cit)
        m_vecTreasure.push_back(cit->second);

    // ����ҳ����
    const INT TreaCount = m_vecTreasure.size();
    if (0 == TreaCount)
    {
        m_nPageCount    =   1;
    }
    else
    {
        // ������ҳ����ҳ
        const INT res   = (INT)(TreaCount / MAX_TREASURE_PLACE);
        const INT rest  = (INT)(TreaCount % MAX_TREASURE_PLACE);
        m_nPageCount = (0 == rest) ? res : res + 1;
    }

    // ѡ�е�һҳ
    m_nCurrPage         =   1;

    // ����ť������
    m_pBtActiveTreasure->SetEnable(false);
}

VOID ClanTreasureFrame::RefreshToUI()
{
    // ��ս���
    for (INT i = 0; i != MAX_TREASURE_PLACE; ++i)
    {
        m_pIbtTrea[i]->RefreshItem();
        m_pStcItemPad[i]->SetPicFile(strItemIcon);
        m_pStcName[i]->SetText(_T(""));
        m_pStcStus[i]->SetText(_T(""));
    }

    SelectItemPad();

    // ˢ��ҳ��
    TCHAR tcbuff[16];
    _itot(m_nCurrPage, tcbuff, 10);
    m_pStcPage->SetText(tcbuff);

    // ���б��򷵻�
    if (m_vecTreasure.empty())
        return;

    // ���ݵ�ǰҳ�����б���ʾ��Χ
    typedef vector<tagTreasureMgrData*>::const_iterator CIT;
    INT treaCount   =   m_vecTreasure.size();

    INT nBegin      =   (m_nCurrPage - 1) * MAX_TREASURE_PLACE;
    INT nEnd        =   nBegin + MAX_TREASURE_PLACE;
    if (nEnd > treaCount)
        nEnd        =   treaCount;

    CIT citBegin    =   m_vecTreasure.begin() + nBegin;
    CIT citEnd      =   m_vecTreasure.begin() + nEnd;

    // ��ʾ���ؼ�
    INT index   =   0;
    for (CIT cit = citBegin; cit != citEnd; ++cit)
    {
        // ���䱦��䵽��ť
        m_pIbtTrea[index]->RefreshItem((*cit)->dwItemID);

        // ��ʾ����
        tstring strName = (*cit)->strNamePrefix + (*cit)->strNameSuffix;
        m_pStcName[index]->SetText(strName.c_str());

        // ��ʾ����״̬/����ֵ
        TCHAR tbuff[128];
        if ((*cit)->bActived)
            _tcscpy_s(tbuff, sizeof(tbuff)/sizeof(TCHAR), g_StrTable[_T("ClanTreaStus1")]);
        else
            _sntprintf(tbuff, sizeof(tbuff)/sizeof(TCHAR), _T("%s %d"), g_StrTable[_T("ClanTreaStus2")], (*cit)->nConNeed);
        m_pStcStus[index]->SetText(tbuff);
        
        // ��һ���ؼ�
        ++index;
    }

    // ��ʾʣ�༤�����
    RefreshActCntToUI();
}

VOID ClanTreasureFrame::RefreshActCntToUI()
{
    TCHAR tbuff[64];
    _sntprintf(tbuff, sizeof(tbuff)/sizeof(TCHAR), _T("%s%d"), g_StrTable[_T("ClanTreaActCount")], ReputeMgr::Inst()->GetActCount(m_eClan));
    m_pStcRestnum->SetText(tbuff);
}

VOID ClanTreasureFrame::PagePrev()
{
    if (m_nCurrPage > 1)
    {
        // ҳ���һ
        --m_nCurrPage;

        // ˢ�½���
        RefreshToUI();
    }
}

VOID ClanTreasureFrame::PageNext()
{
    if (m_nCurrPage < m_nPageCount)
    {
        // ҳ���һ
        ++m_nCurrPage;

        // ˢ�½���
        RefreshToUI();
    }
}

VOID ClanTreasureFrame::BeginActive()
{
    // ��ѡ���򷵻�
    if (GT_INVALID == m_nSelected)
        return;

    // �ҵ���ǰѡ���itemid
    const tagTreasureMgrData* currdata = GetTreaByPos(m_nCurrPage, m_nSelected);
    if (P_VALID(currdata))
    {
        // ����,������Ϣ��OnActiveTreasureRet�д���
        TreasureMgr::Inst()->ActiveTreasure(currdata->dwTreasureID);
    }
}

const tagTreasureMgrData* ClanTreasureFrame::GetTreaByPos( INT currPage, INT pos )
{
    // �ж�pos�Ƿ���Ч
    if (pos < 0 || pos >= MAX_TREASURE_PLACE)
        return NULL;

    // �ҵ���ǰѡ���itemid
    INT treaCount   =   m_vecTreasure.size();
    INT index = (currPage - 1) * MAX_TREASURE_PLACE + pos;
    if (index >= treaCount)
        index = treaCount;

    typedef vector<tagTreasureMgrData*>::const_iterator CIT;
    CIT citSelect   =   m_vecTreasure.begin() + index;

    // �жϴ�id�Ƿ���Ч
    if (citSelect < m_vecTreasure.begin() || citSelect >= m_vecTreasure.end())
        return NULL;
    else
        return *citSelect;
}
