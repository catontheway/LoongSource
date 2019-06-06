#include "StdAfx.h"
#include "CreatureData.h"
#include "..\WorldDefine\move_define.h"

enum ETagNode
{
	ETN_Start,
	ETN_Face = ETN_Start,	// �沿
	ETN_Hat,				// ͷ��
	ETN_Back,				// ����
	ETN_RSld,				// �Ҽ�
	ETN_LSld,				// ���
	ETN_RHd,				// ����
	ETN_LHd,				// ����
	ETN_Wep1, 				// ������
	ETN_Wep2,				// ������
	ETN_Wep3,				// ����1
	ETN_Wep4,				// ����2
	ETN_Wep5,				// ����
	ETN_Wep6,				// ������
	ETN_Num,
};
const tstring g_strTagNodeName[ETN_Num] = 
{
	_T("tag_Face"),       // �沿
	_T("tag_Hat"),        // ͷ��
	_T("tag_Back"),       // ����
	_T("tag_RSld"),       // �Ҽ�
	_T("tag_LSld"),       // ���
	_T("tag_RHd"),        // ����
	_T("tag_LHd"),        // ����
	_T("tag_Wep1"),       // ������
	_T("tag_Wep2"),       // ������
	_T("tag_Wep3"),       // ����1
	_T("tag_Wep4"),       // ����2
	_T("tag_Wep5"),       // ����
	_T("tag_Wep6"),       // ������
};

CreatureData::CreatureData(void)
{
}
CreatureData::~CreatureData(void)
{
}

CreatureData g_creature;
CreatureData* CreatureData::Inst()
{
	return &g_creature;
}

void CreatureData::LoadFromFile()
{
	tstring strPath = g_strLocalPath + _T("\\attdata\\creature_name.xml");
	const TCHAR* const szCreatureName = strPath.c_str();
	const TCHAR* const szCreature = _T("data\\system\\attdata\\creature_proto.xml");

	list<tstring> CreNameFieldList;
	list<tstring> CreFieldList;
	list<tstring> SoundFieldList;
	list<tstring>::iterator iter;

	
	TObjRef<VarContainer> VarCre = CreateObj("CreatureProto", "VarContainer");
	if(!VarCre->Load("VFS_System", szCreature, "id", &CreFieldList))
		IMSG(_T("Load file creature_proto.xml failed\r\n"));
	
	for(iter = CreFieldList.begin(); iter != CreFieldList.end(); ++iter)
	{
		tagCreatureProto att;
		att.dwTypeID = VarCre->GetDword(_T("id"), (*iter).c_str());
		//eType
		att.eType = (ECreatureType)VarCre->GetDword(_T("type"), (*iter).c_str());
		att.nType2 = (INT)VarCre->GetDword(_T("type2"), (*iter).c_str());
		att.nType3 = (INT)VarCre->GetDword(_T("type3"), (*iter).c_str());
		att.eLite = (ERank)VarCre->GetDword(_T("elite"), (*iter).c_str(),ER_Null);
		//family
		att.nFamily = (INT)VarCre->GetDword(_T("family"), (*iter).c_str(), 0);
		//vSize���������ֶ��Ѿ�������ֱ����ʹ��Ĭ��ֵ��size_y+ID�������ֶ�CRC���ظ���
		att.vSize.x = X_DEF_CREATURE_SIZE_X;
		att.vSize.y = X_DEF_CREATURE_SIZE_Y;
		att.vSize.z = X_DEF_CREATURE_SIZE_Z;

		att.eFunctionType = (EFunctionNPCType)VarCre->GetDword(_T("function_type"), (*iter).c_str(), EFNPCT_Null);
		att.uFunctionID.dwCommonID = VarCre->GetDword(_T("function_id"), (*iter).c_str(), GT_INVALID);
		att.eLite = (ERank)VarCre->GetDword(_T("elite"), (*iter).c_str(), ER_Null);
		att.bCanAttack = (BOOL)VarCre->GetDword(_T("can_attack"), (*iter).c_str(), FALSE);
        att.bCanBeAttack = (BOOL)VarCre->GetDword(_T("can_be_attack"), (*iter).c_str(), FALSE);
		
		INT nInspiration = VarCre->GetInt(_T("inspiration"), (*iter).c_str(), GT_INVALID);
		m_mapInspiration.insert(make_pair(att.dwTypeID, nInspiration));

		m_creatureMap.insert(make_pair(att.dwTypeID, att));
	}

	KillObj("CreatureProto");
	
	TObjRef<VarContainer> VarCreName = CreateObj("CreatureName", "VarContainer");
	if(!VarCreName->Load("VFS_System", szCreatureName, "id", &CreNameFieldList))
		IMSG(_T("Load file creature_name.xml failed\r\n"));

	for(iter = CreNameFieldList.begin(); iter != CreNameFieldList.end(); ++iter)
	{
		DWORD dwID = VarCreName->GetDword(_T("id"), (*iter).c_str());
		tagCreatureProto* pAtt = FindInMap(m_creatureMap, dwID);
		if(P_VALID(pAtt))
		{
			_tcscpy( pAtt->szName, VarCreName->GetString(_T("name"), (*iter).c_str(), _T("")) );
			_tcscpy ( pAtt->szModelName, VarCreName->GetString(_T("model"), (*iter).c_str(), _T("")) );
			_tcscpy( pAtt->szTitle, VarCreName->GetString(_T("title"), (*iter).c_str(), _T("")) );
			_tcscpy( pAtt->szTitleIcon, VarCreName->GetString(_T("title_icon"), (*iter).c_str(), _T("")) );
			_tcscpy( pAtt->szIdleSpeech, VarCreName->GetString(_T("idle_speech"), (*iter).c_str(), _T("")) );
			_tcscpy( pAtt->szCombatSpeech, VarCreName->GetString(_T("combat_speech"), (*iter).c_str(), _T("")) );
			_tcscpy( pAtt->szSkillSpeech, VarCreName->GetString(_T("skill_speech"), (*iter).c_str(), _T("")) );
			_tcscpy( pAtt->szDeathSpeech, VarCreName->GetString(_T("death_speech"), (*iter).c_str(), _T("")) );
			pAtt->eAnimationType = (EAniType)VarCreName->GetDword(_T("anitype"), (*iter).c_str(), 1);
			_tcscpy ( pAtt->szEffect, VarCreName->GetString(_T("effect"), (*iter).c_str(), _T("")) );
			_tcscpy( pAtt->szMonsterAI, VarCreName->GetString(_T("MonsterAI"), (*iter).c_str(), _T("")) );
		}
	}

	KillObj("CreatureName");


	TObjRef<Util> pUtil;

	list<tstring> MdlList;
	TObjRef<VarContainer> VarMdlData = CreateObj("CreatureMdlData", "VarContainer");
	tstring strMdlDataPath = g_strLocalPath + _T("\\attdata\\creature_mdl_data.xml");
	if(!VarMdlData->Load("VFS_System", strMdlDataPath.c_str(), "id", &MdlList))
		IMSG(_T("Load file creature_mdl_data.xml failed\r\n"));

	for(iter = MdlList.begin(); iter != MdlList.end(); ++iter)
	{
		// ��������
		for(int i = ETN_Start; i < ETN_Num; i++ )
		{
			tstring strBoneName = VarMdlData->GetString(g_strTagNodeName[i].c_str(),(*iter).c_str(),_T(""));
			if( !strBoneName.empty() )
			{
				DWORD dwKey = MakeNpcBoneNameKey( (*iter).c_str(), g_strTagNodeName[i].c_str() );
				m_npcBoneNameMap.insert(make_pair(dwKey, strBoneName));
			}
		}

		// ��������
		tagCreatureMdlData data;
		data.fBeAttackSfxScale			= VarMdlData->GetFloat( _T("txm_zoom"), (*iter).c_str(), 1.0f );
		data.bCanPitch					= VarMdlData->GetInt( _T("can_pitch"), (*iter).c_str(), FALSE ) == TRUE;
		data.szDeadSound				= VarMdlData->GetString( _T("sd_dead"), (*iter).c_str(), _T("") );
		data.szCombatSound				= VarMdlData->GetString( _T("sd_inter_combat"), (*iter).c_str(), _T("")) ;
		data.szRoarSound				= VarMdlData->GetString( _T("sd_roar"), (*iter).c_str(), _T("") );
		data.szHowlSound				= VarMdlData->GetString( _T("sd_howl"), (*iter).c_str(), _T("") );
		data.szFootSound				= VarMdlData->GetString( _T("sd_foot"), (*iter).c_str(), _T("") );
		data.szNormalAttackBeAtkSfx		= VarMdlData->GetString( _T("tx_natk"), (*iter).c_str(), _T("") );
		 
		const DWORD dwID = pUtil->Crc32( (*iter).c_str() );
		m_mdlData.insert( make_pair( dwID, data ) );
	}
	KillObj("CreatureMdlData");
}

tstring CreatureData::GetNpcMdlPath(const tagCreatureProto *pNpcAtt)
{
	tstringstream stream;

	stream << _T("data\\animation\\npc\\") << pNpcAtt->szModelName << _T("\\");
	switch( pNpcAtt->eAnimationType )
	{
	case EAT_None:
		stream << pNpcAtt->szModelName;
		stream << _T(".fsm");
		break;

	case EAT_KeyFrame:
		stream << pNpcAtt->szModelName;
		stream << _T(".fak");
		break;

	case EAT_Skeleton:
		stream << _T("std.fskel");
		break;
	}

	return stream.str();
}

tstring CreatureData::GetNpcSkinPath( const tagCreatureProto *pNpcAtt, int index )
{
	tstringstream stream;

	if( pNpcAtt->eAnimationType == EAT_Skeleton )
	{
		stream << _T("data\\animation\\npc\\") << pNpcAtt->szModelName << _T("\\") << pNpcAtt->szModelName
			<< _T("_obj") << index << _T(".fskin");
	}

	return stream.str();
}

DWORD CreatureData::MakeNpcBoneNameKey( const TCHAR* szMdlName, const TCHAR* szTagNodeName )
{
	ASSERT(NULL!=szMdlName);
	ASSERT(NULL!=szTagNodeName);
	if( _T('\0') == szMdlName[0] || _T('\0') == szTagNodeName[0] )
		return GT_INVALID;

	tstring strKey = szMdlName;
	strKey += _T(' ');
	strKey += szTagNodeName;
	return TObjRef<Util>()->Crc32(strKey.c_str());
}

const tstring CreatureData::FindNpcBoneName( const DWORD dwTypeID, const TCHAR* szTagNodeName )
{
	tagCreatureProto* pProto = FindInMap( m_creatureMap, dwTypeID );
	if( NULL == pProto )
		return _T("");
	return FindNpcBoneName( pProto->szModelName, szTagNodeName );
}

const tstring CreatureData::FindNpcBoneName( const TCHAR* szMdlName, const TCHAR* szTagNodeName )
{
	const DWORD dwKey = MakeNpcBoneNameKey( szMdlName, szTagNodeName );
	if( GT_INVALID == dwKey )
		return _T("");

	map<DWORD,tstring>::iterator itr = m_npcBoneNameMap.find( dwKey );
	if( itr == m_npcBoneNameMap.end() )
		return _T("");

	return itr->second;
}

const tagCreatureMdlData* CreatureData::FindMdlData(const TCHAR* szMdlName)
{
	ASSERT( NULL != szMdlName && szMdlName[0] != _T('\0') );
	const DWORD dwID = TObjRef<Util>()->Crc32(szMdlName);
	map<DWORD,tagCreatureMdlData>::iterator itr = m_mdlData.find( dwID );
	if( itr != m_mdlData.end() )
		return &itr->second;

	return NULL;
}

INT CreatureData::FindCreatureInspiration( DWORD dwTypeID )
{
	map<DWORD,INT>::iterator it = m_mapInspiration.find(dwTypeID);
	if( it!=m_mapInspiration.end() )
		return it->second;
	
	return 0;
}