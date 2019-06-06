#pragma once

#include "../WorldDefine/pet_define.h"
#include "../WorldDefine/msg_pet.h"
#include "../WorldDefine/pet_skill_define.h"
#include "../WorldDefine/msg_pet_skill.h"
#include "../WorldDefine/pet_equip_define.h"

class Item;


//-----------------------------------------------------------------------------
// ����װ���ṹ
//-----------------------------------------------------------------------------
struct tagPetEquip
{
    INT64           n64ItemID;              //!< װ����Ʒ64λID
    DWORD           dwItemProtoID;          //!< ��Ʒԭ��ID
};

//-----------------------------------------------------------------------------
// ���＼�ܽṹ
//-----------------------------------------------------------------------------
struct tagPetSkill
{
    DWORD	dwTypeID;							// TypeID(���ȼ�)
    bool    bCoolingDown;                       // �Ƿ�����ȴ
    INT     nCurrCoolDown;                      // ��ǰ��ȴʱ��(�������ߺ��ٵ�¼��δ�����ȴʱ��ͬ���ã����������Ϊ0)
    INT     nMaxCoolDown;                       // �����ȴʱ��
    DWORD   dwTimeFirst;                        // ��ʼ��ȴʱ�ĳ�ʼ�ۻ�ʱ��
};


//-----------------------------------------------------------------------------
// ���ó��＼�����ýṹ
//-----------------------------------------------------------------------------

// ҩͯ
struct tagPetSkillSupply
{
    UINT8       u8MinHP;            // HP����n%ʱ����
    INT64       dwHPItemID;         // ��HP��ƷID
    bool        bHPEnabled;         // HP�����Ƿ���

    UINT8       u8MinMP;            // MP����n%ʱ����
    INT64       dwMPItemID;         // ��MP��ƷID
    bool        bisMPEnabled;       // MP�����Ƿ���

    UINT8       u8Toughness;        // �־���
    INT64       dwTouItemID;        // ��Ʒid
    bool        bisTouEnabled;      // �ӳ־��������Ƿ���
};

// ��ʳ
struct tagPetSkillForage
{
    UINT8       u8MinSpirit;        // ��������n%ʱʹ��
    INT64       dwItemID;           // ��ƷID
    bool        bisEnabled;         // �����Ƿ���
};

// ʰȡ
struct tagPetSkillPickup
{
    bool            bMoney:1;             // �Ƿ�ʰȡ��Ǯ
    bool            bEquip:1;             // �Ƿ�ʰȡװ��
    bool            bMaterial:1;          // �Ƿ�ʰȡ����
    bool            bTrophy:1;            // �Ƿ�ʰȡս��Ʒ
    bool            bMedicine:1;          // �Ƿ�ʰȡҩƷ
    bool            bOthers:1;            // �Ƿ�ʰȡ��������
};


//-----------------------------------------------------------------------------
// ����ṹ
//-----------------------------------------------------------------------------

const float   PET_SCALE_PERCENT_BASE  =   10000;

//! struct  tagPetProto
//! brief   ��������ԭʼ����
struct tagPetProto 
{
    DWORD           dwID;                           //!< ����ID(ע�������ӵ�еĳ���ID��ͬ)

    // ��������Ϊ����ģ�ͣ�����ģ��
    tstring         strModelName[EPMT_NUM]; //!< ģ��
    tstring         strModelPath[EPMT_NUM]; //!< ģ��·��
    EAniType        eAniType[EPMT_NUM];     //!< ģ�Ͷ�������
    list<tstring>   lstSkinPath[EPMT_NUM];  //!< ģ����Ƥ·��
    tstring         strAct1;                //!< ��˶�������1
    tstring         strAct2;                //!< ��˶�������2
    Vector3         boundingBox;            //!< �������ģ�ͼ����ײʱ�İ�Χ�д�С
    float           scalePercent;           //!< ��PET_SCALE_PERCENT_BASEΪ�����İٷֱȣ�������ײ�в���Ӱ��
    int             carryLevel;             //!< Я���ȼ�
    bool            bBinding;               //!< �Ƿ��
};


//! \struct  tagPet
//! \brief   ��������Ϸ�����г���
//! \deatil  ���������Խṹ
//! \sa      EPetAttribute
struct tagPet
{
    typedef map<DWORD, tagPetSkill*> _id2skillInfo;
    typedef map<int, INT64> _pos2petbag;
    typedef map<int, tagPetEquip*> _pos2Equip;

    DWORD                   dwID;                       //!< ���������ʱ,����Ϸ�������ɵ�ΨһID
    DWORD                   dwTypeID;                   //!< ��������ID,��ӦtagPetProto�е�ID
    tstring                 strName;                    //!< �����ĳ�����,�Ժ��ͨ��ĳЩ�����޸�
    UPetState               state;                      //!< ����״̬
    INT                     nAttrA[ECSPA_End];          //!< ������ֵ����
    _pos2Equip              mapEquip;                   //!< ����װ����
    _id2skillInfo           Skill;                      //!< ����ӵ�еļ���
    tagPetSkillForage       skillForageCfg;
    tagPetSkillPickup       skillPickupCfg;
    tagPetSkillSupply       skillMedicineFeedCfg;
    _pos2petbag             bag;                        //!< ���������е���Ʒ����(����λ��2��Ʒ64λid)

    tagPet()
    {
        state.byPetState = 0;

        skillForageCfg.bisEnabled   =   false;
        skillForageCfg.dwItemID     =   GT_INVALID;
        skillForageCfg.u8MinSpirit  =   0;

        skillPickupCfg.bEquip   =   false;
        skillPickupCfg.bMaterial=   false;
        skillPickupCfg.bMedicine=   false;
        skillPickupCfg.bMoney   =   false;
        skillPickupCfg.bTrophy  =   false;
        skillPickupCfg.bOthers  =   false;

        skillMedicineFeedCfg.bHPEnabled     =   false;
        skillMedicineFeedCfg.bisMPEnabled   =   false;
        skillMedicineFeedCfg.bisTouEnabled  =   false;
        skillMedicineFeedCfg.dwHPItemID     =   GT_INVALID;
        skillMedicineFeedCfg.dwMPItemID     =   GT_INVALID;
        skillMedicineFeedCfg.dwTouItemID    =   GT_INVALID;
        skillMedicineFeedCfg.u8MinHP        =   0;
        skillMedicineFeedCfg.u8MinMP        =   0;
        skillMedicineFeedCfg.u8Toughness    =   0;
    }
    ~tagPet()
    {
        // �ͷ�װ��
        for (_pos2Equip::iterator it = mapEquip.begin();
            it != mapEquip.end(); ++it)
        {
            SAFE_DELETE(it->second);
        }
        mapEquip.clear();

        // �������
        bag.clear();

        // ��ռ���
        for (_id2skillInfo::iterator it = Skill.begin();
            it != Skill.end(); ++it)
        {
            SAFE_DELETE(it->second);
        }
        Skill.clear();
    }
};

struct tagPetSkillData
{
    DWORD                   dwPetSkillID;	    // ���＼��ID
    DWORD                   dwPetSkillTypeID;   // ���＼������ID
    int                     nLevel;             // ���＼�ܼ���

    EPetskillType           eType;			// ���＼������
    EPetskillType2          eType2;			// ���＼������2
    EPetskillType3          eType3;			// ���＼������3
    EPetskillCastType       eCastType;      // ʩ������
    EPetskillCastCondition  eCastCondition; // ʩ��״̬
    DWORD                   dwCDTime;       // ��ȴʱ��
    DWORD                   cast_condition; // Ͷ������
    int                     wuxing_cost;
    int                     spirit_cost;

    tstring                 strType;        // ����
    tstring                 strName;        // ��������
    tstring                 strDesc;        // ��������
    tstring                 strIconPath;    // ͼ���ļ�
};
