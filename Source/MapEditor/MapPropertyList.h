#pragma once
#include "propertylist.h"

class CMapPropertyList :public CPropertyList
{
public:
	CMapPropertyList(void);
	virtual ~CMapPropertyList(void);

	//--��ʾ��������

	void DisplayNPC(const tagMapNPC* pNPC);
	void DisplayStaticObj(const tagStaticMapObj* pStaticMapObj);
	void DisplayDynamicBlock(const tagMapDynamicBlock* pDynamicBlock);
	void DisplayWayPointObj(const tagMapWayPoint* pWayPointObj);
	void DisplayTriggerObj(const tagMapTrigger* pTriggerObj);
	void DisplaySoundObj(const tagMapSound* pSoundObj);
	void DisplayPointLightObj(const tagMapPointLight* pPointLight);
	void DisplayMapRectObj(const tagMapArea* pMapRect);
	void DisplayPathPoint(const tagMapPathPoint* pPathPoint);
	void DisplaySpawnPoint(const tagMapSpawnPoint* pSpawnPoint);
	void DisplayEventTrigger(const tagMapEventTrigger* pEventTrigger);
	void DisplayNPCNav(const tagNPCNavMapBuildStartPoint* pNPCNav);
	void DisplayPlayerNav(const tagPlayerNavMapBuildStartPoint* pPlayerNav);
	void DisplayMapDoor(const tagMapDoor* pDoor);
	void DisplayMapCarrier(const tagMapCarrier* carrier);
	void DisplayMapTriggerEffect(const tagMapTriggerEffect* pTEffect);

	//--����޸ĵĵ���������
	bool GetModifiedWayPointProp(tagMapWayPoint& WayPoint);
	//--����޸ĵĴ���������
	bool GetModifiedTriggerProp(tagMapTrigger& Trigger);
	//--�õ��޸�NPC����
	bool GetModifiedNPCObjProp(tagMapNPC* p,Vector3& pos,float &fYaw,bool &bIsLoadMDL);
	//--�õ��޸ľ�̬��������
	bool GetModifiedStaticObjProp(tagStaticMapObj* p,Vector3& pos,bool &bIsLoadMDL);
	//--�õ��޸ĵĶ�̬�ϰ�������
	bool GetModifyedDynamicBlockProp(tagMapDynamicBlock* p, Vector3& pos, bool &bIsLoadMDL);
	//--�õ��޸ĵ���Ч����
	bool GetModifiedSoundProp(tagMapSound* pSound);
	//--�õ��޸ĵĵ��Դ����
	bool GetModifiedPointLightProp(tagMapPointLight* pPointLight);
	//--�õ��޸ĵĵ�ͼ��������
	bool GetModifyedMapRectProp(tagMapArea* pMapRect);
	//--�õ��޸ĵ�Ѱ·������
	bool GetModifyedPathPointProp(tagMapPathPoint* pPathPoint);
	//--�õ��޸ĵ�ˢ�ֵ�
	bool GetModifyedSpawnPointProp(tagMapSpawnPoint* pSpawnPoint);
	//--�õ��޸ĵ��¼�������
	bool GetModifyedEventTrigger(tagMapEventTrigger* pEventTrigger);
	//--
	bool GetModifyedNPCNav(tagNPCNavMapBuildStartPoint* pNPCNav);
	//--
	bool GetModifyedPlayerNav(tagPlayerNavMapBuildStartPoint* pPlayerNav);
	//--
	bool GetModifiedMapDoor(tagMapDoor* p, Vector3& pos, bool &bIsLoadMDL);
	//--
	bool GetModifiedMapCarrier(tagMapCarrier* p, bool &bIsLoadMDL);
	//--�õ��޸ĵĴ�����Ч
	bool GetModifyedMapTriggerEffect(tagMapTriggerEffect* p, Vector3& pos, bool &bIsLoadMDL);
private:
	int		m_nTriggerNum ;
	int		m_nMapAreaNum;
};
