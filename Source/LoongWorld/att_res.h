//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: att_res.h
// author: Sxg
// actor:
// data: 2008-05-28
// last:
// brief: ��Դ����������
//-----------------------------------------------------------------------------
#pragma once

#include "../ServerDefine/role_data_define.h"
#include "../WorldDefine/ItemDefine.h"
#include "../WorldDefine/skill_define.h"
#include "../WorldDefine/buff_define.h"
#include "../WorldDefine/trigger_define.h"
#include "../WorldDefine/compose_define.h"
#include "../WorldDefine/MapAttDefine.h"
#include "../WorldDefine/mall_define.h"
#include "../WorldDefine/variable_len.h"
#include "../WorldDefine/guild_define.h"
#include "../WorldDefine/stall_define.h"
#include "../ServerDefine/pet_define.h"
#include "../ServerDefine/consolidate_define.h"
#include "../ServerDefine/title_define.h"
#include "../ServerDefine/instance_define.h"
#include "../ServerDefine/att_res_define.h"
#include "../WorldDefine/pet_define.h"
#include "../ServerDefine/guild_define.h"
#include "famehall_part.h"
#include "../ServerDefine/item_define.h"


struct	tagMotionProto;
struct	tagPetProto;
struct	tagPetSkillProto;
struct	tagPetEquipProto;
struct	tagPetGatherProto;
struct	tagPetWuXingProto;
struct	tagVNBEquipProto;
struct	tagVNBGiftProto;
struct	tagItemProto;
struct	tagShopProto;
struct	tagDakProto;
struct	tagSuitProto;
struct	tagMallItemProto;
struct	tagMallPackProto;
struct	tagSSpawnPointProto;
struct	tagGuildFacilities;
struct	tagGuildUpgradeNeed;
struct	tagCofCSellGood; 
struct	tagCofCBuyGood; 

class	Quest;
//-----------------------------------------------------------------------------
class AttRes
{
public:
	typedef VOID* (WINAPI* Fun_p)(LPVOID, LPCTSTR, INT32);

public:
	BOOL Init();
	VOID Destroy();

public: // �ϲ�ȷ������ -- ������ڵ�ͼ�̵߳��ϲ��߳�ִ��
	// ���¼���ָ�������ļ�
	BOOL ReloadItemProto();

	// ���¼����̳�����
	BOOL ReloadMallProto();

	// ���¼�����Ҫ��log����Ʒid -- item_needlog��
	// ����ϢtagNDBC_LoadItemNeedLog����

public:
	// ����TypeID���������Ƿ��¼log
	VOID ResetItemLog(DWORD dwTypeID[], INT32 nNum);

public:
	//-------------------------------------------------------------------------------------------
	// ����
	//-------------------------------------------------------------------------------------------
	const tagLevelUpEffect* GetLevelUpEffect(INT32 nLevel) const { return &m_LevelUpEffect[nLevel]; }

	//-------------------------------------------------------------------------------------------
	// Ĭ��ֵ
	//-------------------------------------------------------------------------------------------
	INT GetAttDefRole(INT nIndex) { ASSERT( nIndex > ERA_Null && nIndex < ERA_End ); return m_AttDefMinMax[nIndex].nDefRole; }
	INT GetAttDefCreature(INT nIndex) { ASSERT( nIndex > ERA_Null && nIndex < ERA_End ); return m_AttDefMinMax[nIndex].nDefCreature; }
	INT GetAttMin(INT nIndex) { ASSERT( nIndex > ERA_Null && nIndex < ERA_End ); return m_AttDefMinMax[nIndex].nMin; }
	INT GetAttMax(INT nIndex) { ASSERT( nIndex > ERA_Null && nIndex < ERA_End ); return m_AttDefMinMax[nIndex].nMax; }

	//-------------------------------------------------------------------------------------------
	// ���˴ʱ�
	//-------------------------------------------------------------------------------------------
	std::vector<tstring>* GetNameFilterWords() 	{ return &m_vectNameFilter; }
	//std::vector<tstring>* GetChatFilterWords() 	{ return &m_vectChatFilter; }

	//-------------------------------------------------------------------------------------------
	// ��ͬ���԰汾���Ƴ���
	//-------------------------------------------------------------------------------------------
	const tagVariableLen& GetVariableLen() const { return m_VarLen; }

	//-------------------------------------------------------------------------------
	// ��ȡȨ�ޱ�
	//-------------------------------------------------------------------------------
	const tagGuildPower& GetGuildPower(EGuildMemberPos eGuildPos) const
	{
		return m_GuildPowerDefault[eGuildPos];
	}

	const tagGuildKick& GetGuildKick(EGuildMemberPos eGuildPos) const
	{
		return m_GuildKickDefault[eGuildPos];
	}

	const tagGuildAppoint& GetGuildAppoint(EGuildMemberPos eGuildPos) const
	{
		return m_GuildAppointDefault[eGuildPos];
	}

	//-------------------------------------------------------------------------------
	// ��ȡ������ʩ����������Ϣ
	//-------------------------------------------------------------------------------
	BOOL GetGuildUpgradeBaseInfo(BYTE eType, BYTE byLevel, OUT tagGuildFacilities& sInfo);
	BOOL GetGuildUpgradeItemInfo(BYTE eType, BYTE byLevel, OUT tagGuildFacilities& sInfo);

	//-------------------------------------------------------------------------------
	// ��ȡ����������Ϣ
	//-------------------------------------------------------------------------------
	const tagGuildAffairInfo* GetGuildAffairInfo(DWORD dwBuffID)	{ return m_GuildAffairInfo.Peek(dwBuffID); }

	//-------------------------------------------------------------------------------
	// ��ȡ���ɼ�����Ϣ
	//-------------------------------------------------------------------------------
	BOOL GetGuildSkillInfo(DWORD dwSkillID, INT nLevel, tagGuildSkill& sGuildSkill);
	BOOL LoadGuildSkillInfo(TMap<DWORD, tagGuildSkill*>& mapGuildSkill);
	const tagGuildSkill* GetGuildSkillProto(DWORD dwSkillID)		{ return m_GuildSkillInfo.Peek(dwSkillID); }

	//-------------------------------------------------------------------------------
	// ��ȡ����������Ϣ
	//-------------------------------------------------------------------------------
	const tagCommerceInfo*		GetGuildCommerceInfo(INT nLevel);
	const tagCommodityProto*	GetCommodityProto(DWORD dwGoodID)	{ return m_GuildCommodityProto.Peek(dwGoodID); }
	const tagCofCSPProto*		GetCofCSPProto(DWORD dwCofCID)		{ return m_CofCSPProto.Peek(dwCofCID); }
	const FLOAT					GetCofCProfit(DWORD dwDstID, DWORD dwSrcID);
	BOOL LoadCofCGoodInfo(DWORD dwCofCID, TMap<DWORD, tagCofCSellGood*>& mapGoodSell,
		TMap<DWORD, tagCofCBuyGood*>& mapGoodBuy);
	
	//-------------------------------------------------------------------------------------------
	// ��Ʒ
	//-------------------------------------------------------------------------------------------
	tagItemProto* GetItemProto(DWORD dwTypeID)			{ return m_mapItemProto.Peek(dwTypeID); }
	//-------------------------------------------------------------------------------------------
	// װ��
	//-------------------------------------------------------------------------------------------
	tagEquipProto* GetEquipProto(DWORD dwTypeID)		{ return m_mapEquipProto.Peek(dwTypeID); }
	//-------------------------------------------------------------------------------------------
	// ����(��ʯ,ӡ��,�����)
	//-------------------------------------------------------------------------------------------
	tagGemProto* GetGemProto(DWORD dwTypeID)			{ return m_mapGemProto.Peek(dwTypeID); }
	//-------------------------------------------------------------------------------------------
	// �̵�
	//-------------------------------------------------------------------------------------------
	tagShopProto* GetShopProto(DWORD dwShopID)			{ return m_mapShopProto.Peek(dwShopID); }

	//-------------------------------------------------------------------------------------------
	// �̳���Ʒ, ��Ʒ���������Ʒ
	//-------------------------------------------------------------------------------------------
	const tagMallItemProto* GetMallItemProto(DWORD dwID){ return m_mapMallItemProto.Peek(dwID); }
	const tagMallPackProto* GetMallPackProto(DWORD dwID){ return m_mapMallPackProto.Peek(dwID); }
	const tagMallFreeItem*	GetMallFreeProto()			{ return &m_MallFreeItemProto; }
	
	TMap<DWORD, tagMallItemProto*>& GetMallItem()		 { return m_mapMallItemProto; }
	TMap<DWORD, tagMallPackProto*>& GetMallPack()		 { return m_mapMallPackProto; }

	INT	GetMallItemNum()								{ return m_mapMallItemProto.Size(); }
	INT GetMallPackNum()								{ return m_mapMallPackProto.Size(); }

	//-------------------------------------------------------------------------------------------
	// ��վ
	//-------------------------------------------------------------------------------------------
	tagDakProto* GetDakProto(DWORD dwDakID)				{ return m_mapDakProto.Peek(dwDakID); }
	//-------------------------------------------------------------------------------------------
	// װ��Ʒ����������
	//-------------------------------------------------------------------------------------------
	tagEquipQltyPct* GetEquipQltyPct(DWORD dwTypeID)	{ return m_mapEquipQltyPct.Peek(dwTypeID); }
	//-------------------------------------------------------------------------------------------
	// ��װ��̬����
	//-------------------------------------------------------------------------------------------
	const tagSuitProto* GetSuitProto(DWORD dwSuitID)	{ return m_mapSuitProto.Peek(dwSuitID); }
	//-------------------------------------------------------------------------------------------
	// Ʒ��װ�����Բ���
	//-------------------------------------------------------------------------------------------
	const tagEquipQltyEffect* GetEquipQltyEffect(INT32 nQlty) const { return &m_EquipQltyEffect[nQlty]; }

	//-------------------------------------------------------------------------------------------
	// ��������
	//-------------------------------------------------------------------------------------------
	const tagLongHunProto* GetLongHunProto(DWORD dwID) { return m_mapLongHunProto.Peek(dwID); }
	
	const std::vector<DWORD>& GetLongHunSpecVect(INT nType, INT nTypeEx, INT nQlty) const
	{
		return m_vectLongHun[nType][nTypeEx][nQlty];
	}

	//-------------------------------------------------------------------------------------------
	// ʱװƷ��������Ӱ�����
	//-------------------------------------------------------------------------------------------
	const tagFashionGen* GetFashionQltyEffect(INT32 nQlty) const { return &m_FashionGen[nQlty]; }

	//-------------------------------------------------------------------------------------------
	// ʱװ��ɫ���ɸ��ʲ���
	//-------------------------------------------------------------------------------------------
	const tagFashionColorPct* GetFashionColorPct(INT32 nQlty) const { return &m_FashionColorPct[nQlty]; }
	
	//-------------------------------------------------------------------------------------------
	// �õ�����Ҫ��������Ʒ��ָ���б�
	//-------------------------------------------------------------------------------------------
	TList<tagItemProto*> GetItemProtoList();
	//-------------------------------------------------------------------------------------------
	// �õ��������Զ�Ӧ��װ���Ƿ��ǿ��
	//-------------------------------------------------------------------------------------------
	BOOL IsPosyPos(EPosyAtt ePosyAtt, EEquipPos eEquipPos);
	//-------------------------------------------------------------------------------------------
	// �õ��Կ����Զ�Ӧ��װ���Ƿ��ǿ��
	//-------------------------------------------------------------------------------------------
	BOOL IsEngravePos(EEngraveAtt ePosyAtt, EEquipPos eEquipPos);

	//-------------------------------------------------------------------------------------------
	// ����
	//-------------------------------------------------------------------------------------------
	const tagSkillProto* GetSkillProto(DWORD dwTypeID) { return m_mapSkillProto.Peek(dwTypeID); }

	//-------------------------------------------------------------------------------------------
	// ״̬
	//-------------------------------------------------------------------------------------------
	const tagBuffProto* GetBuffProto(DWORD dwTypeID) { return m_mapBuffProto.Peek(dwTypeID); }

	//-------------------------------------------------------------------------------------------
	// ������
	//-------------------------------------------------------------------------------------------
	const tagTriggerProto* GetTriggerProto(DWORD dwTriggerID) { return m_mapTriggerProto.Peek(dwTriggerID); }

	//-------------------------------------------------------------------------------------------
	// ĳ�������Ƿ��ܱ���������Ӱ��
	//-------------------------------------------------------------------------------------------
	BOOL CanBeModified(DWORD dwSkillID) { return m_mapSkillModify.IsExist(dwSkillID); }

	//-------------------------------------------------------------------------------------------
	// �õ���Ӱ��ĳ�����ܵļ����б�
	//-------------------------------------------------------------------------------------------
	tagSkillModify* GetSkillModifier(DWORD dwSkillID) { return m_mapSkillModify.Peek(dwSkillID); }

	//-------------------------------------------------------------------------------------------
	// ����
	//-------------------------------------------------------------------------------------------
	const tagCreatureProto* GetCreatureProto(DWORD dwTypeID) { return m_mapCreatureProto.Peek(dwTypeID); }

	//-------------------------------------------------------------------------------------------
	// ����AI
	//-------------------------------------------------------------------------------------------
	const tagCreatureAI* GetCreatureAI(DWORD dwAIID) { return m_mapCreatureAI.Peek(dwAIID); }

	//-------------------------------------------------------------------------------------------
	// �����������ͼˢ�ֵ�
	//-------------------------------------------------------------------------------------------
	const tagSSpawnPointProto *GetSSpawnPointProto(DWORD dwSpawnPointID) { return m_mapSSpawnPoint.Peek(dwSpawnPointID); }

	//-------------------------------------------------------------------------------------------
	// �������ˢ�ֵ�
	//-------------------------------------------------------------------------------------------
	const tagRandSpawnPointInfo* GetSpawnPointProto(DWORD dwSpawnPoint) { return m_mapSpawnPointProto.Peek(dwSpawnPoint); }

	//-------------------------------------------------------------------------------------------
	// �������ˢ�ֵ�
	//-------------------------------------------------------------------------------------------
	const tagInstance*	GetInstanceProto(DWORD dwMapID)	{ return m_mapInstanceProto.Peek(dwMapID); }

	//-------------------------------------------------------------------------------------------
	// �����в���ʹ�õ���Ʒ
	//-------------------------------------------------------------------------------------------
	const tagInstanceItem* GetInstanceItem(DWORD dwMapID) { return m_mapInstanceItem.Peek(dwMapID); }

	//-------------------------------------------------------------------------------------------
	// �����в���ʹ�õļ���
	//-------------------------------------------------------------------------------------------
	const tagInstanceSkill* GetInstanceSkill(DWORD dwMapID) { return m_mapInstanceSkill.Peek(dwMapID); }

	//-------------------------------------------------------------------------------------------
	// �������ˢ�ֵ�ȼ�ӳ���
	//-------------------------------------------------------------------------------------------
	const tagLevelMapping* GetLevelMapping(INT nLevel) { return m_mapLevelMapping.Peek(nLevel); }

	//-------------------------------------------------------------------------------------------
	// ����
	//-------------------------------------------------------------------------------------------
	const tagPosyProtoSer* GetPosyProto(DWORD dwPosyID) { return m_mapPosyProto.Peek(dwPosyID); }

	//-------------------------------------------------------------------------------------------
	// �Կ�
	//-------------------------------------------------------------------------------------------
	const tagEngraveProtoSer* GetEngraveProto(DWORD dwEngraveID) { return m_mapEngraveProto.Peek(dwEngraveID); }

	//-------------------------------------------------------------------------------------------
	// ��Ƕ,��ӡ,����
	//-------------------------------------------------------------------------------------------
	const tagConsolidateItem* GetConsolidateProto(DWORD dwTypeID) { return m_mapConsolidateProto.Peek(dwTypeID); }

	//-------------------------------------------------------------------------------------------
	// �ϳ�
	//-------------------------------------------------------------------------------------------
	const tagProduceProtoSer* GetProduceProto(DWORD dwFormulaID) { return m_mapProduceProto.Peek(dwFormulaID); }

	//-------------------------------------------------------------------------------------------
	// �㻯,װ���ֽ�
	//-------------------------------------------------------------------------------------------
	const tagDeComposeProtoSer* GetDeComposeProto(DWORD dwFormulaID) { return m_mapDeComposeProto.Peek(dwFormulaID); }

	//-------------------------------------------------------------------------------------------
	// ���
	//-------------------------------------------------------------------------------------------
	const tagQuenchProtoSer *GetQuenchProto(DWORD dwFormulaID) { return m_mapQuenchProto.Peek(dwFormulaID); }

	//-------------------------------------------------------------------------------------------
	// �ƺ�
	//-------------------------------------------------------------------------------------------
	const tagTitleProto *GetTitleProto(UINT16 u16TitleID) { return &m_TitleProto[u16TitleID]; }

	//-------------------------------------------------------------------------------------------
	// �����䱦
	//-------------------------------------------------------------------------------------------
	const tagClanTreasureProto *GetClanTreasureProto(UINT16 u16TreasureID) { return &m_ClanTreasureProto[u16TreasureID]; }

	VOID GetRandVNBEquipProto(std::list<tagVNBEquipProto*>& listEquips)	
	{	
		m_mapVNBEquipProto.ExportAllValue(listEquips);
	}

	const tagVNBGiftProto*	GetRandVNBGiftProto()	
	{	
		tagVNBGiftProto* pProto = NULL;
		DWORD dwId = GT_INVALID;
		if (!m_mapVNBGiftProto.RandPeek(dwId, pProto) || !P_VALID(pProto))
		{
			return NULL;
		}
		else
		{
			return pProto;
		}
	}

	const tagCreatureAI* RandGetCreatureAI()
	{
		DWORD dwAIID = GT_INVALID;
		tagCreatureAI* pAI = NULL;

		m_mapCreatureAI.RandPeek(dwAIID, pAI);

		return pAI;
	}
	const tagCreatureAI* RandGetCreatureAIInGroup(DWORD dwGroupID)
	{
		TList<DWORD>* pList = m_mapCreatureAIGroup.Peek(dwGroupID);
		if( P_VALID(pList) && pList->Size() > 0 )
		{
			DWORD dwAIID = GT_INVALID;
			pList->RandPeek(dwAIID);

			return m_mapCreatureAI.Peek(dwAIID);
		}
		else
		{
			return NULL;
		}
	}

	//-------------------------------------------------------------------------------------------
	// ��ó�������Ĭ����С���ֵ
	//-------------------------------------------------------------------------------------------
// 	INT GetPetDef(INT nPetAtt) { ASSERT(IS_EPA(nPetAtt));	return m_nPetAttDefMinMax[nPetAtt].nDef;	}
// 	INT GetPetMin(INT nPetAtt) { ASSERT(IS_EPA(nPetAtt));	return m_nPetAttDefMinMax[nPetAtt].nMin;	}
// 	INT GetPetMax(INT nPetAtt) { ASSERT(IS_EPA(nPetAtt));	return m_nPetAttDefMinMax[nPetAtt].nMax;	}

	//-------------------------------------------------------------------------------------------
	// ��ó���ԭ��
	//-------------------------------------------------------------------------------------------
	const tagPetProto* GetPetProto(DWORD dwPetTypeID) { return m_mapPetProto.Peek(dwPetTypeID); }

	//-------------------------------------------------------------------------------------------
	// ��ó�������ԭ��
	//-------------------------------------------------------------------------------------------
	const tagPetLvlUpProto* GetPetLvlUpProto(DWORD dwVLevel) { return m_mapPetLvlUpProto.Peek(dwVLevel); }

	//-------------------------------------------------------------------------------------------
	// ��ó�������ԭ��
	//-------------------------------------------------------------------------------------------
	const tagPetLvlUpItemProto* GetPetLvlUpItemProto(DWORD dwTypeID) { return m_mapPetLvlUpItemProto.Peek(dwTypeID); }

	//-------------------------------------------------------------------------------------------
	// ��ó��＼��ԭ��
	//-------------------------------------------------------------------------------------------
	const tagPetSkillProto* GetPetSkillProto(DWORD dwPetSkillTypeID) { return m_mapPetSkillProto.Peek(dwPetSkillTypeID); }

	//-------------------------------------------------------------------------------------------
	// ��ó���װ��ԭ��
	//-------------------------------------------------------------------------------------------
	const tagPetEquipProto* GetPetEquipProto(DWORD dwPetEquipTypeID) { return m_mapPetEquipProto.Peek(dwPetEquipTypeID); }

	//-------------------------------------------------------------------------------------------
	// ��ó�����������ԭ��
	//-------------------------------------------------------------------------------------------
	const tagPetWuXingProto* GetPetWuXingProto(DWORD dwPetWuXingTypeID) { return m_mapPetWuXingProto.Peek(dwPetWuXingTypeID); }

	//-------------------------------------------------------------------------------------------
	// ��ó��＼������
	//-------------------------------------------------------------------------------------------
	const std::list<DWORD>& GetPetNormalSkillList(INT nPetLevel) { return m_PetLevelSkillVec[nPetLevel - 1]; }

	//-------------------------------------------------------------------------------------------
	// VIP̯λ��Ϣ
	//-------------------------------------------------------------------------------------------
	const INT32 GetVIPStallRent(INT nIndex) { return m_nVIPStallRent[nIndex]; }

	//-------------------------------------------------------------------------------------------
	// ��ö���ԭ��
	//-------------------------------------------------------------------------------------------
	const tagMotionProto* GetMotionProto(DWORD dwTypeID) { return m_mapMotionProto.Peek(dwTypeID); }

private:
	//-------------------------------------------------------------------------------------------
	// ���ó�ʼ��ĳ����Դ��һ����¼�Ĵ�����
	//-------------------------------------------------------------------------------------------
	VOID SetInitOneRes(Fun_p pFun) { m_pFun = pFun; }

	//-------------------------------------------------------------------------------------------
	// ��ʼ����Դ��ģ�溯�� -- ����map��
	//-------------------------------------------------------------------------------------------
	template<class K, class T> 
	BOOL LoadResMap(TMap<K, T*> &mapRes, LPCTSTR szFileName, LPCTSTR szFileName2 = NULL);

	//-------------------------------------------------------------------------------------------
	// �ͷ���Դ��ģ�溯��
	//-------------------------------------------------------------------------------------------
	template<class K, class T> VOID FreeResMap(TMap<K, T*> &mapRes);

	//-------------------------------------------------------------------------------------------
	// ��ʼ����Դ��ģ�溯�� -- ����array��
	//-------------------------------------------------------------------------------------------
	template<class T>
	BOOL LoadResArray(T *arrayRes, INT32 nIndexStart, INT32 nIndexEnd, LPCTSTR szFileName);

private:
	//-------------------------------------------------------------------------------------------
	// ��ʼ�����˴ʱ�
	//-------------------------------------------------------------------------------------------
	BOOL InitFilterWords(OUT vector<tstring>& vectFilterWords, LPCTSTR szFileName);

	//-------------------------------------------------------------------------------------------
	// �ӽű��������л�ȡ��������
	//-------------------------------------------------------------------------------------------
	VOID InitVarLen();

private:
	//-------------------------------------------------------------------------------------------
	// ��ʼ����Ʒ��һ����¼
	//-------------------------------------------------------------------------------------------
	VOID* InitOneItemProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// ��ʼ��װ����һ����¼
	//-------------------------------------------------------------------------------------------
	VOID* InitOneEquipProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// ��ʼ����ʯ�ȵ�һ����¼
	//-------------------------------------------------------------------------------------------
	VOID* InitOneGemProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// ��ʼ����װ��һ����¼
	//-------------------------------------------------------------------------------------------
	VOID* InitOneSuitProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// ��ʼ��һ��װ��Ʒ����������
	//-------------------------------------------------------------------------------------------
	VOID* InitOneEquipQltyPct(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// ��ʼ������������
	//-------------------------------------------------------------------------------------------
	VOID* InitOneLongHunProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// ��ʼ���̵�
	//-------------------------------------------------------------------------------------------
	VOID* InitOneShopProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// ��ʼ���̳���Ʒ����Ʒ��
	//-------------------------------------------------------------------------------------------
	VOID  InitOneMallItemProtoBase(OUT LPVOID pProtoType, IN LPCTSTR szField);
	VOID* InitOneMallItemProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);
	VOID* InitOneMallPackProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// ��ʼ����վ
	//-------------------------------------------------------------------------------------------
	VOID* InitOneDakProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// ��ʼ�����ܵ�һ����¼
	//-------------------------------------------------------------------------------------------
    VOID* InitOneSkillProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// ��ʼ��buff��һ����¼
	//-------------------------------------------------------------------------------------------
	VOID* InitOneBuffProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// ��ʼ��trigger��һ����¼
	//-------------------------------------------------------------------------------------------
	VOID* InitOneTriggerProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// ��ʼ��Creature��һ����¼
	//-------------------------------------------------------------------------------------------
	VOID* InitOneCreatureProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// ��ʼ��Creature_ai��һ����¼
	//-------------------------------------------------------------------------------------------
	VOID* InitOneCreatureAIProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ�����Ƶ�һ����¼
	//--------------------------------------------------------------------------------------------
	VOID* InitOnePosyProto(OUT LPVOID pProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ���Կ̵�һ����¼
	//--------------------------------------------------------------------------------------------
	VOID* InitOneEngraveProto(OUT LPVOID pProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ����Ƕ,��ӡ,������һ����¼
	//--------------------------------------------------------------------------------------------
	VOID* InitOneConsolidateProto(OUT LPVOID pProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ������ܵ�һ����¼
	//--------------------------------------------------------------------------------------------
	VOID* InitOneProduceProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ���,ͨ�÷ֽ�
	//--------------------------------------------------------------------------------------------
	VOID* InitOneDeComposeProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ���
	//--------------------------------------------------------------------------------------------
	VOID* InitOneQuenchProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// �����в���ʹ�õ���Ʒ
	//--------------------------------------------------------------------------------------------
	VOID* InitOneInsItemProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// �����в���ʹ�õļ���
	//--------------------------------------------------------------------------------------------
	VOID* InitOneInsSkillProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// �������ˢ�ֵ�
	//--------------------------------------------------------------------------------------------
	VOID* InitOneSpawnPointProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ������̬ˢ�ֵ�ȼ�ӳ���
	//--------------------------------------------------------------------------------------------
	VOID* InitOneLevelMapping(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ��������̬����
	//--------------------------------------------------------------------------------------------
	VOID* InitOneInstanceProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ���Ǹ���ˢ�ֵ�ԭ��
	//--------------------------------------------------------------------------------------------
	VOID* InitOneSSpawnPointProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ����������Ĭ����С���ֵ
	//--------------------------------------------------------------------------------------------
//	VOID InitPetAttDefMinMax(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//--------------------------------------------------------------------------------------------
	// ��ʼ������ԭ��
	//--------------------------------------------------------------------------------------------
	VOID* InitOnePetProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ����������ԭ��
	//--------------------------------------------------------------------------------------------
	VOID* InitOnePetLvlUpProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ������������Ʒԭ��
	//--------------------------------------------------------------------------------------------
	VOID* InitOnePetLvlUpItemProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ�����＼��ԭ��
	//--------------------------------------------------------------------------------------------
	VOID* InitOnePetSkillProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ������װ��ԭ��
	//--------------------------------------------------------------------------------------------
	VOID* InitOnePetEquipProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ���������������ԭ��
	//--------------------------------------------------------------------------------------------
	VOID* InitOnePetWuXingProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ������������Ʒ
	//--------------------------------------------------------------------------------------------
	VOID* InitOneVNBGiftProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ������������Ʒ
	//--------------------------------------------------------------------------------------------
	VOID* InitOneVNBEquipProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ������ɼ���
	//--------------------------------------------------------------------------------------------
	VOID* InitOnePetGatherProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
	//--------------------------------------------------------------------------------------------
	// ��ʼ�����＼��ԭ��
	//--------------------------------------------------------------------------------------------
	VOID InitPetSkillsVec();

	//--------------------------------------------------------------------------------------------
	// ��ʼ��������ʩ����������Ϣ
	//--------------------------------------------------------------------------------------------
	VOID* InitOneGuildUpgradeProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ������������Ϣ
	//--------------------------------------------------------------------------------------------
	VOID* InitOneGuildAffairProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ�����ɼ�����Ϣ
	//--------------------------------------------------------------------------------------------
	VOID* InitOneGuildSkillProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ������������Ϣ
	//--------------------------------------------------------------------------------------------
	VOID* InitOneGuildCommerceProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
	VOID* InitOneCommodityProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
	VOID* InitOneCofCProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
	VOID* InitOneCofCSPProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ�����Զ���
	//--------------------------------------------------------------------------------------------
	VOID* InitOneMotionProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

private:
	//-------------------------------------------------------------------------------------------
	// ���������������Ӱ�쾲̬����
	//-------------------------------------------------------------------------------------------
	VOID InitRoleAttLevelUp(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// ��������������Ե�Ĭ��ֵ����Сֵ�����ֵ
	//-------------------------------------------------------------------------------------------
	VOID InitAttDefMinMax(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// ����Ʒ��װ�����Բ���
	//-------------------------------------------------------------------------------------------
	VOID InitEquipQltyEffect(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// ����ʱװ���ɲ���
	//-------------------------------------------------------------------------------------------
	VOID InitFashionQltyEffect(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// ����ʱװ����ʱ��ɫ����
	//-------------------------------------------------------------------------------------------
	VOID InitFashionColorPct(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// ������װ����λ��һ����¼
	//-------------------------------------------------------------------------------------------
	VOID InitOnePosyPosProto(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// �ƺŵ�һ����¼
	//-------------------------------------------------------------------------------------------
	VOID InitOneTitleProto(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// �����䱦��һ����¼
	//-------------------------------------------------------------------------------------------
	VOID InitOneClanTreasureProto(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// ��ʼ���̳������Ʒ(����һ��)
	//-------------------------------------------------------------------------------------------
	VOID InitOneMallFreeItemProto(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// ��ʼ������ĳ��ְλȨ��
	//-------------------------------------------------------------------------------------------
	VOID InitOnePosGuildPower(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);
	VOID InitOnePosGuildKick(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);
	VOID InitOnePosGuildAppoint(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// ��ʼ��VIP̯λ
	//-------------------------------------------------------------------------------------------
	VOID InitOneVIPStallProto(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

private:
	//-------------------------------------------------------------------------------------------
	// ����Ӱ��ĳһ���ܣ�ĳһ״̬��ĳһ�����������м����б�
	//-------------------------------------------------------------------------------------------
	VOID LoadModifyMap();

	//-------------------------------------------------------------------------------------------
	// ����AI����
	//-------------------------------------------------------------------------------------------
	VOID GroupCreatureAI();

	//-------------------------------------------------------------------------------------------
	// ��ʼ������������������
	//-------------------------------------------------------------------------------------------
	VOID InitLongHunAss();
	VOID InitLongHunCanActivePos();

private:
	TSFPTrunk<AttRes>		m_Trunk;
	TObjRef<Util>			m_pUtil;
	TObjRef<VarContainer>	m_pVar;
	Fun_p					m_pFun;

	std::vector<tstring>				m_vectNameFilter;
	//std::vector<tstring>				m_vectChatFilter;

	tagVariableLen						m_VarLen;
	
	TMap<DWORD, tagItemProto*>			m_mapItemProto;
	TMap<DWORD, tagEquipProto*>			m_mapEquipProto;

	TMap<DWORD, tagGemProto*>			m_mapGemProto;
	TMap<DWORD, tagEquipQltyPct*>		m_mapEquipQltyPct;

	TMap<DWORD, tagLongHunProto*>		m_mapLongHunProto;
	std::vector<DWORD>					m_vectLongHun[X_LONGHUN_TYPE_NUM][X_LONGHUN_TYPEEX_NUM][X_EQUIP_QUALITY_NUM]; // ���������������ṹ

	TMap<DWORD, tagShopProto*>			m_mapShopProto;
	TMap<DWORD, tagDakProto*>			m_mapDakProto;

	TMap<DWORD, tagMallItemProto*>		m_mapMallItemProto;
	TMap<DWORD, tagMallPackProto*>		m_mapMallPackProto;
	tagMallFreeItem						m_MallFreeItemProto;

	TMap<DWORD, tagSuitProto*>			m_mapSuitProto;

    TMap<DWORD, tagSkillProto*>			m_mapSkillProto;
	TMap<DWORD, tagBuffProto*>			m_mapBuffProto;
	TMap<DWORD, tagTriggerProto*>		m_mapTriggerProto;

	TMap<DWORD, tagSkillModify*>		m_mapSkillModify;

	TMap<DWORD, tagCreatureProto*>		m_mapCreatureProto;
	TMap<DWORD, tagCreatureAI*>			m_mapCreatureAI;
	TMap<DWORD, TList<DWORD>*>			m_mapCreatureAIGroup;

	// Jason 2009-12-7 �����͸������ļ���Ϣ
	TMap<int,	tagConfigLessingOfLoong*>	m_mapConfigInfoOfLessingLoong;

	tagLevelUpEffect					m_LevelUpEffect[MAX_ROLE_LEVEL+1];		// �±�͵ȼ���Ӧ
	tagAttDefMinMax						m_AttDefMinMax[ERA_End];
	tagEquipQltyEffect					m_EquipQltyEffect[X_EQUIP_QUALITY_NUM];
	tagFashionGen						m_FashionGen[X_EQUIP_QUALITY_NUM];
	tagFashionColorPct					m_FashionColorPct[X_EQUIP_QUALITY_NUM];

	// Ĭ�ϰ��ɳ�ԱȨ�ޱ�
	tagGuildPower						m_GuildPowerDefault[X_GUILD_POS_NUM];
	tagGuildKick						m_GuildKickDefault[X_GUILD_POS_NUM];
	tagGuildAppoint						m_GuildAppointDefault[X_GUILD_POS_NUM];

	// ������ʩ����������Ʒ
	TMap<DWORD, tagGuildUpgradeNeed*>	m_GuildUpgradeNeedInfo;

	// ��������
	TMap<DWORD, tagGuildAffairInfo*>	m_GuildAffairInfo;

	// ���ɼ���
	TMap<DWORD, tagGuildSkill*>			m_GuildSkillInfo;

	// ��������
	TMap<DWORD, tagCommerceInfo*>		m_GuildCommerceInfo;
	TMap<DWORD, tagCommodityProto*>		m_GuildCommodityProto;
	TMap<INT64, tagCofCProto*>			m_CofCProto;
	TMap<DWORD, tagCofCSPProto*>		m_CofCSPProto;

	// ������װ����λ 
	tagConsolidatePos					m_PosyPos[EPosyAtt_End][MAX_CONSOLIDATE_POS_QUANTITY];
	TMap<DWORD, tagPosyProtoSer*>		m_mapPosyProto;			// ���ƾ�̬��
	// ���Կ�װ����λ
	tagConsolidatePos					m_EngravePos[EEngraveAtt_End][MAX_CONSOLIDATE_POS_QUANTITY];
	TMap<DWORD, tagEngraveProtoSer*>	m_mapEngraveProto;		// �Կ̾�̬��
	// ��Ƕ,��ӡ,������̬��
	TMap<DWORD, tagConsolidateItem*>	m_mapConsolidateProto;
	// �ϳ�
	TMap<DWORD, tagProduceProtoSer*>	m_mapProduceProto;
	// ���
	TMap<DWORD, tagQuenchProtoSer*>		m_mapQuenchProto;
	// �㻯,װ���ֽ�
	TMap<DWORD, tagDeComposeProtoSer*>	m_mapDeComposeProto;
	// �ƺ�
	tagTitleProto						m_TitleProto[MAX_TITLE_NUM];
	// �����䱦
	tagClanTreasureProto				m_ClanTreasureProto[CLAN_TREASURE_NUM];
	// �������ˢ�ֵ�
	TMap<DWORD, tagRandSpawnPointInfo*> m_mapSpawnPointProto;
	// ������̬����
	TMap<DWORD, tagInstance*>			m_mapInstanceProto;
	// �����в���ʹ�õ���Ʒ
	TMap<DWORD, tagInstanceItem*>		m_mapInstanceItem;
	// �����в���ʹ�õļ���
	TMap<DWORD, tagInstanceSkill*>		m_mapInstanceSkill;
	// �������ˢ�ֵ�ȼ�ӳ���
	TMap<INT, tagLevelMapping*>			m_mapLevelMapping;
	// �Ǹ���ˢ�ֵ�
	TMap<INT, tagSSpawnPointProto*>		m_mapSSpawnPoint;

	// ����ԭ��
	TMap<DWORD, tagPetProto*>			m_mapPetProto;

	// ��������ԭ��
	TMap<DWORD, tagPetLvlUpProto*>		m_mapPetLvlUpProto;

	// ��������ԭ��
	TMap<DWORD, tagPetLvlUpItemProto*>	m_mapPetLvlUpItemProto;

	// ���＼��ԭ��
	TMap<DWORD, tagPetSkillProto*>		m_mapPetSkillProto;
	
	// ����ȼ���Ӧ�̶�����id
	std::list<DWORD>					m_PetLevelSkillVec[NUM_PET_VLEVEL];

	// ����װ��ԭ��
	TMap<DWORD, tagPetEquipProto*>		m_mapPetEquipProto;

	// ����ɼ���
	TMap<DWORD, tagPetGatherProto*>		m_mapPetGatherProto;

	// �������������
	TMap<DWORD, tagPetWuXingProto*>		m_mapPetWuXingProto;

	TMap<DWORD, tagVNBGiftProto*>		m_mapVNBGiftProto;
	TMap<DWORD, tagVNBEquipProto*>		m_mapVNBEquipProto;

	// ��������ֵ
//	tagPetDefMinMax						m_nPetAttDefMinMax[EPA_NUM];

	// VIP̯λ���(�±���ID��Ӧ)
	INT32								m_nVIPStallRent[VIP_STALL_MAX_NUM];

	TMap<DWORD, tagMotionProto*>		m_mapMotionProto;
public:
	// GM�������
	TMap<DWORD, tagEquipProto*>			GetEquipMap(){return m_mapEquipProto;}	


	// Jason 2009-12-7 �����͸������ļ���Ϣ
	tagConfigLessingOfLoong* GetConfigInfoOfLessingLoong(const int & ndx);
private:
	VOID* AttRes::InitLessingLoong(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
	TMap<INT64, tagPhilosophersStoneConfig*>		m_mapPhilosophersStoneConfig;
	VOID* InitPhilosophersStoneConfig(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
public:
	tagPhilosophersStoneConfig* GetConfigInfoOfPhilosophersStone(INT nStoneType,DWORD srcItemTypeID);
};

extern AttRes g_attRes;
