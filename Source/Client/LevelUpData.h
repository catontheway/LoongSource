#pragma once
#include "BaseData.h"
#include "..\WorldDefine\skill_define.h"
struct tagExperience
{
	INT			nLevel;					//�ȼ�
	INT			nExp;					//��һ����Ҫ�ľ���
	INT			nAtt[X_ERA_ATTA_NUM];	//һ������
	INT			nCost[ESCT_End];		//���������Ե����ֵ
	tagExperience(void)
	{
		memset(this, 0x0, sizeof(tagExperience));
	}
};

class LevelUpData :
	public BaseData
{
public:
	LevelUpData(void);
	~LevelUpData(void);

	static LevelUpData* Inst();

	/** �����ɫ�������������ļ�
	*/
	virtual void LoadFromFile();

	const tagExperience* FindExpData(DWORD dwLevel)
	{
		return FindInMap(m_mapExp, dwLevel);
	}

private:
	map<DWORD, tagExperience>			m_mapExp;
};