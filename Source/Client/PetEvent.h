#pragma once

#include "../WorldDefine/msg_pet.h"
#include "../WorldDefine/pet_skill_define.h"

//! �������Է����仯������ö��
enum EPetAttrChangeType
{
    EPACT_Init,          //!< ���г�ʼ��
    EPACT_Remove,        //!< ɾ������
    EPACT_PetValue,      //!< �������﷢����ֵ���Ա仯
    EPACT_PetDisp,       //!< ��ʾ���Ա仯
    EPACT_PetEquip,      //!< װ���仯
    EPACT_PetUnEquip,    //!< װ��ж��
    EPACT_PetState,      //!< ����״̬�仯
    EPACT_PetSkill       //!< ���＼�ܱ仯
};

//! \file   PetEvent.h
//! \brief  ���������Ϸ�¼�
//! \author hyu
//! \date   2009-4-16
//! \last   2009-4-16

//! \struct tagGetRolePetsIDEvent
//! \brief  ���ؽ�ɫ�������г�����Ϣ
struct tagGetRolePetsIDEvent : public tagGameEvent
{
    DWORD               dwRoleID;       //!< ��ɫID
    list<DWORD>         lstPets;        //!< �����б�

    tagGetRolePetsIDEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) :
    tagGameEvent(szEventName, pSenderFrame) {}
};

////! \struct tagGetPetAttrEvent
////! \brief  ���س�������
//struct tagGetPetAttrEvent : public tagGameEvent 
//{
//    DWORD           dwRoleID;           //!< ��ɫID
//    DWORD           dwPetID;            //!< ����ID
//    tagPetInitAttr  petAttr;            //!< ����ṹ
//
//    tagGetPetAttrEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) :
//    tagGameEvent(szEventName, pSenderFrame) {}
//};

//! \struct tagPetAttrChange
//! \brief  �������Է����仯
struct tagPetAttrChange : public tagGameEvent 
{
    EPetAttrChangeType  eType;              //!< �仯����

    DWORD           dwPetID;            //!< ����ID(����eType=EPACT_Init,EPACT_PetCall,EPACT_PetUnEquip��Ч)

    tagPetAttrChange(LPCTSTR szEventName, GameFrame* pSenderFrame) :
    tagGameEvent(szEventName, pSenderFrame) {}
};

//! \struct tagUsePetEggEvent
//! \brief  ʹ�ó��ﵰ�¼�
struct tagUsePetEggEvent : public tagGameEvent
{
    INT64       n64ItemID;           //!< ��Ʒ64λID
    tagUsePetEggEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) :
    tagGameEvent(szEventName, pSenderFrame) {}
};

//! \struct tagUsePetFoodEvent
//! \brief  ʹ�ó���ҩƷ�¼�
struct tagUsePetFoodEvent : public tagGameEvent
{
    INT64       n64ItemID;           //!< ��Ʒ64λID
    tagUsePetFoodEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) :
    tagGameEvent(szEventName, pSenderFrame) {}
};

//! \struct tagUsePetEquip
//! \brief  ʹ�ó���װ���¼�
struct tagUsePetEquip : public tagGameEvent
{
    INT64       n64ItemID;           //!< ��Ʒ64λID
    tagUsePetEquip(LPCTSTR szEventName, GameFrame* pSenderFrame) :
    tagGameEvent(szEventName, pSenderFrame) {}
};

//! \struct tagPetDispChange
//! \brief  ���������ʾ��Ϣ���뱾�ػ���֪ͨ
struct tagPetDispChange : public tagGameEvent 
{
    DWORD           dwPetID;            //!< ����ID
    tagPetDispChange(LPCTSTR szEventName, GameFrame* pSenderFrame) :
    tagGameEvent(szEventName, pSenderFrame) {}
};

//! \struct tagPetSkillCfgOpen
//! \brief  ����򿪳��＼�����ô���
struct tagPetSkillCfgOpen : public tagGameEvent 
{
    EPetskillType   eSkillType;        //!< ��������
    DWORD           dwPetID;            //!< ����ID
    DWORD           dwSkillID;          //!< ����ID
    tagPetSkillCfgOpen(LPCTSTR szEventName, GameFrame* pSenderFrame) :
    tagGameEvent(szEventName, pSenderFrame) {}
};

//! \struct tagEnterMount
//! \brief  
struct tagEnterMount : public tagGameEvent 
{
	DWORD			dwOwnerID;
	DWORD           dwPetID;           //!< ����ID
	DWORD           dwTypeID;          //!< TypeID
	tagEnterMount(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName, pSenderFrame)
		, dwPetID(GT_INVALID)
		, dwTypeID(GT_INVALID)
	{}
};

//! \struct tagLeaveMount
//! \brief  
struct tagLeaveMount : public tagGameEvent 
{
	DWORD           dwPetID;           //!< ����ID
	tagLeaveMount(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName, pSenderFrame)
		, dwPetID(GT_INVALID)
	{}
};
