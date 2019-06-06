#pragma once
#include "NPC.h"
#include "../WorldDefine/pet_define.h"

class   FSM_PET;
struct  tagPetProto;
struct  tagNS_GetRemotePetState;

//! \class  EPetStus
//! \brief  �����Ԧ״̬
enum EPetStus
{
    EPS_Followed,   //!< ����
    EPS_RidingMove, //!< �����
    EPS_RidingIdle, //!< �������
    EPS_Flying,     //!< ����
    EPS_Num
};

//! \class  Pet
//! \breif  ���ٻ�����
//! \author hyu
//! \date   2009-3-31
//! \base   NPC
//! \sa     NPC,Role,RoleMgr
class Pet : public NPC
{
public:
    Pet(void);
    virtual ~Pet(void);

    // ����
    virtual void InitFSM();
    //virtual void AttachSceneNodeToSG(SceneGraph* pSG);
    //virtual void DetachSceneNodeFromSG(SceneGraph* pSG);
    virtual void Update();
    virtual void OnNetCmd(tagNetCmd* pNetCmd);
    virtual void OnGameEvent(tagGameEvent* pGameEvent);
    //virtual SceneNode* GetSceneNode() { return m_pEntityNode; }
    //virtual void GetHeadPos(Vector3& out);
    // ������ײ�д�С
    virtual Vector3 GetRoleSize();
    void SetCurrScale(const Vector3& vScale) {m_vCurrScale = vScale;}
    // ����ģ������
    void SetScale(float scale);
    virtual bool IsDead();
    virtual bool IsAttributeInited() const;
    //virtual void Hide( const bool bHide );
    //virtual bool IsHide() const { return m_bHide; }

    //! ������ж��ģ�ͣ����ⲿ�������ýڵ�ʹ��
    void DeleteSceneNode(SceneGraph* pSG);

    //! ���ų��ﶯ��,������ɫ��˷��п���
    void PlayDrivingTrack(EPetStus eStus);

    //! ��ó�������ģ��
    const tagPetProto* GetProto() const { return m_pPetProto; }

    //! ��ó�������ID
    DWORD GetTypeID() { return m_dwTypeID; }

    //! ��ó�������RoleID
    DWORD GetOwnerID() { return m_dwOwnerID; }

    //! ��ó����ٶ�
    float GetPetSpeed();

    //! ��õ�ǰģ������
    EPetModelType GetCurrMdlType();

    //! ��ʼ��
    virtual void InitPetAttribute(tagNS_GetRemotePetState* pCmd);

    //! ���ó�������id
    void SetPetTypeID( DWORD dwTypeID ) { m_dwTypeID = dwTypeID; }

    //! �رմ˷���
    virtual void InitAttribute( tagNS_GetRemoteCreatureState* pCmd ) {};

    //! ����״̬��
    void SetFSMEnabled(bool bVal) { m_bFSMEnabled = bVal; }
    bool IsFSMEnabled() {return m_bFSMEnabled;}

    //! ��������������
    void RidersCountReset() { m_nRidersCount = 0; }
    void RidersCountInc() { ++m_nRidersCount; }
    void RidersCountDec() { --m_nRidersCount; }
    int GetRidersCount() { return m_nRidersCount; }

    //! ��ʱ��/�رճ��︩��,���ȼ�����tagCreatureMdlData�е�bCanPitch����
    void SetPetPitch(bool bAllowPitch) { m_bPitch = bAllowPitch; }
    bool bAllowPitch() { return m_bPitch; }

protected:

    UPetState       m_uPetState;        //!< ��ǰ����״̬
    FSM_PET*        m_pFSM_PET;         //!< ����״̬��
    tagPetProto*    m_pPetProto;        //!< ������������
    DWORD			m_dwTypeID;         //!< ��������id
    DWORD           m_dwOwnerID;        //!< ����������ɫID

    bool            m_bInitialized;     //!< �Ƿ��ʼ��������

    bool            m_bFSMEnabled;      //!< �Ƿ���ͣ״̬��

    int             m_nRidersCount;     //!< ��ǰ�����ϵ�����

    Vector3         m_vCurrScale;       //!< �����������ϵ��

    bool            m_bPitch;           //!< �Ƿ�򿪳��︩��

    DECL_RTTI(Pet);
};
