#include "StdAfx.h"
#include ".\mappropertylist.h"
#include ".\PlugMapEditor.h"

CMapPropertyList::CMapPropertyList(void)
{
	m_nTriggerNum = 0;
	m_nMapAreaNum = 0;
}

CMapPropertyList::~CMapPropertyList(void)
{
}



void CMapPropertyList::DisplayNPC(const tagMapNPC* pNPC)
{
	CString szBuffer;
	ASSERT(pNPC != NULL);

	szBuffer.Format(_T("%u"), pNPC->dwTypeID);
	AddPropItem(new CPropertyItem(_T("����ID"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pNPC->vPos.x);
	AddPropItem(new CPropertyItem(_T("X����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pNPC->vPos.y);
	AddPropItem(new CPropertyItem(_T("Y����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pNPC->vPos.z);
	AddPropItem(new CPropertyItem(_T("Z����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pNPC->fYaw);
	AddPropItem(new CPropertyItem(_T("����"), szBuffer, PIT_EDIT, szBuffer));

	AddPropItem(new CPropertyItem(_T("Ѳ�ߵ���������"), pNPC->szName, PIT_EDIT, szBuffer));

	if(pNPC->bCollide)             
		szBuffer = _T("��");
	else 
		szBuffer = _T("��");

	AddPropItem(new CPropertyItem(_T("�Ƿ�ʹ����ײ"), szBuffer, PIT_COMBO, _T("��|��|")));

	szBuffer.Format(_T("%f"), pNPC->fRadius);
	AddPropItem(new CPropertyItem(_T("��Ѩ�뾶"), szBuffer, PIT_EDIT, szBuffer));


}

void CMapPropertyList::DisplayDynamicBlock(const tagMapDynamicBlock* pDynamicBlock)
{
	CString szBuffer;
	ASSERT(pDynamicBlock != NULL);

	szBuffer.Format(_T("%f"), pDynamicBlock->vPos.x);
	AddPropItem(new CPropertyItem(_T("X����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pDynamicBlock->vPos.y);
	AddPropItem(new CPropertyItem(_T("Y����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pDynamicBlock->vPos.z);
	AddPropItem(new CPropertyItem(_T("Z����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pDynamicBlock->fScale[0]);
	AddPropItem(new CPropertyItem(_T("X����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pDynamicBlock->fScale[1]);
	AddPropItem(new CPropertyItem(_T("Y����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pDynamicBlock->fScale[2]);
	AddPropItem(new CPropertyItem(_T("Z����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), _RadToDeg(pDynamicBlock->fYaw));
	AddPropItem(new CPropertyItem(_T("����"), szBuffer, PIT_EDIT, szBuffer));

	AddPropItem(new CPropertyItem(_T("ģ������"), pDynamicBlock->szModelPath , PIT_FILE,_T("")));
}

void CMapPropertyList::DisplayMapDoor(const tagMapDoor* pDoor)
{
	CString szBuffer;
	ASSERT(pDoor != NULL);

	szBuffer.Format(_T("%f"), pDoor->vPos.x);
	AddPropItem(new CPropertyItem(_T("X����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pDoor->vPos.y);
	AddPropItem(new CPropertyItem(_T("Y����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pDoor->vPos.z);
	AddPropItem(new CPropertyItem(_T("Z����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pDoor->fScale[0]);
	AddPropItem(new CPropertyItem(_T("X����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pDoor->fScale[1]);
	AddPropItem(new CPropertyItem(_T("Y����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pDoor->fScale[2]);
	AddPropItem(new CPropertyItem(_T("Z����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), _RadToDeg(pDoor->fYaw));
	AddPropItem(new CPropertyItem(_T("����"), szBuffer, PIT_EDIT, szBuffer));

	AddPropItem(new CPropertyItem(_T("ģ������"), pDoor->szModelPath , PIT_FILE,_T("")));

	if(pDoor->bInitState)
		szBuffer = _T("��");
	else 
		szBuffer = _T("��");
	AddPropItem(new CPropertyItem(_T("[�ؿ�]�ų�ʼ״̬"), szBuffer, PIT_COMBO, _T("��|��|")));

	if(pDoor->bBlockingTileNPC)
		szBuffer = _T("��");
	else 
		szBuffer = _T("��");
	AddPropItem(new CPropertyItem(_T("[�ؿ�]�赲���ӵ���NPC"), szBuffer, PIT_COMBO, _T("��|��|")));

	szBuffer.Format(_T("%u"), pDoor->dwTypeID);
	AddPropItem(new CPropertyItem(_T("��TypeID"), szBuffer, PIT_EDIT, szBuffer));

}

void CMapPropertyList::DisplayMapCarrier(const tagMapCarrier* carrier)
{
	CString szBuffer;
	ASSERT(carrier != NULL);

	AddPropItem(new CPropertyItem(_T("ģ������"), carrier->szModelPath , PIT_FILE,_T("")));

	szBuffer.Format(_T("%s"), carrier->szWayPoint);
	AddPropItem(new CPropertyItem(_T("����������"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), carrier->fYaw);
	AddPropItem(new CPropertyItem(_T("��ת"), szBuffer, PIT_EDIT, szBuffer));

	if( carrier->bSurfaceType==0 )
		szBuffer = _T("��");
	else if( carrier->bSurfaceType==1 )
		szBuffer = _T("ѩ��");
	else if( carrier->bSurfaceType==2 )
		szBuffer = _T("ʯ��");
	else if( carrier->bSurfaceType==3 )
		szBuffer = _T("��");
	else if( carrier->bSurfaceType==4 )
		szBuffer = _T("�ݵ�");
	else if( carrier->bSurfaceType==5 )
		szBuffer = _T("ɳ��");
	else if( carrier->bSurfaceType==6 )
		szBuffer = _T("����");
	else if( carrier->bSurfaceType==7 )
		szBuffer = _T("����");
	else
		szBuffer = _T("ľͷ");
	AddPropItem(new CPropertyItem(_T("[�ؿ�]��������"), szBuffer, PIT_COMBO, _T("��|ѩ��|ʯ��|��|�ݵ�|ɳ��|����|����|ľͷ|")));

	szBuffer.Format(_T("%f"), carrier->fSpeed);
	AddPropItem(new CPropertyItem(_T("�ƶ��ٶ�"), szBuffer, PIT_EDIT, szBuffer));
}

void CMapPropertyList::DisplayStaticObj(const tagStaticMapObj* pStaticMapObj)
{
	CString szBuffer;
	ASSERT(pStaticMapObj != NULL);

	szBuffer.Format(_T("%f"), pStaticMapObj->fPos[0]);
	AddPropItem(new CPropertyItem(_T("X����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pStaticMapObj->fPos[1]);
	AddPropItem(new CPropertyItem(_T("Y����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pStaticMapObj->fPos[2]);
	AddPropItem(new CPropertyItem(_T("Z����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), _RadToDeg(pStaticMapObj->fRotate[1]));
	AddPropItem(new CPropertyItem(_T("X����ת�Ƕ�"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), _RadToDeg(pStaticMapObj->fRotate[0]));
	AddPropItem(new CPropertyItem(_T("Y����ת�Ƕ�"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), _RadToDeg(pStaticMapObj->fRotate[2]));
	AddPropItem(new CPropertyItem(_T("Z����ת�Ƕ�"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pStaticMapObj->fScale[0]);
	AddPropItem(new CPropertyItem(_T("X����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pStaticMapObj->fScale[1]);
	AddPropItem(new CPropertyItem(_T("Y����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pStaticMapObj->fScale[2]);
	AddPropItem(new CPropertyItem(_T("Z����"), szBuffer, PIT_EDIT, szBuffer));

	AddPropItem(new CPropertyItem(_T("ģ������"), pStaticMapObj->szMdlPath, PIT_FILE,_T("")));

	szBuffer = pStaticMapObj->szScriptName;
	AddPropItem(new CPropertyItem(_T("[�ؿ�]�����ű�"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%d"),pStaticMapObj->dwPickItemID);
	AddPropItem(new CPropertyItem(_T("[�ؿ�]�ɼ�����Ʒ"), szBuffer, PIT_EDIT, szBuffer));

	if(pStaticMapObj->bShowMiniMap)
		szBuffer = _T("��");
	else 
		szBuffer = _T("��");
	AddPropItem(new CPropertyItem(_T("С��ͼ����ʾ"), szBuffer, PIT_COMBO, _T("��|��|")));

	if(pStaticMapObj->bLightMap)
		szBuffer = _T("��");
	else 
		szBuffer = _T("��");
	AddPropItem(new CPropertyItem(_T("LightMap����"), szBuffer, PIT_COMBO, _T("��|��|")));

	if(pStaticMapObj->byViewLevel==0)
		szBuffer = _T("�Զ�����");
	else if(pStaticMapObj->byViewLevel==1)
		szBuffer = _T("��");
	else if(pStaticMapObj->byViewLevel==2)
		szBuffer = _T("��");
	else
		szBuffer = _T("Զ");
	AddPropItem(new CPropertyItem(_T("��Ұ�ȼ�"), szBuffer, PIT_COMBO, _T("�Զ�����|��|��|Զ|")));


	szBuffer.Format(_T("%f"), pStaticMapObj->fMtlDiffuseFactor);
	AddPropItem(new CPropertyItem(_T("������ǿ����ǿϵ��"), szBuffer, PIT_EDIT, szBuffer));

	if(pStaticMapObj->bNoShadow)
		szBuffer = _T("��");
	else
		szBuffer = _T("��");
	AddPropItem(new CPropertyItem(_T("��������Ӱ"), szBuffer, PIT_COMBO, _T("��|��|")));

	if( pStaticMapObj->bSurfaceType==0 )
		szBuffer = _T("��");
	else if( pStaticMapObj->bSurfaceType==1 )
		szBuffer = _T("ѩ��");
	else if( pStaticMapObj->bSurfaceType==2 )
		szBuffer = _T("ʯ��");
	else if( pStaticMapObj->bSurfaceType==3 )
		szBuffer = _T("��");
	else if( pStaticMapObj->bSurfaceType==4 )
		szBuffer = _T("�ݵ�");
	else if( pStaticMapObj->bSurfaceType==5 )
		szBuffer = _T("ɳ��");
	else if( pStaticMapObj->bSurfaceType==6 )
		szBuffer = _T("����");
	else if( pStaticMapObj->bSurfaceType==7 )
		szBuffer = _T("����");
	else
		szBuffer = _T("ľͷ");
	AddPropItem(new CPropertyItem(_T("[�ؿ�]��������"), szBuffer, PIT_COMBO, _T("��|ѩ��|ʯ��|��|�ݵ�|ɳ��|����|����|ľͷ|")));

	if(pStaticMapObj->bPathfinding)
		szBuffer = _T("��");
	else
		szBuffer = _T("��");
	AddPropItem(new CPropertyItem(_T("[�ؿ�]���Ѱ·��ͨ��"), szBuffer, PIT_COMBO, _T("��|��|")));

	if(pStaticMapObj->bNotSupportNPCNavMap)
		szBuffer = _T("��");
	else
		szBuffer = _T("��");
	AddPropItem(new CPropertyItem(_T("[�ؿ�]��֧��NPC���ӵ���ͼ"), szBuffer, PIT_COMBO, _T("��|��|")));

	if( pStaticMapObj->byBoxSize==0 )
		szBuffer = _T("��ͨ");
	else if( pStaticMapObj->byBoxSize==1 )
		szBuffer = _T("С");
	else
		szBuffer = _T("��");
	AddPropItem(new CPropertyItem(_T("[��ײ]�������ɺ��Ӵ�С"), szBuffer, PIT_COMBO, _T("��|��ͨ|С|")));
}

void CMapPropertyList::DisplayMapTriggerEffect(const tagMapTriggerEffect* pTEffect)
{
	CString szBuffer;
	ASSERT(pTEffect != NULL);

	szBuffer.Format(_T("%f"), pTEffect->fPos[0]);
	AddPropItem(new CPropertyItem(_T("X����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pTEffect->fPos[1]);
	AddPropItem(new CPropertyItem(_T("Y����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pTEffect->fPos[2]);
	AddPropItem(new CPropertyItem(_T("Z����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), _RadToDeg(pTEffect->fRotate[1]));
	AddPropItem(new CPropertyItem(_T("X����ת�Ƕ�"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), _RadToDeg(pTEffect->fRotate[0]));
	AddPropItem(new CPropertyItem(_T("Y����ת�Ƕ�"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), _RadToDeg(pTEffect->fRotate[2]));
	AddPropItem(new CPropertyItem(_T("Z����ת�Ƕ�"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pTEffect->fScale[0]);
	AddPropItem(new CPropertyItem(_T("X����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pTEffect->fScale[1]);
	AddPropItem(new CPropertyItem(_T("Y����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pTEffect->fScale[2]);
	AddPropItem(new CPropertyItem(_T("Z����"), szBuffer, PIT_EDIT, szBuffer));

	AddPropItem(new CPropertyItem(_T("ģ������"), pTEffect->szMdlPath, PIT_FILE,_T("")));
}

void CMapPropertyList::DisplayWayPointObj(const tagMapWayPoint* pWayPointObj)
{
	CString szBuffer;
	ASSERT(pWayPointObj != NULL);

	szBuffer.Format(_T("%s"), pWayPointObj->szName);
	AddPropItem(new CPropertyItem(_T("����������"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%d"), pWayPointObj->byIndex);
	AddPropItem(new CPropertyItem(_T("����������"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pWayPointObj->vPos.x);
	AddPropItem(new CPropertyItem(_T("X����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pWayPointObj->vPos.y);
	AddPropItem(new CPropertyItem(_T("Y����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pWayPointObj->vPos.z);
	AddPropItem(new CPropertyItem(_T("Z����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pWayPointObj->vRange.x);
	AddPropItem(new CPropertyItem(_T("X�᷽��Χ"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pWayPointObj->vRange.y);
	AddPropItem(new CPropertyItem(_T("Y�᷽��Χ"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pWayPointObj->vRange.z);
	AddPropItem(new CPropertyItem(_T("Z�᷽��Χ"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pWayPointObj->fSuspend);
	AddPropItem(new CPropertyItem(_T("�ؾ�ͣ��ʱ��"), szBuffer, PIT_EDIT, szBuffer));

	
}

void CMapPropertyList::DisplayTriggerObj(const tagMapTrigger* pTriggerObj)
{
	m_nTriggerNum = 0;
	CString szBuffer;
	CString id;
	ASSERT(pTriggerObj != NULL);

	if(pTriggerObj->eType==EMT_Null)             
		szBuffer=_T("��");
	else if(pTriggerObj->eType==EMT_GotoNewMap)  
		szBuffer=_T("�����л���");
	else if(pTriggerObj->eType==EMT_Script)
		szBuffer=_T("�ű�����");
	
	AddPropItem(new CPropertyItem(_T("����������"), szBuffer, PIT_COMBO, _T("��|�����л���|�ű�����|")));
	
	for(int i=0; i<(int)pTriggerObj->region.size(); i++)
	{
		if(pTriggerObj->region[i].x<=0.0f 
			|| pTriggerObj->region[i].y<0.0f
			|| pTriggerObj->region[i].z<=0.0f)
			break;
		id.Format(_T("��%d����X����"), i);
		szBuffer.Format(_T("%f"), pTriggerObj->region[i].x);
		AddPropItem(new CPropertyItem(id, szBuffer, PIT_EDIT, szBuffer));

		id.Format(_T("��%d����Y����"), i);
		szBuffer.Format(_T("%f"), pTriggerObj->region[i].y);
		AddPropItem(new CPropertyItem(id, szBuffer, PIT_EDIT, szBuffer));

		id.Format(_T("��%d����Z����"), i);
		szBuffer.Format(_T("%f"), pTriggerObj->region[i].z);
		AddPropItem(new CPropertyItem(id, szBuffer, PIT_EDIT, szBuffer));
		m_nTriggerNum ++;
	}

	szBuffer.Format(_T("%f"), pTriggerObj->fHeight);
	AddPropItem(new CPropertyItem(_T("�߶�"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%s"), pTriggerObj->szMapName);
	AddPropItem(new CPropertyItem(_T("�����л����ͼ����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%s"), pTriggerObj->szWayPoint);
	AddPropItem(new CPropertyItem(_T("Ŀ���ͼλ��"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%s"), pTriggerObj->szScriptName);
	AddPropItem(new CPropertyItem(_T("�ű�����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%u"), pTriggerObj->dwParam);
	AddPropItem(new CPropertyItem(_T("����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%u"), pTriggerObj->dwQuestSerial);
	AddPropItem(new CPropertyItem(_T("�������к�"), szBuffer, PIT_EDIT, szBuffer));
}

void CMapPropertyList::DisplayEventTrigger(const tagMapEventTrigger* pEventTrigger)
{
	CString szBuffer;
	CString id;
	ASSERT(pEventTrigger != NULL);

	if(pEventTrigger->eCondition==ETC_Null)             
		szBuffer=_T("��");

	AddPropItem(new CPropertyItem(_T("��������"), szBuffer, PIT_COMBO, _T("��|")));

	szBuffer.Format(_T("%u"), pEventTrigger->dwConditionParam1);
	AddPropItem(new CPropertyItem(_T("��������1"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%u"), pEventTrigger->dwConditionParam2);
	AddPropItem(new CPropertyItem(_T("��������2"), szBuffer, PIT_EDIT, szBuffer));
		
	if(pEventTrigger->eResult==ETR_Null)             
		szBuffer=_T("��");

	AddPropItem(new CPropertyItem(_T("��������"), szBuffer, PIT_COMBO, _T("��|")));

	szBuffer.Format(_T("%u"), pEventTrigger->dwResultParam1);
	AddPropItem(new CPropertyItem(_T("�������1"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%u"), pEventTrigger->dwResultParam2);
	AddPropItem(new CPropertyItem(_T("�������2"), szBuffer, PIT_EDIT, szBuffer));

}

void CMapPropertyList::DisplayMapRectObj(const tagMapArea* pMapArea)
{
	CString szBuffer;
	CString id;
	m_nMapAreaNum = 0;
	ASSERT(pMapArea != NULL);

	if(pMapArea->eType==EMA_Null)             
		szBuffer=_T("��");
	else if(pMapArea->eType==EMA_PVP)  
		szBuffer=_T("PVP����");
	else if(pMapArea->eType==EMA_Safe)  
		szBuffer=_T("��ȫ����");
	else if(pMapArea->eType==EMA_Stall)  
		szBuffer=_T("��̯����");
	else if(pMapArea->eType==EMA_Display)  
		szBuffer=_T("��ʾ����");
	else if(pMapArea->eType==EMA_Prison)	
		szBuffer=_T("��������");	
	else if(pMapArea->eType==EMA_Script)	
		szBuffer=_T("�ű�����");	

	AddPropItem(new CPropertyItem(_T("����������"), szBuffer, PIT_COMBO, _T("��|PVP����|��ȫ����|��̯����|��ʾ����|��������|�ű�����|")));

	for(int i=0; i<(int)pMapArea->region.size(); i++)
	{
		if(pMapArea->region[i].x<=0.0f 
			|| pMapArea->region[i].y<0.0f
			|| pMapArea->region[i].z<=0.0f)
			break;
		id.Format(_T("��%d����X����"), i);
		szBuffer.Format(_T("%f"), pMapArea->region[i].x);
		AddPropItem(new CPropertyItem(id, szBuffer, PIT_EDIT, szBuffer));

		id.Format(_T("��%d����Y����"), i);
		szBuffer.Format(_T("%f"), pMapArea->region[i].y);
		AddPropItem(new CPropertyItem(id, szBuffer, PIT_EDIT, szBuffer));

		id.Format(_T("��%d����Z����"), i);
		szBuffer.Format(_T("%f"), pMapArea->region[i].z);
		AddPropItem(new CPropertyItem(id, szBuffer, PIT_EDIT, szBuffer));
		m_nMapAreaNum ++;
	}

	szBuffer.Format(_T("%f"), pMapArea->fHeight);
	AddPropItem(new CPropertyItem(_T("�߶�"), szBuffer, PIT_EDIT, szBuffer));
	szBuffer.Format(_T("%ld"), pMapArea->dwMiniMapSize);
	AddPropItem(new CPropertyItem(_T("����ͼ��С"), szBuffer, PIT_EDIT, szBuffer));

	if(pMapArea->bDistFog)
		szBuffer = _T("��");
	else
		szBuffer = _T("��");
	AddPropItem(new CPropertyItem(_T("�Ƿ񵥶�������"), szBuffer, PIT_COMBO, _T("��|��|")));

	tagDistFog fog;
	ASSERT( sizeof(fog) == sizeof(pMapArea->byDistFog) );
	memcpy(&fog, pMapArea->byDistFog, sizeof(fog));
	szBuffer.Format(_T("RGB(%d,%d,%d)"),fog.color.R, fog.color.G, fog.color.B);
	AddPropItem(new CPropertyItem(_T("����ɫ"), szBuffer, PIT_COLOR, _T("")));
	szBuffer.Format(_T("%f"), fog.start);
	AddPropItem(new CPropertyItem(_T("��start"), szBuffer, PIT_EDIT, szBuffer));
	szBuffer.Format(_T("%f"), fog.end);
	AddPropItem(new CPropertyItem(_T("��end"), szBuffer, PIT_EDIT, szBuffer));
	szBuffer.Format(_T("%f"), fog.density);
	AddPropItem(new CPropertyItem(_T("��density"), szBuffer, PIT_EDIT, szBuffer));
}

void CMapPropertyList::DisplaySoundObj(const tagMapSound* pSoundObj)
{
	CString szBuffer;
	ASSERT(pSoundObj != NULL);

	szBuffer.Format(_T("%f"), pSoundObj->fPos[0]);
	AddPropItem(new CPropertyItem(_T("X����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pSoundObj->fPos[1]);
	AddPropItem(new CPropertyItem(_T("Y����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pSoundObj->fPos[2]);
	AddPropItem(new CPropertyItem(_T("Z����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pSoundObj->fRange);
	AddPropItem(new CPropertyItem(_T("��Χ"), szBuffer, PIT_EDIT, szBuffer));

	AddPropItem(new CPropertyItem(_T("��Ч�ļ�����"), pSoundObj->szFileName, PIT_FILE,_T("")));

	szBuffer.Format(_T("%f"), pSoundObj->fVolume);
	AddPropItem(new CPropertyItem(_T("����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pSoundObj->fNoAtteRange);
	AddPropItem(new CPropertyItem(_T("û��˥���ķ�Χ"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%u"), pSoundObj->dwInterval);
	AddPropItem(new CPropertyItem(_T("����һ����Чʱ��[����]"), szBuffer, PIT_EDIT, szBuffer));
}

void CMapPropertyList::DisplayPathPoint(const tagMapPathPoint* pPathPoint)
{
	CString szBuffer;
	ASSERT(pPathPoint != NULL);

	szBuffer.Format(_T("%f"), pPathPoint->vPos.x);
	AddPropItem(new CPropertyItem(_T("X����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pPathPoint->vPos.y);
	AddPropItem(new CPropertyItem(_T("Y����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pPathPoint->vPos.z);
	AddPropItem(new CPropertyItem(_T("Z����"), szBuffer, PIT_EDIT, szBuffer));

	//szBuffer.Format(_T("%f"), pPathPoint->fRange);
	//AddPropItem(new CPropertyItem(_T("��Χ"), szBuffer, PIT_EDIT, szBuffer));
}

void CMapPropertyList::DisplayNPCNav(const tagNPCNavMapBuildStartPoint* pNPCNav)
{
	CString szBuffer;
	ASSERT(pNPCNav != NULL);

	szBuffer.Format(_T("%f"), pNPCNav->vPos.x);
	AddPropItem(new CPropertyItem(_T("X����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pNPCNav->vPos.y);
	AddPropItem(new CPropertyItem(_T("Y����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pNPCNav->vPos.z);
	AddPropItem(new CPropertyItem(_T("Z����"), szBuffer, PIT_EDIT, szBuffer));
}

void CMapPropertyList::DisplayPlayerNav(const tagPlayerNavMapBuildStartPoint* pPlayerNav)
{
	CString szBuffer;
	ASSERT(pPlayerNav != NULL);

	szBuffer.Format(_T("%f"), pPlayerNav->vPos.x);
	AddPropItem(new CPropertyItem(_T("X����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pPlayerNav->vPos.y);
	AddPropItem(new CPropertyItem(_T("Y����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pPlayerNav->vPos.z);
	AddPropItem(new CPropertyItem(_T("Z����"), szBuffer, PIT_EDIT, szBuffer));
}

void CMapPropertyList::DisplaySpawnPoint(const tagMapSpawnPoint* pSpawnPoint)
{
	CString szBuffer;
	ASSERT(pSpawnPoint != NULL);
	
	szBuffer.Format(_T("%f"), pSpawnPoint->vPos.x);
	AddPropItem(new CPropertyItem(_T("X����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pSpawnPoint->vPos.y);
	AddPropItem(new CPropertyItem(_T("Y����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pSpawnPoint->vPos.z);
	AddPropItem(new CPropertyItem(_T("Z����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%u"), pSpawnPoint->dwGroupID);
	AddPropItem(new CPropertyItem(_T("������ID"), szBuffer, PIT_EDIT, szBuffer));


	if(pSpawnPoint->bCollide)
		szBuffer = _T("��");
	else
		szBuffer = _T("��");
	AddPropItem(new CPropertyItem(_T("�Ƿ���ײ"), szBuffer, PIT_COMBO, _T("��|��|")));

}

void CMapPropertyList::DisplayPointLightObj(const tagMapPointLight* pPointLight)
{
	CString szBuffer;
	ASSERT(pPointLight != NULL);

	szBuffer.Format(_T("%f"), pPointLight->fPos[0]);
	AddPropItem(new CPropertyItem(_T("X����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pPointLight->fPos[1]);
	AddPropItem(new CPropertyItem(_T("Y����"), szBuffer, PIT_EDIT, szBuffer));

	szBuffer.Format(_T("%f"), pPointLight->fPos[2]);
	AddPropItem(new CPropertyItem(_T("Z����"), szBuffer, PIT_EDIT, szBuffer));


	szBuffer.Format(_T("RGB(%d,%d,%d)"),
		(BYTE)(pPointLight->diffuse[0]*255), (BYTE)(pPointLight->diffuse[1]*255), (BYTE)(pPointLight->diffuse[2]*255));
	AddPropItem(new CPropertyItem(_T("������"), szBuffer, PIT_COLOR, _T("")));

	szBuffer.Format(_T("RGB(%d,%d,%d)"),
		(BYTE)(pPointLight->specular[0]*255), (BYTE)(pPointLight->specular[1]*255), (BYTE)(pPointLight->specular[2]*255));
	AddPropItem(new CPropertyItem(_T("�����"), szBuffer, PIT_COLOR, _T("")));

	szBuffer.Format(_T("%f"), pPointLight->range);
	AddPropItem(new CPropertyItem(_T("Ӱ�췶Χ"), szBuffer, PIT_EDIT, szBuffer));


	float atten = pPointLight->attenuation[1] /** pPointLight->range*/;
	szBuffer.Format(_T("%f"), atten);
	AddPropItem(new CPropertyItem(_T("˥��"), szBuffer, PIT_EDIT, szBuffer));

	if(pPointLight->bDynamicPL)
		szBuffer = _T("��");
	else 
		szBuffer = _T("��");
	AddPropItem(new CPropertyItem(_T("��̬��Դ "), szBuffer, PIT_COMBO, _T("��|��|")));

	szBuffer.Format(_T("%f"), pPointLight->fIncModulus);
	AddPropItem(new CPropertyItem(_T("��ǿϵ��"), szBuffer, PIT_EDIT, szBuffer));
	
	szBuffer.Format(_T("%f"), pPointLight->fnoBlockRange);
	AddPropItem(new CPropertyItem(_T("���ڵ���Χ"), szBuffer, PIT_EDIT, szBuffer));
}



bool CMapPropertyList::GetModifiedWayPointProp(tagMapWayPoint& WayPoint)
{
	CString szNewName =  ((CPropertyItem*)GetItemDataPtr(0))->m_curValue;
	CString szNewIndex = ((CPropertyItem*)GetItemDataPtr(1))->m_curValue;
	FLOAT fNewX = (FLOAT)_tstof(((CPropertyItem*)GetItemDataPtr(2))->m_curValue);
	FLOAT fNewY = (FLOAT)_tstof(((CPropertyItem*)GetItemDataPtr(3))->m_curValue);
	FLOAT fNewZ =    (FLOAT)_tstof(((CPropertyItem*)GetItemDataPtr(4))->m_curValue);
	FLOAT fXZone =    (FLOAT)_tstof(((CPropertyItem*)GetItemDataPtr(5))->m_curValue);
	FLOAT fYZone =    (FLOAT)_tstof(((CPropertyItem*)GetItemDataPtr(6))->m_curValue);
	FLOAT fZZone =    (FLOAT)_tstof(((CPropertyItem*)GetItemDataPtr(7))->m_curValue);
	FLOAT fSuspend = (FLOAT)_tstof(((CPropertyItem*)GetItemDataPtr(8))->m_curValue);

	if(szNewName==_T("") || szNewIndex==_T("")) 
	{
		AfxMessageBox(_T("�뽫������д����"));
		return false;
	}
	if(szNewName.GetLength()>31)
	{
		AfxMessageBox(_T("���������ֹ���"));
		return false;
	}
	if(_ttoi(szNewIndex)<0)
	{
		AfxMessageBox(_T("�������ô���"));
		return false;
	}
	_tcscpy_s(WayPoint.szName,sizeof(WayPoint.szName)/sizeof(TCHAR),szNewName.GetBuffer());
	WayPoint.byIndex=_ttoi(szNewIndex);

	WayPoint.vPos = Vector3(fNewX, fNewY, fNewZ);
	WayPoint.vRange = Vector3(fXZone, fYZone, fZZone);
	WayPoint.fSuspend = fSuspend;
	return true;
}

bool CMapPropertyList::GetModifiedTriggerProp(tagMapTrigger& Trigger)
{
	CString szNewType	= ((CPropertyItem*) GetItemDataPtr(0))->m_curValue;
	int nIndex = 1;
	for(int i=0; i<m_nTriggerNum; i++)
	{
		Vector3 vPos;
		vPos.x = (FLOAT)_tstof( ((CPropertyItem*)GetItemDataPtr(nIndex++))->m_curValue );
		vPos.y = (FLOAT)_tstof( ((CPropertyItem*)GetItemDataPtr(nIndex++))->m_curValue );
		vPos.z = (FLOAT)_tstof( ((CPropertyItem*)GetItemDataPtr(nIndex++))->m_curValue );
		Trigger.region.push_back(vPos);
		
	}
	Vector3 max = Trigger.region[0], min = Trigger.region[0];
	for(int i=1; i<m_nTriggerNum; i++)
	{
		//����
		if(Trigger.region[i].x>=max.x)
			max.x = Trigger.region[i].x;
		if(Trigger.region[i].y>=max.y)
			max.y = Trigger.region[i].y;
		if(Trigger.region[i].z>=max.z)
			max.z = Trigger.region[i].z;

		//��С��
		if(Trigger.region[i].x<=min.x)
			min.x = Trigger.region[i].x;
		if(Trigger.region[i].y<=min.y)
			min.y = Trigger.region[i].y;
		if(Trigger.region[i].z<=min.z)
			min.z = Trigger.region[i].z;	
	}
	Trigger.box.max = max;
	Trigger.box.min = min;

	FLOAT fHeight		= (FLOAT)_tstof( ((CPropertyItem*)GetItemDataPtr(nIndex++))->m_curValue );
	CString szMapName	= ((CPropertyItem*) GetItemDataPtr(nIndex++))->m_curValue;
	CString szWayPoint	= ((CPropertyItem*)GetItemDataPtr(nIndex++))->m_curValue ;
	CString szScriptName= ((CPropertyItem*) GetItemDataPtr(nIndex++))->m_curValue;
	DWORD dwParam		= (DWORD)_tstoi( ((CPropertyItem*)GetItemDataPtr(nIndex++))->m_curValue );
	DWORD dwQuest		= (DWORD)_tstoi( ((CPropertyItem*)GetItemDataPtr(nIndex++))->m_curValue );


	if(szNewType==_T("��"))
		Trigger.eType=EMT_Null;
	else if(szNewType==_T("�����л���"))
		Trigger.eType=EMT_GotoNewMap;
	else if(szNewType==_T("�ű�����"))
		Trigger.eType=EMT_Script;
	Trigger.fHeight = fHeight;
	Trigger.box.max.y = Trigger.fHeight + Trigger.box.min.y;
	Trigger.dwParam = dwParam;
	Trigger.dwQuestSerial = dwQuest;
	
	_tcscpy_s(Trigger.szMapName,sizeof(Trigger.szMapName)/sizeof(TCHAR),szMapName.GetBuffer());
	_tcscpy_s(Trigger.szWayPoint,sizeof(Trigger.szWayPoint)/sizeof(TCHAR),szWayPoint.GetBuffer());
	_tcscpy_s(Trigger.szScriptName,sizeof(Trigger.szScriptName)/sizeof(TCHAR),szScriptName.GetBuffer());
		
	return true;
}

bool CMapPropertyList::GetModifyedEventTrigger(tagMapEventTrigger* pEventTrigger)
{
	CString szCondition	= ((CPropertyItem*) GetItemDataPtr(0))->m_curValue;
	if(szCondition == _T("��"))
		pEventTrigger->eCondition = ETC_Null;

	pEventTrigger->dwConditionParam1 =  (DWORD)_tstoi( ((CPropertyItem*)GetItemDataPtr(1))->m_curValue );
	pEventTrigger->dwConditionParam2 =  (DWORD)_tstoi( ((CPropertyItem*)GetItemDataPtr(2))->m_curValue );


	CString szResult = ((CPropertyItem*) GetItemDataPtr(3))->m_curValue;
	if(szResult == _T("��"))
		pEventTrigger->eResult = ETR_Null;

	pEventTrigger->dwResultParam1 =  (DWORD)_tstoi( ((CPropertyItem*)GetItemDataPtr(4))->m_curValue );
	pEventTrigger->dwResultParam2 =  (DWORD)_tstoi( ((CPropertyItem*)GetItemDataPtr(5))->m_curValue );

	return true;
}

//--�õ��޸ĵĵ�ͼ��������
bool CMapPropertyList::GetModifyedMapRectProp(tagMapArea* pMapRect)
{
	CString szNewType	= ((CPropertyItem*) GetItemDataPtr(0))->m_curValue;
	if(szNewType==_T("��"))
		pMapRect->eType = EMA_Null;
	else if(szNewType==_T("PVP����"))
		pMapRect->eType = EMA_PVP;
	else if(szNewType==_T("��ȫ����"))
		pMapRect->eType = EMA_Safe;
	else if(szNewType==_T("��̯����"))
		pMapRect->eType = EMA_Stall;
	else if(szNewType==_T("��ʾ����"))
		pMapRect->eType = EMA_Display;
	else if(szNewType==_T("��������"))
		pMapRect->eType = EMA_Prison;
	else if(szNewType==_T("�ű�����"))
		pMapRect->eType = EMA_Script;

	int nIndex = 1;
	for(int i=0; i<m_nMapAreaNum; i++)
	{
		pMapRect->region[i].x = (FLOAT)_tstof( ((CPropertyItem*)GetItemDataPtr(nIndex++))->m_curValue );
		pMapRect->region[i].y = (FLOAT)_tstof( ((CPropertyItem*)GetItemDataPtr(nIndex++))->m_curValue );
		pMapRect->region[i].z = (FLOAT)_tstof( ((CPropertyItem*)GetItemDataPtr(nIndex++))->m_curValue );

	}
	Vector3 max = pMapRect->region[0], min = pMapRect->region[0];
	for(int i=1; i<m_nMapAreaNum; i++)
	{
		//����
		if(pMapRect->region[i].x>=max.x)
			max.x = pMapRect->region[i].x;
		if(pMapRect->region[i].y>=max.y)
			max.y = pMapRect->region[i].y;
		if(pMapRect->region[i].z>=max.z)
			max.z = pMapRect->region[i].z;

		//��С��
		if(pMapRect->region[i].x<=min.x)
			min.x = pMapRect->region[i].x;
		if(pMapRect->region[i].y<=min.y)
			min.y = pMapRect->region[i].y;
		if(pMapRect->region[i].z<=min.z)
			min.z = pMapRect->region[i].z;	
	}
	pMapRect->box.max = max;
	pMapRect->box.min = min;

	FLOAT fHeight		= (FLOAT)_tstof( ((CPropertyItem*)GetItemDataPtr(nIndex++))->m_curValue );
	pMapRect->fHeight = fHeight;
	pMapRect->box.max.y = pMapRect->fHeight + pMapRect->box.min.y;
	pMapRect->dwMiniMapSize = (DWORD)_tstol( ((CPropertyItem*)GetItemDataPtr(nIndex++))->m_curValue );

	// ��
	CString szIsFog	= ((CPropertyItem*) GetItemDataPtr(nIndex++))->m_curValue;
	if(szIsFog==_T("��"))
		pMapRect->bDistFog = true;
	else if(szIsFog==_T("��"))
		pMapRect->bDistFog = false;
	tagDistFog fog;
	memset( &fog, 0, sizeof(fog) );
	COLORREF clr = CPropertyList::Str2Color(((CPropertyItem*) GetItemDataPtr(nIndex++))->m_curValue);
	fog.color.R = GetRValue(clr);
	fog.color.G = GetGValue(clr);
	fog.color.B = GetBValue(clr);
	fog.color.A = 0xff;
	fog.start   = (float)_tstof( ((CPropertyItem*)GetItemDataPtr(nIndex++))->m_curValue );
	fog.end     = (float)_tstof( ((CPropertyItem*)GetItemDataPtr(nIndex++))->m_curValue );
	fog.density = (float)_tstof( ((CPropertyItem*)GetItemDataPtr(nIndex++))->m_curValue );
	ASSERT( sizeof(fog) == sizeof(pMapRect->byDistFog) );
	memcpy(pMapRect->byDistFog, &fog, sizeof(fog));

	return true;
}


//�õ��޸�NPC����
bool CMapPropertyList::GetModifiedNPCObjProp(tagMapNPC* p,Vector3& pos,float &fYaw,bool &bIsLoadMDL)
{
	pos.x = (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(1))->m_curValue);
	pos.y = (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(2))->m_curValue);
	pos.z = (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(3))->m_curValue);
	fYaw  =	(FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(4))->m_curValue);

	CPropertyItem* pItem=(CPropertyItem*) GetItemDataPtr(5);
	_tcscpy_s(p->szName, sizeof(p->szName)/sizeof(TCHAR)-1, pItem->m_curValue);

	CString szNewType	= ((CPropertyItem*) GetItemDataPtr(6))->m_curValue;
	if(szNewType==_T("��"))
		p->bCollide = true;
	else if(szNewType==_T("��"))
		p->bCollide = false;

	p->fRadius = (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(7))->m_curValue);

	DWORD id = _ttoi((((CPropertyItem*) GetItemDataPtr(0))->m_curValue)); 
	bIsLoadMDL=false;
	if(id != p->dwTypeID)
	{
		if(g_mapEditor->GetMap()->GetAttDataBase()->FindCreatureAtt(id) == NULL)
		{
			AfxMessageBox(_T("��Ч������ID"));
			return false;
		}
	}

	if(id != p->dwTypeID)
	{
		p->dwTypeID = id;
		bIsLoadMDL=true;
	}



	return true; 
}

//--�õ��޸ĵĶ�̬�ϰ�������
bool CMapPropertyList::GetModifyedDynamicBlockProp(tagMapDynamicBlock* p, Vector3& pos, bool &bIsLoadMDL)
{
	pos.x	=  (float)_tstof(((CPropertyItem*) GetItemDataPtr(0))->m_curValue);
	pos.y	=  (float)_tstof(((CPropertyItem*) GetItemDataPtr(1))->m_curValue);
	pos.z	=  (float)_tstof(((CPropertyItem*) GetItemDataPtr(2))->m_curValue);

	p->fScale[0] =  (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(3))->m_curValue);
	p->fScale[1] =  (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(4))->m_curValue);
	p->fScale[2] =  (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(5))->m_curValue);

	p->fYaw	=  (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(6))->m_curValue);

	p->fYaw = _DegToRad(p->fYaw);

	bIsLoadMDL=false;
	CString temp = ((CPropertyItem*) GetItemDataPtr(7))->m_curValue ;
	if(temp != p->szModelPath)
	{
		memset(&p->szModelPath, 0, sizeof(p->szModelPath));
		memcpy(&p->szModelPath, temp.GetBuffer(), temp.GetLength());
		bIsLoadMDL=true;
	}

	return true;
}

bool CMapPropertyList::GetModifiedMapDoor(tagMapDoor* p, Vector3& pos, bool &bIsLoadMDL)
{
	pos.x	=  (float)_tstof(((CPropertyItem*) GetItemDataPtr(0))->m_curValue);
	pos.y	=  (float)_tstof(((CPropertyItem*) GetItemDataPtr(1))->m_curValue);
	pos.z	=  (float)_tstof(((CPropertyItem*) GetItemDataPtr(2))->m_curValue);

	p->fScale[0] =  (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(3))->m_curValue);
	p->fScale[1] =  (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(4))->m_curValue);
	p->fScale[2] =  (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(5))->m_curValue);

	p->fYaw	=  (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(6))->m_curValue);

	p->fYaw = _DegToRad(p->fYaw);

	bIsLoadMDL=false;
	CString temp = ((CPropertyItem*) GetItemDataPtr(7))->m_curValue ;
	if(temp != p->szModelPath)
	{
		memset(&p->szModelPath, 0, sizeof(p->szModelPath));
		memcpy(&p->szModelPath, temp.GetBuffer(), temp.GetLength());
		bIsLoadMDL=true;
	}
	CString szTemp =  ((CPropertyItem*) GetItemDataPtr(8))->m_curValue;
	if(szTemp==_T("��"))
		p->bInitState = TRUE;
	else 
		p->bInitState = FALSE;

	szTemp =  ((CPropertyItem*) GetItemDataPtr(9))->m_curValue;
	if(szTemp==_T("��"))
		p->bBlockingTileNPC = TRUE;
	else 
		p->bBlockingTileNPC = FALSE;

	p->dwTypeID =  (DWORD)_ttoi(((CPropertyItem*) GetItemDataPtr(10))->m_curValue);


	return true;
}

bool CMapPropertyList::GetModifiedMapCarrier(tagMapCarrier* p, bool &bIsLoadMDL)
{
	bIsLoadMDL=false;
	CString temp = ((CPropertyItem*) GetItemDataPtr(0))->m_curValue ;
	if(temp != p->szModelPath)
	{
		memset(&p->szModelPath, 0, sizeof(p->szModelPath));
		memcpy(&p->szModelPath, temp.GetBuffer(), temp.GetLength());
		bIsLoadMDL=true;
	}

	temp = ((CPropertyItem*) GetItemDataPtr(1))->m_curValue ;
	if(!temp.IsEmpty())
	{
		_tcscpy_s(p->szWayPoint,sizeof(p->szWayPoint)/sizeof(TCHAR)-1,temp);
	}
	else
	{
		memset(p->szWayPoint, 0 ,sizeof(p->szWayPoint));
	}

	p->fYaw =  (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(2))->m_curValue);

	temp = ((CPropertyItem*) GetItemDataPtr(3))->m_curValue;
	if( temp == _T("��") )
		p->bSurfaceType = 0;
	else if( temp == _T("ѩ��") )
		p->bSurfaceType = 1;
	else if( temp == _T("ʯ��") )
		p->bSurfaceType = 2;
	else if( temp == _T("��") )
		p->bSurfaceType = 3;
	else if( temp == _T("�ݵ�") )
		p->bSurfaceType = 4;
	else if( temp == _T("ɳ��") )
		p->bSurfaceType = 5;
	else if( temp == _T("����") )
		p->bSurfaceType = 6;
	else if( temp == _T("����") )
		p->bSurfaceType = 7;
	else
		p->bSurfaceType = 8;

	p->fSpeed =  (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(4))->m_curValue);

	return true;
}

//�õ��޸ľ�̬��������
bool CMapPropertyList::GetModifiedStaticObjProp(tagStaticMapObj* p,Vector3& pos,bool &bIsLoadMDL)
{
	pos.x =  (float)_tstof(((CPropertyItem*) GetItemDataPtr(0))->m_curValue);
	pos.y =  (float)_tstof(((CPropertyItem*) GetItemDataPtr(1))->m_curValue);
	pos.z =  (float)_tstof(((CPropertyItem*) GetItemDataPtr(2))->m_curValue);

	p->fRotate[1] =  (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(3))->m_curValue);
	p->fRotate[0] =  (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(4))->m_curValue);
	p->fRotate[2] =  (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(5))->m_curValue);

	p->fRotate[1]=_DegToRad(p->fRotate[1]);
	p->fRotate[0]=_DegToRad(p->fRotate[0]);
	p->fRotate[2]=_DegToRad(p->fRotate[2]);

	p->fScale[0] =  (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(6))->m_curValue);
	p->fScale[1] =  (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(7))->m_curValue);
	p->fScale[2] =  (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(8))->m_curValue);
	

	bIsLoadMDL=false;

	CString temp = ((CPropertyItem*) GetItemDataPtr(9))->m_curValue ;
	if(temp != p->szMdlPath)
	{
		memset(&p->szMdlPath, 0, sizeof(p->szMdlPath));
		memcpy(&p->szMdlPath, temp.GetBuffer(), temp.GetLength());
		bIsLoadMDL=true;
	}

	temp = ((CPropertyItem*) GetItemDataPtr(10))->m_curValue ;
	if(!temp.IsEmpty())
	{
		_tcscpy_s(p->szScriptName,sizeof(p->szScriptName)/sizeof(TCHAR)-1,temp);
	}
	else
	{
		memset(p->szScriptName,0,sizeof(p->szScriptName));
	}

	temp = ((CPropertyItem*) GetItemDataPtr(11))->m_curValue ;
	p->dwPickItemID = _ttoi(temp);

	CString szTemp =  ((CPropertyItem*) GetItemDataPtr(12))->m_curValue;
	if(szTemp==_T("��"))
		p->bShowMiniMap = true;
	else 
		p->bShowMiniMap = false;

	szTemp = ((CPropertyItem*) GetItemDataPtr(13))->m_curValue;
	if(szTemp == _T("��"))
		p->bLightMap = true;
	else
		p->bLightMap = false;

	szTemp = ((CPropertyItem*) GetItemDataPtr(14))->m_curValue;
	if(szTemp == _T("�Զ�����"))
		p->byViewLevel = 0;
	else if(szTemp == _T("��"))
		p->byViewLevel = 1;
	else if(szTemp == _T("��"))
		p->byViewLevel = 2;
	else
		p->byViewLevel = 3;

	p->fMtlDiffuseFactor = (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(15))->m_curValue);
	if( p->fMtlDiffuseFactor<=0.0f )
	{
		p->fMtlDiffuseFactor=1.5f;
		szTemp.Format(_T("%f"),p->fMtlDiffuseFactor);
		((CPropertyItem*) GetItemDataPtr(15))->m_curValue=szTemp;
	}

	szTemp = ((CPropertyItem*) GetItemDataPtr(16))->m_curValue;
	if(szTemp == _T("��"))
		p->bNoShadow = true;
	else
		p->bNoShadow = false;

	szTemp = ((CPropertyItem*) GetItemDataPtr(17))->m_curValue;
	if( szTemp == _T("��") )
		p->bSurfaceType = 0;
	else if( szTemp == _T("ѩ��") )
		p->bSurfaceType = 1;
	else if( szTemp == _T("ʯ��") )
		p->bSurfaceType = 2;
	else if( szTemp == _T("��") )
		p->bSurfaceType = 3;
	else if( szTemp == _T("�ݵ�") )
		p->bSurfaceType = 4;
	else if( szTemp == _T("ɳ��") )
		p->bSurfaceType = 5;
	else if( szTemp == _T("����") )
		p->bSurfaceType = 6;
	else if( szTemp == _T("����") )
		p->bSurfaceType = 7;
	else
		p->bSurfaceType = 8;

	szTemp = ((CPropertyItem*) GetItemDataPtr(18))->m_curValue;
	if(szTemp == _T("��"))
		p->bPathfinding = true;
	else
		p->bPathfinding = false;

	szTemp = ((CPropertyItem*) GetItemDataPtr(19))->m_curValue;
	if(szTemp == _T("��"))
		p->bNotSupportNPCNavMap = true;
	else
		p->bNotSupportNPCNavMap = false;

	szTemp = ((CPropertyItem*) GetItemDataPtr(20))->m_curValue;
	if( szTemp == _T("��ͨ") )
		p->byBoxSize = 0;
	else if( szTemp == _T("С") )
		p->byBoxSize = 1;
	else
		p->byBoxSize = 2;

	return true;
}

bool CMapPropertyList::GetModifyedMapTriggerEffect(tagMapTriggerEffect* p, Vector3& pos, bool &bIsLoadMDL)
{
	pos.x =  (float)_tstof(((CPropertyItem*) GetItemDataPtr(0))->m_curValue);
	pos.y =  (float)_tstof(((CPropertyItem*) GetItemDataPtr(1))->m_curValue);
	pos.z =  (float)_tstof(((CPropertyItem*) GetItemDataPtr(2))->m_curValue);

	p->fRotate[1] =  (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(3))->m_curValue);
	p->fRotate[0] =  (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(4))->m_curValue);
	p->fRotate[2] =  (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(5))->m_curValue);

	p->fRotate[1]=_DegToRad(p->fRotate[1]);
	p->fRotate[0]=_DegToRad(p->fRotate[0]);
	p->fRotate[2]=_DegToRad(p->fRotate[2]);

	p->fScale[0] =  (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(6))->m_curValue);
	p->fScale[1] =  (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(7))->m_curValue);
	p->fScale[2] =  (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(8))->m_curValue);


	bIsLoadMDL=false;

	CString temp = ((CPropertyItem*) GetItemDataPtr(9))->m_curValue ;
	if(temp != p->szMdlPath)
	{
		memset(&p->szMdlPath, 0, sizeof(p->szMdlPath));
		memcpy(&p->szMdlPath, temp.GetBuffer(), temp.GetLength());
		bIsLoadMDL=true;
	}

	return true;
}

//--�õ��޸ĵ���Ч����
bool CMapPropertyList::GetModifiedSoundProp(tagMapSound* pSound)
{
	CString szName =((CPropertyItem*) GetItemDataPtr(4))->m_curValue ;
	if(szName==_T(""))
	{
		AfxMessageBox(_T("��Ч�ļ����Ʋ���Ϊ��"));
	    return false;
	}
	pSound->fPos[0] = (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(0))->m_curValue);
	pSound->fPos[1] = (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(1))->m_curValue);
	pSound->fPos[2] = (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(2))->m_curValue);
	pSound->fRange	= (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(3))->m_curValue);
	_tcscpy_s(pSound->szFileName,sizeof(pSound->szFileName)/sizeof(TCHAR),szName.GetBuffer());
	pSound->fVolume	= (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(5))->m_curValue);
	pSound->fNoAtteRange	= (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(6))->m_curValue);
	pSound->dwInterval = (DWORD)_tstoi(((CPropertyItem*) GetItemDataPtr(7))->m_curValue);

	return true;
}

bool CMapPropertyList::GetModifyedNPCNav(tagNPCNavMapBuildStartPoint* pNPCNav)
{
	pNPCNav->vPos.x = (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(0))->m_curValue);
	pNPCNav->vPos.y = (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(1))->m_curValue);
	pNPCNav->vPos.z = (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(2))->m_curValue);

	return true;
}

bool CMapPropertyList::GetModifyedPlayerNav(tagPlayerNavMapBuildStartPoint* pPlayerNav)
{
	pPlayerNav->vPos.x = (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(0))->m_curValue);
	pPlayerNav->vPos.y = (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(1))->m_curValue);
	pPlayerNav->vPos.z = (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(2))->m_curValue);

	return true;
}

//--�õ��޸ĵ�Ѱ·������
bool CMapPropertyList::GetModifyedPathPointProp(tagMapPathPoint* pPathPoint)
{
	pPathPoint->vPos.x = (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(0))->m_curValue);
	pPathPoint->vPos.y = (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(1))->m_curValue);
	pPathPoint->vPos.z = (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(2))->m_curValue);

	//pPathPoint->fRange = (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(3))->m_curValue);
	return true;
}

//--�õ��޸ĵ�ˢ�ֵ�
bool CMapPropertyList::GetModifyedSpawnPointProp(tagMapSpawnPoint* pSpawnPoint)
{
	pSpawnPoint->vPos.x = (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(0))->m_curValue);
	pSpawnPoint->vPos.y = (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(1))->m_curValue);
	pSpawnPoint->vPos.z = (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(2))->m_curValue);
	pSpawnPoint->dwGroupID = (DWORD)_tstoi( ((CPropertyItem*)GetItemDataPtr(3))->m_curValue );
	CString szTemp = ((CPropertyItem*) GetItemDataPtr(4))->m_curValue;
	if(szTemp == _T("��"))
		pSpawnPoint->bCollide = true;
	else 
		pSpawnPoint->bCollide = false;
	return true;
}

//--�õ��޸ĵĵ��Դ����
bool CMapPropertyList::GetModifiedPointLightProp(tagMapPointLight* pPointLight)
{
	pPointLight->fPos[0] = (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(0))->m_curValue);
	pPointLight->fPos[1] = (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(1))->m_curValue);
	pPointLight->fPos[2] = (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(2))->m_curValue);

	COLORREF clr;

	clr = CPropertyList::Str2Color(((CPropertyItem*) GetItemDataPtr(3))->m_curValue);
	pPointLight->diffuse[0] = GetRValue(clr)/255.0f;
	pPointLight->diffuse[1] = GetGValue(clr)/255.0f;
	pPointLight->diffuse[2] = GetBValue(clr)/255.0f;
	pPointLight->diffuse[3] = 1.0f;

	clr = CPropertyList::Str2Color(((CPropertyItem*) GetItemDataPtr(4))->m_curValue);
	pPointLight->specular[0] = GetRValue(clr)/255.0f;
	pPointLight->specular[1] = GetGValue(clr)/255.0f;
	pPointLight->specular[2] = GetBValue(clr)/255.0f;
	pPointLight->specular[3] =1.0f;

	//Ӱ�췶Χ
	pPointLight->range = (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(5))->m_curValue);

	//˥��ֵ
	pPointLight->attenuation[0] = 0.0f;
	pPointLight->attenuation[1] = (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(6))->m_curValue);/*/pPointLight->range*/
	pPointLight->attenuation[2] = 0.0f;

	CString szTemp = ((CPropertyItem*) GetItemDataPtr(7))->m_curValue;
	if(szTemp == _T("��"))
		pPointLight->bDynamicPL = true;
	else 
		pPointLight->bDynamicPL = false;

	FLOAT fModulus = 0.0f;
	fModulus = (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(8))->m_curValue);
	if(fModulus>=2.0f)
		pPointLight->fIncModulus = 2.0f;
	else if(fModulus<0.0f)
		pPointLight->fIncModulus = 0.0f;
	else 
		pPointLight->fIncModulus = fModulus;

	FLOAT fnoBlockRange = 0.0f;
	fnoBlockRange = (FLOAT)_tstof(((CPropertyItem*) GetItemDataPtr(9))->m_curValue);
	if(fnoBlockRange<0.0f)
		pPointLight->fnoBlockRange = 0.0f;
	else 
		pPointLight->fnoBlockRange = fnoBlockRange;

	return true;
}


