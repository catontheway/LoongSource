//------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: quest_mgr.h
// author: lh
// actor:
// data: 2008-7-24
// last:
// brief: ���������
// remark: ���������������������ļ��غ͹������
//------------------------------------------------------------------------------------------------------
#include "stdafx.h"

#include "../WorldDefine/QuestDef.h"

#include "quest_mgr.h"
#include "script_mgr.h"
#include "unit.h"
#include "creature.h"

#include "../ServerDefine/class_define.h"

//-------------------------------------------------------------------------------------------------------
// ��ʼ��
//-------------------------------------------------------------------------------------------------------
BOOL QuestMgr::Init()
{
	CreateObj("QuestContainer", "VarContainer");
	m_pVar = "QuestContainer";

	// �������ļ�world.ini�У����������ļ�·��
	TObjRef<VarContainer> pVar = "VarContainer";

	m_strQuestPath					=	pVar->GetString(_T("path"), _T("quest"));
	m_strQuestFileList				=	pVar->GetString(_T("list"), _T("quest"));
	m_strAcceptQuestNPCRelation		=	pVar->GetString(_T("acceptQuest_NPC_relation"), _T("quest"));
	m_strCompleteQuestNPCRelation	=	pVar->GetString(_T("completeQuest_NPC_relation"), _T("quest"));

	// ��������ļ�·��Ϊ�գ��򷵻�FALSE
	if( m_strQuestPath.empty() || FALSE == LoadQuest(m_strQuestFileList.c_str()) )
	{
		return FALSE;
	}
	
	// ��ȡ�������ȡNPC��ϵ�ļ�
	if( !m_strAcceptQuestNPCRelation.empty() ) 
	{
		LoadAcceptQuestNPCRelation(m_strAcceptQuestNPCRelation.c_str());
	}
	// ��ȡ���������NPC��ϵ�ļ�
	if( !m_strCompleteQuestNPCRelation.empty() )
	{
		LoadCompleteQuestNPCRelation(m_strCompleteQuestNPCRelation.c_str());
	}

	return TRUE;
}

//------------------------------------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------------------------------------
VOID QuestMgr::Destroy()
{
	// ɾ�����е���������
	tagQuestProto* pProto = NULL;

	QuestMap::TMapIterator it = m_mapAllQuests.Begin();
	while( m_mapAllQuests.PeekNext(it, pProto) )
	{
		SAFE_DEL(pProto);
	}
	m_mapAllQuests.Clear();

	// ɾ�����е�NPC�����������
	TList<UINT16>* pList = NULL;
	QuestRelationMap::TMapIterator it2 = m_mapAcceptQuestNPCRelation.Begin();
	while( m_mapAcceptQuestNPCRelation.PeekNext(it2, pList) )
	{
		SAFE_DEL(pList);
	}
	m_mapAcceptQuestNPCRelation.Clear();

	it2 = m_mapCompleteQuestNPCRelation.Begin();
	while( m_mapCompleteQuestNPCRelation.PeekNext(it2, pList) )
	{
		SAFE_DEL(pList);
	}
	m_mapCompleteQuestNPCRelation.Clear();

	
	TList<DWORD>* pList2 = NULL;
	NPCRelationMap::TMapIterator it3 = m_mapAcceptNPCRelation.Begin();
	while( m_mapAcceptNPCRelation.PeekNext(it3, pList2) )
	{
		SAFE_DEL(pList2);
	}
	m_mapAcceptNPCRelation.Clear();

	it3 = m_mapCompleteNPCRelation.Begin();
	while( m_mapCompleteNPCRelation.PeekNext(it3, pList2) )
	{
		SAFE_DEL(pList2);
	}
	m_mapCompleteNPCRelation.Clear();

}

//---------------------------------------------------------------------------------------
// ������������
//---------------------------------------------------------------------------------------
BOOL QuestMgr::LoadQuest(LPCTSTR szPath)
{
	list<tstring> listField;
	m_pVar->Load(NULL, szPath, "name", &listField);

	// ���������ļ����ϣ����������ļ�
	list<tstring>::iterator it = listField.begin();
	for(; it != listField.end(); ++it)
	{
		tagQuestProto* pQuest = new tagQuestProto;
		ZeroMemory(pQuest, sizeof(tagQuestProto));

		tstring name = m_strQuestPath;
		name += _T("\\") + (*it) + _T(".xml");
		DWORD dwErrorCode = LoadQuestFromXml(pQuest, name.c_str());
		if( 1 == dwErrorCode )
		{
			IMSG(_T("Load Quest From Xml Fail. name: %s\r\n"), name.c_str());
			SAFE_DEL(pQuest);
		}
		else if( 2 == dwErrorCode )
		{
			SAFE_DEL(pQuest);
			continue;
		}
		else 
		{
			m_mapAllQuests.Add(pQuest->id, pQuest);
		}
	}

	return TRUE;
}

//--------------------------------------------------------------------------------
// ��XML�ļ��ж�ȡ��������
//--------------------------------------------------------------------------------
DWORD QuestMgr::LoadQuestFromXml(tagQuestProto* pQuest, LPCTSTR szFileName)
{
	if ( !P_VALID(pQuest) || !P_VALID(szFileName) ) return 1;

	// ����XML
	XmlDocument doc;
	if( false == doc.LoadFile(NULL, szFileName) ) return 1;

	XmlHandle hDoc(&doc);
	XmlHandle hRoot(0);

	// �Ƿ��������ļ�
	XmlElement *pElem = hDoc.FirstChildElement("quest").Element();
	if( !P_VALID(pElem) ) return 2;

	hRoot = XmlHandle( pElem );

	stringstream ss;
	string str;
	XmlNodeMap xmlNode;

	XmlElement* pPropertyNode			=	hRoot.FirstChild("property").FirstChild().Element();
	XmlElement* pAcceptConditionsNode	=	hRoot.FirstChild("accept-conditions").FirstChild().Element();
	XmlElement* pCompleteConditionsNode	=	hRoot.FirstChild("complete-conditions").FirstChild().Element();
	XmlElement* pRewNode				=	hRoot.FirstChild( "reward" ).FirstChild().Element();

	LoadXmlNode(pPropertyNode, xmlNode);
	LoadXmlNode(pAcceptConditionsNode, xmlNode);
	LoadXmlNode(pCompleteConditionsNode, xmlNode);
	LoadXmlNode(pRewNode, xmlNode);

	SetQuestProto(pQuest, xmlNode);
	return 0;	
}

//--------------------------------------------------------------------------------------------------------
// ��ȡXML�ڵ�
//--------------------------------------------------------------------------------------------------------
BOOL QuestMgr::LoadXmlNode(XmlElement* pNode, XmlNodeMap& xmlNodeMap)
{
	// �Ƿ���ڸýڵ�
	if ( !P_VALID(pNode) ) return FALSE;

	const char* pChar = NULL;
	XmlAttribute* pAttrib = NULL;
	while(pNode)
	{
		if(P_VALID(pNode))
		{
			pAttrib =  pNode->FirstAttribute();

			string firstName;
			if( P_VALID(pAttrib) )
			{
				firstName = pAttrib->Name();
			}

			while(pAttrib)
			{
				if( pAttrib->Name() != NULL && pAttrib->Value() != NULL )
				{
					if( NULL == pAttrib->Previous() )
					{
						xmlNodeMap.insert(make_pair(firstName, string(pAttrib->Value())));
					}
					else
					{
						string tmp = firstName;
						tmp += "-";
						tmp += pAttrib->Name();
						xmlNodeMap.insert( make_pair(tmp, pAttrib->Value()));
					}
				}
				pAttrib = pAttrib->Next();
			}
			pNode =  pNode->NextSiblingElement();
		}
	}
	return TRUE;
}

//--------------------------------------------------------------------------------------------------
// ����XML�ڵ�������������
//--------------------------------------------------------------------------------------------------
BOOL QuestMgr::SetQuestProto(tagQuestProto* pQuestProto, XmlNodeMap& xmlNodeMap)
{
	// complete-conditions
	pQuestProto->quest_flags = 0;

	// property
	SetQuestUINT16("ID", pQuestProto->id, xmlNodeMap);
	SetQuestUINT16("PrevQuestID", pQuestProto->prev_quest_id, xmlNodeMap);
	SetQuestUINT16("PrevQuest2ID", pQuestProto->prev_quest2_id, xmlNodeMap );
	SetQuestINT32("PrevQuestRelation", pQuestProto->prev_quest_relation, xmlNodeMap);
	SetQuestUINT16("NextQuestID", pQuestProto->next_quest_id, xmlNodeMap);
	SetQuestUINT16("UncmpleteQuestID", pQuestProto->uncomplete_quest_id, xmlNodeMap);
	SetQuestBool("AutoAddQuest", pQuestProto->auto_add_quest, xmlNodeMap);
	SetQuestBool("ShowQuestDialog", pQuestProto->show_quest_dialog, xmlNodeMap );
	SetQuestBool("NoDelete", pQuestProto->no_delete, xmlNodeMap );

	pQuestProto->accept_flags = 0;
	SetQuestDwordFlags("QuestPlayerAccept", pQuestProto->accept_flags, Quest_Accept_Flag_Player, xmlNodeMap);
	SetQuestDwordFlags("QuestSystemRelease", pQuestProto->accept_flags, Quest_Accept_Flag_System, xmlNodeMap);
	SetQuestDwordFlags("QuestItemAccept", pQuestProto->accept_flags, Quest_Accept_Flag_Item, xmlNodeMap);

	SetQuestDword("QuestType", (DWORD&)pQuestProto->type, xmlNodeMap);
	SetQuestDword("TargetMode", (DWORD&)pQuestProto->target_mode,xmlNodeMap);
	SetQuestBYTE("SuggestedPlayers", pQuestProto->suggested_players, xmlNodeMap);
	SetQuestDword("LimitTime", pQuestProto->limit_time, xmlNodeMap);
	SetQuestBYTE("DoQuestTimes", pQuestProto->accept_times, xmlNodeMap);
	SetQuestBool("Periodic", pQuestProto->period, xmlNodeMap);
	SetQuestDword("QuestPeriodic", (DWORD&)pQuestProto->period_type, xmlNodeMap);
	SetQuestDword("Week", (DWORD&)pQuestProto->week, xmlNodeMap);
	SetQuestBool("Repeatable", pQuestProto->repeatable, xmlNodeMap);

	for(INT i = 0; i < QUEST_ACCEPT_NPC; ++i)
	{
		stringstream ss;
		string str;
		ss << "AcceptQuestNPCID" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->accept_quest_npc[i], xmlNodeMap);
	}

	for(INT i = 0; i < QUEST_COMPLETE_NPC; ++i)
	{
		stringstream ss;
		string str;
		ss << "CompleteQuestNPCID" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->complete_quest_npc[i], xmlNodeMap);
	}

	for( int i = 0; i < QUEST_ITEMS_COUNT ; ++i )
	{
		stringstream ss;
		string str;
		ss << "InvestigateObjectID" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->investigate_objects[i], xmlNodeMap );

		if( pQuestProto->investigate_objects[i] > 0 )
		{
			pQuestProto->quest_flags = pQuestProto->quest_flags | Quest_Flag_Invest;
		}
	}

	SetQuestDword("SrcItemID", pQuestProto->src_item, xmlNodeMap);
	SetQuestINT16("SrcItemID-value", pQuestProto->src_item_num, xmlNodeMap);
	SetQuestFloat("Destination_X", pQuestProto->destination_x, xmlNodeMap);
	SetQuestFloat("Destination_Y", pQuestProto->destination_y, xmlNodeMap);
	SetQuestFloat("Destination_Z", pQuestProto->destination_z, xmlNodeMap);

	//accept-conditions
	SetQuestINT32("GetQuestRequiredMaxLevel", pQuestProto->accept_req_max_level, xmlNodeMap);
	SetQuestINT32("GetQuestRequiredMinLevel", pQuestProto->accept_req_min_level, xmlNodeMap);

	for(INT i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
	{
		stringstream ss;
		string str;
		ss << "GetQuestRequiredRep" << i + 1 << endl;
		ss >> str;	
		SetQuestINT32(str, pQuestProto->accept_req_rep[i], xmlNodeMap);
		string tmp = str;
		tmp += "-max";
		SetQuestINT32(tmp, pQuestProto->accept_req_max_rep[i], xmlNodeMap);
		tmp = str;
		tmp += "-min";
		SetQuestINT32(tmp, pQuestProto->accept_req_min_rep[i], xmlNodeMap);
	}
	for(INT i = 0; i < QUEST_SKILLS_COUNT; ++i)
	{
		stringstream ss;
		string str;
		ss << "GetQuestRequiredSkill" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->accept_req_skill[i], xmlNodeMap);
		string tmp = str;
		tmp += "-value";
		SetQuestINT32(tmp, pQuestProto->accept_req_skill_val[i], xmlNodeMap);
	}

	SetQuestBYTE("GetQuestRequiredSex", pQuestProto->sex, xmlNodeMap);

	DWORD dwTmpClassLimit = 0;
	SetQuestDword("GetQuestRequiredClass", (DWORD&)dwTmpClassLimit, xmlNodeMap);
	pQuestProto->class_type = ClassTable[dwTmpClassLimit];
	//SetQuestDword("GetQuestRequiredClass", (DWORD&)pQuestProto->class_type, xmlNodeMap);
	
	SetQuestDword("GetQuestRequiredMapID", pQuestProto->accept_req_map, xmlNodeMap);

	for(INT i = 0; i < QUEST_ITEMS_COUNT; ++i)
	{
		stringstream ss;
		string str;
		ss << "GetQuestRequiredItemID" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->accept_req_item[i], xmlNodeMap);
		string tmp = str;
		tmp += "-value";
		SetQuestINT16(tmp, pQuestProto->accept_req_item_num[i], xmlNodeMap);
	}
	for(INT i = 0; i < QUEST_ATTS_COUNT; ++i)
	{
		stringstream ss;
		string str;
		ss << "GetQuestRequiredAtt" << i + 1 << endl;
		ss >> str;	
		SetQuestINT32(str, pQuestProto->accept_req_att[i], xmlNodeMap);
		string tmp = str;
		tmp += "-value";
		SetQuestINT16(tmp, pQuestProto->accept_req_att_val[i], xmlNodeMap);
	}

	SetQuestBool("DelRestrictedItem", pQuestProto->del_req_item, xmlNodeMap);

	for(INT i = 0; i < QUEST_TRIGGERS_COUNT; ++i)
	{
		stringstream ss;
		string str;
		ss << "GetQuestReqTriggersID" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->accept_req_trriger[i], xmlNodeMap);
	}

	SetQuestBool("GetQuestReqMarried", pQuestProto->married, xmlNodeMap);
	SetQuestBYTE("GetQuestReqLoverNum", pQuestProto->lover_num, xmlNodeMap);
	SetQuestINT32("GetQuestReqMoneyNum", pQuestProto->accept_req_money, xmlNodeMap );
	SetQuestBool("GetQuestDelMoneyNum", pQuestProto->accept_del_money, xmlNodeMap);

	for(INT i = 0; i < QUEST_ITEMS_COUNT; ++i)
	{
		stringstream ss;
		string str;
		ss << "CompleteQuestRequiredItemID" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->complete_req_item[i], xmlNodeMap);
		string tmp = str;
		tmp += "-value";
		SetQuestINT16(tmp, pQuestProto->complete_req_item_num[i], xmlNodeMap);
		if( pQuestProto->complete_req_item_num[i] > 0 )
		{
			pQuestProto->quest_flags = pQuestProto->quest_flags | Quest_Flag_ITEM;
		}
	}
	SetQuestBool("RequiredOnlyOneItem", pQuestProto->only_one_item, xmlNodeMap);
	SetQuestBool("DeleteItem", pQuestProto->delete_item, xmlNodeMap);

	for(INT i = 0; i < QUEST_CREATURES_COUNT; ++i)
	{
		stringstream ss;
		string str;
		ss << "CompleteQuestRequiredCreatureID" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->complete_req_creature[i*3], xmlNodeMap);
		string tmp = str;
		tmp += "-id2";
		SetQuestDword(tmp, pQuestProto->complete_req_creature[i*3+1], xmlNodeMap);
		tmp = str;
		tmp += "-id3";
		SetQuestDword(tmp, pQuestProto->complete_req_creature[i*3+2], xmlNodeMap);
		tmp = str;
		tmp += "-value";
		SetQuestINT16(tmp, pQuestProto->complete_req_creature_num[i], xmlNodeMap);
		if( pQuestProto->complete_req_creature_num[i] > 0 )
		{
			pQuestProto->quest_flags = pQuestProto->quest_flags | Quest_Flag_KILL;
		}
	}

	SetQuestBool("RequiredCreatureLevel", pQuestProto->creature_level, xmlNodeMap);
	SetQuestBool("RequiredOnlyOneCreature", pQuestProto->only_one_creature, xmlNodeMap);

	for(INT i = 0; i < QUEST_NPC_COUNT; ++i)
	{
		stringstream ss;
		string str;
		ss << "CompleteQuestRequiredNPC" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->complete_req_npc[i], xmlNodeMap);
		if( pQuestProto->complete_req_npc[i] > 0 )
		{
			pQuestProto->quest_flags = pQuestProto->quest_flags | Quest_Flag_NPC_TALK;
		}
	}

	SetQuestBool("RequiredOnlyInOrder", pQuestProto->only_in_order, xmlNodeMap);
	SetQuestBool("RequiredOnlyOneNPC", pQuestProto->only_one_npc, xmlNodeMap);

	for(INT i = 0; i < QUEST_TRIGGERS_COUNT; ++i)
	{
		stringstream ss;
		string str;
		ss << "CompleteQuestReqTriggersID" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->complete_req_trigger[i], xmlNodeMap );
		if( pQuestProto->complete_req_trigger[i] > 0 )
		{
			pQuestProto->quest_flags = pQuestProto->quest_flags | Quest_Flag_Trigger;
		}
	}
	SetQuestBool("RequiredOnlyOneTrigger", pQuestProto->only_one_trigger, xmlNodeMap);

	SetQuestINT32("CompleteQuestRequiredMoney", pQuestProto->complete_req_money, xmlNodeMap);
	SetQuestBool("CompleteQuestDelMoney", pQuestProto->complete_del_money, xmlNodeMap);
	SetQuestDword("CompleteQuestRequiredMapID", pQuestProto->complete_req_map, xmlNodeMap);
	SetQuestFloat("CompleteQuestRequiredMap_X", pQuestProto->complete_req_map_x, xmlNodeMap);
	SetQuestFloat("CompleteQuestRequiredMap_Y", pQuestProto->complete_req_map_y, xmlNodeMap);
	SetQuestFloat("CompleteQuestRequiredMap_Z", pQuestProto->complete_req_map_z, xmlNodeMap);
	SetQuestFloat("CompleteQuestRequiredMap_Radius", pQuestProto->complete_req_map_radius, xmlNodeMap);

	SetQuestBool("CompleteQuestRequiredMap_Random", pQuestProto->coordinate_random, xmlNodeMap);
	SetQuestBool("RequiredOnlyOneCondition", pQuestProto->only_one_condition, xmlNodeMap);

	SetQuestBool("CompleteQuestReqMarried", pQuestProto->complete_req_married, xmlNodeMap);
	SetQuestBYTE("CompleteQuestReqLoverNum", pQuestProto->complete_req_lover_num, xmlNodeMap);
	SetQuestINT32("CompleteQuestReqLevel", pQuestProto->complete_req_level, xmlNodeMap);

	SetQuestDword("CompleteQuestEventType", (DWORD&)pQuestProto->event_type, xmlNodeMap);
	SetQuestDword("UseItemEvent_Item_ID", pQuestProto->use_item, xmlNodeMap);
	SetQuestDword("UseSkillEvent_Skill_ID", pQuestProto->use_skill, xmlNodeMap);

	// Reword
	SetQuestINT32("RewMoney", pQuestProto->rew_money, xmlNodeMap);
	SetQuestINT32("RewXP", pQuestProto->rew_xp, xmlNodeMap);

	for(INT i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
	{
		stringstream ss;
		string str;
		ss << "RewRequiredRep" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->rew_rep[i], xmlNodeMap);
		string tmp = str;
		tmp += "-value";
		SetQuestINT32(tmp, pQuestProto->rew_rep_val[i], xmlNodeMap);
	}	

	//����
	for( int i = 0; i < QUEST_CONTRIBUTION_COUNT; ++i )
	{
		stringstream ss;
		string str;
		ss << "RewRequiredContribution" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->rew_contribution[i], xmlNodeMap );
		string tmp = str;
		tmp += "-value";
		SetQuestINT32(tmp, pQuestProto->rew_contribution_val[i], xmlNodeMap );
	}

	for(INT i = 0; i < QUEST_REW_ITEM; ++i)
	{
		stringstream ss;
		string str;
		ss << "RewItemID" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->rew_item[i], xmlNodeMap);
		string tmp = str;
		tmp += "-value";
		SetQuestINT16(tmp, pQuestProto->rew_item_num[i], xmlNodeMap);
		tmp = str;
		tmp += "-quality";
		SetQuestINT16(tmp, pQuestProto->rew_item_quality[i], xmlNodeMap);
	}

	for(INT i = 0; i < QUEST_REW_ITEM; ++i)
	{
		stringstream ss;
		string str;
		ss << "RewChoicesItemID" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->rew_choice_item[i], xmlNodeMap);
		string tmp = str;
		tmp += "-value";
		SetQuestINT16(tmp, pQuestProto->rew_choice_item_num[i], xmlNodeMap);
		tmp = str;
		tmp += "-quality";
		SetQuestINT16(tmp, pQuestProto->rew_choice_quality[i], xmlNodeMap);
	}

	for(INT i = 0; i < QUEST_ATTS_COUNT; ++i)
	{
		stringstream ss;
		string str;
		ss << "RewAtt" << i + 1 << endl;
		ss >> str;	
		SetQuestINT32(str, pQuestProto->rew_att[i], xmlNodeMap);
		string tmp = str;
		tmp += "-value";
		SetQuestINT16(tmp, pQuestProto->rew_att_val[i], xmlNodeMap);
	}

	for(INT i = 0; i < QUEST_SKILLS_COUNT; ++i)
	{
		stringstream ss;
		string str;
		ss << "RewSkill" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->rew_skill[i], xmlNodeMap);
		string tmp = str;
		tmp += "-value";
		SetQuestINT32(tmp, pQuestProto->rew_skill_val[i], xmlNodeMap);
	}

	for(INT i = 0; i < QUEST_REW_BUFF; ++i)
	{
		stringstream ss;
		string str;
		ss << "RewBuff" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->rew_buff[i], xmlNodeMap);
	}
	return TRUE;
}

//----------------------------------------------------------------------------
// string ----> DWORD
//----------------------------------------------------------------------------
VOID QuestMgr::SetQuestDword(std::string str, DWORD& dwQuest, XmlNodeMap& xmlNodeMap)
{
	XmlNodeMap::iterator pos = xmlNodeMap.find(str);
	if( pos != xmlNodeMap.end() )
	{
		dwQuest = atol(pos->second.c_str());
		xmlNodeMap.erase(pos);
	}
}

//----------------------------------------------------------------------------
// string ----> DWORD FLAGs
//----------------------------------------------------------------------------
VOID QuestMgr::SetQuestDwordFlags(std::string str, DWORD& dwQuest, INT32 nMask, XmlNodeMap& xmlNodeMap)
{
	XmlNodeMap::iterator pos = xmlNodeMap.find(str);
	if( pos != xmlNodeMap.end() )
	{
		DWORD n = atol(pos->second.c_str());
		if( n != 0 )
			dwQuest = dwQuest | nMask;
		xmlNodeMap.erase(pos);
	}
}

//----------------------------------------------------------------------------
// string ----> UINT16
//----------------------------------------------------------------------------
VOID QuestMgr::SetQuestUINT16(std::string str, UINT16& unQuest, XmlNodeMap& xmlNodeMap)
{
	XmlNodeMap::iterator pos = xmlNodeMap.find(str);
	if( pos != xmlNodeMap.end() )
	{
		unQuest = (UINT16)atoi(pos->second.c_str());
		xmlNodeMap.erase(pos);
	}
}

//----------------------------------------------------------------------------
// string ----> BYTE
//----------------------------------------------------------------------------
VOID QuestMgr::SetQuestBYTE(std::string str, BYTE& byQuest, XmlNodeMap& xmlNodeMap)
{
	XmlNodeMap::iterator pos = xmlNodeMap.find(str);
	if ( pos != xmlNodeMap.end() )
	{
		byQuest = (BYTE)atoi(pos->second.c_str());
		xmlNodeMap.erase(pos);
	}
}

//----------------------------------------------------------------------------
// string ----> INT16
//----------------------------------------------------------------------------
VOID QuestMgr::SetQuestINT16(std::string str, INT16& n16Quest, XmlNodeMap& xmlNodeMap)
{
	XmlNodeMap::iterator pos = xmlNodeMap.find(str);
	if( pos != xmlNodeMap.end() )
	{
		n16Quest = (INT16)atoi(pos->second.c_str());
		xmlNodeMap.erase(pos);
	}
}

//----------------------------------------------------------------------------
// string ----> INT32
//----------------------------------------------------------------------------
VOID QuestMgr::SetQuestINT32(std::string str, INT32& nQuest, XmlNodeMap& xmlNodeMap)
{
	XmlNodeMap::iterator pos = xmlNodeMap.find(str);
	if( pos != xmlNodeMap.end() )
	{
		nQuest = atoi(pos->second.c_str());
		xmlNodeMap.erase(pos);
	}
}

//----------------------------------------------------------------------------
// string ----> BOOL
//----------------------------------------------------------------------------
VOID QuestMgr::SetQuestBool(std::string str, BOOL& bQuest, XmlNodeMap& xmlNodeMap)
{
	XmlNodeMap::iterator pos = xmlNodeMap.find(str);
	if ( pos != xmlNodeMap.end() )
	{
		bQuest = atol(pos->second.c_str());
		xmlNodeMap.erase(pos);
	}
}

// ----------------------------------------------------------------------------
// string ----> Float
// ----------------------------------------------------------------------------
void  QuestMgr::SetQuestFloat(std::string str, FLOAT& fQuest, XmlNodeMap& xmlNodeMap)
{
	XmlNodeMap::iterator pos = xmlNodeMap.find(str);
	if ( pos != xmlNodeMap.end() )
	{
		fQuest = (FLOAT)atof(pos->second.c_str());
		xmlNodeMap.erase(pos);
	}
}

//-------------------------------------------------------------------------------
// ��ȡ�����ȡNPC�б�
//-------------------------------------------------------------------------------
BOOL QuestMgr::LoadAcceptQuestNPCRelation(LPCTSTR szFileName)
{
	// ���map
	m_mapAcceptQuestNPCRelation.Clear();

	// ����xml�ļ�
	if( !P_VALID(szFileName) )
	{
		return FALSE;
	}

	XmlDocument doc;
	if( false == doc.LoadFile(NULL, szFileName) ) 
		return FALSE;

	XmlHandle hDoc(&doc);

	// �Ƿ��������ϵ�ļ�
	XmlHandle eleHandle = hDoc.FirstChildElement("accept-quest-relation").FirstChildElement();
	XmlElement* pElem = eleHandle.Element();
	if ( !P_VALID(pElem) ) return FALSE;

	// ���ζ�ȡ���Զ�
	while( P_VALID(pElem) )
	{
		const char* pNPCID		=	pElem->Attribute("NPCID");
		const char* pQuestID	=	pElem->Attribute("QuestID");

		if( P_VALID(pNPCID) && P_VALID(pQuestID) )
		{
			DWORD dwNPCID = atol(pNPCID);
			UINT16 u16QuestID = (UINT16)atol(pQuestID);

			TList<UINT16>* pList = m_mapAcceptQuestNPCRelation.Peek(dwNPCID);
			if( !P_VALID(pList) )
			{
				pList = new TList<UINT16>;
				m_mapAcceptQuestNPCRelation.Add(dwNPCID, pList);
			}

			pList->PushBack(u16QuestID);

			TList<DWORD>* pList2 = m_mapAcceptNPCRelation.Peek(u16QuestID);
			if( !P_VALID(pList2) )
			{
				pList2 = new TList<DWORD>;
				m_mapAcceptNPCRelation.Add(u16QuestID, pList2);
			}
			pList2->PushBack(dwNPCID);
		}

		pElem = pElem->NextSiblingElement();	
	}

	return TRUE;
}

//-----------------------------------------------------------------------------------------------
// ��ȡ�������NPC�б�
//-----------------------------------------------------------------------------------------------
BOOL QuestMgr::LoadCompleteQuestNPCRelation(LPCTSTR szFileName)
{
	// ���map
	m_mapCompleteQuestNPCRelation.Clear();

	// ����xml�ļ�
	if( !P_VALID(szFileName) )
	{
		return FALSE;
	}

	XmlDocument doc;
	if( false == doc.LoadFile(NULL,szFileName) ) 
		return FALSE;

	XmlHandle hDoc( &doc );

	// �Ƿ��������ϵ�ļ�
	XmlHandle eleHandle = hDoc.FirstChildElement("complete-quest-relation").FirstChildElement();
	XmlElement *pElem = eleHandle.Element();
	if( !pElem ) return FALSE;

	while( P_VALID(pElem) )
	{
		const char* pNPCID		=	pElem->Attribute("NPCID");
		const char* pQuestID	=	pElem->Attribute("QuestID");

		if( P_VALID(pNPCID) && P_VALID(pQuestID) )
		{
			DWORD dwNPCID = atol(pNPCID);
			UINT16 u16QuestID = (UINT16)atol(pQuestID);

			TList<UINT16>* pList = m_mapCompleteQuestNPCRelation.Peek(dwNPCID);
			if( !P_VALID(pList) )
			{
				pList = new TList<UINT16>;
				m_mapCompleteQuestNPCRelation.Add(dwNPCID, pList);
			}
			pList->PushBack(u16QuestID);

			TList<DWORD>* pList2 = m_mapCompleteNPCRelation.Peek(u16QuestID);
			if( !P_VALID(pList2) )
			{
				pList2 = new TList<DWORD>;
				m_mapCompleteNPCRelation.Add(u16QuestID, pList2);
			}
			pList2->PushBack(dwNPCID);
		}

		pElem = pElem->NextSiblingElement();

	}
	return TRUE;
}

//--------------------------------------------------------------------------------
// ĳ��NPC�Ƿ���Խ�ȡ������
//--------------------------------------------------------------------------------
BOOL QuestMgr::IsNPCAcceptQuest(Creature* pCreature, UINT16 u16QuestID)
{
	if( FALSE == P_VALID(pCreature) ) return FALSE;

	TList<UINT16>* pList = m_mapAcceptQuestNPCRelation.Peek(pCreature->GetTypeID());

	if( !P_VALID(pList) || FALSE == pList->IsExist(u16QuestID) )
		return FALSE;

	return TRUE;
}

//--------------------------------------------------------------------------------
// ĳ��NPC�Ƿ������ɸ�����
//--------------------------------------------------------------------------------
BOOL QuestMgr::IsNPCCompleteQuest(Creature* pCreature, UINT16 u16QuestID)
{
	if( FALSE == P_VALID(pCreature) ) return FALSE;

	TList<UINT16>* pList = m_mapCompleteQuestNPCRelation.Peek(pCreature->GetTypeID());

	if( !P_VALID(pList) || FALSE == pList->IsExist(u16QuestID) )
		return FALSE;

	return TRUE;
}

//---------------------------------------------------------------------------------
// ĳ�������Ƿ�Ҫͨ��NPC�ύ
//---------------------------------------------------------------------------------
BOOL QuestMgr::IsNeedNPCCompleteQuest(UINT16 u16QuestID)
{
	return m_mapCompleteNPCRelation.IsExist(u16QuestID);
}

//---------------------------------------------------------------------------------
// ĳ�������Ƿ�Ҫͨ��NPC��ȡ
//---------------------------------------------------------------------------------
BOOL QuestMgr::IsNeedNPCAcceptQuest(UINT16 u16QuestID)
{
	return m_mapAcceptNPCRelation.IsExist(u16QuestID);
}

QuestMgr g_questMgr;