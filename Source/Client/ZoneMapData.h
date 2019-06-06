#pragma once

#include ".\BaseData.h"

/** \��ͼ�������
*/
struct tagMapMarkData
{
	tstring             strName;
	Vector3             vPos;
};

/** \��ͼ��Ƿ���
*/
struct tagMapMarkType
{
	tstring					strType;
	vector<tagMapMarkData>	vecMarks;
};


/** \�����ͼ����
*/
struct tagZoneMapData
{
	DWORD						dwID;			// ��ͼID������szName���ɵ�CRC
	vector<tagMapMarkType>		vecMarkTypes;	// ��ͼ��Ƿ����б�
};

/** \class ZoneMapData
	\brief �����ͼ����
*/
class ZoneMapData
	: public BaseData
{
public:
	ZoneMapData(void);
	~ZoneMapData(void);

	static ZoneMapData* Inst();
	/** \���������ļ�
	*/
	virtual void LoadFromFile();
	/** \���������ͼ����
	*/
	const tagZoneMapData* FindZoneMapData(DWORD dwMapID)
	{
		return FindInMap(m_mapZoneMapData, dwMapID);
	}
private:
	map<DWORD, tagZoneMapData>		m_mapZoneMapData;
};