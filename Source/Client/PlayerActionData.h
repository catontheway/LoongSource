#pragma once

#include ".\BaseData.h"

/** \��Ҷ�������
*/
struct tagPlayerActionData
{
	DWORD dwAttackTime;  // ��ʼ������ʱ���
};

/** \class PlayerActionData
	\brief ��Ҷ�������
*/
class PlayerActionData
	: public BaseData
{
public:
	PlayerActionData(void);
	~PlayerActionData(void);

	static PlayerActionData* Inst();
	/** \���������ļ�
	*/
	virtual void LoadFromFile();
	/** \������Ч����
	*/
	const tagPlayerActionData* FindData(const TCHAR* szActName);
private:
	map<DWORD, tagPlayerActionData>		m_mapPlayerActionData;
};