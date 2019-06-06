#pragma once

#include "..\WorldDefine\MapAttDefine.h"
#include ".\BaseData.h"

/** ��ͼ��ʾ�����߼�����
*/
struct tagMapShowAreaData
{
	DWORD       dwObjID;
	tstring     szTitle;			// ������ʾ������
	AABBox      box;                // ��Χ��
	DWORD		dwMiniMapSize;      // ����ͼ��С
};

/** ��ͼ�߼�����
*/
struct tagMapLogicData
{
	DWORD						dwID;				//��ͼID������szName���ɵ�CRC
	tstring						szName;				//�����ڲ�����
	tstring						szShowName;			//��ʾ����
	Vector2						vSize;				//��С
	EMapType					eMapType;			//��ͼ����
	vector<tagMapShowAreaData>  vecShowAreaData;    //��ʾ���������б�
	//more
};

/** \class MapLogicData
	\brief ��ͼ�߼�����
*/
class MapLogicData
	: public BaseData
{
public:
	MapLogicData(void);
	~MapLogicData(void);

	static MapLogicData* Inst();

	/** ���������ļ�
	*/
	virtual void LoadFromFile();
	/** ��ȡ��ͼ�߼�����
	*/
	const tagMapLogicData* FindMapLogicData(DWORD dwMapID)
	{
		return FindInMap(m_mapMapLogicData, dwMapID);
	}
	/** ��ȡ��ͼ��ʾ����
	*/
	LPCTSTR FindMapShowName(DWORD dwMapID);
	/** ��ȡ��ͼ��ʾ��������
	*/
	bool GetMapShowAreaList(DWORD dwMapID,vector<tagMapShowAreaData>& vecData);
	const tagMapShowAreaData* FindMapShowAreaData(DWORD dwMapID, DWORD dwAreaObjID);
	/** ��ȡ��������
	*/
	const map<DWORD, tagMapLogicData>& GetDataMap() { return m_mapMapLogicData; }
private:
	map<DWORD, tagMapLogicData>		m_mapMapLogicData;		// ��ͼ�߼�����

};