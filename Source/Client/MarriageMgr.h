#pragma once

struct tagNS_RemoteChangeSpouse;

//! \class  MarriageMgr
//! \brief  ����������
//! \author hyu
//! \date   2009-9-9
class MarriageMgr
{
public:
    MarriageMgr(void);
    ~MarriageMgr(void);

    void Init();
    void Destroy();
    void Update();

    //! Singleton
    static MarriageMgr& Inst();

    //! ������ش�����
    bool ShowErrMsg(DWORD dwErrCode);

    //! ��ʾ���˵��
    DWORD ShowMarriageManule(tagGameEvent *pEvent);

private:
    //! ����������Ϣ
    DWORD OnNS_RemoteChangeSpouse   (tagNS_RemoteChangeSpouse*  pMsg, DWORD);

    //------------------------------------------------------------------------
    // ����
    //------------------------------------------------------------------------
    TSFPTrunk<MarriageMgr>      m_Trunk;
    TObjRef<Util>				m_pUtil;
    TObjRef<NetCmdMgr>			m_pCmdMgr;
    TObjRef<GameFrameMgr>		m_pFrameMgr;
    TObjRef<NetSession>			m_pSession;
};
