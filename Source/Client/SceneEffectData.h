#pragma once
#include "basedata.h"

struct tagScnEffectData
{
    tstring     strPath;
};

//! \class  SceneEffectData
//! \breif  ��Ӱ��Ч�����ļ�
//! \base   BaseData
//! \author hyu
//! \date   2009-7-23
//! \last   2009-7-23
class SceneEffectData :
    public BaseData
{
public:
    typedef map<DWORD, tagScnEffectData> _id2sfxData;

    SceneEffectData(void);
    ~SceneEffectData(void);

    static SceneEffectData* Inst();

    //! ��ó���ԭ�������ļ�
    virtual void LoadFromFile();

    //! �����Ч·��
    tstring FindSfxPath(DWORD sfxID);

private:
    _id2sfxData     m_mapScnSfx;
};
