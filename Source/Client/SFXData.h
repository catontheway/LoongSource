#pragma once

#include ".\BaseData.h"

/** \��Ч����
*/
struct tagSFXData
{
	bool bCanMove;   // �Ƿ���ƶ�
};

/** \class SFXData
	\brief ��Ч����
*/
class SFXData
	: public BaseData
{
public:
	SFXData(void);
	~SFXData(void);

	static SFXData* Inst();
	/** \���������ļ�
	*/
	virtual void LoadFromFile();
	/** \������Ч����
	*/
	const tagSFXData* FindData(const TCHAR* szSFXName);
private:
	map<DWORD, tagSFXData>		m_mapSFXData;
};