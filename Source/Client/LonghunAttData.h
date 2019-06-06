#pragma once
#include "basedata.h"

struct tagLongHunProtoClient;
/** \class LonghunAttData
	\brief �����������Թ�����
*/
class LonghunAttData :
	public BaseData
{
public:
	LonghunAttData(void);
	~LonghunAttData(void);

	static LonghunAttData* Inst();
	
	/** \��������������Ϣ
	*/
	virtual void LoadFromFile();

	const tagLongHunProtoClient* FindLonghunData(DWORD dwID);

private:
	map<DWORD, tagLongHunProtoClient>		m_mapLunghun;
};
