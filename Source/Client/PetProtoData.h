#pragma once
#include "basedata.h"
#include "PetDef.h"
#include "../WorldDefine/pet_skill_define.h"

//! \class  PetProtoData
//! \breif  ����ԭʼ����
//! \base   BaseData
//! \author hyu
//! \date   2009-4-2
//! \last   2009-4-2
//! \sa     Pet,PetManager
class PetProtoData :
    public BaseData
{
public:
    PetProtoData(void);
    ~PetProtoData(void);

    typedef map<DWORD, tagPetProto> _id2pet;
    typedef map<DWORD, tagPetSkillData> _id2petskill;

    static PetProtoData* Inst();

    //! ��ó���ԭ�������ļ�
    virtual void LoadFromFile();

    //! ��ó���ȫ��ԭ������
    const _id2pet& GetMapId2Pet() { return m_mapPetProto; }

    //! ��ó���ԭ������
    const tagPetProto* FindPetProto(DWORD typeId) { return FindInMap(m_mapPetProto, typeId); }

    //! ��ó��＼������
    const tagPetSkillData* FindPetSkill(DWORD skillId) { return FindInMap(m_mapPetSkillProto, skillId); }

    //! ��ü�����ȴʱ��
    int GetPetSkillMaxCd(DWORD skillId);

    //! ��ȡ������˶���
    tstring GetAction1(DWORD typeId);
    tstring GetAction2(DWORD typeId);
private:
    //! ���ɳ���ģ����ظ���
    void GeneratePath(tagPetProto* pPetData);

    _id2pet             m_mapPetProto;
    _id2petskill        m_mapPetSkillProto;
};
