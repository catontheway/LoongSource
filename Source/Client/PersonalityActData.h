#pragma once

#include ".\BaseData.h"
#include "..\WorldDefine\motion_define.h"

struct tagPersonAct : public tagMotionProto
{
	// ���Զ�������
	enum EType
	{
		T_All,       // ȫ������ֻ��һ�Σ�
		T_Upper,     // ��������ֻ��һ�Σ�
		T_3Act,      // 3������
	};
	DWORD			dwID;
	tstring			strCommand1;		// ָ��1
	tstring			strCommand2;		// ָ��2
	tstring			strAct1;			// ���붯����T_All��T_Upper���ŵĶ���
	tstring			strAct2;			// ���ֶ���
	tstring			strAct3;			// �뿪����
	EType			eType;				// ����

	tagPersonAct() : dwID( GT_INVALID ), eType( T_All ){}
};

class PersonalityActData : 
	public BaseData
{
public:
	PersonalityActData(void);
	~PersonalityActData(void);

	static PersonalityActData* Inst();

	/** ������Զ��������ļ�
	*/
	virtual void LoadFromFile();

	const tagPersonAct* FindPersonAct(DWORD id)
	{
		return FindInMap(m_mapPersonAct, id);
	}

	map<DWORD, tagPersonAct>& GetPersonActMap()
	{
		return m_mapPersonAct;
	}

private:
	map<DWORD, tagPersonAct>			m_mapPersonAct;
};