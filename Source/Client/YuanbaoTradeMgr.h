#pragma once

#include "..\WorldDefine\msg_mall.h"
#include "CurrencyMgr.h"

//! Ԫ�������˻�
struct tagRoleYBAccount
{
    DWORD       dwRoleID;       //!< ���ID
    INT         nYuanBao;       //!< Ԫ������
    INT64       nMoney;         //!< ��Ǯ����
    BOOL        bHasSellOrder;  //!< �Ƿ��м��۶���
    BOOL        bHasBuyOrder;   //!< �Ƿ����չ�����        

    tagRoleYBAccount() {memset(this, 0, sizeof(*this));}
};

//! Ԫ��ͬ����������
enum EYuanBaoSynRet
{
    EYBR_SaveYB2Account         =   0,
    EYBR_DepositAccountYB       =   1,
    EYBR_SaveSilver2Account     =   2,
    EYBR_DepositAccountSilver   =   3,
    EYBR_SubmitSellOrder        =   4,
    EYBR_SubmitBuyOrder         =   5,
    EYBR_DeleteOrder            =   6,

    EYBR_GetSellPriceList       =   7,
    EYBR_GetBuyPriceList        =   8,
    EYBR_GetYBAccount           =   9,
    EYBR_SynSellPriceList       =   10,
    EYBR_SynBuyPriceList        =   11,
    EYBR_GetYuanbaoOrder        =   12,
    EYBR_SynAccoutYB            =   13,
    EYBR_SynAccoutSilver        =   14,
    EYBR_SynAccout              =   15
};

//! ͬ��Ԫ����Ϣ�ķ���ֵ
struct tagYBReturnEvnt : public tagGameEvent
{
    EYuanBaoSynRet  eType;      //!< ��������
    bool            bRet;       //!< �Ƿ�ɹ�
    DWORD           dwErrCode;  //!< ������
    tagYBReturnEvnt(LPCTSTR szEventName, GameFrame* pSenderFrame)
        : tagGameEvent(szEventName, pSenderFrame) {}
};

//! \class YuanbaoTradeMgr
//! \brief Ԫ�����׹�����
//! \author hyu
//! \date 2009-3-16
class YuanbaoTradeMgr
{
public:
    YuanbaoTradeMgr(void);
    ~YuanbaoTradeMgr(void);

    VOID Init();
    VOID Destroy();

    //! Singleton
    static YuanbaoTradeMgr* Inst();

    typedef map<INT, tagYBPriceList*> _MapPrice;
    typedef map<DWORD, tagYuanBaoOrder*> _MapOrder;

    
    //------------------------------------------------------------------------
    // ����ӿ�
    //------------------------------------------------------------------------

    //! ���Ԫ���˻���Ǯ
    INT64 GetMoneyInAccount() { return m_YBAccount.nMoney; }

    //! ���Ԫ���˻�Ԫ��
    INT GetYuanBaoInAccount() { return m_YBAccount.nYuanBao; }

    //! ��ü����б�
    const _MapPrice* GetPrice2SellList() { return &m_mapSellListByPrice; }

    //! ����չ��б�
    const _MapPrice* GetPrice2BuyList() { return &m_mapBuyListByPrice; }

    //! ��ö����б�
    const _MapOrder* GetID2OrderList() { return &m_mapOrderByID; }

private:
    // ��������
    YuanbaoTradeMgr(const YuanbaoTradeMgr&);
    YuanbaoTradeMgr& operator = (const YuanbaoTradeMgr&);


    //------------------------------------------------------------------------
    // �����¼�
    //------------------------------------------------------------------------
public:
    //! ��������˻���Ԫ��
    //! \param nNum Ԫ������
    VOID BeginNC_SaveYB2Account(INT nNum);

    //! �ӽ����˻���ȡԪ��
    //! \param nNum Ԫ������
    VOID BeginNC_DepositAccountYB(INT nNum);

    //! ��������˻����Ǯ
    //! \param nNum ��Ǯ��
    VOID BeginNC_SaveSilver2Account(INT64 nNum);

    //! �ӽ����˻���ȡ��Ǯ
    //! \param nNum ��Ǯ��
    VOID BeginNC_DepositAccountSilver(INT64 nNum);

    //! ��ȡ����Ԫ�����׼۸���б�ͽ����˻���Ϣ
    VOID BeginNC_SynYBTradeInfo();

    //! �ύԪ�����۶���
    VOID BeginNC_SubmitSellOrder(INT nNum, INT64 nPrice);

    //! �ύԪ���չ�����
    VOID BeginNC_SubmitBuyOrder(INT nNum, INT64 nPrice);

    //! �����Ҷ���
    VOID BeginNC_GetYuanbaoOrder();

    //! ɾ������
    VOID BeginNC_DeleteOrder(DWORD dwOrderID, EYBOTYPE eYBOType);
private:
    //! ͬ�������˻���Ԫ����
    DWORD OnNS_SynAccoutYB(tagNS_SynAccoutYB* msg, DWORD);

    //! ͬ�������˻��Ľ�Ǯ��
    DWORD OnNS_SynAccoutSilver(tagNS_SynAccoutSilver* msg, DWORD);

    //! ͬ�������˻�
    DWORD OnNS_SynAccout(tagNS_SynAccout* msg, DWORD);

    //! ��������˻���Ԫ��(����)
    DWORD OnNS_SaveYB2Account(tagNS_SaveYB2Account* msg, DWORD);

    //! �ӽ����˻���ȡԪ��(����)
    DWORD OnNS_DepositAccountYB(tagNS_DepositAccountYB* msg, DWORD);

    //! ��������˻����Ǯ(����)
    DWORD OnNS_SaveSilver2Account(tagNS_SaveSilver2Account* msg, DWORD);
    
    //! �ӽ����˻���ȡ��Ǯ(����)
    DWORD OnNS_DepositAccountSilver(tagNS_DepositAccountSilver* msg, DWORD);

    //! ��ȡ����Ԫ�����׼۸���б�ͽ����˻���Ϣ(����)
    DWORD OnNS_GetSellPriceList(tagNS_GetSellPriceList* msg, DWORD);
    DWORD OnNS_GetBuyPriceList(tagNS_GetBuyPriceList* msg, DWORD);
    DWORD OnNS_GetYBAccount(tagNS_GetYBAccount* msg, DWORD);

    //! ͬ�����ۼ۸��б�
    DWORD OnNS_SynSellPriceList(tagNS_SynSellPriceList* msg,DWORD);

    //! ͬ������۸��б�
    DWORD OnNS_SynBuyPriceList(tagNS_SynBuyPriceList* msg, DWORD);

    //! �ύԪ�����۶���(����)
    DWORD OnNS_SubmitSellOrder(tagNS_SubmitSellOrder* msg, DWORD);
    
    //! �ύԪ���չ�����(����)
    DWORD OnNS_SubmitBuyOrder(tagNS_SubmitBuyOrder* msg, DWORD);
    
    //! �����Ҷ���(����)
    DWORD OnNS_GetYuanbaoOrder(tagNS_GetYuanbaoOrder* msg, DWORD);

    //! ɾ������(����)
    DWORD OnNS_DeleteOrder(tagNS_DeleteOrder* msg, DWORD);

    //------------------------------------------------------------------------
    // ������������
    //------------------------------------------------------------------------

    //! ���³��ۼ۸��б���,�������޴������
    VOID UpdateSellPriceList(tagYBPriceList* pList);
    VOID ClearSellPriceList();

    //! �չ��۸��б���,�������޴������
    VOID UpdateBuyPriceList(tagYBPriceList* pList);
    VOID ClearBuyPriceList();

    //! ������Ҷ����б���
    VOID InsertOrderList(tagYuanBaoOrder* pOrder);
    VOID ClearOrderList();

    //! ���Ԫ�������˻�
    void ClearYBAccount();

    //------------------------------------------------------------------------
    // ����
    //------------------------------------------------------------------------

    _MapPrice                   m_mapSellListByPrice;       //!< ���ۼ۸��б���

    _MapPrice                   m_mapBuyListByPrice;        //!< �չ��۸��б���

    _MapOrder                   m_mapOrderByID;             //!< ��Ҷ����б���

    tagRoleYBAccount            m_YBAccount;                //!< ���Ԫ�������˻�

    //------------------------------------------------------------------------
    // ����
    //------------------------------------------------------------------------
    TSFPTrunk<YuanbaoTradeMgr>  m_Trunk;
    TObjRef<Util>				m_pUtil;
    TObjRef<NetCmdMgr>			m_pCmdMgr;
    TObjRef<GameFrameMgr>		m_pFrameMgr;
    TObjRef<NetSession>			m_pSession;
    CurrencyMgr*                m_pCurrency;
};
