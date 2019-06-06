#pragma once
#include "buttonex.h"

struct tagPetSkillData;

//! \class  PetSkillButton
//! \brief  ���＼�ܰ�ť
//! \author hyu
//! \date   2009-7-22
class PetSkillButton :
    public ButtonEx
{
public:
    PetSkillButton(void);
    ~PetSkillButton(void);

    virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
    virtual VOID Render();
    virtual VOID Update();
    virtual VOID Destroy();
    virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);
    virtual VOID SendEvent(tagGUIEvent* pEvent);
    virtual VOID OnEvent(tagGUIEvent* pEvent); // �����¼�

    // ˢ�¼��ܰ�ť
    void RefrashSkill(DWORD dwPetID, DWORD dwSkillID);

    // ȡ�ð�ť��ӵ�еļ���ԭ������
    const tagPetSkillData* GetSkillData()
    {
        return m_pSkillData;
    }

private:

    //���õ�ǰ��ȴʱ��
    void SetCDTime(float time) { m_fCDCount = time; }

    //���õ�ǰ��ת��
    void SetRestoreRatio(float time) { m_fRestoreRatio = time; }

    const tagPetSkillData* m_pSkillData;    //!< ���＼������
};
