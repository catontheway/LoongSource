#pragma once

#include "../WorldDefine/pet_define.h"
#include "../WorldDefine/pet_equip_define.h"
#include "../WorldDefine/msg_pet_equip.h"
#include "PetDef.h"

struct  tagPet;
struct  tagPetDispInfo;
struct  tagNS_GetRolePetsID;
struct  tagNS_GetPetAttr;
struct  tagNS_GetPetDispInfo;
struct  tagNS_PetDispInfoChange;
struct  tagNS_GetRoleCurrPetID;
struct  tagNS_CallPet;
struct  tagNS_UsePetEgg;
struct  tagUsePetEggEvent;
struct  tagMsgInputBoxEvent;
struct  tagNS_PetAttrChange;
class   Item;
struct  tagPetEquip;
struct  tagPetSkill;
struct  tagNS_PetPourExp;
struct  tagNS_PetSkillUse;
struct  tagNS_AddPetSkill;
struct  tagNS_RemovePetSkill;
struct  tagPetSkillMsgInfo;
struct  tagUsePetFoodEvent;

const int MAX_PET_COUNT             = 5;
const int MAX_PET_GATHER_DISTANCE   = 50;   //!< unit:tile

//! \class  PetManager
//! \author hyu
//! \date   2009-3-27
//! \brief  ���������
//! \remark singleton
class PetManager
{
public:
    typedef map<DWORD, tagPet*>         _id2pet;
    typedef map<DWORD, tagPetDispInfo*> _id2petdisp;
public:
    PetManager(void);
    ~PetManager(void);

    VOID Init();
    VOID Destroy();

    void Update();

    //! Singleton
    static PetManager* Inst();

    //! ���ͻ�ó�����������(dwPetIDΪGT_INVALIDʱ����ý�ɫ���г�����Ϣ)
    void BeginNC_GetPetAttr( DWORD dwRoleID, DWORD dwPetID );
    DWORD OnNS_GetPetAttr(tagNS_GetPetAttr* pMsg, DWORD);

    //! ����������Է����仯
    DWORD OnNS_PetAttrChange(tagNS_PetAttrChange* pMsg, DWORD);

    //! ���ĳһ�������ֵ��ʾ��Ϣ(��Ҫ���ػ�����),�緵��NULL,��������
    const tagPetDispInfo* FindPetDispInfo(DWORD dwRoleID, DWORD dwPetID );
    DWORD OnNS_GetPetDispInfo(tagNS_GetPetDispInfo* pMsg, DWORD);
    DWORD OnNS_PetDispInfoChange(tagNS_PetDispInfoChange* pMsg, DWORD);

    //! ɾ������
    void BeginNC_DeletePet(DWORD dwPetID);
    DWORD OnNS_DeletePet(tagNS_DeletePet* pMsg, DWORD);

    //! ���ó���״̬
    void BeginSetPetState( DWORD dwPetID, EPetState petState, bool bEnabled);

    //! ��ע����
    void BeginNC_GetPetPourExpMoneyNeed(DWORD dwPetID);
    DWORD OnNS_GetPetPourExpMoneyNeed(tagNS_GetPetPourExpMoneyNeed* pMsg, DWORD);
    void BeginPourExp( DWORD dwPetID );
    DWORD OnNS_PetPourExp(tagNS_PetPourExp* pMsg, DWORD);

    //! ��������
    void BeginNC_PetSetLock(DWORD dwPetID, INT64 n64ItemID, bool bSet);
    DWORD OnNS_PetSetLock(tagNS_PetSetLock* pMsg, DWORD);
    void SetPetLock(INT64 n64ItemID, bool bSet);

    //! ��Ҵ�����װ��
    void BeginNC_PetEquip(DWORD dwPetID, INT8 pos, INT64 n64ItemID);
    DWORD OnNS_PetEquip(tagNS_PetEquip* pMsg, DWORD);

    //! ���ж�س���װ��
    void BeginNC_PetUnequip(DWORD dwPetID, INT64 n64ItemID, INT16 n16PosDst);
    DWORD OnNS_PetUnequip(tagNS_PetUnequip* pMsg, DWORD);

    //! ���ͬһװ��������λ��
    void BeginNC_PetEquipPosSwap(DWORD dwPetID, INT64 n64ItemID, INT8 n8PosDst);
    DWORD OnNS_PetEquipPosSwap(tagNS_PetEquipPosSwap* pMsg, DWORD);

    // �������
    void BeginNS_PetLearnSkill(DWORD dwPetID, INT64 n64ItemID);
    DWORD OnNS_PetLearnSkill(tagNS_PetLearnSkill* pMsg, DWORD);
    //! ʹ�ü���
    void TryToUsePetSkill(DWORD dwPetID, DWORD dwSkillID);
    //! ʹ��С������
    void TryToSellItem(DWORD dwPetID, DWORD dwSkillID);
    //! ʹ��ѵ������
    void TryToTraining(DWORD dwPetID, DWORD dwSkillTypeID, DWORD dwPetSkillID);

    //! ����ʹ�ü�������
    void BeginNC_PetSkillUse(DWORD dwPetID, DWORD dwSkillID, BYTE* pData, size_t dataLength);
    //! ʹ�ü������󷵻�
    DWORD OnNS_PetSkillUse(tagNS_PetSkillUse* pMsg, DWORD);

    //! ���һ������
    DWORD OnNS_AddPetSkill(tagNS_AddPetSkill* pMsg, DWORD);

    //! ����һ������
    DWORD OnNS_RemovePetSkill(tagNS_RemovePetSkill* pMsg, DWORD);

    //! ��ǰ��ȴʱ��
    void GetSkillCDTime(DWORD dwPetID, DWORD dwSkillID, FLOAT &fCDCount, FLOAT &fRestoreRatio);
    //! �����Ƿ���ȴ
    void UpdateSkillCD();

    //! ��ʼ���ü�����ȴ
    void SetSkillCD(tagPetSkill* pSkill, int tick);

    //! �յ�������ͬ����ʼ��ȴ��Ϣ
    DWORD OnNS_PetSkillTimer(tagNS_PetSkillTimer* pMsg,DWORD);



    // ����������Ϣ
    void SetSkillForageCfg(DWORD dwPetID, tagPetSkillForage& cfg);
    void SetSkillPickupCfg(DWORD dwPetID, tagPetSkillPickup& cfg);
    void SetSkillSupplyCfg(DWORD dwPetID, tagPetSkillSupply& cfg);

    //! �Զ�ʹ�ó��＼��
    void AutoUseSkill();

    //! ������Ʒʹ��
    void OnUseLvlupStep(Item* pItem);
    DWORD OnNS_PetUpStep(tagNS_PetUpStep* pMsg, DWORD);
    DWORD OnBoxPetUpStep(tagMsgBoxEvent* pEvent);
    DWORD m_pLupSkillID;
    DWORD m_pLupStep;

    //! ��ע��Ʒʹ��
    void OnUseExpPour(Item* pItem);
    DWORD OnBoxUseExpPour(tagMsgBoxEvent* pEvent);
    INT64 m_tmpPourItemID;

    //! ��������ʹ��
    void OnUseEnhance(Item* pItem);
    DWORD OnNS_PetEnhance(tagNS_PetEnhance* pMsg, DWORD);

    //! ����ҩƷʹ��
    DWORD OnUsePetFood(tagUsePetFoodEvent* pEvent);
    DWORD OnNS_PetFood(tagNS_PetFood* pMsg, DWORD);

    // �������

    //! ��������
    const _id2pet* GetPetList() { return &m_mapPetsOfLP; }
    //! ���س���ṹ
    const tagPet* GetPet(DWORD dwPetID);
    //! ���ص�ǰ�ٻ��ĳ���(GT_INVALIDΪ���绽����)
    DWORD GetCurrPetID();
    //! ���س����װ��
    const tagPetEquip* FindPetEquip(DWORD dwPetID, INT8 pos);
    //! ���س���ļ���
    const tagPetSkill* FindPetSkill(DWORD dwPetID, DWORD dwSkillID);
    //! �����Ͳ���
    const tagPetSkill* FindPetSkillbyType(DWORD dwPetID, EPetskillType eType);
    //! ���س���״̬
    bool IsPetCalled(DWORD dwPetID);
    bool IsPetWorking(DWORD dwPetID);
    bool IsPetPreparing(DWORD dwPetID);
    bool IsPetRiding(DWORD dwPetID);
    //! �õ������Я���ȼ�
    int GetCarryLevel(DWORD dwPetID);
    //! �õ����ﵱǰ�������
    int GetPetCurrMountNum(DWORD dwPetID);
    //! �õ���������������
    int GetPetMaxMountNum(DWORD dwPetID);
    
    // �����������

    void AddToBag(DWORD petId, int pos, Item* pItem);
    void PopFromBag(DWORD petId, int pos);
    void SwapBagItem(DWORD petId, int posSrc, int posDst);
    void ClearBag(DWORD petId);
    Item* GetItem(DWORD petId, int pos);   // ��Ч����GT_INVALID
    Item* GetItem(DWORD petId, INT64 n64ItemID);    // ��Ч����GT_INVALID

    bool IsExistInErrGroundItem(INT64 n64serial);

private:
    //! �ͷ���Դ
    void ClearAll();

    //! ʹ�ó��ﵰ
    DWORD OnUsePetEggEventHandler(tagUsePetEggEvent* pEvent);
    //! ��������Ϣ�򷵻ص��¼�
    DWORD OnCommitPetName(tagMsgInputBoxEvent* pEvent);
    //! ʹ������ﵰ��ķ���
    DWORD OnNS_UsePetEgg(tagNS_UsePetEgg* pMsg, DWORD);

    //! ����һ��װ��������װ����
    bool AddEquip(DWORD dwPetID, const tagPetEquipMsgInfo& petEquip);
    //! �ӳ���װ�����Ƴ�һ��װ��
    bool RemoveEquip(DWORD dwPetID, INT64 n64ItemID);
    bool RemoveEquip(DWORD dwPetID, int pos);

    //! ��ʾ������
    void ShowErrorMessage(DWORD dwErrCode);

    //! ����
public:
    bool AddSkill(DWORD dwPetID, const tagPetSkillMsgInfo& petSkill);
    bool RemoveSkill(DWORD dwPetID, DWORD dwSkillID);
    // ! �Ƿ�ǰ״̬����Ͷ�ż���
    bool IfCanCastSkill(DWORD dwPetID, DWORD dwSkillID, DWORD &dwErrCodeOut);

private:
    //------------------------------------------------------------------------
    // ����
    //------------------------------------------------------------------------
    TSFPTrunk<PetManager>       m_Trunk;
    TObjRef<Util>				m_pUtil;
    TObjRef<NetCmdMgr>			m_pCmdMgr;
    TObjRef<GameFrameMgr>		m_pFrameMgr;
    TObjRef<NetSession>			m_pSession;

    //------------------------------------------------------------------------
    // ������ҳ������Ի���
    //------------------------------------------------------------------------
private:
    _id2pet                     m_mapPetsOfLP;          //!< ������ҳ��ﻺ��

    _id2petdisp                 m_mapPetDispBuffer;     //!< ������ʾ��Ϣ����
    set<DWORD>                  m_setDispSentReq;       //!< ���͵����������ʾ��Ϣ�����б�

    INT                         m_nPetNameMaxLength;    //!< ����������󳤶�

    INT64                       m_n64CurrPetEggID;      //!< ��ǰҪʹ�õĳ��ﵰid

    bool                        bHasInitialized;        //!< �Ƿ��ʼ�����

    DWORD                       m_autoSkillFirst;       //!< ʹ���Զ�������Ҫ���ʱ��

    set<INT64>                  m_setErrGroundItemSerial;  //!< �Զ�ʰȡ����ĵ������ʱ���
    DWORD                       m_ErrGroundItemTime;        //!< �ϴ����ʰȡ�����б�ʱ��
};
