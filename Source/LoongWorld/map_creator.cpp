//----------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: map_creator.cpp
// author: aslan
// actor:
// data: 2008-6-27
// last:
// brief: ��ͼ��������ͳ��������е�MapMgr��ά������MapMgr�������ڣ����ṩ��ͼ��̬���ԣ�ȫ�ֶ���
//----------------------------------------------------------------------------------------------
#include "StdAfx.h"

#include "../WorldDefine/msg_map.h"

#include "map_creator.h"
#include "map.h"
#include "map_mgr.h"
#include "role.h"
#include "role_mgr.h"
#include "world_session.h"
#include "login_session.h"
#include "map_instance.h"
#include "map_instance_stable.h"
#include "map_restrict.h"
#include "game_guarder.h"
#include "map_instance_script.h"
#include "creature.h"
#include "TimeChecker.h"

#define POOL_SIZE 50*1024*1024		// ��ʼ�ش�СΪ50M


MapCreator::MapCreator()
: m_MapEndEvent(TRUE, FALSE), m_nNumMapMgr(0), m_nWhichEvent(1)
{

}

MapCreator::~MapCreator()
{
	Destroy();
}

//-----------------------------------------------------------------------------
// ��ʼ�����е�ͼ������Ϣ�����е�ͼ������
//-----------------------------------------------------------------------------
BOOL MapCreator::Init()
{
	m_pUtil			=	"Util";
	m_pThread		=	"Thread";
	m_dwInstanceIDGen	=	1000;

	RegisterAFMap();			// ע���ͼ����
	RegisterAFMapRestrict();	// ע���ͼ���Թ���

	// ��ʼ����ͼ�ڴ��
	NavKernel::Inst()->Init(POOL_SIZE, true);
	NavResMgr::Inst()->EnableIOAsync(false);//�رն��̶߳���

	// ��ȡ��ͼ�߼�����xml��
	const TCHAR* szMapLogicProtoFile = _T("data/system/attdata/map_logic_proto.xml");
	CreateObj("MapLogicContainer", "VarContainer");
	TObjRef<VarContainer> pLogic = "MapLogicContainer";
	pLogic->Load(NULL, szMapLogicProtoFile, "id");

	// ��ȡ��ͼ�����ļ����������ļ��ж�ȡҪ���صĵ�ͼ�ļ���
	CreateObj("MapContainer", "VarContainer");
	TObjRef<VarContainer> pVar = "MapContainer";
	pVar->Load("VirtualFileSys", _T("server_config/world/map_list.ini"));

	// ���ظ����ļ�Ȩ�ܺ�
	ZeroMemory(m_nInstanceNumCur, sizeof(m_nInstanceNumCur));
	m_nInstanceCoefNumLimit = pVar->GetInt(_T("coef_num"),	_T("instance"), 1000);
	m_nInstanceCoefNumCur = 0;

	// ���ظ�����ͼ����
	INT nMapNum = pVar->GetInt(_T("num"), _T("map"));
	if( nMapNum <= 0 ) return FALSE;

	TCHAR szTemp[X_SHORT_NAME] = {'\0'};
	for(INT n = 0; n < nMapNum; ++n)
	{
		_stprintf(szTemp, _T("map%d"), n+1);
		LPCTSTR szMapFileName = (LPCTSTR)pVar->GetString(szTemp, _T("map"));
		if( FALSE == P_VALID(szMapFileName) ) return FALSE;

		if( FALSE == LoadMapInfo(szMapFileName, pLogic) )
		{
			ASSERT(0);
			return FALSE;
		}
	}

	// ��ȡ������ͼ
	LPCTSTR szBornMapName = (LPCTSTR)pVar->GetString(_T("name"), _T("born_map"));
	m_dwBornMapID = m_pUtil->Crc32(szBornMapName);

	const tagMapInfo* pBornMapInfo = GetMapInfo(m_dwBornMapID);
	ASSERT( P_VALID(pBornMapInfo) );

	// ��ȡ������ͼ������
	m_nBornPosNum = pVar->GetInt(_T("waypoint_num"), _T("born_map"));
	ASSERT( m_nBornPosNum > 0 );

	m_pvBornPos = new Vector3[m_nBornPosNum];

	TCHAR szBornWPName[X_SHORT_NAME];
	for(INT n = 0; n < m_nBornPosNum; n++)
	{
		_stprintf(szTemp, _T("waypoint_%d"), (n+1));
		IFASTCODE->MemCpy(szBornWPName, pVar->GetString(szTemp, _T("born_map"), _T("")), sizeof(szBornWPName));
		DWORD dwBornWPID = m_pUtil->Crc32(szBornWPName);

		const tagMapWayPointInfoList* pWPList = pBornMapInfo->mapWayPointList.Peek(dwBornWPID);
		if( P_VALID(pWPList) && pWPList->list.Size() > 0 )
		{
			tagWayPointInfo info = pWPList->list.Front();
			m_pvBornPos[n] = info.vPos;
		}
		else
		{
			IMSG(_T("Can't find the reborn waypoint of the map %s\r\n"), pBornMapInfo->szMapFileName);
		}
	}


	// ��ȡ������ͼ
	LPCTSTR szPrisonMapName = (LPCTSTR)pVar->GetString(_T("name"), _T("prison_map"));
	m_dwPrisonMapID = m_pUtil->Crc32(szPrisonMapName);

	const tagMapInfo* pPrisonMapInfo = GetMapInfo(m_dwPrisonMapID);
	if( P_VALID(pPrisonMapInfo) )
	{

		TCHAR szPrisonWPName[X_SHORT_NAME];
		IFASTCODE->MemCpy(szPrisonWPName, pVar->GetString(_T("inprisonpoint"), _T("prison_map"), _T("")), sizeof(szPrisonWPName));
		DWORD dwPrisonWPID = m_pUtil->Crc32(szPrisonWPName);

		const tagMapWayPointInfoList* pWPList = pPrisonMapInfo->mapWayPointList.Peek(dwPrisonWPID);
		if( P_VALID(pWPList) && pWPList->list.Size() > 0 )
		{
			tagWayPointInfo info = pWPList->list.Front();
			m_PutInPrisonPoint = info.vPos;
		}
		else
		{
			IMSG(_T("Can't find the putin prison waypoint of the map %s\r\n"), pPrisonMapInfo->szMapFileName);
		}

		IFASTCODE->MemCpy(szPrisonWPName, pVar->GetString(_T("outprisonpoint"), _T("prison_map"), _T("")), sizeof(szPrisonWPName));
		dwPrisonWPID = m_pUtil->Crc32(szPrisonWPName);

		pWPList = pPrisonMapInfo->mapWayPointList.Peek(dwPrisonWPID);
		if( P_VALID(pWPList) && pWPList->list.Size() > 0 )
		{
			tagWayPointInfo info = pWPList->list.Front();
			m_PutOutPrisonPoint = info.vPos;
		}
		else
		{
			IMSG(_T("Can't find the putout prison waypoint of the map %s\r\n"), pPrisonMapInfo->szMapFileName);
		}

	}
	else
	{
		ASSERT(0);
		IMSG(_T("Can't find the prison map %s\r\n"), szPrisonMapName);
	}

	// ���ִ�ĸ�������
	m_nMaxStableInstanceNum = pVar->GetInt(_T("num"), _T("initial_map"));


	KillObj("MapContainer");
	KillObj("MapLogicContainer");

	// �������е�MapInfo�����ɸ���MapMgr
	tagMapInfo* pInfo  = NULL;
	m_mapMapInfo.ResetIterator();
	while( m_mapMapInfo.PeekNext(pInfo) )
	{
		MapMgr* pMapMgr = CreateMapMgr(pInfo);
		if( !P_VALID(pMapMgr) )
		{
			ASSERT(0);
			return FALSE;
		}

		m_mapMapMgr.Add(pInfo->dwID, pMapMgr);
	}

	return TRUE;
}

//----------------------------------------------------------------------------
// �����ͼ������Ϣ
//----------------------------------------------------------------------------
BOOL MapCreator::LoadMapInfo(LPCTSTR szFileMapName, TObjRef<VarContainer>& pLogic)
{
	ASSERT( P_VALID(szFileMapName) );
	if( FALSE == P_VALID(szFileMapName) ) return FALSE;

	// ���ȶ�ȡMB�ļ�����ȡ��Ҫ�ĵ�ͼ����
	TCHAR szFullFileMapName[MAX_PATH] = {_T('\0')};
	_sntprintf_s(szFullFileMapName, MAX_PATH, MAX_PATH, _T("data\\map\\%s\\%s.mb"), szFileMapName, szFileMapName);

	CreateObj("MapFile", "VirtualFileSys");
	TObjRef<VirtualFileSys> pVFS = "MapFile";
	HANDLE dwHandle = (HANDLE)pVFS->Open(szFullFileMapName);
	if( FALSE == GT_VALID(dwHandle) ) return FALSE;

	tagMapHeader header;
	pVFS->Read(&header, sizeof(header), (DWORD)dwHandle);

	tagMapInfo* pInfo = new tagMapInfo;
	if( FALSE == pInfo )
	{
		pVFS->Close((DWORD)dwHandle);
		KillObj("MapFile");
		return FALSE;
	}

	// ���ص�ͼ������
	LoadMapWayPoint(&header, (DWORD)dwHandle, pInfo);
	// ���ص�ͼ������
	LoadMapTrigger(&header, (DWORD)dwHandle, pInfo);
	// ���ص�ͼ����
	LoadMapArea(&header, (DWORD)dwHandle, pInfo);
	// ���ص�ͼ����
	LoadMapCreature(&header, (DWORD)dwHandle, pInfo);
	// ���ص�ͼˢ�ֵ�
	LoadMapSpawnPoint(&header, (DWORD)dwHandle, pInfo);
	// ���ص�ͼ������Ч

	// ��ֵpInfo�ĸ����߼�����
	pInfo->dwID					=	m_pUtil->Crc32(szFileMapName);
	pInfo->nWidth				=	pLogic->GetInt(_T("width"), szFileMapName);
	pInfo->nHeight				=	pLogic->GetInt(_T("height"), szFileMapName);
	pInfo->nVisDist				=	pLogic->GetInt(_T("visdist"), szFileMapName, VIS_DIST);
	pInfo->eType				=	(EMapType)pLogic->GetDword(_T("type"), szFileMapName);
	pInfo->eNoarmalMapType		=	(ENormalMapType)pLogic->GetDword(_T("normal_map_type"), szFileMapName);
	pInfo->eInstanceRank		=	(EMapInstanceRank)pLogic->GetDword(_T("instance_rank"), szFileMapName, EMIR_NoLimit);
	pInfo->dwMaxNumLimit		=	pLogic->GetDword(_T("instance_num_limit"), szFileMapName, DEFAULT_MAX_INSTANCE_NUM);
	pInfo->dwWeatherID			=	GT_INVALID;
	_tcsncpy(pInfo->szMapFileName, szFileMapName, MCalTCharArrayNum(pInfo->szMapFileName) - 1);
	
	// �����
	TCHAR szRebornWPName[X_SHORT_NAME];
	_tcsncpy(szRebornWPName, pLogic->GetString(_T("reborn_waypoint"), szFileMapName), MCalTCharArrayNum(szRebornWPName) - 1);

	DWORD dwRebornWPID = m_pUtil->Crc32(szRebornWPName);
	const tagMapWayPointInfoList* pWPList = pInfo->mapWayPointList.Peek(dwRebornWPID);
	if( P_VALID(pWPList) && pWPList->list.Size() > 0 )
	{
		tagWayPointInfo info = pWPList->list.Front();
		pInfo->vRebornPos = info.vPos;
	}
	else
	{
		IMSG(_T("Can't find the reborn waypoint of the map %s\r\n"), pInfo->szMapFileName);
	}
	// ���ص�ͼ�е���
	LoadMapDoor(&header, (DWORD)dwHandle, pInfo);

	m_mapMapInfo.Add(pInfo->dwID, pInfo);

	// ����Ǹ����������ھ�Ҫ����һ�������ĵ���ͼģ��
	if( EMT_Normal != pInfo->eType )
	{
		NavMap* pNav = LoadInstanceTemplate(pInfo);
		if( !P_VALID(pNav) )
		{
			ASSERT(0);
			return FALSE;
		}
		else
		{
			m_mapInstanceTemplate.Add(pInfo->dwID, pNav);
		}
	}

	KillObj("MapFile");

	return TRUE;
}

//----------------------------------------------------------------------------
// ���ص�ͼ������
//----------------------------------------------------------------------------
BOOL MapCreator::LoadMapWayPoint(tagMapHeader* pHeader, DWORD handle, tagMapInfo* pInfo)
{
	ASSERT( P_VALID(pHeader) && GT_VALID(handle) && P_VALID(pInfo) );
	TObjRef<VirtualFileSys> pVFS = "MapFile";
	ASSERT( P_VALID(pVFS) );

	if( pHeader->nNumWayPoint <= 0 )
	{
		return TRUE;
	}

	// ���ݵ������ƫ��������λ�ļ�
	pVFS->Seek(handle, pHeader->dwWayPointOffset, GT_SEEK_SET);

	tagMapWayPoint temp;
	for(INT n = 0; n < pHeader->nNumWayPoint; n++)
	{
		pVFS->Read(&temp, sizeof(temp), handle);
		DWORD dwWayPointID = m_pUtil->Crc32(temp.szName);

		tagMapWayPointInfoList* pList = pInfo->mapWayPointList.Peek(dwWayPointID);

		if( !P_VALID(pList) )
		{
			pList = new tagMapWayPointInfoList;
			pList->dwID = dwWayPointID;
			pInfo->mapWayPointList.Add(dwWayPointID, pList);
		}

		// ����һ��·����
		tagWayPointInfo point;
		point.vPos = temp.vPos;
		point.vRange = temp.vRange;
		
		pList->list.PushBack(point);
	}

	return TRUE;
}

//----------------------------------------------------------------------------
// ���ص�ͼ������
//----------------------------------------------------------------------------
BOOL MapCreator::LoadMapTrigger(tagMapHeader* pHeader, DWORD handle, tagMapInfo* pInfo)
{
	ASSERT( P_VALID(pHeader) && GT_VALID(handle) && P_VALID(pInfo) );
	TObjRef<VirtualFileSys> pVFS = "MapFile";
	ASSERT( P_VALID(pVFS) );

	if( pHeader->nNumTrigger <= 0 )
	{
		return TRUE;
	}

	pVFS->Seek(handle, pHeader->dwTriggerOffset, GT_SEEK_SET);

	DWORD dwObjID = GT_INVALID;
	INT nRegionPointNum = 0;
	TCHAR szMapName[X_LONG_NAME] = {0};
	TCHAR szWayPointName[X_SHORT_NAME] = {0};
	vector<POINT> region;

	for(INT n = 0; n < pHeader->nNumTrigger; n++)
	{
		// ���ȶ�ȡid�����
		pVFS->Read(&dwObjID, sizeof(dwObjID), handle);

		tagMapTriggerInfo* pTrigger = pInfo->mapTrigger.Peek(dwObjID);
		ASSERT( !P_VALID(pTrigger) );

		pTrigger = new tagMapTriggerInfo;

		// ����id������
		pTrigger->dwObjID = dwObjID;
		pVFS->Read(&pTrigger->eType, sizeof(pTrigger->eType), handle);

		// ��ȡ�����
		pVFS->Read(&nRegionPointNum, sizeof(nRegionPointNum), handle);
		ASSERT(nRegionPointNum > 0);

		region.clear();
		region.resize(nRegionPointNum);
		Vector3 vPos;
		POINT pt;
		for(INT m = 0; m < nRegionPointNum; m++)
		{
			pVFS->Read(&vPos, sizeof(vPos), handle);
			pt.x = LONG(vPos.x / FLOAT(TILE_SCALE));
			pt.y = LONG(vPos.z / FLOAT(TILE_SCALE));
			region[m] = pt;		// ��������
		}
		pTrigger->hRgn = CreatePolygonRgn((POINT*)&region[0], nRegionPointNum, ALTERNATE);
		ASSERT(NULL != pTrigger->hRgn);

		// ��ȡ��Χ��
		pVFS->Read(&pTrigger->box.max, sizeof(pTrigger->box.max), handle);
		pVFS->Read(&pTrigger->box.min, sizeof(pTrigger->box.min), handle);
		// ��ȡ�߶�
		pVFS->Read(&pTrigger->fHeight, sizeof(pTrigger->fHeight), handle);
		
		// ��ȡ��ͼ����
		pVFS->Read(szMapName, sizeof(szMapName), handle);

		// ��ȡ����������
		pVFS->Read(szWayPointName, sizeof(szWayPointName), handle);

		// ת����ID
		pTrigger->dwMapID = m_pUtil->Crc32(szMapName);
		pTrigger->dwWayPointID = m_pUtil->Crc32(szWayPointName);

		// ��תһ��λ�ã�Խ��srciptName��
		pVFS->Seek(handle, sizeof(TCHAR) * X_SHORT_NAME, GT_SEEK_CUR);

		// ��ȡֵ
		pVFS->Read(&pTrigger->dwParam, sizeof(pTrigger->dwParam), handle);

		// ��ת����һ��λ�ã�Խ��bFlag, bLock��
		pVFS->Seek(handle, sizeof(bool)+sizeof(bool), GT_SEEK_CUR);

		// ��ȡ�������к�
		pVFS->Read(&pTrigger->dwQuestSerial, sizeof(pTrigger->dwQuestSerial), handle);

		// ��ת����һ��λ�ã�Խ��byReserved��
		pVFS->Seek(handle, sizeof(BYTE)*124, GT_SEEK_CUR);

		// ��ӵ��б���
		pInfo->mapTrigger.Add(pTrigger->dwObjID, pTrigger);
	}

	return TRUE;
}

//----------------------------------------------------------------------------
// ��ȡ��ͼ������Ϣ
//----------------------------------------------------------------------------
BOOL MapCreator::LoadMapArea(tagMapHeader* pHeader, DWORD handle, tagMapInfo* pInfo)
{
	ASSERT( P_VALID(pHeader) && GT_VALID(handle) && P_VALID(pInfo) );
	TObjRef<VirtualFileSys> pVFS = "MapFile";
	ASSERT( P_VALID(pVFS) );

	if( pHeader->nNumMapRect <= 0 )
	{
		return TRUE;
	}

	pVFS->Seek(handle, pHeader->dwMapRectOffset, GT_SEEK_SET);

	DWORD dwObjID = GT_INVALID;
	EMapArea eType = EMA_Null;
	INT nRegionPointNum = 0;
	vector<POINT> region;

	for(INT n = 0; n < pHeader->nNumMapRect; n++)
	{
		pVFS->Read(&dwObjID, sizeof(dwObjID), handle);
		pVFS->Read(&eType, sizeof(eType), handle);

		// ��ʾ�������������Ҫ���أ���ת����һ��
		if( eType == EMA_Null || eType == EMA_Display )
		{
			pVFS->Read(&nRegionPointNum, sizeof(nRegionPointNum), handle);
			ASSERT( nRegionPointNum > 0 );

			INT nOffset = sizeof(Vector3)*nRegionPointNum + sizeof(AABBox) + sizeof(FLOAT)
							+ sizeof(bool) + sizeof(bool) + sizeof(BYTE)*128;

			pVFS->Seek(handle, nOffset, GT_SEEK_CUR);
			continue;
		}

		tagMapAreaInfo* pArea = new tagMapAreaInfo;

		// ����ID������
		pArea->dwObjID = dwObjID;
		pArea->eType = eType;

		// ���������
		pVFS->Read(&nRegionPointNum, sizeof(nRegionPointNum), handle);
		ASSERT(nRegionPointNum > 0);
		
		region.clear();
		region.resize(nRegionPointNum);
		Vector3 vPos;
		POINT pt;
		for(INT m = 0; m < nRegionPointNum; m++)
		{
			pVFS->Read(&vPos, sizeof(vPos), handle);
			pt.x = LONG(vPos.x / FLOAT(TILE_SCALE));
			pt.y = LONG(vPos.z / FLOAT(TILE_SCALE));
			region[m] = pt;
		}
		pArea->hRgn = CreatePolygonRgn((POINT*)&region[0], nRegionPointNum, ALTERNATE);
		ASSERT(NULL != pArea->hRgn);

		// ���������
		pVFS->Read(&pArea->box.max, sizeof(pArea->box.max), handle);
		pVFS->Read(&pArea->box.min, sizeof(pArea->box.min), handle);
		// ����߶�
		pVFS->Read(&pArea->fHeight, sizeof(pArea->fHeight), handle);

		// ��ת����һ����
		pVFS->Seek(handle, sizeof(bool)+sizeof(bool)+sizeof(BYTE)*128, GT_SEEK_CUR);

		// �������ͼ��뵽��ͬ���б�
		switch (eType)
		{
		case EMA_Safe:
			pInfo->mapSafeArea.Add(dwObjID, pArea);
			break;
		case EMA_PVP:
			pInfo->mapPVPArea.Add(dwObjID, pArea);
			break;
		case EMA_Stall:
			pInfo->mapStallArea.Add(dwObjID, pArea);
			break;
		case EMA_Prison:
			pInfo->mapPrisonArea.Add(dwObjID, pArea);
			break;
		case EMA_Script:
			pInfo->mapScriptArea.Add(dwObjID, pArea);
			break;
		default:
			SAFE_DEL(pArea);
			break;
		}

	}
	return TRUE;
}

//----------------------------------------------------------------------------
// ��ȡ��ͼ����
//----------------------------------------------------------------------------
BOOL MapCreator::LoadMapCreature(tagMapHeader* pHeader, DWORD handle, tagMapInfo* pInfo)
{
	ASSERT( P_VALID(pHeader) && GT_VALID(handle) && P_VALID(pInfo) );
	TObjRef<VirtualFileSys> pVFS = "MapFile";
	ASSERT( P_VALID(pVFS) );

	if( pHeader->nNumNPC <= 0 )
	{
		return TRUE;
	}

	// ���ݵ�ͼ�����ƫ��������λ�ļ�
	pVFS->Seek(handle, pHeader->dwNpcOffset, GT_SEEK_SET);

	tagMapNPC temp;
	for(INT n = 0; n < pHeader->nNumNPC; n++)
	{
		pVFS->Read(&temp, sizeof(temp), handle);

		tagMapCreatureInfo* pCreature = pInfo->mapCreatureInfo.Peek(temp.dwObjID);
		ASSERT( !P_VALID(pCreature) );

		pCreature = new tagMapCreatureInfo;
		pCreature->dwObjID = temp.dwObjID;
		pCreature->dwTypeID = temp.dwTypeID;
		pCreature->vPos = temp.vPos;
		pCreature->fYaw = 270 - temp.fYaw;
		pCreature->bCollide = temp.bCollide;
		pCreature->patrolList = pInfo->mapWayPointList.Peek(m_pUtil->Crc32(temp.szName));

		// ���뵽�б���
		pInfo->mapCreatureInfo.Add(pCreature->dwObjID, pCreature);
	}

	return TRUE;
}

//----------------------------------------------------------------------------
// ��ȡˢ�ֵ���Ϣ
//----------------------------------------------------------------------------
BOOL MapCreator::LoadMapSpawnPoint(tagMapHeader* pHeader, DWORD handle, tagMapInfo* pInfo)
{
	ASSERT( P_VALID(pHeader) && GT_VALID(handle) && P_VALID(pInfo) );
	TObjRef<VirtualFileSys> pVFS = "MapFile";
	ASSERT( P_VALID(pVFS) );

	if( pHeader->nNumSpawnPoint <= 0 )
	{
		return TRUE;
	}

	// ���ݵ�ͼ�����ƫ��������λ�ļ�
	pVFS->Seek(handle, pHeader->dwSpawnPointOffset, GT_SEEK_SET);

	tagMapSpawnPoint temp;
	for(INT n = 0; n < pHeader->nNumSpawnPoint; n++)
	{
		pVFS->Read(&temp, sizeof(temp), handle);

		tagMapSpawnPointInfo* pSpawnPoint = pInfo->mapSpawnPoint.Peek(temp.dwObjID);
		ASSERT( !P_VALID(pSpawnPoint) );

		pSpawnPoint = new tagMapSpawnPointInfo;
		pSpawnPoint->dwObjID = temp.dwObjID;
		pSpawnPoint->dwSpawnPointID = temp.dwGroupID;
		pSpawnPoint->nLevelInc = temp.nLevelInc;
		pSpawnPoint->bCollide = temp.bCollide;
		pSpawnPoint->vPos = temp.vPos;
		
		// ���뵽�б���
		pInfo->mapSpawnPoint.Add(pSpawnPoint->dwObjID, pSpawnPoint);
	}

	return TRUE;
}

//----------------------------------------------------------------------------
// ��ȡ��ͼ��Ч��Ϣ
//----------------------------------------------------------------------------
BOOL MapCreator::LoadMapTriggerEffect(tagMapHeader* pHeader, DWORD handle, tagMapInfo* pInfo)
{
	ASSERT( P_VALID(pHeader) && GT_VALID(handle) && P_VALID(pInfo) );
	TObjRef<VirtualFileSys> pVFS = "MapFile";
	ASSERT( P_VALID(pVFS) );

	if( pHeader->nNumMapTriggerEffect <= 0 )
	{
		return TRUE;
	}

	// ���ݵ�ͼ�����ƫ��������λ�ļ�
	pVFS->Seek(handle, pHeader->dwMapTriggerEffectOffset, GT_SEEK_SET);

	tagMapTriggerEffect temp;
	for(INT n = 0; n < pHeader->nNumMapTriggerEffect; n++)
	{
		pVFS->Read(&temp, sizeof(temp), handle);

		tagMapTriggerEffect* pTriggerEffect = pInfo->mapTriggerEffect.Peek(temp.dwObjID);
		ASSERT( !P_VALID(pTriggerEffect) );

		pTriggerEffect = new tagMapTriggerEffect;
		pTriggerEffect->dwObjID = temp.dwObjID;

		// ���뵽�б���
		pInfo->mapTriggerEffect.Add(pTriggerEffect->dwObjID, pTriggerEffect);
	}

	return TRUE;
}

//----------------------------------------------------------------------------
// ��ȡ��ͼ����Ϣ
//----------------------------------------------------------------------------
BOOL MapCreator::LoadMapDoor(tagMapHeader *pHeader, DWORD handle, tagMapInfo *pInfo)
{
	ASSERT( P_VALID(pHeader) && GT_VALID(handle) && P_VALID(pInfo) );
	TObjRef<VirtualFileSys> pVFS = "MapFile";
	ASSERT( P_VALID(pVFS) );

	if( pHeader->nNumMapDoor <= 0 )
	{
		return TRUE;
	}

	// ���ݵ�ͼ�����ƫ��������λ�ļ�
	pVFS->Seek(handle, pHeader->dwMapDoorOffset, GT_SEEK_SET);

	tagMapDoor temp;
	for(INT n = 0; n < pHeader->nNumMapDoor; n++)
	{
		pVFS->Read(&temp, sizeof(temp), handle);

		tagMapDoor* pMapDoor = pInfo->mapDoor.Peek(temp.dwObjID);
		ASSERT( !P_VALID(pMapDoor) );

		pMapDoor = new tagMapDoor;
		pMapDoor->dwObjID			= temp.dwObjID;
		pMapDoor->dwTypeID			= temp.dwTypeID;
		pMapDoor->vPos				= temp.vPos;
		pMapDoor->fYaw				= temp.fYaw;
		pMapDoor->bInitState		= temp.bInitState;
		pMapDoor->bBlockingTileNPC	= temp.bBlockingTileNPC;
		memcpy(pMapDoor->fScale, temp.fScale, sizeof(temp.fScale));

		// ���뵽�б���
		pInfo->mapDoor.Add(pMapDoor->dwObjID, pMapDoor);
	}

	return TRUE;
}

//----------------------------------------------------------------------------
// Ԥ�����ɸ�����ͼ��NavMap�������ɸ���ʱֱ�Ӹ��ƣ��ɹ�ʱ�������m_mapInstanceTemplate
//----------------------------------------------------------------------------
NavMap* MapCreator::LoadInstanceTemplate(tagMapInfo* pInfo)
{
	ASSERT( P_VALID(pInfo) );
	ASSERT( EMT_Normal != pInfo->eType );

	// ����һ��VFS
	IFS* pVfs = Kernel::Inst()->DefaultFS();
	NavMap* pNav = new NavMap;
	if( !P_VALID(pNav) )
	{
		return NULL;
	}

	pNav->LoadFromFile(pInfo->szMapFileName, pVfs, pVfs, NavMap::ELO_Collider | NavMap::ELO_NPCNavMap | NavMap::ELO_NPCNavGraph);
	Vector3 zoneSize(FLOAT_MAX, FLOAT_MAX, FLOAT_MAX);
	pNav->SetViewZoneSize(zoneSize);
	Vector3 zoneCenter(0, 0, 0);
	pNav->InvalidViewZone(zoneCenter);

	return pNav;
}

//----------------------------------------------------------------------------------
// ����
//----------------------------------------------------------------------------------
VOID MapCreator::Destroy()
{
	// ɾ�����е�MapMgr
	m_mapMapMgr.ResetIterator();
	MapMgr* pMapMgr = NULL;

	while( m_mapMapMgr.PeekNext(pMapMgr) )
	{
		SAFE_DEL(pMapMgr);
	}

	m_mapMapMgr.Clear();

	// ɾ�����еĸ���ģ��
	m_mapInstanceTemplate.ResetIterator();
	NavMap* pNav = NULL;

	while( m_mapInstanceTemplate.PeekNext(pNav) )
	{
		SAFE_DEL(pNav);
	}

	m_mapInstanceTemplate.Clear();

	// ɾ�����еĵ�ͼ������Ϣ
	m_mapMapInfo.ResetIterator();
	tagMapInfo* pInfo = NULL;

	while( m_mapMapInfo.PeekNext(pInfo) )
	{
		// ���ˢ�ֵ�
		tagMapSpawnPointInfo* pSpawnPoint = NULL;
		pInfo->mapSpawnPoint.ResetIterator();
		while( pInfo->mapSpawnPoint.PeekNext(pSpawnPoint) )
		{
			SAFE_DEL(pSpawnPoint);
		}
		pInfo->mapSpawnPoint.Clear();

		// ��յ�ͼ����
		tagMapCreatureInfo* pMapCreatureInfo = NULL;
		pInfo->mapCreatureInfo.ResetIterator();
		while( pInfo->mapCreatureInfo.PeekNext(pMapCreatureInfo) )
		{
			SAFE_DEL(pMapCreatureInfo);
		}
		pInfo->mapCreatureInfo.Clear();

		// �������
		tagMapAreaInfo* pArea = NULL;

		pInfo->mapSafeArea.ResetIterator();
		while( pInfo->mapSafeArea.PeekNext(pArea) )
		{
			SAFE_DEL(pArea);
		}
		pInfo->mapSafeArea.Clear();

		pInfo->mapPVPArea.ResetIterator();
		while( pInfo->mapPVPArea.PeekNext(pArea) )
		{
			SAFE_DEL(pArea);
		}
		pInfo->mapPVPArea.Clear();

		pInfo->mapStallArea.ResetIterator();
		while( pInfo->mapStallArea.PeekNext(pArea) )
		{
			SAFE_DEL(pArea);
		}
		pInfo->mapStallArea.Clear();

		pInfo->mapPrisonArea.ResetIterator();
		while( pInfo->mapPrisonArea.PeekNext(pArea) )
		{
			SAFE_DEL(pArea);
		}
		pInfo->mapPrisonArea.Clear();

		pInfo->mapScriptArea.ResetIterator();
		while( pInfo->mapScriptArea.PeekNext(pArea) )
		{
			SAFE_DEL(pArea);
		}
		pInfo->mapScriptArea.Clear();


		// ��մ�����
		tagMapTriggerInfo* pTrigger = NULL;
		pInfo->mapTrigger.ResetIterator();
		while( pInfo->mapTrigger.PeekNext(pTrigger) )
		{
			SAFE_DEL(pTrigger);
		}
		pInfo->mapTrigger.Clear();

		// ��յ�����
		tagMapWayPointInfoList* pList = NULL;
		pInfo->mapWayPointList.ResetIterator();
		while( pInfo->mapWayPointList.PeekNext(pList) )
		{
			SAFE_DEL(pList);
		}
		pInfo->mapWayPointList.Clear();

		// ��ճ�����Ч
		tagMapTriggerEffect* pEffect = NULL;
		pInfo->mapTriggerEffect.ResetIterator();
		while (pInfo->mapTriggerEffect.PeekNext(pEffect))
		{
			SAFE_DEL(pEffect);
		}
		pInfo->mapTriggerEffect.Clear();

		// �����
		tagMapDoor* pDoor = NULL;
		pInfo->mapDoor.ResetIterator();
		while (pInfo->mapDoor.PeekNext(pDoor))
		{
			SAFE_DEL(pDoor);
		}
		pInfo->mapDoor.Clear();
		
		// ��յ�ͼ��̬���Խṹ
		SAFE_DEL(pInfo);
	}

	m_mapMapInfo.Clear();

	// ɾ���������ڴ�
	SAFE_DEL_ARRAY(m_pvBornPos);

	// ��ע�����е�ע����
	UnRegisterAll();
}

//--------------------------------------------------------------------------------
// ע���ͼ��������
//--------------------------------------------------------------------------------
VOID MapCreator::RegisterAFMap()
{
	m_AFMap.Register(TWrap<Map>(),					EMT_Normal);
	m_AFMap.Register(TWrap<MapInstanceNormal>(),	EMT_Instance);
	m_AFMap.Register(TWrap<MapInstanceStable>(),	EMT_System);
	m_AFMap.Register(TWrap<MapInstanceScript>(),	EMT_ScriptCreate);
}

//--------------------------------------------------------------------------------
// ע���ͼ������������
//--------------------------------------------------------------------------------
VOID MapCreator::RegisterAFMapRestrict()
{
	m_AFRestrict.Register(TWrap<MapRestrictNormal>(),		EMT_Normal);
	m_AFRestrict.Register(TWrap<MapRestrictInstance>(),		EMT_Instance);
	m_AFRestrict.Register(TWrap<MapRestrictStable>(),		EMT_System);
	m_AFRestrict.Register(TWrap<MapRestrictScript>(),		EMT_ScriptCreate);
	//m_AFRestrict.Register(TWrap<MapRestrictPVP>,		EMT_PVP);
}

//--------------------------------------------------------------------------------
// ��ע��ӿ�
//--------------------------------------------------------------------------------
VOID MapCreator::UnRegisterAll()
{
	m_AFMap.Unregister(TWrap<Map>(),				EMT_Normal);
	m_AFMap.Unregister(TWrap<MapInstanceNormal>(),	EMT_Instance);
	m_AFMap.Unregister(TWrap<MapInstanceStable>(),	EMT_System);
	m_AFMap.Unregister(TWrap<MapInstanceScript>(),	EMT_ScriptCreate);

	m_AFRestrict.Unregister(TWrap<MapRestrictNormal>(),		EMT_Normal);
	m_AFRestrict.Unregister(TWrap<MapRestrictInstance>(),	EMT_Instance);
	m_AFRestrict.Unregister(TWrap<MapRestrictStable>(),		EMT_System);
	m_AFRestrict.Unregister(TWrap<MapRestrictScript>(),		EMT_ScriptCreate);
	//m_AFRestrict.Unregister(TWrap<MapRestrictPVP>,			EMT_PVP);

}

//--------------------------------------------------------------------------------
// ������ͨ��ͼ
//--------------------------------------------------------------------------------
Map* MapCreator::CreateAFMap(const tagMapInfo* pInfo)
{
	if( !P_VALID(pInfo) || EMT_Normal != pInfo->eType ) return NULL;

	return m_AFMap.Create(pInfo->eType);
}

//--------------------------------------------------------------------------------
// ���ɸ�����ͼ
//--------------------------------------------------------------------------------
MapInstance* MapCreator::CreateAFMapInstance(const tagMapInfo* pInfo)
{
	if( !P_VALID(pInfo) || EMT_Normal == pInfo->eType ) return NULL;

	return static_cast<MapInstance*>(m_AFMap.Create(pInfo->eType));
}

//---------------------------------------------------------------------------------
// �ͷŵ�ͼ
//---------------------------------------------------------------------------------
VOID MapCreator::DestroyAFMap(Map* pMap)
{
	if( !P_VALID(pMap) ) return;

	const tagMapInfo* pInfo = pMap->GetMapInfo();
	if( !P_VALID(pInfo) ) return;

	m_AFMap.Destroy(pMap, pInfo->eType);
}

//--------------------------------------------------------------------------------
// ���ɵ�ͼ����
//--------------------------------------------------------------------------------
MapRestrict* MapCreator::CreateAFMapRestrict(const tagMapInfo* pInfo)
{
	if( !P_VALID(pInfo) ) return NULL;

	return m_AFRestrict.Create(pInfo->eType);
}

//--------------------------------------------------------------------------------
// �ͷŵ�ͼ����
//--------------------------------------------------------------------------------
VOID MapCreator::DestroyAFMapRestrict(MapRestrict* pRestrict)
{
	if( !P_VALID(pRestrict) ) return;

	const tagMapInfo* pInfo = pRestrict->GetMapInfo();
	if( !P_VALID(pInfo) ) return;

	m_AFRestrict.Destroy(pRestrict, pInfo->eType);
}

//--------------------------------------------------------------------------------
// ֹͣ���е�ͼ�������߳�
//--------------------------------------------------------------------------------
VOID MapCreator::StopAllMapMgr()
{
	MapMgr::StopThread();
	StartAllMapMgr();	// ���������̣߳������������˳�����Ϊ�����Ѿ������˽�����־

	m_mapMapMgr.ResetIterator();
	MapMgr* pMapMgr = NULL;

	while( m_mapMapMgr.PeekNext(pMapMgr) )
	{
		tstring& strThreadName = pMapMgr->GetThreadName();
		m_pThread->WaitForThreadDestroy(strThreadName.c_str(), INFINITE);
	}
}

//------------------------------------------------------------------------------
// ����һ����ͼ��Ӧ��MapMgr������MapMgr�߳�
//------------------------------------------------------------------------------
MapMgr* MapCreator::CreateMapMgr(tagMapInfo* pInfo)
{
	ASSERT( P_VALID(pInfo) );

	MapMgr* pMapMgr = new MapMgr;
	if( !P_VALID(pMapMgr) )
		return NULL;

	if( FALSE == pMapMgr->Init(pInfo) )
	{
		SAFE_DEL(pMapMgr);
		return NULL;
	}

	return pMapMgr;
}

//----------------------------------------------------------------------------------
// ���ݵ�ͼID�͸���IDȷ��һ��Ψһ��ͼ
//----------------------------------------------------------------------------------
Map* MapCreator::GetMap(DWORD dwMapID, DWORD dwInstance)
{
	MapMgr* pMapMgr = GetMapMgr(dwMapID);
	if( !P_VALID(pMapMgr) ) return NULL;

	Map* pMap = NULL;

	if( pMapMgr->IsNormalMap() )	pMap = pMapMgr->GetSingleMap();
	else							pMap = pMapMgr->GetInstance(dwInstance);

	return pMap;
}

//------------------------------------------------------------------------------------------
// ��ͼ�������ĸ���
//------------------------------------------------------------------------------------------
VOID MapCreator::Update()
{
	CHECK_TIME();

	// �������е�ͼ�߳�
	StartAllMapMgr();

	// �ȴ����е�ͼ�̱߳��θ������
	WaitAllMapMgrEnd();

	// �������֮��ĺ������������в�����
	UpdateAllDelayedStuff();
}

//-------------------------------------------------------------------------------------------
// ÿ��Tick���е�ͼ�߳���ͣ��Ĳ�����һ��Ҫ����˳�����������õ�˳���ܵߵ�
//-------------------------------------------------------------------------------------------
VOID MapCreator::UpdateAllDelayedStuff()
{
	// ���沢�˳������Ѿ������ߵ����
	LogoutAllRemovedRolePerTick();

	// ��������Ҫ����ѡ�˽�������
	DealAllReturnCharacterRole();

	// ��������Ҫ�л���ͼ�����
	DealAllChangeMapRole();

	// ɾ�����б�TickҪɾ���ĸ���
	DealAllDestroiedInstance();


}

//--------------------------------------------------------------------------------------------
// ���沢�˳������Ѿ������ߵ����
//--------------------------------------------------------------------------------------------
VOID MapCreator::LogoutAllRemovedRolePerTick()
{
	DWORD dwRoleID = m_listLogOutRole.PopFront();

	while( P_VALID(dwRoleID) )
	{
		Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
		if( !P_VALID(pRole) )
		{
			dwRoleID = m_listLogOutRole.PopFront();
			continue;
		}

		// �ҵ�����ҵǳ�ʱ���ڵĵ�ͼ
		Map* pMap = pRole->GetMap();
		if( P_VALID(pMap) )
		{
			pMap->RoleLogOut(pRole->GetID());
		}

		// �������
		pRole->SaveToDB();

		// ���Session
		PlayerSession* pSession = pRole->GetSession();
		if( P_VALID(pSession) )
		{
			// IMSG(_T("player logout in mapcreator, sessionid=%u, internalid=%u\r\n"), pSession->GetSessionID(), pSession->GetInternalIndex());
			g_worldSession.RemoveSession(pSession->GetSessionID());
		}

		g_roleMgr.DeleteRole(pRole->GetID());
		g_loginSession.SendPlayerLogout(pSession->GetSessionID());
		g_worldSession.PlayerLogout();
		g_gameGuarder.Logout(pSession->GetSessionID(), pSession->GetAccount());
		SAFE_DEL(pSession);

		// �õ���һ�����
		dwRoleID = m_listLogOutRole.PopFront();
	}
}

//---------------------------------------------------------------------------------------------
// �������б�tickҪ�л���ͼ�����
//---------------------------------------------------------------------------------------------
VOID MapCreator::DealAllChangeMapRole()
{
	tagChangeMapInfo info = m_listChangeMapRole.PopFront();

	while( P_VALID(info.dwRoleID) )
	{
		Role* pRole = g_roleMgr.GetRolePtrByID(info.dwRoleID);
		if( !P_VALID(pRole) )
		{
			info = m_listChangeMapRole.PopFront();
			continue;
		}

		MapMgr* pDestMapMgr = g_mapCreator.GetMapMgr(info.dwDestMapID);
		if( !P_VALID(pDestMapMgr) )
		{
			info = m_listChangeMapRole.PopFront();
			continue;
		}

		Map* pDestMap = pDestMapMgr->CanEnter(pRole, info.dwMisc);
		if( !P_VALID(pDestMap) )
		{
			info = m_listChangeMapRole.PopFront();
			continue;
		}

		// ��ԭ��ͼ����
		Map* pSrcMap = pRole->GetMap();
		BOOL bChgMap = (info.dwDestMapID != pSrcMap->GetMapID());
		if( P_VALID(pSrcMap) ) pSrcMap->RoleLeaveMap(pRole->GetID(), bChgMap);

		// ���뵽�µ�ͼ
		pRole->SetMapID(info.dwDestMapID);
		pRole->GetMoveData().Reset(info.vPos.x, info.vPos.y, info.vPos.z, pRole->GetMoveData().m_vFace.x, pRole->GetMoveData().m_vFace.y, pRole->GetMoveData().m_vFace.z);

		pDestMap->AddRole(pRole);

		// ȡ��һ�����
		info = m_listChangeMapRole.PopFront();
	}
}

//---------------------------------------------------------------------------------------------
// �������б�tickҪ�ص�ѡ�˽�������
//---------------------------------------------------------------------------------------------
VOID MapCreator::DealAllReturnCharacterRole()
{
	DWORD dwRoleID = m_listReturnCharacterRole.PopFront();

	while( P_VALID(dwRoleID) )
	{
		Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);

		if( P_VALID(pRole) )
		{
			pRole->GetMap()->RoleLeaveMap(pRole->GetID(), FALSE);

			// �ҵ�Session�����ø�������
			PlayerSession* pSession = pRole->GetSession();
			pSession->Refresh();

			// ���뵽ѡ�˽�����
			g_worldSession.AddGlobalSession(pSession);

			// �����ɫ����ɾ����ɫ
			pRole->SaveToDB();
			g_roleMgr.DeleteRole(pRole->GetID());
		}

		// �õ�һ���µ����
		dwRoleID = m_listReturnCharacterRole.PopFront();
	}
}

//---------------------------------------------------------------------------------------------
// ɾ�����б�tickҪɾ���ĸ���
//---------------------------------------------------------------------------------------------
VOID MapCreator::DealAllDestroiedInstance()
{
	tagInstanceDestroyInfo info = m_listDestroyInstance.PopFront();

	while( P_VALID(info.dwMapID) )
	{
		// �ҵ�MapMgr
		MapMgr* pMapMgr = GetMapMgr(info.dwMapID);
		if( !P_VALID(pMapMgr) )
		{
			info = m_listDestroyInstance.PopFront();
			continue;
		}

		// �ҵ�����
		MapInstance* pInstance = pMapMgr->GetInstance(info.dwInstanceID);
		if( !P_VALID(pInstance) )
		{
			info = m_listDestroyInstance.PopFront();
			continue;
		}

		// ɾ������
		pMapMgr->DestroyInstance(pInstance);

		// ɾ����һ��
		info = m_listDestroyInstance.PopFront();
	}
}

//-------------------------------------------------------------------------------------------------
// �ⲿָ��ĳ������ɾ��
//-------------------------------------------------------------------------------------------------
VOID MapCreator::SetInstanceDelete(DWORD dwMapID, DWORD dwInstanceID)
{
	MapMgr* pMapMgr = GetMapMgr(dwMapID);
	if( !P_VALID(pMapMgr) ) return;

	MapInstance* pInstance = pMapMgr->GetInstance(dwInstanceID);
	if( !P_VALID(pInstance) ) return;

	pInstance->SetDelete();
}

//-------------------------------------------------------------------------------------------------
// �ҵ�ĳ����ͼ�ӹ����
//-------------------------------------------------------------------------------------------------
BOOL MapCreator::TakeOverRoleWhenOnline(Role* pRole)
{
	if( !P_VALID(pRole) ) return FALSE;

	DWORD dwMapID = pRole->GetMapID();		// �������ʱ�ĵ�ͼID
	Vector3 vPos = pRole->GetCurPos();		// �������ʱ������

	MapMgr* pMapMgr = g_mapCreator.GetMapMgr(dwMapID);
	if( !P_VALID(pMapMgr) )
	{
		ILOG->Write(_T("Can not find a map when player online, roleid=%u, mapid=%u\n"), pRole->GetID(), dwMapID);
		return FALSE;
	}

	Map* pMap = pMapMgr->CanTakeOverWhenOnline(pRole, dwMapID, vPos);

	while( !P_VALID(pMap) )
	{
		pMapMgr = g_mapCreator.GetMapMgr(dwMapID);
		pMap = pMapMgr->CanTakeOverWhenOnline(pRole, dwMapID, vPos);

		if( !P_VALID(pMap) && dwMapID == pMapMgr->GetMapInfo()->dwID )	// ���ܽӹܣ������ڵ�ͼ�������Լ���˵���д���
		{
			ILOG->Write(_T("find a duplicated map when player online, roleid=%u, mapid=%u\n"), pRole->GetID(), dwMapID);
			break;
		}
	}

	if( P_VALID(pMap) )
	{
		// �ҵ���һ��Ŀ���ͼ
		pRole->SetMapID(dwMapID);
		pRole->GetMoveData().Reset(vPos.x, vPos.y, vPos.z, pRole->GetFaceTo().x, pRole->GetFaceTo().y, pRole->GetFaceTo().z);

		pMap->AddRole(pRole);

		return TRUE;
	}
	else
	{
		// û���ҵ�Ŀ���ͼ���򴫻س�����
		dwMapID = g_mapCreator.GetBornMapID();
		vPos = g_mapCreator.RandGetOneBornPos();

		pMapMgr = g_mapCreator.GetMapMgr(dwMapID);
		pMap = pMapMgr->CanTakeOverWhenOnline(pRole, dwMapID, vPos);

		if( !P_VALID(pMap) )
		{
			ILOG->Write(_T("Critical Error, role can not enter the born map!!!, roleid=%u\n"), pRole->GetID());
			return FALSE;
		}

		pRole->SetMapID(dwMapID);
		pRole->GetMoveData().Reset(vPos.x, vPos.y, vPos.z, pRole->GetFaceTo().x, pRole->GetFaceTo().y, pRole->GetFaceTo().z);

		pMap->AddRole(pRole);

		return TRUE;
	}
}

const Vector3 MapCreator::GetReBornPoint( DWORD dwRebornMapID )
{
	Vector3 vRt;
	vRt.x = vRt.y = vRt.z = GT_INVALID;
	const tagMapInfo *pMapInfo = g_mapCreator.GetMapInfo(dwRebornMapID);
	if(P_VALID(pMapInfo))
	{
		vRt = pMapInfo->vRebornPos;
	}
	else
	{
		// ������ͼû�ҵ�
		ASSERT(0);
	}

	return vRt;
}

//-------------------------------------------------------------------------------------------------
// �Ƿ���Դ�������
//-------------------------------------------------------------------------------------------------
BOOL MapCreator::CanCreateInstance(const tagMapInfo* pInfo, MapMgr* pMapMgr)
{
	if( EMT_Normal == pInfo->eType ) return FALSE;
	if( pInfo->eInstanceRank < EMIR_NoLimit || pInfo->eInstanceRank >= EMIR_End ) return FALSE;

	if( EMIR_NoLimit == pInfo->eInstanceRank ) return TRUE;

	if(pMapMgr->GetCurInstanceNum() >= pInfo->dwMaxNumLimit )	return FALSE;

	// ����һ�¼�Ȩ�ܺ��Ƿ��������
	if( m_nInstanceCoefNumCur + INSTANCE_COEF[pInfo->eInstanceRank] > m_nInstanceCoefNumLimit ) return FALSE;

	return TRUE;
}

//-------------------------------------------------------------------------------------------------
// ����һ������
//-------------------------------------------------------------------------------------------------
VOID MapCreator::AddInstance(const tagMapInfo* pInfo, MapMgr* pMapMgr)
{
	if( EMT_Normal == pInfo->eType ) return;
	if( pInfo->eInstanceRank < EMIR_NoLimit || pInfo->eInstanceRank >= EMIR_End ) return;

	pMapMgr->ChangeInstanceNum(1);
	m_nInstanceNumCur[pInfo->eInstanceRank] += 1;
	m_nInstanceCoefNumCur += INSTANCE_COEF[pInfo->eInstanceRank];
}

//-------------------------------------------------------------------------------------------------
// �Ƴ�һ������
//-------------------------------------------------------------------------------------------------
VOID MapCreator::RemoveInstance(const tagMapInfo* pInfo, MapMgr* pMapMgr)
{
	if( EMT_Normal == pInfo->eType ) return;
	if( pInfo->eInstanceRank < EMIR_NoLimit || pInfo->eInstanceRank >= EMIR_End ) return;

	pMapMgr->ChangeInstanceNum(-1);
	m_nInstanceNumCur[pInfo->eInstanceRank] -= 1;
	m_nInstanceCoefNumCur -= INSTANCE_COEF[pInfo->eInstanceRank];
}

////-----------------------------------------------------------------------------------------------
//// ��������
////---------------------------------------------------------------------------------------------
//BOOL MapCreator::CreateInstance(DWORD dwCreateID, DWORD dwTeamID, MapMgr *pDestMapMgr, DWORD dwDestMapID, EInstanceHardMode eInstanceHardMode)
//{
//	Map *pInstance = new MapInstanceNormal(m_dwInstanceIDGen, dwCreateID, dwTeamID, eInstanceHardMode);
//	if( !P_VALID(pInstance) )
//	{
//		return FALSE;
//	}
//
//	tagMapInfo* pDestMapInfo = m_mapMapInfo.Peek(dwDestMapID);
//	if(FALSE == pInstance->Init(pDestMapInfo))
//	{
//		SAFE_DEL(pInstance);
//		return FALSE;
//	}
//
//	pDestMapMgr->AddInstance(m_dwInstanceIDGen, pInstance);
//
//	return TRUE;
//}
//
////---------------------------------------------------------------------------------------------
//// ���븱��
////---------------------------------------------------------------------------------------------
//BOOL MapCreator::EnterInstance(Role *pRole, MapMgr* pMapMgr, tagChangeMapInfo& info)
//{
//	DWORD dwErrorCode = E_Success;
//
//	const tagInstance *pInstance = g_attRes.GetInstanceProto(info.dwDestMapID);
//	if(!P_VALID(pInstance))
//	{
//		IMSG(_T("Can't find Instance Proto %u\r\n"), info.dwDestMapID);
//
//		return FALSE;
//	}
//
//	// С�ӽ���
//	if(pRole->GetTeamID() != GT_INVALID)
//	{
//		const CTeam *pTeam = g_groupMgr.GetTeamPtr(pRole->GetTeamID());
//		if(!P_VALID(pTeam))
//			return FALSE;
//
//		DWORD dwInstanceID = pTeam->GetOwnInstanceID();
//
//		// ��������
//		if(dwInstanceID == GT_INVALID)
//		{
//			dwErrorCode = CanCreateInstance(pRole, info, pInstance);
//
//			if(dwErrorCode == E_Success)
//			{
//				if(FALSE == CreateInstance(pRole->GetID(), pRole->GetTeamID(), pMapMgr, info.dwDestMapID, (EInstanceHardMode)info.dwMisc))
//					return FALSE;
//
//				// ����С�Ӹ���ID
//				pTeam->SetOwnInstanceID(m_dwInstanceIDGen);
//				pTeam->SetOwnInstanceMapID(info.dwDestMapID);
//
//				// ѯ������Ƿ�֪ͨ����
//				if(pInstance->bNoticeTeamate == true)
//				{
//					tagNS_InstanceNofity	send;
//					pRole->GetSession()->SendMessage(&send, send.dwSize);
//				}
//
//				// ���Ӹ���ID����
//				++m_dwInstanceIDGen;
//			}
//		}
//		else	// �������и���
//		{
//			Map *pMap = pMapMgr->GetRoleInstancePtr(pRole);
//			if(!P_VALID(pMap))
//				return FALSE;
//
//			dwErrorCode = pMap->CanEnter(pRole, info.dwDestMapID);
//		}
//	}
//	else // ���˽���
//	{
//		dwErrorCode = CanCreateInstance(pRole, info, pInstance);
//
//		if(dwErrorCode == E_Success)
//		{
//			if(FALSE == CreateInstance(pRole->GetID(), pRole->GetTeamID(), pMapMgr, info.dwDestMapID, (EInstanceHardMode)info.dwMisc))
//				return FALSE;
//
//			// ���õ��˸���ID
//			pRole->SetMyOwnInstanceID(m_dwInstanceIDGen);
//
//			// ���Ӹ���ID����
//			++m_dwInstanceIDGen;
//		}
//	}
//
//	Map		*pMap = pMapMgr->GetRoleInstancePtr(pRole);
//	DWORD	dwTimeLimit = GT_INVALID;
//	INT		nRoleNum = 0;
//
//	if(E_Success == dwErrorCode && !P_VALID(pMap))
//	{
//		ASSERT(0);
//		return FALSE;
//	}
//
//	if(P_VALID(pMap))
//	{
//		dwTimeLimit = pMap->CalTimeLimit();
//	}
//	
//	// ���ؿͻ��˽��븱���Ƿ�ɹ�
//	tagNS_EnterInstance		send;
//	send.dwTimeLimit = dwTimeLimit;
//	send.dwErrorCode = dwErrorCode;
//	pRole->GetSession()->SendMessage(&send, send.dwSize);
//
//	if(dwErrorCode == E_Success)
//		return TRUE;
//	else
//		return FALSE;
//}
//
////---------------------------------------------------------------------------------------------
//// �Ƿ��ܴ�������
////---------------------------------------------------------------------------------------------
//DWORD MapCreator::CanCreateInstance(Role *pRole, tagChangeMapInfo& info, const tagInstance* pInstance)
//{
//	// �Ƿ��ѡ�����Ѷ�
//	if(pInstance->bSelectHard == false)
//	{
//		// ����ѡ
//		if((EInstanceHardMode)info.dwMisc != EIHM_Normal)
//			return GT_INVALID;
//	}
//	else // ��ѡ
//	{
//		// �Ƿ��ѡͨ���Ѷ�
//		if((EInstanceHardMode)info.dwMisc == EIHM_Normal && pInstance->bSelectNormal != true)
//			return GT_INVALID;
//
//		// �Ƿ��ѡ��Ӣ�Ѷ�
//		if((EInstanceHardMode)info.dwMisc == EIHM_Elite && pInstance->bSelectElite != true)
//			return GT_INVALID;
//
//		// �Ƿ��ѡħ���Ѷ�
//		if((EInstanceHardMode)info.dwMisc == EIHM_Devil && pInstance->bSelectDevil != true)
//			return GT_INVALID;
//	}
//
//	// �����ȼ�����
//	if(pInstance->nLevelDownLimit > pRole->GetLevel())
//		return E_Instance_Level_Down_Limit;
//
//	// �����ȼ�����
//	if(pInstance->nLevelUpLimit < pRole->GetLevel())
//		return E_Instance_Level_Up_Limit;
//
//	// ��������
//	if(pInstance->nNumDownLimit > 1)
//	{
//		const CTeam *pTeam = g_groupMgr.GetTeamPtr(pRole->GetTeamID());
//		if(!P_VALID(pTeam))
//			return GT_INVALID;
//		else if(pTeam->GetMemNum() < pInstance->nNumDownLimit) 
//			return E_Instance_Role_Lack;
//	}
//
//	// �Ƿ���ͬһ����
//	if(pRole->GetMapID() == info.dwDestMapID)
//		return GT_INVALID;
//
//	return E_Success;
//}

MapCreator g_mapCreator;