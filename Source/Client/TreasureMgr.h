#pragma once

#include "ItemProtoData.h"
#include "ItemMgr.h"
#include "PlayerNameTab.h"
#include "..\WorldDefine\msg_famehall.h"

//! ������ҳ��Լ����䱦��������
struct tagActTreasureRezult : tagGameEvent
{
    DWORD           dwTreasureid;   //!< �䱦id
    DWORD           dwErrCode;      //!< �����룬�μ�msg_famehall.h

    tagActTreasureRezult(LPCTSTR szEventName, GameFrame* pSenderFrame)
        : tagGameEvent(szEventName, pSenderFrame) {}
};

//! �������㲥���䱦����ɹ�����¼�
struct tagNewActivedTreasure : tagGameEvent
{
    DWORD        dwTreasureid;      //!< �䱦id

    tagNewActivedTreasure(LPCTSTR szEventName, GameFrame* pSenderFrame)
        : tagGameEvent(szEventName, pSenderFrame) {}
};

//! \struct tagTreasureMgrData
//! \brief TreasureMgrʹ�õ������ṹ
struct tagTreasureMgrData
{
    ECLanType       eClanType;      //!< ��������
    DWORD           dwTreasureID;   //!< �䱦id
    DWORD           dwItemID;       //!< ��Ӧ��itemid
    DWORD           dwNameID;       //!< ����ID
    tstring         strNamePrefix;  //!< ����ǰ׺����Դ��NameID
    tstring         strNameSuffix;  //!< ���ֺ�׺��Item��������
    INT             nConNeed;       //!< ������������
    bool            bActived;       //!< ����״̬
    DWORD           dwShopID;       //!< �̵�ID
};

//! \class TreasureMgr
//! \brief �����䱦������
//! \detail �����䱦Ҳ����Ʒ��Ҳ������װ��������ͨ���䱦idȡitem,Ҳ����ͨ��item���Ƿ����䱦
//! \author hyu
//! \date 2009-3-6
class TreasureMgr
{
public:
    typedef map<DWORD, tagTreasureMgrData*>      _MapTreasure;
    typedef multimap<DWORD, tagTreasureMgrData*> _MultiMapTrea;    

    TreasureMgr(void);
    ~TreasureMgr(void);

    static TreasureMgr* Inst();

    VOID Init();
    VOID Update();
    VOID Destroy();

    //! ���������䱦
    //! \param dwTreasureID �䱦id
    //! \brief �����������ͼ����䱦����,���ؽ����OnNS_ActiveTreasure
    //! \sa OnNS_ActiveTreasure
    VOID ActiveTreasure(DWORD dwTreasureID);

    //! ͨ��TreasureID����ItemID,��Ч����GT_INVALID
    DWORD GetItemIDbyTreaID(DWORD dwTreaID);

    //! ͨ��ItemID�ж��Ƿ���Treasure,�ǵĻ�����TreasureID,��Ч����GT_INVALID
    DWORD GetTreaIDbyItemID(DWORD dwItemID, ECLanType eType);

    //! �жϵ�ǰ�������䱦�Ƿ��Ѽ���
    bool IsTreaActivedbyItemID(DWORD dwItemID, ECLanType eType);

    //! ͨ��ItemID�ж��Ƿ���Treasure,�ǵĻ������䱦ȫ��,��Ч���ؿմ�
    tstring GetTreaNamebyItemID(DWORD dwItemID, ECLanType eType);

    //! �õ��䱦ȫ��
    tstring GetTreasureName(DWORD dwTreaID);

    //! �õ��䱦���Ĺ���,��Ч����GT_INVALID
    INT GetContributNeed(DWORD dwTreaID);
    
    //! ֱ�ӵõ�����
    const _MapTreasure GetAllTreaByTreaID() {return m_mapTreasurebyTrID;}
    const _MultiMapTrea GetAllTreaByItemID() {return m_mapTreasurebyItemID;}
    const _MapTreasure GetAllTreaByTreaID(ECLanType eVal) {return m_mapTreasurebyTrIDA[eVal];}
    const _MultiMapTrea GetAllTreaByItemID(ECLanType eVal) {return m_mapTreasurebyItemIDA[eVal];}
    const _MapTreasure GetAllActived(ECLanType eVal) {return m_mapActivedbyTrIDA[eVal];}
    const _MapTreasure GetAllUnActived(ECLanType eVal) {return m_mapUnActivedbyTrIDA[eVal];}

private:
    //! ��proto������
    VOID LoadData();

    //! �������
    VOID ClearData();

    //! ��ȡ���ֻ���
    DWORD OnGetNameByNameID(tagGetNameByNameID* msg);

    //! ������������䱦,������Ϣ��ReputeMgr.cpp�﷢�ͣ�����ֻ�˽���
    DWORD OnNS_GetActClanTreasure(tagNS_GetActClanTreasure* pNetCmd, DWORD dwParam);

    //! �����������������䱦��������Ĺ㲥
    DWORD OnNS_NewActivedTreasure(tagNS_NewActivedTreasure* pNetCmd, DWORD dwParam);

    //! ������Ҽ���һ���䱦��ķ���
    DWORD OnNS_ActiveTreasure(tagNS_ActiveTreasure* pNetCmd, DWORD dwParam);

    TSFPTrunk<TreasureMgr>  m_Trunk;
    TObjRef<Util>           m_pUtil;
    TObjRef<NetCmdMgr>      m_pCmdMgr;
    TObjRef<NetSession>     m_pSession;
    TObjRef<GameFrameMgr>   m_pFrameMgr;

    _MapTreasure            m_mapTreasurebyTrID;                //!< ͨ���䱦id���䱦
    _MultiMapTrea           m_mapTreasurebyItemID;              //!< ͨ��itemid���䱦
    _MapTreasure            m_mapTreasurebyTrIDA[ECLT_NUM];     //!< ��������� by TreasureID
    _MultiMapTrea           m_mapTreasurebyItemIDA[ECLT_NUM];   //!< ��������� by ItemID
    _MapTreasure            m_mapActivedbyTrIDA[ECLT_NUM];      //!< ��������� ȫ��������䱦 by TreasureID
    _MapTreasure            m_mapUnActivedbyTrIDA[ECLT_NUM];    //!< ��������� ȫ��δ�����䱦 by TreasureID

    _MultiMapTrea           m_mulmapNameIDReq;                  //!< ��nametableȡ�䱦��ǰ׺���п���ͬһ�����ּ����˶���䱦
};
