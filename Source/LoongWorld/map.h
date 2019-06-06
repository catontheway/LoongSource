//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: map.h
// author: Aslan
// actor:
// data: 2008-6-27
// last:
// brief: ��ͼ�࣬����һ��ȷ���ĵ�ͼʵ��������Ϊһ�������ĵ�ͼʵ��
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/time.h"
#include "../WorldDefine/msg_map.h"
#include "../WorldDefine/msg_combat.h"
#include "../WorldDefine/ItemDefine.h"

#include "script_mgr.h"

class PlayerSession;
class Unit;
class Role;
class Creature;
class Pet;
class Map;
class Shop;
class MapScript;
class NPCTeamMgr;
class GuildCofC;

struct tagItem;
struct tagEquip;

//-------------------------------------------------------------------------------
// Ĭ�ϸ����������
//-------------------------------------------------------------------------------
const DWORD DEFAULT_MAX_INSTANCE_NUM	=	500;


//-------------------------------------------------------------------------------
// ����ȡվ���߶ȵ����߶Ⱥ���С�߶�
//-------------------------------------------------------------------------------
const FLOAT GET_HEIGHT_MAX_Y = 10000000.0f;
const FLOAT GET_HEIGHT_MIN_Y = -10000000.0f;

const INT CHECK_GROUND_ITEM_TICK_INTERVAL1M = 1 * 60 * TICK_PER_SECOND; // 1����
const INT CHECK_GROUND_ITEM_TICK_INTERVAL3M = 3 * 60 * TICK_PER_SECOND; // 3����

//--------------------------------------------------------------------------------
// �����������ڿ��Ʋ�ͬ���𸱱�����������
//--------------------------------------------------------------------------------
enum EMapInstanceRank
{
	EMIR_NoLimit	=	0,				// ������
	EMIR_512		=	1,				// 512x512�������µĸ���
	EMIR_1024		=	2,				// 1024x1024�ĸ���
	EMIR_2048		=	3,				// 2048x2048�ĸ���
	EMIR_3072		=	4,				// 3072x3072�����ϵĸ���

	EMIR_End		=	5,
};

const INT INSTANCE_COEF[EMIR_End] = {0, 1, 3, 6, 9};	// ÿ������ռ�ñ��ص�Ȩֵ

//--------------------------------------------------------------------------------
// �������˵�ͼ������ṹ
//--------------------------------------------------------------------------------
struct tagWayPointInfo
{
	Vector3		vPos;
	Vector3		vRange;

public:
	tagWayPointInfo(DWORD)
	{
		vPos.x = vPos.y = vPos.z = -1.0;
	}
	tagWayPointInfo()
	{

	}
};

struct tagMapWayPointInfoList
{
	DWORD							dwID;					// ������ID
	mutable TList<tagWayPointInfo>	list;					// �������б�

	tagMapWayPointInfoList() { dwID = GT_INVALID; }
};

//--------------------------------------------------------------------------------
// �������˵�ͼ�������ṹ
//--------------------------------------------------------------------------------
struct tagMapTriggerInfo
{
	DWORD 				dwObjID;					// ��ͼ����ID
	EMapTrigger			eType;						// ����
	HRGN				hRgn;						// �����������
	AABBox				box;						// ��Χ��
	FLOAT				fHeight;					// �߶�
	DWORD				dwMapID;					// �л���ͼ��������Ŀ���ͼID
	DWORD				dwWayPointID;				// �л���ͼ��������Ŀ���ͼ������ID
	DWORD				dwParam;					// ����
	DWORD				dwQuestSerial;				// �������������

	tagMapTriggerInfo()
	{
		dwObjID = GT_INVALID;
		eType = EMT_Null;
		fHeight = 0.0f;
		dwMapID = GT_INVALID;
		dwWayPointID = GT_INVALID;
		dwParam = GT_INVALID;
		dwQuestSerial = GT_INVALID;
	}
	~tagMapTriggerInfo()
	{
		DeleteObject(hRgn);
	}
};

//--------------------------------------------------------------------------------
// �������˵�ͼ����ṹ
//--------------------------------------------------------------------------------
struct tagMapAreaInfo
{
	DWORD 				dwObjID;			// ��ͼ����
	EMapArea			eType;				// ����
	HRGN				hRgn;				// �����������
	AABBox				box;				// ��Χ��
	FLOAT				fHeight;			// �߶�

	tagMapAreaInfo()
	{
		dwObjID = GT_INVALID;
		eType = EMA_Null;
		fHeight = 0.0f;
	}
	~tagMapAreaInfo()
	{
		DeleteObject(hRgn);
	}
};

//--------------------------------------------------------------------------------
// ���ＰNPC�����ɽṹ
//--------------------------------------------------------------------------------
struct tagMapCreatureInfo
{
	DWORD 					dwObjID;			// ��ͼ����ID
	DWORD 					dwTypeID;			// ����ID
	Vector3					vPos;				// ��ʼ����
	FLOAT 					fYaw;				// ��ʼ����
	tagMapWayPointInfoList*	patrolList;			// Ѳ�ߵĵ������б�
	BOOL					bCollide;			// �Ƿ�ʹ����ײ

	tagMapCreatureInfo() { ZeroMemory(this, sizeof(*this)); }
};

//--------------------------------------------------------------------------------
// ��������ˢ�ֵ�ṹ
//--------------------------------------------------------------------------------
struct tagMapSpawnPointInfo
{
	DWORD				dwObjID;				// ��ͼ����id
	DWORD				dwSpawnPointID;			// ˢ�ֵ�id��СID��
	INT					nLevelInc;				// �������ˢ��ʱ�ĵȼ�������
	BOOL				bCollide;				// �Ƿ�ʹ����ײ
	Vector3				vPos;					// ����

	tagMapSpawnPointInfo() { ZeroMemory(this, sizeof(*this)); }
};


//--------------------------------------------------------------------------------
// ��ͼ��̬����
//--------------------------------------------------------------------------------
struct tagMapInfo
{
	typedef TMap<DWORD, tagMapWayPointInfoList*>	MapWayPointListMap;		// ����������
	typedef TMap<DWORD, tagMapTriggerInfo*>			MapTriggerMap;			// ����������
	typedef TMap<DWORD,	tagMapAreaInfo*>			MapAreaMap;				// ��������
	typedef TMap<DWORD, tagMapCreatureInfo*>		MapCreatureInfoMap;		// ��������
	typedef TMap<DWORD, tagMapSpawnPointInfo*>		MapSpawnPointMap;		// ����ˢ�ֵ�
	typedef TMap<DWORD, tagMapTriggerEffect*>		MapTriggerEffectMap;	// ������Ч
	typedef TMap<DWORD, tagMapDoor*>				MapDoorMap;				// ��

	DWORD					dwID;							// ��ͼID
	TCHAR					szMapFileName[X_SHORT_NAME];	// ��ͼ�ļ�����

	EMapType				eType;							// ��ͼ����
	ENormalMapType			eNoarmalMapType;				// ��ͨ��ͼ������
	EMapInstanceRank		eInstanceRank;					// ��������
	DWORD					dwMaxNumLimit;					// ���������������
	
	INT						nWidth;							// ����
	INT						nHeight;						// ���
	INT						nVisDist;						// ���Ӿ���
	DWORD					dwWeatherID;					// ��Ӧ����������ID
	Vector3					vRebornPos;						// �����
	DWORD					dwPopMapID;						// �ӳ����д��ͳ��ĵ�ͼID
	Vector3					vPopPos;						// �ӳ����д��ͳ��ĵ�ͼ����

	mutable MapWayPointListMap		mapWayPointList;		// ����������
	mutable MapTriggerMap			mapTrigger;				// ����������
	mutable MapAreaMap				mapSafeArea;			// ��ȫ��
	mutable MapAreaMap				mapPVPArea;				// PVP��
	mutable MapAreaMap				mapStallArea;			// ��̯��
	mutable MapAreaMap				mapPrisonArea;			// ������
	mutable MapAreaMap				mapScriptArea;			// �ű�����
	mutable MapCreatureInfoMap		mapCreatureInfo;		// ����������
	mutable MapSpawnPointMap		mapSpawnPoint;			// ����ˢ�ֵ�
	mutable MapTriggerEffectMap		mapTriggerEffect;		// ������Ч
	mutable	MapDoorMap				mapDoor;				// ��
};

//------------------------------------------------------------------------
// ��ͼ�� ����ID ������
//------------------------------------------------------------------------
class GroundItemIDGen
{
public:
	GroundItemIDGen():m_n64LastValid(1){}
	INT64 GenValidID()
	{
		return m_n64LastValid++;
	}
private:
	INT64		m_n64LastValid;
};

enum EGroundItemUpdate
{
	EGIU_Null		= -1,
	EGIU_Remove		= 0,	// �Ƴ�
	EGIU_Sync		= 1,	// ͬ��
};

//------------------------------------------------------------------------
// ���������ϵ���Ʒ�ṹ
//------------------------------------------------------------------------
struct tagGroundItem
{
	INT64					n64ID;				// ����ID
	DWORD					dwTypeID;			// ����ID
	INT						nNum;				// �������Ǯ

	tagItem*				pItem;				// ��Ʒָ��
	DWORD					dwPutDownUnitID;	// ������Ʒ��UnitID
	DWORD					dwOwnerID;			// ������ɫ
	DWORD					dwTeamID;			// ��������
	DWORD					dwGroupID;			// �����Ŷ�ID
	INT						nTickCountDown;		// ������Ʒ��tick����ʱ
	Vector3					vPos;				// ��Ʒ����


	tagGroundItem(	INT64 n64ID_, DWORD dwTypeID_, INT nNum_, tagItem* pItem_, 
					Vector3 vPos_, DWORD dwOwnerID_, DWORD dwTeamID_, DWORD dwGroupID_, DWORD dwPutDownUnitID_)
	{
		n64ID			=	n64ID_;
		dwTypeID		=	dwTypeID_;
		nNum			=	nNum_;

		pItem			=	pItem_;
		vPos			=	vPos_;
		dwOwnerID		=	dwOwnerID_;
		dwTeamID		=	dwTeamID_;
		dwGroupID		=	dwGroupID_;
		dwPutDownUnitID	=	dwPutDownUnitID_;
		nTickCountDown	=	0;
	}


	// ������£�����TRUEʱɾ������
	BOOL Update()
	{
		EGroundItemUpdate eRtv = EGIU_Null;
		// ����ʱ��
		++nTickCountDown;

		// 1��������ʰȡ
		if (CHECK_GROUND_ITEM_TICK_INTERVAL1M == nTickCountDown)
		{
			if (GT_VALID(dwOwnerID) || GT_VALID(dwTeamID))
				eRtv = EGIU_Null;
			else
				eRtv = EGIU_Sync;

			dwOwnerID	= GT_INVALID;
			dwTeamID	= GT_INVALID;
		}
		// 3������ʧ
		else if (nTickCountDown >= CHECK_GROUND_ITEM_TICK_INTERVAL3M)
		{
			eRtv = EGIU_Remove;
		}
		return eRtv;
	}

	// ����
	VOID DestroyItem()
	{
		if (P_VALID(pItem))
		{
			::Destroy(pItem);
		}
	}

	// �Ƿ���Ч
	BOOL IsValid()
	{
		if (dwTypeID == TYPE_ID_MONEY && !P_VALID(pItem))
		{
			return TRUE;
		}
		else if (dwTypeID != TYPE_ID_MONEY && P_VALID(pItem))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

};

//----------------------------------------------------------------------------------
// ���ӵ�ש
//----------------------------------------------------------------------------------

// �����м���ϰ˷���
enum EDirection
{
	ED_Center = 0,
	ED_Top = 1,
	ED_Bottom = 2,
	ED_Left = 3,
	ED_Right = 4,
	ED_LeftTop = 5,
	ED_LeftBottom = 6,
	ED_RightTop = 7,
	ED_RightBottom = 8,
	ED_End = 9,
};

struct tagVisTile
{
	TMap<DWORD, Role*>				mapRole;
	TMap<DWORD, Creature*>			mapCreature;
	TMap<INT64, tagGroundItem*>		mapGroundItem;
	TMap<DWORD, Unit*>				mapInvisibleUnit;
};

//---------------------------------------------------------------------------------
// ����ID������
//---------------------------------------------------------------------------------
class CreatureIDGen
{
public:
	VOID Init(const tagMapInfo* pInfo)
	{
		// ���ǵ���Ѩˢ���Լ�ʵʱˢ���Ĺ�����������֮��������100��ID
		INT nCreatureNum = pInfo->mapCreatureInfo.Size() + pInfo->mapSpawnPoint.Size() + 100;

		DWORD dwMinID = (DWORD)MIN_CREATURE_ID;
		for(INT n = 0; n < nCreatureNum; n++)
		{
			m_listFreeCreatureID.PushBack(dwMinID+(DWORD)n);
		}
		m_dwMaxCreatureID = dwMinID + nCreatureNum;
	}

	DWORD GetNewCreatureID()
	{
		DWORD dwCreatureID = m_listFreeCreatureID.PopFront();

		if( !P_VALID(dwCreatureID) )
		{
			// list���Ѿ�û�ж����id�ˣ���������100��
			for(INT n = 1; n < 100; n++)
			{
				m_listFreeCreatureID.PushBack(m_dwMaxCreatureID+(DWORD)n);
			}
			dwCreatureID = m_dwMaxCreatureID;
			m_dwMaxCreatureID += 100;
		}

		return dwCreatureID;
	}

	DWORD ReturnCreatureID(DWORD dwCreatureID)
	{
		ASSERT( IS_CREATURE(dwCreatureID) );
		m_listFreeCreatureID.PushFront(dwCreatureID);
	}

private:
	TList<DWORD>		m_listFreeCreatureID;
	DWORD				m_dwMaxCreatureID;
};

//---------------------------------------------------------------------------------
// ��ID������
//---------------------------------------------------------------------------------
class DoorIDGen
{
public:
	DoorIDGen():m_dwDoorIDValid(MIN_DOOR_ID){}
	DWORD GetValidDoorID()
	{
		return m_dwDoorIDValid++;
	}

private:
	DWORD m_dwDoorIDValid;
};

//---------------------------------------------------------------------------------------
// ��ͼ��
//---------------------------------------------------------------------------------------
class Map : public ScriptData<ESD_Map>
{
public:
	typedef	TMap<DWORD, PlayerSession*>		SessionMap;
	typedef TMap<DWORD, Role*>				RoleMap;
	typedef TMap<DWORD, Creature*>			CreatureMap;
	typedef TMap<INT64, tagGroundItem*>		GroundItemMap;
	typedef TMap<DWORD, Shop*>				ShopMap;
	typedef TMap<DWORD, GuildCofC*>			CofCMap;

public:
	Map();
	virtual ~Map();

	//-------------------------------------------------------------------------------------
	// ���������º�����
	//-------------------------------------------------------------------------------------
	BOOL				Init(const tagMapInfo* pInfo);
	VOID				Update();
	VOID				Destroy();

	//-------------------------------------------------------------------------------------
	// ����Get
	//-------------------------------------------------------------------------------------
	DWORD				GetMapID()			{ return m_dwID; }
	DWORD				GetInstanceID()		{ return m_dwInstanceID; }
	EMapType			GetMapType()		{ return m_pInfo->eType; }
	ENormalMapType		GetNormalMapType()	{ return m_pInfo->eNoarmalMapType; }
	NavMap*				GetNavMap()			{ return m_pNav; }
	const tagMapInfo*	GetMapInfo()		{ return m_pInfo; }
	INT					GetRoleNum()		{ return m_mapRole.Size(); }
	NPCTeamMgr*			GetTeamMgr()		{ return m_pTeamMgr; }
	RoleMap&			GetRoleMap()		{ return m_mapRole; }

	//------------------------------------------------------------------------------------------
	// ˫��
	//------------------------------------------------------------------------------------------
	FLOAT				GetExpRate() ;
	FLOAT				GetLootRate();

	//--------------------------------------------------------------------------------------
	// ���ӵ�ש���
	//--------------------------------------------------------------------------------------
	VOID				GetVisTile(INT nVisIndex, tagVisTile* visTile[ED_End]);
	VOID				GetVisTile(Vector3& pos, FLOAT fRange, tagVisTile* visTile[ED_End]);
	VOID				GetVisTileAdd(INT nOldVisIndex, INT nNewVisIndex, tagVisTile* pVisTile[ED_End]);
	VOID				GetVisTileDec(INT nOldVisIndex, INT nNewVisIndex, tagVisTile* pVisTile[ED_End]);
	VOID				GetVisTileAddAndDec(INT nOldVisIndex, INT nNewVisIndex, tagVisTile* pVisTileAdd[ED_End], tagVisTile* pVisTileDec[ED_End]);

	//--------------------------------------------------------------------------------------
	// ��ң�����͵�����Ʒ����Ӻ�ɾ��
	//--------------------------------------------------------------------------------------
	virtual VOID		AddRole(Role* pRole);
	virtual INT			CanEnter(Role *pRole)			{ return TRUE; }
	VOID				RoleLogOut(DWORD dwID);
	virtual VOID		RoleLeaveMap(DWORD dwID, BOOL bChgMap);
	VOID				AddCreatureOnLoad(Creature* pCreature);
	VOID				AddCreature(Creature* pCreature);
	VOID				RemoveCreature(Creature* pCreature);
	VOID				RemovePet(Pet* pPet);
	VOID				AddGroundItem(tagGroundItem *pGroundItem);
	VOID				RemoveGroundItem(tagGroundItem* pGroundItem);
	VOID 				SSpawnPtReplaceCreature(Creature* pDeadCreature);
	Creature*			CreateCreature(DWORD dwTypeID, Vector3& vPos, Vector3& vFaceTo, DWORD dwSpawnerID=GT_INVALID, BOOL bCollide=FALSE, CHAR* pPatrolListName=NULL);
	VOID				DeleteCreature(DWORD dwID);

	//---------------------------------------------------------------------------------------
	// ��Ϸ����Һ�����Ĳ�ѯ
	//---------------------------------------------------------------------------------------
	Role*				FindRole(DWORD dwID)			{ return m_mapRole.Peek(dwID); }		// һ���ڵ�ͼ�߳�֮��ʹ��
	Creature*			FindCreature(DWORD dwID)		{ return m_mapCreature.Peek(dwID); }	// һ���ڵ�ͼ�߳�֮��ʹ��
	Pet*				FindPet(DWORD dwID);				// һ���ڵ�ͼ�߳�֮��ʹ��;	
	Unit*				FindUnit(DWORD dwID);
	tagGroundItem*		GetGroundItem(INT64 n64Serial)	{ return m_mapGroundItem.Peek(n64Serial); }
	Shop*				GetShop(DWORD dwNPCID)			{ return m_mapShop.Peek(dwNPCID); }
	GuildCofC*			GetCofC(DWORD dwNPCID)			{ return m_mapCofC.Peek(dwNPCID); }

	//--------------------------------------------------------------------------------------
	// ���ڵ�ͼ�Ź���ĸ���ͬ������
	//--------------------------------------------------------------------------------------
	VOID				SyncMovementToBigVisTile(Unit* pSelf);
	VOID				SyncRemoveUnitToBigVisTile(Unit* pSelf);
	VOID				SyncChangeVisTile(Unit* pSelf, INT nOldVisIndex, INT nNewVisIndex);
	VOID				SyncRoleLeaveMap(Role* pSelf);
	VOID				SendBigVisTileMsg(Unit* pSelf, const LPVOID pMsg, const DWORD dwSize);
	VOID				SendBigVisTileMsg(tagVisTile *pVisTile[ED_End], const LPVOID pMsg, const DWORD dwSize);
	VOID				SendMapMessage(const LPVOID pMsg, const DWORD dwSize);
	VOID				SyncRemovedUnits(Unit* pSelf, tagVisTile* pVisTileDec[ED_End]);
	VOID				SyncAddedUnits(Unit* pSelf, tagVisTile* pVisTileAdd[ED_End]);
	VOID				SyncAddGroundItem(tagGroundItem *pGroundItem, tagVisTile *pVisTileAdd[ED_End]);
	VOID				SyncGroundItemState(tagGroundItem *pGroundItem, tagVisTile *pVisTileAdd[ED_End]);
	VOID				SyncRemoveGroundItem(tagGroundItem *pGroundItem, tagVisTile *pVisTileAdd[ED_End]);

	//--------------------------------------------------------------------------------------
	// ��ͼ��Ϣ��ͬ��
	//--------------------------------------------------------------------------------------
	VOID				SendSceneEffect(Role* pSelf);
	VOID				PlaySceneEffect(ESceneEffectType eType, DWORD dwObjID, Vector3 vPos, DWORD dwEffectID);
	VOID				StopSceneEffect(DWORD dwObjID);

	//----------------------------------------------------------------------------------------
	// ��ͼ�������
	//----------------------------------------------------------------------------------------
	DWORD				CheckArea(Role* pRole);							// ����������ڵĸ������򣨷��������־λ��

	//----------------------------------------------------------------------------------------
	// �����⼰����
	//----------------------------------------------------------------------------------------
	BOOL				IsPosValid(const Vector3& vVec);
	VOID				FixPos(Vector3& vVec);

	//----------------------------------------------------------------------------------------
	// ��ͼ�������������
	//----------------------------------------------------------------------------------------
	BOOL				IsInTrigger(Role* pRole, DWORD dwMapTriggerID);	// �Ƿ��ڴ�������
	DWORD				GetTriggerQuestSerial(DWORD dwMapTriggerID);	// �õ�ĳ��trigger��Ӧ���������к�
	
	//----------------------------------------------------------------------------------------
	// ������ص�һЩ����
	//----------------------------------------------------------------------------------------
	BOOL				CanPutDown(Creature* pCreature, INT nIndex, Vector3& vPos);
	VOID				PutDownItem(Creature* pCreature, tagItem* pLootItem, DWORD dwOwnerID, DWORD dwTeamID, Vector3& vPos);
	VOID				PutDownMoney(Creature* pCreature, INT nMoney, Role* pRole, Vector3 vPos);
	BOOL				GetCanGoPosFromIndex(INT32 nIndex, INT32 nIndexX, INT32 nIndexZ, INT32 n, Vector3& vPos);
	INT64				GenGroundItemID()		{ return m_GroundItemIDGen.GenValidID(); }

	//----------------------------------------------------------------------------------------
	// ��ͼս��ϵͳ������أ���ҪӦ���ڸ���
	//----------------------------------------------------------------------------------------
	BOOL				CanUseItem(DWORD dwTypeID);
	BOOL				CanUseSkill(DWORD dwItemID);

	//----------------------------------------------------------------------------------------
	// ���ҹ�ϵ�ж�
	//----------------------------------------------------------------------------------------
	DWORD				GetFriendEnemyFlag(Unit* pSrc, Unit* pTarget, BOOL& bIgnore);

	//------------------------------------------------------------------------------------------
	// ��ͼ�������
	//------------------------------------------------------------------------------------------
	tagMapAreaInfo*		IsInArea(tagMapInfo::MapAreaMap& mapArea, AABBox& roleBox, INT nTileX, INT nTileZ);

	//----------------------------------------------------------------------------------------
	// һЩ��������
	//----------------------------------------------------------------------------------------
	VOID				SendGotoNewMapToPlayer(Role* pSelf);
	BOOL				InSameBigVisTile(Unit* pSelf, Unit* pRemote);
	INT					WorldPosToVisIndex(const Vector3& pos);
	BOOL				IfCanGo(FLOAT fX, FLOAT fZ);
	BOOL				IfCanDirectGo(FLOAT fSrcX, FLOAT fSrcY, FLOAT fDestX, FLOAT fDestZ, POINT* pNearPos=NULL);
	BOOL				IfWillSwim(Vector3& vPos, FLOAT fHeight, FLOAT& fY);
	BOOL				IfWillOnWater(Vector3& vPos, FLOAT& fY);
	BOOL				IsRayCollide(const Vector3& vPos1, const Vector3& vPos2, DWORD dwFlag = 0);
	FLOAT				GetGroundHeight(FLOAT fX, FLOAT fZ);
	FLOAT				GetGroundAndModelHeight(AABBox& box);
	NavCollider*        GetNavCollider();

	VOID				OpenCloseDoor(DWORD dwObjID, BOOL bOpen);
	VOID				OpenCloseAllDoors(BOOL bOpen);

	template<typename UnitOperation>
	VOID				ForEachUnitInBigVisTile(Unit* pUnit, UnitOperation oper);
	template<typename UnitOperation>
	VOID				ForEachUnitInMap(UnitOperation oper);

	//-----------------------------------------------------------------------------------------
	// ��ͼ�¼�
	//-----------------------------------------------------------------------------------------
	VOID				OnRoleDie(Role* pRole, Unit* pKiller);
	VOID				OnRevive(Role* pRole, ERoleReviveType eType, INT &nReviveHP, INT &nReviveMP, FLOAT &fx, FLOAT &fy, FLOAT &fz, DWORD &dwRebornMapID);
	VOID				OnCreatureDie(Creature* pCreature, Unit* pKiller);
	VOID				OnCreatureDisappear(Creature* pCreature);
	VOID				OnEnterTrigger(Role* pRole, tagMapTriggerInfo* pInfo);
	VOID				OnEnterArea(Role* pRole, tagMapAreaInfo* pInfo);
	INT		            CanInviteJoinTeam();
	INT 				CanLeaveTeam();
	INT					CanChangeLeader();
	INT					CanKickMember();
	BOOL				CanSetSafeguard();

	//------------------------------------------------------------------------------------------
	// Ǳ��
	//------------------------------------------------------------------------------------------
	VOID				Lurk(Unit *pUnit);
	VOID				UnLurk(Unit *pUnit);
	VOID				UpdateLurk2BigVisTileRole(Unit *pUnit);
	VOID				UpdateBigVisTileLurkUnit2Role(Role* pRole);

	
	//------------------------------------------------------------------------------------------
	// �ӿ�
	//------------------------------------------------------------------------------------------
	DWORD				CalMovementMsgEx(Unit* pSelf, LPBYTE pMsg, DWORD dwSize)
	{
		return CalMovementMsg(pSelf, pMsg, dwSize);
	}
protected:
	//-----------------------------------------------------------------------------------------
	// ��ʼ��
	//-----------------------------------------------------------------------------------------
	BOOL				InitLogicalInfo();						// ���ݵ�ͼ������Ϣ��ʼ�������߼���Ϣ
	BOOL				InitAllMapCreature();					// ���ɵ�ͼ��ĳ�ʼ����
	BOOL				InitAllFixedCreature();					// ���ɵ�ͼ������аڷŹ���
	virtual BOOL		InitAllSpawnPointCreature();			// ��������ˢ�µ����
	BOOL				InitAllFixedDoor();						// ���ɵ�ͼ���������
	VOID				InitNestCreature(Creature* pCreature);	// ��ʼ����Ѩ����
	VOID				InitTeamCreature(Creature* pCreature);  // ��ʼ��С�ӹ���
	BOOL				AddShop(DWORD dwNPCID, DWORD dwShopID);	// ��ʼ���̵�
	BOOL				AddCofC(DWORD dwNPCID, DWORD dwCofCID);	// ��ʼ���̻�

	//------------------------------------------------------------------------------------------
	// ��ͼ�¼�
	//------------------------------------------------------------------------------------------
	VOID				OnInit();								// ��ʼ��ʱ

	//------------------------------------------------------------------------------------------
	// ����
	//------------------------------------------------------------------------------------------
	VOID				UpdateSession();			// ����õ�ͼ���������ҵ�������Ϣ
	VOID				UpdateAllObjects();			// ���¸õ�ͼ�����ж���
	VOID				UpdateAllShops();			// ���¸õ�ͼ�����ж���
	VOID				UpdateAllCofC();			// ���¸õ�ͼ�����ж���

	//------------------------------------------------------------------------------------------
	// �Ź������
	//------------------------------------------------------------------------------------------
	VOID				AddToVisTile(Unit* pUnit, INT nVisIndex);
	VOID				RemoveFromVisTile(Unit* pUnit);
	VOID				AddGroundItemToVisTile(tagGroundItem* pGroundItem, INT nVisIndex);
	VOID				RemoveGroundItemFromVisTile(tagGroundItem* pGroundItem, INT nVisIndex);

	//------------------------------------------------------------------------------------------
	// ����ͬ��
	//------------------------------------------------------------------------------------------
	VOID				SyncBigVisTileInvisUnit2Role(Role* pRole, tagVisTile *pVisTile[ED_End]);
	VOID				SyncBigVisTileVisUnitAndGroundItem2Role(Role* pRole, tagVisTile *pVisTile[ED_End]);

	VOID				SyncInvisUnit2BigVisTileRole(Unit* pUnit, tagVisTile *pVisTile[ED_End], 
													const LPVOID pMsg, const DWORD dwSize);
	VOID				SyncVisUnit2BigVisTileRole(Unit* pUnit, tagVisTile *pVisTile[ED_End], 
													const LPVOID pMsg, const DWORD dwSize);

	VOID				SyncRemoveBigVisTileUnitAndGroundItem2Role(Role* pRole, tagVisTile *pVisTileDec[ED_End]);

	
	//------------------------------------------------------------------------------------------
	// ��������
	//------------------------------------------------------------------------------------------
	DWORD				CalMovementMsg(Unit* pSelf, LPBYTE pMsg, DWORD dwSize);
protected:
	TObjRef<Util>			m_pUtil;

	DWORD					m_dwID;					// ��ͼID
	DWORD					m_dwInstanceID;			// ����ID����ͨ��ͼ��IDΪGT_INVALID��

	//-------------------------------------------------------------------------------------------
	// ��������
	//-------------------------------------------------------------------------------------------
	SessionMap				m_mapSession;			// ��ͼ�����session�б�
	RoleMap					m_mapRole;				// ��ͼ������������
	CreatureMap				m_mapCreature;			// ��ͼ�������������
	CreatureMap				m_mapRespawnCreature;	// ������ȴ�ˢ�µ�����
	GroundItemMap			m_mapGroundItem;		// ���������Ʒ
	ShopMap					m_mapShop;				// ��ͼ����������̵�<dwNpcID, pShop>
	CofCMap					m_mapCofC;				// ���������̻�

	TMap<DWORD, DWORD>		m_mapDoorObj2Gen;		// ��ObjID������IDӳ���
	//-------------------------------------------------------------------------------------------
	// �Ź���
	//-------------------------------------------------------------------------------------------
	INT						m_nVisTileArrayWidth;	// �����ӵ�ש���
	INT						m_nVisTileArrayHeight;	// �����ӵ�ש����
	tagVisTile*				m_pVisTile;				// ���ӵ�ש����̬���ɵ�����

	//-------------------------------------------------------------------------------------------
	// ��ͼ����
	//-------------------------------------------------------------------------------------------
	const tagMapInfo*		m_pInfo;				// ��ͼ������Ϣ���ⲿ��̬���ԣ�����ɾ����
	NavMap*					m_pNav;					// ����ͼ
	const MapScript*		m_pScript;				// ��ͼ�ű�

	//-------------------------------------------------------------------------------------------
	// �Ѵ���������Ч�б�
	//-------------------------------------------------------------------------------------------
	TList<DWORD>			m_listSceneEffect;		// �Ѵ���������ЧObjID�б�
	Mutex					m_Mutex;				// ��Ч�б���

	//-------------------------------------------------------------------------------------------
	// ����ID������
	//-------------------------------------------------------------------------------------------
	CreatureIDGen			m_CreatureIDGen;		// ����ID������

	//-------------------------------------------------------------------------------------------
	// ����ID������
	//-------------------------------------------------------------------------------------------
	GroundItemIDGen			m_GroundItemIDGen;		// ����ID������

	//-------------------------------------------------------------------------------------------
	// ����С�ӹ�����
	//-------------------------------------------------------------------------------------------
	NPCTeamMgr*				m_pTeamMgr;

	//-------------------------------------------------------------------------------------------
	// ��ID������
	//-------------------------------------------------------------------------------------------
	DoorIDGen				m_DoorIDGen;			// ��ID������
};

//-----------------------------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------------------------
class MapInstance : public Map
{
public:
	MapInstance() : m_bDelete(FALSE), m_bEnd(FALSE) {}
	virtual ~MapInstance() {}

	virtual BOOL	Init(const tagMapInfo* pInfo, DWORD dwInstanceID, Role* pCreator=NULL, DWORD dwMisc=GT_INVALID) = 0;
	virtual VOID	Update() = 0;
	virtual VOID	Destroy() = 0;

	virtual VOID	AddRole(Role* pRole) = 0;
	virtual	VOID	RoleLeaveMap(DWORD dwID, BOOL bChgMap) = 0;
	virtual INT		CanEnter(Role *pRole)		{ if( IsDelete() ) return E_Instance_End_Delete; else return E_Success; }
	virtual BOOL	CanDestroy()				{ return IsDelete() && m_mapRole.Empty(); }

	virtual VOID	OnDestroy() = 0;

	//---------------------------------------------------------------------------------------------
	// ����������ɾ����־λ
	//---------------------------------------------------------------------------------------------
	VOID			SetDelete()					{ if( IsDelete() ) return; InterlockedExchange((LPLONG)&m_bDelete, TRUE); OnDelete(); }
	VOID			SetEnd()					{ if( IsEnd() ) return; InterlockedExchange((LPLONG)&m_bEnd, TRUE); }
	BOOL			IsEnd()						{ return m_bEnd; }
	BOOL			IsDelete()					{ return m_bDelete; }

protected:
	virtual VOID	OnDelete() = 0;						// ��������ɾ����־ʱ�Ĵ���ע�⣬�������ڸ�������ʱ����
	virtual BOOL	InitAllSpawnPointCreature() = 0;	// ��������ˢ�µ����

protected:
	volatile BOOL			m_bDelete;					// ����ɾ����־λ
	volatile BOOL			m_bEnd;						// ����������־λ
};

//------------------------------------------------------------------------------------------------
// ĳ�����Ƿ��Ƿ��ǿ����ߵ�
//------------------------------------------------------------------------------------------------
inline BOOL Map::IfCanGo(FLOAT fX, FLOAT fZ)
{
	return GetNavMap()->GetNPCNavMap()->IfCanGo(fX, fZ);
}

//---------------------------------------------------------------------------------------
// ĳ������֮���Ƿ����ͨ��
//---------------------------------------------------------------------------------------
inline BOOL Map::IfCanDirectGo(FLOAT fSrcX, FLOAT fSrcY, FLOAT fDestX, FLOAT fDestZ, POINT* pNearPos)
{
	return GetNavMap()->GetNPCNavMap()->IfCanDirectGo(fSrcX, fSrcY, fDestX, fDestZ, pNearPos);
}

//----------------------------------------------------------------------------------------
// �õ�����߶�
//----------------------------------------------------------------------------------------
inline FLOAT Map::GetGroundHeight(FLOAT fX, FLOAT fZ)
{
	return GetNavMap()->GetNPCNavMap()->SafeGetHeight(fX, fZ);
}

//----------------------------------------------------------------------------------------
// �õ�ĳ�����Ƿ�Ҫ��Ӿ
//----------------------------------------------------------------------------------------
inline BOOL Map::IfWillSwim(Vector3& vPos, FLOAT fHeight, FLOAT& fY)
{
	return GetNavMap()->GetCollider()->IfWillSwim(vPos, fHeight, fY);
}


//----------------------------------------------------------------------------------------
// �õ�ĳ�����Ƿ���ˮ��
//----------------------------------------------------------------------------------------
inline BOOL Map::IfWillOnWater(Vector3& vPos, FLOAT& fY)
{
	return GetNavMap()->GetCollider()->IfWillOnWater(vPos, fY);
}

//----------------------------------------------------------------------------------------
// ������߼��
//----------------------------------------------------------------------------------------
inline BOOL Map::IsRayCollide(const Vector3& vPos1, const Vector3& vPos2, DWORD dwFlag/* = 0*/)
{
	Ray ray;
	ray.origin = vPos1;
	ray.dir = vPos2 - vPos1;
	ray.length = Vec3Dist(vPos1, vPos2);
	Vec3Normalize(ray.dir);

	if( GetNavMap()->GetCollider()->RayCollideBoxAndTrn(ray, dwFlag) )
	{
		return TRUE;
	}

	return FALSE;
}

//----------------------------------------------------------------------------------------
// �õ������ģ�͸߶�
//----------------------------------------------------------------------------------------
inline FLOAT Map::GetGroundAndModelHeight(AABBox& box)
{
	// ��Yָ���ĸ߶�������
	AABBox boxTemp = box;
	boxTemp.max.y = box.min.y + 10.0f;
	boxTemp.min.y = GET_HEIGHT_MIN_Y;

	FLOAT fIntersectY = 0.0f;
	GetNavMap()->GetCollider()->AABBoxCollideBoxTopAndTrn(boxTemp, 0.0f, fIntersectY);
	return fIntersectY;
}

//------------------------------------------------------------------------------------------------------------------
// �������
//------------------------------------------------------------------------------------------------------------------
inline VOID Map::OnRoleDie(Role* pRole, Unit* pKiller)
{
	if( P_VALID(m_pScript) )
	{
		m_pScript->OnRoleDie(pRole, pKiller, this);
	}
}

//------------------------------------------------------------------------------------------------------------------
// ����Ƿ���Կ���PK����
//------------------------------------------------------------------------------------------------------------------
inline BOOL Map::CanSetSafeguard()
{
	if( P_VALID(m_pScript) )
	{
		return m_pScript->CanSetSafeguard(this);
	}

	return TRUE;
}

//------------------------------------------------------------------------------------------------------------------
// ����Ƿ����ʹ����Ʒ
//------------------------------------------------------------------------------------------------------------------
inline BOOL Map::CanUseItem(DWORD dwTypeID)
{
	if( P_VALID(m_pScript) ) 
	{
		return m_pScript->CanUseItem(this, dwTypeID);
	}

	return TRUE;
}

//------------------------------------------------------------------------------------------------------------------
// ����Ƿ����ʹ�ü���
//------------------------------------------------------------------------------------------------------------------
inline BOOL Map::CanUseSkill(DWORD dwTypeID)
{
	if( P_VALID(m_pScript) )
	{
		return m_pScript->CanUseSkill(this, dwTypeID);
	}

	return TRUE;
}

//------------------------------------------------------------------------------------------------------------------
// ��Ҹ���
//------------------------------------------------------------------------------------------------------------------
inline VOID Map::OnRevive(Role* pRole, ERoleReviveType eType, INT &nReviveHP, INT &nReviveMP, FLOAT &fx, FLOAT &fy, FLOAT &fz, DWORD &dwRebornMapID)
{
	if( P_VALID(m_pScript) )
	{
		m_pScript->Revive(pRole, eType, nReviveHP, nReviveMP, fx, fy, fz, dwRebornMapID);
	}
}

//-------------------------------------------------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------------------------------------------------
inline VOID Map::OnCreatureDie(Creature* pCreature, Unit* pKiller)
{
	if( P_VALID(m_pScript) )
	{
		m_pScript->OnCreatureDie(pCreature, pKiller, this);
	}
}

//-------------------------------------------------------------------------------------------------------------------
// ������ʧ
//-------------------------------------------------------------------------------------------------------------------
inline VOID Map::OnCreatureDisappear(Creature* pCreature)
{
	if( P_VALID(m_pScript) ) 
	{
		m_pScript->OnCreatureDisappear(pCreature, this);
	}
}

//--------------------------------------------------------------------------------------------------------------------
// ��ҽ��봥����
//--------------------------------------------------------------------------------------------------------------------
inline VOID Map::OnEnterTrigger(Role* pRole, tagMapTriggerInfo* pInfo)
{
	if( P_VALID(m_pScript) )
	{
		m_pScript->OnEnterTrigger(pRole, pInfo, this);
	}
}

//---------------------------------------------------------------------------------------------------------------------
// ��ҽ�������
//---------------------------------------------------------------------------------------------------------------------
inline VOID Map::OnEnterArea(Role* pRole, tagMapAreaInfo* pInfo)
{
	if( P_VALID(m_pScript) )
	{
		m_pScript->OnEnterArea(pRole, pInfo, this);
	}
}

//---------------------------------------------------------------------------------------------------------------------
// ���ɼ���λͬ������Χ���
//---------------------------------------------------------------------------------------------------------------------
inline VOID Map::SyncVisUnit2BigVisTileRole(Unit* pUnit, tagVisTile *pVisTile[ED_End], 
											const LPVOID pMsg, const DWORD dwSize)
{
	SendBigVisTileMsg(pVisTile, pMsg, dwSize);
}

//---------------------------------------------------------------------------------------------------------------------
// �Ƿ������������
//---------------------------------------------------------------------------------------------------------------------
inline INT Map::CanInviteJoinTeam()
{
	if( P_VALID(m_pScript) )
	{
		return m_pScript->CanInviteJoinTeam(this);
	}

	return 0;
}

//---------------------------------------------------------------------------------------------------------------------
// �Ƿ������뿪����
//---------------------------------------------------------------------------------------------------------------------
inline INT Map::CanLeaveTeam()
{
	if( P_VALID(m_pScript) )
	{
		return m_pScript->CanLeaveTeam(this);
	}

	return 0;
}

//---------------------------------------------------------------------------------------------------------------------
// �Ƿ������ƽ��ӳ�
//---------------------------------------------------------------------------------------------------------------------
inline INT Map::CanChangeLeader()
{
	if( P_VALID(m_pScript) )
	{
		return m_pScript->CanChangeLeader(this);
	}

	return 0;
}

//---------------------------------------------------------------------------------------------------------------------
// �Ƿ�����ߵ����
//---------------------------------------------------------------------------------------------------------------------
inline INT Map::CanKickMember()
{
	if( P_VALID(m_pScript) ) 
	{
		return m_pScript->CanKickMember(this);
	}

	return 0;
}

//-----------------------------------------------------------------------------------------------------------------------
// ���ҹ�ϵ�ж�
//-----------------------------------------------------------------------------------------------------------------------
inline DWORD Map::GetFriendEnemyFlag(Unit* pSrc, Unit* pTarget, BOOL& bIgnore)
{
	if( !P_VALID(m_pScript) )
	{
		bIgnore = FALSE;
		return 0;
	}

	return m_pScript->FriendEnemy(this, pSrc, pTarget, bIgnore);
}

//-----------------------------------------------------------------------------------------------------------------------
// ����Ƿ�
//-----------------------------------------------------------------------------------------------------------------------
inline BOOL Map::IsPosValid(const Vector3& vVec)
{
	// ����Ƿ񳬳��߽�
	if( vVec.x < 0.0f || vVec.x >= FLOAT(m_pInfo->nWidth * TILE_SCALE) ||
		vVec.z < 0.0f || vVec.z >= FLOAT(m_pInfo->nHeight * TILE_SCALE) )
		return FALSE;

	return TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
// ��������
//--------------------------------------------------------------------------------------------------------------------------
inline VOID Map::FixPos(Vector3& vVec)
{
	// ������Χ
	if( vVec.x < 0.0f )				vVec.x = 0.0f;
	if( vVec.z < 0.0f )				vVec.z = 0.0f;
	if( vVec.x >= FLOAT(m_pInfo->nWidth * TILE_SCALE) )		vVec.x = FLOAT(m_pInfo->nWidth * TILE_SCALE) - 1.0f;
	if( vVec.z >= FLOAT(m_pInfo->nHeight * TILE_SCALE) )	vVec.z = FLOAT(m_pInfo->nHeight * TILE_SCALE) - 1.0f;

	// �����߶�
	FLOAT fGroundHeight = GetGroundHeight(vVec.x, vVec.z);
	if( vVec.y < fGroundHeight ) vVec.y = fGroundHeight;
}

template<typename UnitOperation>
VOID Map::ForEachUnitInBigVisTile(Unit* pSelf, UnitOperation oper )
{
	if( !P_VALID(pSelf) ) return;

	tagVisTile* pVisTile[ED_End] = {0};
	GetVisTile(pSelf->GetVisTileIndex(), pVisTile);

	for(INT n = 0; n < ED_End; n++)
	{
		if( NULL == pVisTile[n] )
			continue;

		// �ҵ�ÿ����ש����
		TMap<DWORD, Role*>& mapRole = pVisTile[n]->mapRole;
		mapRole.ResetIterator();
		Role* pRole = NULL;

		while( mapRole.PeekNext(pRole) )
		{
			if( P_VALID(pRole) )
			{
				oper(pRole);				
			}
		}
	}
}

template<typename UnitOperation>
VOID Map::ForEachUnitInMap( UnitOperation oper )
{
	// �ҵ�ÿ����ͼ����
	RoleMap::TMapIterator itrR = m_mapRole.Begin();
	Role* pRole = NULL;

	while( m_mapRole.PeekNext(itrR, pRole) )
	{
		if( P_VALID(pRole) && pred(pRole))
		{
			oper(pRole);				
		}
	}

	// �ҵ�ÿ����ͼ�Ĺ���
	CreatureMap::TMapIterator itrC = m_mapCreature.Begin();
	Creature* pCreature = NULL;

	while( m_mapCreature.PeekNext(itrC, pCreature) )
	{
		if( P_VALID(pCreature) && pred(pCreature))
		{
			oper(pCreature);				
		}
	}
}
