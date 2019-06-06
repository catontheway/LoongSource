/////////////////////////////////////////////////////////////////////////////
// Program:     loong����༭��
// Name:        QuestMgr.cpp
// Created:     2008-11-12
/////////////////////////////////////////////////////////////////////////////
#include "wx/wx.h"
#include "QuestMgr.h"
#include <sstream>
#include "..\WorldDefine\Questdef.h"


// ----------------------------------------------------------------------------
//���캯��
// ----------------------------------------------------------------------------
QuestMgr::QuestMgr() :m_pQuestTmp(NULL)
{

}

// ----------------------------------------------------------------------------
//��������
// ----------------------------------------------------------------------------
QuestMgr::~QuestMgr()
{
	//ɾ����ʱQuest
	if(m_pQuestTmp)
		delete m_pQuestTmp;

	//ɾ��quest
	QuestMap::iterator it = m_mapQuest.begin();
	for (; it != m_mapQuest.end();)
	{
		delete it->second->pQuestProto;
		delete it->second;
		it = m_mapQuest.erase(it);
	}
}

// ----------------------------------------------------------------------------
// ���������ļ�
// ----------------------------------------------------------------------------
BOOL QuestMgr::LoadQuest(LPCTSTR pPath)
{
	if(pPath == NULL)
		return FALSE;

	TCHAR szType[512];
	_stprintf(szType,_T("%s\\RType.xml"),pPath);
	LoadQuestType(szType);

	//����ָ��Ŀ¼�µ����������ļ��������б���
	FindFile(pPath, m_listQuestFile, m_listQuestLocalFile);
	
	//���������ļ�����
	list<wstring>::iterator it = m_listQuestFile.begin();
	for (; it != m_listQuestFile.end(); ++it)
	{
		LoadQuestFile((*it).c_str());
	}

	//���������ļ�����
	it = m_listQuestLocalFile.begin();
	for (; it != m_listQuestLocalFile.end(); ++it)
	{
		LoadQuestLocal((*it).c_str());
	}

	return TRUE;
}

// ----------------------------------------------------------------------------
// ���������ļ�
// ----------------------------------------------------------------------------
BOOL QuestMgr::SaveQuest(UINT16 id)
{
	QuestMap::iterator pos = m_mapQuest.find(id);
	if (pos == m_mapQuest.end())
		return FALSE;

	tagQuestProto *pQuestProto = pos->second->pQuestProto;
	wstring file = pos->second->file_path;

	if (file.empty() || pQuestProto == NULL)
		return FALSE;

	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "UTF-8", "" );
	doc.LinkEndChild( decl );

	TiXmlElement* root = new TiXmlElement( "quest" );
	doc.LinkEndChild( root );

	//block quest property
	{
		TiXmlElement* propertyNode = new TiXmlElement( "property" );
		root->LinkEndChild( propertyNode );

		//����ID
		SaveEntry( propertyNode, "ID", pQuestProto->id );
		//ǰһ������1ID
		SaveEntry( propertyNode, "PrevQuestID", pQuestProto->prev_quest_id );
		//ǰһ������2ID
		SaveEntry( propertyNode, "PrevQuest2ID", pQuestProto->prev_quest2_id );
		//ǰ������֮���ϵ
		SaveEntry( propertyNode, "PrevQuestRelation", pQuestProto->prev_quest_relation );
		//��������ID
		SaveEntry( propertyNode, "NextQuestID", pQuestProto->next_quest_id );
		//��������ID
		SaveEntry( propertyNode, "UncmpleteQuestID", pQuestProto->uncomplete_quest_id );
		//��ǰ������ɺ󣬺��������Ƿ��Զ����ص��������
		SaveEntry( propertyNode, "AutoAddQuest", pQuestProto->auto_add_quest );
		//��ȡ������Ƿ񵯳��������
		SaveEntry( propertyNode, "ShowQuestDialog", pQuestProto->show_quest_dialog );
		//�����ܱ�ɾ��
		SaveEntry( propertyNode, "NoDelete", pQuestProto->no_delete );
		//�����ȡ����--���������ȡ
		SaveEntry( propertyNode, "QuestPlayerAccept", pQuestProto->accept_flags & Quest_Accept_Flag_Player );
		//�����ȡ����--ϵͳ����
		SaveEntry( propertyNode, "QuestSystemRelease", pQuestProto->accept_flags & Quest_Accept_Flag_System );
		//�����ȡ����--����Ʒ��ȡ
		SaveEntry( propertyNode, "QuestItemAccept", pQuestProto->accept_flags & Quest_Accept_Flag_Item );
		//��������
		SaveEntry( propertyNode, "QuestType", pQuestProto->type );
		//����Ŀ������
		SaveEntry( propertyNode, "TargetMode", pQuestProto->target_mode );
		//������Ҹ���
		SaveEntry( propertyNode, "SuggestedPlayers", pQuestProto->suggested_players );
		//������ɵ�����ʱ��
		SaveEntry( propertyNode, "LimitTime", pQuestProto->limit_time );
		//ÿ����ҽ�ȡ�������
		SaveEntry( propertyNode, "DoQuestTimes", pQuestProto->accept_times );
		//�Ƿ�����������
		SaveEntry( propertyNode, "Periodic", pQuestProto->period );
		//�����������ÿ�� ÿ�£�
		SaveEntry( propertyNode, "QuestPeriodic", pQuestProto->period_type );
		//��������
		SaveEntry( propertyNode, "Week", pQuestProto->week );
		//�����Ƿ�����ظ�
		SaveEntry( propertyNode, "Repeatable", pQuestProto->repeatable );
		
		//�����ȡNPCID
		SaveEntry( propertyNode, "AcceptQuestNPCID1", pQuestProto->accept_quest_npc[0] );
		SaveEntry( propertyNode, "AcceptQuestNPCID2", pQuestProto->accept_quest_npc[1] );
		SaveEntry( propertyNode, "AcceptQuestNPCID3", pQuestProto->accept_quest_npc[2] );
		SaveEntry( propertyNode, "AcceptQuestNPCID4", pQuestProto->accept_quest_npc[3] );
		SaveEntry( propertyNode, "AcceptQuestNPCID5", pQuestProto->accept_quest_npc[4] );
		SaveEntry( propertyNode, "AcceptQuestNPCID6", pQuestProto->accept_quest_npc[5] );

		//�������NPCID
		SaveEntry( propertyNode, "CompleteQuestNPCID1", pQuestProto->complete_quest_npc[0] );
		SaveEntry( propertyNode, "CompleteQuestNPCID2", pQuestProto->complete_quest_npc[1] );
		SaveEntry( propertyNode, "CompleteQuestNPCID3", pQuestProto->complete_quest_npc[2] );
		SaveEntry( propertyNode, "CompleteQuestNPCID4", pQuestProto->complete_quest_npc[3] );
		SaveEntry( propertyNode, "CompleteQuestNPCID5", pQuestProto->complete_quest_npc[4] );
		SaveEntry( propertyNode, "CompleteQuestNPCID6", pQuestProto->complete_quest_npc[5] );
	
		//�ɵ������ID
		SaveEntry( propertyNode, "InvestigateObjectID1", pQuestProto->investigate_objects[0] );
		SaveEntry( propertyNode, "InvestigateObjectID2", pQuestProto->investigate_objects[1] );
		SaveEntry( propertyNode, "InvestigateObjectID3", pQuestProto->investigate_objects[2] );
		SaveEntry( propertyNode, "InvestigateObjectID4", pQuestProto->investigate_objects[3] );
		SaveEntry( propertyNode, "InvestigateObjectID5", pQuestProto->investigate_objects[4] );
		SaveEntry( propertyNode, "InvestigateObjectID6", pQuestProto->investigate_objects[5] );
		SaveEntry( propertyNode, "InvestigateObjectID7", pQuestProto->investigate_objects[6] );
		SaveEntry( propertyNode, "InvestigateObjectID8", pQuestProto->investigate_objects[7] );

		//������ƷID������
		SaveEntry( propertyNode, "SrcItemID", pQuestProto->src_item, pQuestProto->src_item_num );

		//����Ŀ�ĵ�����
		//��float ת�����ַ���
		wstringstream ss;
		wstring wstr;
		ss << pQuestProto->destination_x;
		SaveEntry( propertyNode, "Destination_X", ss.str().c_str() );

		ss.str(_T(""));
		ss << pQuestProto->destination_y;
		SaveEntry( propertyNode, "Destination_Y", ss.str().c_str() );
		
		ss.str(_T(""));
		ss << pQuestProto->destination_z;
		SaveEntry( propertyNode, "Destination_Z", ss.str().c_str() );
	}

	//block quest accept-conditions
	{
		TiXmlElement* acceptNode = new TiXmlElement( "accept-conditions" );
		root->LinkEndChild( acceptNode );

		//��ȡ��������Ҫ�����ȼ�
		SaveEntry( acceptNode, "GetQuestRequiredMaxLevel", pQuestProto->accept_req_max_level );
		//��ȡ��������Ҫ����С�ȼ�
		SaveEntry( acceptNode, "GetQuestRequiredMinLevel", pQuestProto->accept_req_min_level );
		//��ȡ��������Ҫ������,��Сֵ,���ֵ
		for ( int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i )
		{
			stringstream ss;
			string str;
			ss << "GetQuestRequiredRep" << i + 1 << endl;
			ss >> str;
			//SaveEntry( acceptNode, str.c_str(), pQuest->dwGetQuestRequiredRep[i] );
			//SaveEntry( acceptNode, "GetQuestRequiredMinRepValue", pQuest->dwGetQuestRequiredMinRepValue[i] );
			//SaveEntry( acceptNode, "GetQuestRequiredMaxRepValue", pQuest->dwGetQuestRequiredMaxRepValue[i] );
			SaveMaxEntry( acceptNode, str.c_str(), pQuestProto->accept_req_rep[i], pQuestProto->accept_req_max_rep[i], 
				pQuestProto->accept_req_min_rep[i] );
		}
		//��ȡ��������Ҫ���Ա�
		SaveEntry( acceptNode, "GetQuestRequiredSex", pQuestProto->sex );
		//��ȡ��������Ҫ��ְҵ	
		SaveEntry( acceptNode, "GetQuestRequiredClass", pQuestProto->class_type );
		//��ȡ��������Ҫ�ĵ�ͼ
		SaveEntry( acceptNode, "GetQuestRequiredMapID", pQuestProto->accept_req_map );
		//��ȡ��������Ҫ�ļ���,����ֵ
		for ( int i = 0; i < QUEST_SKILLS_COUNT; ++i )
		{
			stringstream ss;
			string str;
			ss << "GetQuestRequiredSkill" << i + 1 << endl;
			ss >> str;
			//SaveEntry( acceptNode, str.c_str(), pQuest->dwGetQuestRequiredSkill[i] );
			//SaveEntry( acceptNode, "GetQuestRequiredSkillValue", pQuest->dwGetQuestRequiredSkillValue[i] );
			SaveEntry( acceptNode, str.c_str(), pQuestProto->accept_req_skill[i], pQuestProto->accept_req_skill_val[i] );
		}
		//��ȡ��������Ҫ����ƷID,����
		for ( int i = 0; i < QUEST_ITEMS_COUNT; ++i )
		{
			stringstream ss;
			string str;
			ss << "GetQuestRequiredItemID" << i + 1 << endl;
			ss >> str;
			//SaveEntry( acceptNode, str.c_str(), pQuest->dwGetQuestRequiredItemID[i] );
			//SaveEntry( acceptNode, "GetQuestRequiredItemCount", pQuest->dwGetQuestRequiredItemCount[i] );
			SaveEntry( acceptNode, str.c_str(), pQuestProto->accept_req_item[i], pQuestProto->accept_req_item_num[i] );
		}
		//������Ʒ�Ƿ�ɾ��
		SaveEntry( acceptNode, "DelRestrictedItem", pQuestProto->del_req_item );

		//��ȡ�����������������,ֵ
		for ( int i = 0; i < QUEST_ATTS_COUNT; ++i )
		{
			stringstream ss;
			string str;
			ss << "GetQuestRequiredAtt" << i + 1 << endl;
			ss >> str;	
			SaveEntry( acceptNode, str.c_str(), pQuestProto->accept_req_att[i], pQuestProto->accept_req_att_val[i] );
		}

		//��ȡ�������败����
		for ( int i = 0; i < QUEST_TRIGGERS_COUNT; ++i )
		{
			stringstream ss;
			string str;
			ss << "GetQuestReqTriggersID" << i + 1 << endl;
			ss >> str;	
			SaveEntry( acceptNode, str.c_str(), pQuestProto->accept_req_trriger[i] );
		}

		//�Ƿ��Ƿ���
		SaveEntry( acceptNode, "GetQuestReqMarried", pQuestProto->married );
		//���¸���
		SaveEntry( acceptNode, "GetQuestReqLoverNum", pQuestProto->lover_num );
		//��Ǯ����
		SaveEntry( acceptNode, "GetQuestReqMoneyNum", pQuestProto->accept_req_money );
		//�Ƿ�ɾ����Ǯ
		SaveEntry( acceptNode, "GetQuestDelMoneyNum", pQuestProto->accept_del_money );
	}

	//block quest complete-conditions
	{
		TiXmlElement* completeNode = new TiXmlElement( "complete-conditions" );
		root->LinkEndChild( completeNode );

		//�����������Ҫ����ƷID,����
		for ( int i = 0; i < QUEST_ITEMS_COUNT; ++i )
		{
			stringstream ss;
			string str;
			ss << "CompleteQuestRequiredItemID" << i + 1 << endl;
			ss >> str;
			//SaveEntry( completeNode, str.c_str(), pQuest->dwCompleteQuestRequiredItemID[i] );
			//SaveEntry( completeNode, "CompleteQuestRequiredItemIDCount", pQuest->dwCompleteQuestRequiredItemIDCount[i] );
			SaveEntry( completeNode, str.c_str(), pQuestProto->complete_req_item[i], pQuestProto->complete_req_item_num[i] );
		}
		//�Ƿ��Ѽ�������һ�־Ϳ����
		SaveEntry( completeNode, "RequiredOnlyOneItem", pQuestProto->only_one_item );
		//���������Ƿ�ɾ����Ʒ
		SaveEntry( completeNode, "DeleteItem", pQuestProto->delete_item );

		//�����������Ҫɱ�Ĺ�,����
		for ( int i = 0; i < QUEST_CREATURES_COUNT; ++i )
		{
			stringstream ss;
			string str;
			ss << "CompleteQuestRequiredCreatureID" << i + 1 << endl;
			ss >> str;
			//SaveEntry( completeNode, str.c_str(), pQuest->dwCompleteQuestRequiredCreatureID[i] );
			//SaveEntry( completeNode, "CompleteQuestRequiredCreatureCount", pQuest->dwCompleteQuestRequiredCreatureCount[i] );
			SaveCreatureEntry( completeNode, str.c_str(), pQuestProto->complete_req_creature[i*3], 
					pQuestProto->complete_req_creature[i*3+1], pQuestProto->complete_req_creature[i*3+2],
					pQuestProto->complete_req_creature_num[i] );
		}
		//�Ƿ��ɱ��ָ��������ͬһ�ȼ�����Ϳ����
		SaveEntry( completeNode, "RequiredCreatureLevel", pQuestProto->creature_level );
		//�Ƿ��ɱ��ָ��������һ�ֹ���Ϳ����
		SaveEntry( completeNode, "RequiredOnlyOneCreature", pQuestProto->only_one_creature );
		//�����������Ҫ�Ի���NPC����
		for ( int i = 0; i < QUEST_NPC_COUNT; ++i )
		{
			stringstream ss;
			string str;
			ss << "CompleteQuestRequiredNPC" << i + 1 << endl;
			ss >> str;
			SaveEntry( completeNode, str.c_str(), pQuestProto->complete_req_npc[i] );
		}
		//�Ƿ���밴˳��Ի�
		SaveEntry( completeNode, "RequiredOnlyInOrder", pQuestProto->only_in_order );
		//�Ƿ�ֻ��һ��NPC�Ի���Ϳ����
		SaveEntry( completeNode, "RequiredOnlyOneNPC", pQuestProto->only_one_npc );
		//�����������Ҫ�Ĵ�����ID
		for ( int i = 0; i < QUEST_TRIGGERS_COUNT; ++i )
		{
			stringstream ss;
			string str;
			ss << "CompleteQuestReqTriggersID" << i + 1 << endl;
			ss >> str;
			SaveEntry( completeNode, str.c_str(), pQuestProto->complete_req_trigger[i] );
		}
		//�Ƿ�ֻ����һ���Ϳ����
		SaveEntry( completeNode, "RequiredOnlyOneTrigger", pQuestProto->only_one_trigger );
		//�����������Ҫ�Ľ�Ǯ
		SaveEntry( completeNode, "CompleteQuestRequiredMoney", pQuestProto->complete_req_money );
		//��������Ƿ�ɾ������Ҫ�Ľ�Ǯ
		SaveEntry( completeNode, "CompleteQuestDelMoney", pQuestProto->complete_del_money );
		//�����������Ҫ�ĵ�ͼID
		SaveEntry( completeNode, "CompleteQuestRequiredMapID", pQuestProto->complete_req_map );
		//�����������Ҫ�ĵ�ͼX����
		//��float ת�����ַ���
		wstringstream ss;
		wstring wstr;
		ss << pQuestProto->complete_req_map_x << endl;
		ss >> wstr;
		SaveEntry( completeNode, "CompleteQuestRequiredMap_X", wstr.c_str() );
		//�����������Ҫ�ĵ�ͼY����
		ss.clear();
		ss << pQuestProto->complete_req_map_y << endl;
		ss >> wstr;
		SaveEntry( completeNode, "CompleteQuestRequiredMap_Y", wstr.c_str() );
		//�����������Ҫ�ĵ�ͼZ����
		ss.clear();
		ss << pQuestProto->complete_req_map_z << endl;
		ss >> wstr;
		SaveEntry( completeNode, "CompleteQuestRequiredMap_Z", wstr.c_str() );
		//�����������Ҫ�ĵ�ͼ�뾶
		ss.clear();
		ss << pQuestProto->complete_req_map_radius << endl;
		ss >> wstr;
		SaveEntry( completeNode, "CompleteQuestRequiredMap_Radius", wstr.c_str() );
		//��������Ƿ��������
		SaveEntry( completeNode, "CompleteQuestRequiredMap_Random", pQuestProto->coordinate_random );
		//�¼�
		SaveEntry( completeNode, "CompleteQuestEventType", pQuestProto->event_type );
		SaveEntry( completeNode, "UseItemEvent_Item_ID", pQuestProto->use_item );
		SaveEntry( completeNode, "UseSkillEvent_Skill_ID", pQuestProto->use_skill );

		//�Ƿ����������������һ��������������
		SaveEntry( completeNode, "RequiredOnlyOneCondition", pQuestProto->only_one_condition );

		//�Ƿ��Ƿ���
		SaveEntry( completeNode, "CompleteQuestReqMarried", pQuestProto->complete_req_married );

		//���¸���
		SaveEntry( completeNode, "CompleteQuestReqLoverNum", pQuestProto->complete_req_lover_num );

		//�ȼ�
		SaveEntry( completeNode, "CompleteQuestReqLevel", pQuestProto->complete_req_level );
	}

	//block quest reward
	{
		TiXmlElement* rewardNode = new TiXmlElement( "reward" );
		root->LinkEndChild( rewardNode );

		//������ɺ󣬽����Ľ�Ǯ
		SaveEntry( rewardNode, "RewMoney", pQuestProto->rew_money );
		//������ɺ󣬽����ľ���ֵ
		SaveEntry( rewardNode, "RewXP", pQuestProto->rew_xp );
		//������������,����ֵ
		for ( int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i )
		{
			stringstream ss;
			string str;
			ss << "RewRequiredRep" << i + 1 << endl;
			ss >> str;
			//SaveEntry( rewardNode, str.c_str(), pQuest->dwRewRequiredRep[i] );
			//SaveEntry( rewardNode, "RewRequiredRepValue", pQuest->dwRewRequiredRepValue[i] );
			SaveEntry( rewardNode, str.c_str(), pQuestProto->rew_rep[i], pQuestProto->rew_rep_val[i] );
		}
		//�������Ĺ���,����ֵ
		for ( int i = 0; i < QUEST_CONTRIBUTION_COUNT; ++i )
		{
			stringstream ss;
			string str;
			ss << "RewRequiredContribution" << i + 1 << endl;
			ss >> str;
			SaveEntry( rewardNode, str.c_str(), pQuestProto->rew_contribution[i], pQuestProto->rew_contribution_val[i] );
		}
		//����������ƷID,����
		for ( int i = 0; i < QUEST_REW_ITEM; ++i )
		{
			stringstream ss;
			string str;
			ss << "RewItemID" << i + 1 << endl;
			ss >> str;
			//SaveEntry( rewardNode, str.c_str(), pQuest->dwRewItemID[i] );
			//SaveEntry( rewardNode, "RewItemCount", pQuest->dwRewItemCount[i] );
			SaveItem( rewardNode, str.c_str(), pQuestProto->rew_item[i], pQuestProto->rew_item_num[i], pQuestProto->rew_item_quality[i] );
		}
		//�������Ŀ�ѡ��ƷID,����
		for ( int i = 0; i < QUEST_REW_ITEM; ++i )
		{
			stringstream ss;
			string str;
			ss << "RewChoicesItemID" << i + 1 << endl;
			ss >> str;
			//SaveEntry( rewardNode, str.c_str(), pQuest->dwRewChoicesItemID[i] );
			//SaveEntry( rewardNode, "RewChoicesItemCount", pQuest->dwRewChoicesItemCount[i] );
			SaveItem( rewardNode, str.c_str(), pQuestProto->rew_choice_item[i], pQuestProto->rew_choice_item_num[i], pQuestProto->rew_choice_quality[i] );
		}
		//������������,ֵ
		for ( int i = 0; i < QUEST_ATTS_COUNT; ++i )
		{
			stringstream ss;
			string str;
			ss << "RewAtt" << i + 1 << endl;
			ss >> str;
			//SaveEntry( rewardNode, str.c_str(), pQuest->dwRewAtt[i] );
			//SaveEntry( rewardNode, "RewAttValue", pQuest->dwRewAttValue[i] );
			SaveEntry( rewardNode, str.c_str(), pQuestProto->rew_att[i], pQuestProto->rew_att_val[i] );
		}

		//�������ļ���,ֵ
		for ( int i = 0; i < QUEST_SKILLS_COUNT; ++i )
		{
			stringstream ss;
			string str;
			ss << "RewSkill" << i + 1 << endl;
			ss >> str;
			//SaveEntry( rewardNode, str.c_str(), pQuest->dwRewSkill[i] );
			//SaveEntry( rewardNode, "RewSkillValue", pQuest->dwRewSkillValue[i] );
			SaveEntry( rewardNode, str.c_str(), pQuestProto->rew_skill[i], pQuestProto->rew_skill_val[i] );
		}
		//��������buff
		for ( int i = 0; i < QUEST_REW_BUFF; ++i )
		{
			stringstream ss;
			string str;
			ss << "RewBuff" << i + 1 << endl;
			ss >> str;
			SaveEntry( rewardNode, str.c_str(), pQuestProto->rew_buff[i] );
		}
	}

	string strFileName = ToAnsi( pos->second->file_path );
	doc.SaveFile( strFileName.c_str() );

	//�������ļ������ֲ������Ϊһ���ļ� Local
	TiXmlDocument doc_local;
	TiXmlDeclaration* decl_local = new TiXmlDeclaration( "1.0", "UTF-8", "" );
	doc_local.LinkEndChild( decl_local );

	TiXmlElement* root_local = new TiXmlElement( "quest-local" );
	doc_local.LinkEndChild( root_local );
	{
		//����ID
		SaveEntry( root_local, "ID", pQuestProto->id );
		//��������
		SaveEntry( root_local, "Name", pQuestProto->name );
		//��������
		SaveEntry( root_local, "Conditions", pQuestProto->conditions );
		//�������
		SaveEntry( root_local, "Overview", pQuestProto->over_view );
	}
	size_t find = strFileName.rfind(".");
	if ( find == string::npos )
		return FALSE;
	string str = strFileName.substr(0,find);
	str += ".local";
	doc_local.SaveFile( str.c_str() );
	return TRUE;
}


// ----------------------------------------------------------------------------
// ɾ�������ļ�
// ----------------------------------------------------------------------------
void  QuestMgr::DeleteQuest(UINT16 id)
{
	
}

// ----------------------------------------------------------------------------
// �����ƶ�Ŀ¼�������������ļ����뼯��
// ----------------------------------------------------------------------------
void QuestMgr::FindFile(LPCTSTR pFolder, list<wstring>& xmlMap, list<wstring>& localMap)
{
	TCHAR szFullPathFileName[MAX_PATH];
	TCHAR szFileName[MAX_PATH];
	WIN32_FIND_DATA FileData = {0};
	BOOL bFinished = FALSE;

	_sntprintf(szFullPathFileName, MAX_PATH-1, _T("%s\\*.*"), pFolder);
	HANDLE hSearch = FindFirstFile(szFullPathFileName, &FileData);
	if (hSearch == INVALID_HANDLE_VALUE)
		return;

	while (!bFinished)
	{
		_sntprintf(szFileName, MAX_PATH-1, _T("%s\\%s"), pFolder, FileData.cFileName);
		if (FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (_tcscmp(FileData.cFileName, _T(".")) && _tcscmp(FileData.cFileName, _T("..")) && _tcscmp(FileData.cFileName, _T(".svn")))
			{
				m_listQuestDir.push_back(FileData.cFileName);
				FindFile(szFileName, xmlMap, localMap);
			}
		}
		else
		{
			//�ļ���չ��ƥ��
			if(IsExtensionOK(szFileName, _T("xml")))
				xmlMap.push_back(szFileName);
			else if (IsExtensionOK(szFileName, _T("local")))
				localMap.push_back(szFileName);
		}

		if (!FindNextFile(hSearch,&FileData))
		{
			if (GetLastError() == ERROR_NO_MORE_FILES)
				bFinished = TRUE;
		}
	}
}	

// ----------------------------------------------------------------------------
// ���������ļ�
// ----------------------------------------------------------------------------
void QuestMgr::LoadQuestFile(LPCTSTR pPath)
{
	if (pPath == NULL || !_tcscmp(pPath, _T(""))) 
		return;

	//��ȡxml
	//����XML
	string strDoc = ToAnsi(wstring(pPath));
	TiXmlDocument doc(strDoc.c_str());
	if (doc.LoadFile() == false) 
		return;

	TiXmlHandle hDoc(&doc);
	TiXmlHandle hRoot(0);

	//�Ƿ��������ļ�
	TiXmlElement *pElem = hDoc.FirstChildElement("quest").Element();
	if (!pElem) 
	   return;

	hRoot = TiXmlHandle( pElem );

	stringstream ss;
	string str;

	XmlNodeMap xmlNode;

	TiXmlElement* pPropertyNode = hRoot.FirstChild( "property" ).FirstChild().Element();
	TiXmlElement* pAcceptConditionsNode = hRoot.FirstChild( "accept-conditions" ).FirstChild().Element();
	TiXmlElement* pCompleteConditionsNode = hRoot.FirstChild( "complete-conditions" ).FirstChild().Element();
	TiXmlElement* pRewNode = hRoot.FirstChild( "reward" ).FirstChild().Element();
	LoadXmlNode(pPropertyNode, xmlNode);
	LoadXmlNode(pAcceptConditionsNode, xmlNode);
	LoadXmlNode(pCompleteConditionsNode, xmlNode);
	LoadXmlNode(pRewNode, xmlNode);

	tagQuest* pQuest = new tagQuest;
	pQuest->pQuestProto = new tagQuestProto;
	ZeroMemory(pQuest->pQuestProto, sizeof(tagQuestProto));

	//Ϊ����ֵ
	SetQuestProto(pQuest->pQuestProto, xmlNode);
	//ȡ�������ļ�Ŀ¼
	pQuest->file_path = wstring(pPath);
	size_t index = pQuest->file_path.rfind(_T("\\"));
	if (index != wstring::npos)
	{
		pQuest->file_name = pQuest->file_path.substr(index+1);
		wstring tmp = pQuest->file_path.substr(0, index);
		index = tmp.rfind(_T("\\"));
		if (index != wstring::npos)
		{
			pQuest->file_dir = tmp.substr(index + 1);
		}
	}
	//���뵽���񼯺���
	//��������Ƿ����
	QuestMap::iterator pos = m_mapQuest.find(pQuest->pQuestProto->id);
	if (pos != m_mapQuest.end())
	{
		delete pQuest->pQuestProto;
		delete pQuest;
	}
	else
	{
		m_mapQuest.insert(make_pair(pQuest->pQuestProto->id,pQuest));
	}
}


// ----------------------------------------------------------------------------
// ��������Local�ļ�
// ----------------------------------------------------------------------------
void QuestMgr::LoadQuestLocal(LPCTSTR pPath)
{
	if (pPath == NULL || !_tcscmp(pPath, _T(""))) 
		return;

	//��ȡxml
	//����XML
	string strDoc = ToAnsi(wstring(pPath));
	TiXmlDocument doc(strDoc.c_str());
	if (doc.LoadFile() == false) 
		return;

	TiXmlHandle hDoc(&doc);
	TiXmlHandle hRoot(0);

	//�Ƿ�������Local�ļ�
	TiXmlElement *pElem = hDoc.FirstChildElement("quest-local").Element();
	if (!pElem) 
		return;

	hRoot = TiXmlHandle( pElem );

	TiXmlElement *pNode = hRoot.FirstChild().Element();

	TiXmlAttribute* pAttrib = NULL;
	//ID
	pAttrib = pNode->FirstAttribute();
	if (pAttrib == NULL)
		return;
	
	INT16 id = atoi(pAttrib->Value());
	pNode = pNode->NextSiblingElement();
	
	//����ID�Ƿ����
	QuestMap::iterator pos = m_mapQuest.find(id);
	if (pos == m_mapQuest.end())
		return;

	tagQuest *pQuest = pos->second;
	if (pQuest == NULL)
		return;
	if(pQuest->pQuestProto == NULL)
		return;

	//Name
	if (pNode != NULL)
	{
		pAttrib = pNode->FirstAttribute();
		if(pAttrib!= NULL)
			_tcsncpy( pQuest->pQuestProto->name, ToGBKW(pAttrib->Value()).c_str(), X_SHORT_NAME );
		pNode = pNode->NextSiblingElement();
	}

	//condition
	if (pAttrib != NULL)
	{
		pAttrib = pNode->FirstAttribute();
		if(pAttrib!= NULL)
			_tcsncpy( pQuest->pQuestProto->conditions, ToGBKW(pAttrib->Value()).c_str(), X_SHORT_NAME );
		pNode = pNode->NextSiblingElement();
	}
	//overview
	if (pAttrib != NULL)
	{
		pAttrib = pNode->FirstAttribute();
		if(pAttrib!= NULL)
			_tcsncpy( pQuest->pQuestProto->over_view, ToGBKW(pAttrib->Value()).c_str(), QUEST_OVERVIEW );
	}
	pQuest->local_path = pPath;
}

BOOL QuestMgr::LoadQuestType( LPCTSTR szPath )
{
	if (szPath == NULL || !_tcscmp(szPath, _T(""))) 
		return FALSE;

	//��ȡxml
	//����XML
	string strDoc = ToAnsi(wstring(szPath));
	TiXmlDocument doc(strDoc.c_str());
	if (doc.LoadFile() == false)
		return FALSE;

	TiXmlHandle hDoc(&doc);
	TiXmlHandle hRoot(0);

	//�Ƿ��������ļ�
	TiXmlElement *pElem = hDoc.FirstChildElement("root").Element();
	if (!pElem) 
		return FALSE;

	hRoot = TiXmlHandle( pElem );

	stringstream ss;
	string str;

	XmlNodeArray xmlNode;

	TiXmlElement* pPropertyNode = hRoot.FirstChild().Element();
	LoadXmlNode(pPropertyNode, xmlNode);

	//
	while( !xmlNode.empty() )
	{
		XmlNodeArray::iterator iter = xmlNode.begin();
		INT16 id = (INT16)atoi(iter->second.c_str());
		xmlNode.erase(iter);

		iter = xmlNode.begin();
		TCHAR name[X_SHORT_NAME];
		_tcsncpy( name, ToGBKW( iter->second.c_str() ).c_str(), X_SHORT_NAME );
		xmlNode.erase(iter);

		// colorֱ����������
		iter = xmlNode.begin();
		if(iter != xmlNode.end() && iter->first == "id-color")
			xmlNode.erase(iter);

		//category����ȥ����
		iter = xmlNode.begin();
		if(iter != xmlNode.end() && iter->first == "id-category")
			xmlNode.erase(iter);

		m_questType.push_back(make_pair(id,name));

	}

	return TRUE;
}

// ----------------------------------------------------------------------------
// ��ȡxml�ڵ�
// ----------------------------------------------------------------------------
void QuestMgr::LoadXmlNode(TiXmlElement *pNode, XmlNodeMap &xmlNodeMap)
{
	//�Ƿ���ڸýڵ�
	if (! pNode) 
		return;

	const char* pChar = NULL;
	TiXmlAttribute* pAttrib = NULL;
	while( pNode)
	{
		if ( pNode)
		{
			pAttrib =  pNode->FirstAttribute();
			string firstName;
			if (pAttrib != NULL)
			{
				firstName = pAttrib->Name();
			}
			while(pAttrib)
			{
				if (pAttrib->Name() != NULL && pAttrib->Value() != NULL)
				{
					if (pAttrib->Previous() == NULL)
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
}

void QuestMgr::LoadXmlNode( TiXmlElement* pNode, XmlNodeArray& xmlNodeArray )
{
	//�Ƿ���ڸýڵ�
	if (! pNode) 
		return;

	const char* pChar = NULL;
	TiXmlAttribute* pAttrib = NULL;
	while( pNode)
	{
		if ( pNode)
		{
			pAttrib =  pNode->FirstAttribute();
			string firstName;
			if (pAttrib != NULL)
			{
				firstName = pAttrib->Name();
			}
			while(pAttrib)
			{
				if (pAttrib->Name() != NULL && pAttrib->Value() != NULL)
				{
					if (pAttrib->Previous() == NULL)
					{
						xmlNodeArray.push_back(make_pair(firstName, string(pAttrib->Value())));
					}
					else
					{
						string tmp = firstName;
						tmp += "-";
						tmp += pAttrib->Name();
						xmlNodeArray.push_back( make_pair(tmp, pAttrib->Value()));
					}

				}
				pAttrib = pAttrib->Next();
			}

			pNode =  pNode->NextSiblingElement();
		}
	}
}

// ----------------------------------------------------------------------------
//������������
// ----------------------------------------------------------------------------
void QuestMgr::SetQuestProto(tagQuestProto *pQuestProto, XmlNodeMap &xmlNodeMap)
{
	//property
	SetQuestUINT16("ID", pQuestProto->id, xmlNodeMap);
	SetQuestString("Name", pQuestProto->name, X_SHORT_NAME, xmlNodeMap );
	SetQuestUINT16("PrevQuestID", pQuestProto->prev_quest_id, xmlNodeMap );
	SetQuestUINT16("PrevQuest2ID", pQuestProto->prev_quest2_id, xmlNodeMap );
	SetQuestINT32("PrevQuestRelation", pQuestProto->prev_quest_relation, xmlNodeMap);
	SetQuestUINT16("NextQuestID", pQuestProto->next_quest_id, xmlNodeMap );
	SetQuestUINT16("UncmpleteQuestID", pQuestProto->uncomplete_quest_id, xmlNodeMap );
	SetQuestBool("AutoAddQuest", pQuestProto->auto_add_quest, xmlNodeMap );
	SetQuestBool("ShowQuestDialog", pQuestProto->show_quest_dialog, xmlNodeMap );
	SetQuestBool("NoDelete", pQuestProto->no_delete, xmlNodeMap );

	SetQuestDwordFlags("QuestPlayerAccept", pQuestProto->accept_flags, Quest_Accept_Flag_Player, xmlNodeMap );
	SetQuestDwordFlags("QuestSystemRelease", pQuestProto->accept_flags, Quest_Accept_Flag_System, xmlNodeMap );
	SetQuestDwordFlags("QuestItemAccept", pQuestProto->accept_flags, Quest_Accept_Flag_Item, xmlNodeMap );
	SetQuestINT32("TargetMode", (INT32&)pQuestProto->target_mode, xmlNodeMap);

	SetQuestString("Conditions", pQuestProto->conditions, X_SHORT_NAME, xmlNodeMap );
	SetQuestString("Overview", pQuestProto->over_view, QUEST_OVERVIEW, xmlNodeMap );
	SetQuestDword("QuestType", (DWORD&)pQuestProto->type, xmlNodeMap );
	SetQuestBYTE("SuggestedPlayers", pQuestProto->suggested_players, xmlNodeMap );
	SetQuestDword("LimitTime", pQuestProto->limit_time, xmlNodeMap );
	SetQuestBYTE("DoQuestTimes", pQuestProto->accept_times, xmlNodeMap );
	SetQuestBool("Periodic", pQuestProto->period, xmlNodeMap );
	SetQuestDword("QuestPeriodic", (DWORD&)pQuestProto->period_type, xmlNodeMap );
	SetQuestDword("Week", (DWORD&)pQuestProto->week, xmlNodeMap );
	SetQuestBool("Repeatable", pQuestProto->repeatable, xmlNodeMap );

	for( int i = 0; i < 6 ; ++i )
	{
		stringstream ss;
		string str;
		ss << "AcceptQuestNPCID" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->accept_quest_npc[i], xmlNodeMap );
	}

	for( int i = 0; i < 6 ; ++i )
	{
		stringstream ss;
		string str;
		ss << "CompleteQuestNPCID" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->complete_quest_npc[i], xmlNodeMap );
	}

	for( int i = 0; i < QUEST_ITEMS_COUNT ; ++i )
	{
		stringstream ss;
		string str;
		ss << "InvestigateObjectID" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->investigate_objects[i], xmlNodeMap );
	}

	SetQuestDword("SrcItemID", pQuestProto->src_item, xmlNodeMap );
	SetQuestINT16("SrcItemID-value", pQuestProto->src_item_num, xmlNodeMap );
	SetQuestFloat("Destination_X", pQuestProto->destination_x, xmlNodeMap);
	SetQuestFloat("Destination_Y", pQuestProto->destination_y, xmlNodeMap);
	SetQuestFloat("Destination_Z", pQuestProto->destination_z, xmlNodeMap);

	//accept-conditions
	SetQuestINT32("GetQuestRequiredMaxLevel", pQuestProto->accept_req_max_level, xmlNodeMap);
	SetQuestINT32("GetQuestRequiredMinLevel", pQuestProto->accept_req_min_level, xmlNodeMap);
	for( int i = 0; i < QUEST_REPUTATIONS_COUNT ; ++i )
	{
		stringstream ss;
		string str;
		ss << "GetQuestRequiredRep" << i + 1 << endl;
		ss >> str;	
		SetQuestINT32(str, pQuestProto->accept_req_rep[i], xmlNodeMap );
		string tmp = str;
		tmp += "-max";
		SetQuestINT32(tmp, pQuestProto->accept_req_max_rep[i], xmlNodeMap );
		tmp = str;
		tmp += "-min";
		SetQuestINT32(tmp, pQuestProto->accept_req_min_rep[i], xmlNodeMap );
	}
	for( int i = 0; i < QUEST_SKILLS_COUNT ; ++i )
	{
		stringstream ss;
		string str;
		ss << "GetQuestRequiredSkill" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->accept_req_skill[i], xmlNodeMap );
		string tmp = str;
		tmp += "-value";
		SetQuestINT32(tmp, pQuestProto->accept_req_skill_val[i], xmlNodeMap );
	}
	SetQuestBYTE("GetQuestRequiredSex", pQuestProto->sex, xmlNodeMap);
	SetQuestDword("GetQuestRequiredClass", (DWORD&)pQuestProto->class_type, xmlNodeMap);
	SetQuestDword("GetQuestRequiredMapID", pQuestProto->accept_req_map, xmlNodeMap);
	for( int i = 0; i < QUEST_ITEMS_COUNT ; ++i )
	{
		stringstream ss;
		string str;
		ss << "GetQuestRequiredItemID" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->accept_req_item[i], xmlNodeMap );
		string tmp = str;
		tmp += "-value";
		SetQuestINT16(tmp, pQuestProto->accept_req_item_num[i], xmlNodeMap );
	}
	for( int i = 0; i < QUEST_ATTS_COUNT ; ++i )
	{
		stringstream ss;
		string str;
		ss << "GetQuestRequiredAtt" << i + 1 << endl;
		ss >> str;	
		SetQuestINT32(str, pQuestProto->accept_req_att[i], xmlNodeMap );
		string tmp = str;
		tmp += "-value";
		SetQuestINT16(tmp, pQuestProto->accept_req_att_val[i], xmlNodeMap );
	}
	SetQuestBool("DelRestrictedItem", pQuestProto->del_req_item, xmlNodeMap);
	for( int i = 0; i < QUEST_TRIGGERS_COUNT ; ++i )
	{
		stringstream ss;
		string str;
		ss << "GetQuestReqTriggersID" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->accept_req_trriger[i], xmlNodeMap );
	}
	SetQuestBool("GetQuestReqMarried", pQuestProto->married, xmlNodeMap);
	SetQuestBYTE("GetQuestReqLoverNum", pQuestProto->lover_num, xmlNodeMap);
	SetQuestINT32("GetQuestReqMoneyNum", pQuestProto->accept_req_money, xmlNodeMap );
	SetQuestBool("GetQuestDelMoneyNum", pQuestProto->accept_del_money, xmlNodeMap);
	
	//complete-conditions
	for( int i = 0; i < QUEST_ITEMS_COUNT; ++i )
	{
		stringstream ss;
		string str;
		ss << "CompleteQuestRequiredItemID" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->complete_req_item[i], xmlNodeMap );
		string tmp = str;
		tmp += "-value";
		SetQuestINT16(tmp, pQuestProto->complete_req_item_num[i], xmlNodeMap );
	}
	SetQuestBool("RequiredOnlyOneItem", pQuestProto->only_one_item, xmlNodeMap);
	SetQuestBool("DeleteItem", pQuestProto->delete_item, xmlNodeMap);

	for( int i = 0; i < QUEST_CREATURES_COUNT; ++i )
	{
		stringstream ss;
		string str;
		ss << "CompleteQuestRequiredCreatureID" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->complete_req_creature[i*3], xmlNodeMap );
		string tmp = str;
		tmp += "-id2";
		SetQuestDword(tmp, pQuestProto->complete_req_creature[i*3+1], xmlNodeMap );
		tmp = str;
		tmp += "-id3";
		SetQuestDword(tmp, pQuestProto->complete_req_creature[i*3+2], xmlNodeMap );
		tmp = str;
		tmp += "-value";
		SetQuestINT16(tmp, pQuestProto->complete_req_creature_num[i], xmlNodeMap );
	}

	SetQuestBool("RequiredCreatureLevel", pQuestProto->creature_level, xmlNodeMap);
	SetQuestBool("RequiredOnlyOneCreature", pQuestProto->only_one_creature, xmlNodeMap);

	for( int i = 0; i < QUEST_NPC_COUNT ; ++i )
	{
		stringstream ss;
		string str;
		ss << "CompleteQuestRequiredNPC" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->complete_req_npc[i], xmlNodeMap );
	}

	SetQuestBool("RequiredOnlyInOrder", pQuestProto->only_in_order, xmlNodeMap);
	SetQuestBool("RequiredOnlyOneNPC", pQuestProto->only_one_npc, xmlNodeMap);

	for( int i = 0; i < QUEST_TRIGGERS_COUNT; ++i )
	{
		stringstream ss;
		string str;
		ss << "CompleteQuestReqTriggersID" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->complete_req_trigger[i], xmlNodeMap );
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

	SetQuestDword("CompleteQuestEventType", (DWORD&)pQuestProto->event_type, xmlNodeMap );
	SetQuestDword("UseItemEvent_Item_ID", pQuestProto->use_item, xmlNodeMap);
	SetQuestDword("UseSkillEvent_Skill_ID", pQuestProto->use_skill, xmlNodeMap);
	//Reword
	SetQuestINT32("RewMoney", pQuestProto->rew_money, xmlNodeMap);
	SetQuestINT32("RewXP", pQuestProto->rew_xp, xmlNodeMap);
	
	for( int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i )
	{
		stringstream ss;
		string str;
		ss << "RewRequiredRep" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->rew_rep[i], xmlNodeMap );
		string tmp = str;
		tmp += "-value";
		SetQuestINT32(tmp, pQuestProto->rew_rep_val[i], xmlNodeMap );
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

	for( int i = 0; i < QUEST_REW_ITEM; ++i )
	{
		stringstream ss;
		string str;
		ss << "RewItemID" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->rew_item[i], xmlNodeMap );
		string tmp = str;
		tmp += "-value";
		SetQuestINT16(tmp, pQuestProto->rew_item_num[i], xmlNodeMap );
		tmp = str;
		tmp += "-quality";
		SetQuestINT16(tmp, pQuestProto->rew_item_quality[i], xmlNodeMap );
	}

	for( int i = 0; i < QUEST_REW_ITEM; ++i )
	{
		stringstream ss;
		string str;
		ss << "RewChoicesItemID" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->rew_choice_item[i], xmlNodeMap );
		string tmp = str;
		tmp += "-value";
		SetQuestINT16(tmp, pQuestProto->rew_choice_item_num[i], xmlNodeMap );
		tmp = str;
		tmp += "-quality";
		SetQuestINT16(tmp, pQuestProto->rew_choice_quality[i], xmlNodeMap );
	}

	for( int i = 0; i < QUEST_ATTS_COUNT; ++i )
	{
		stringstream ss;
		string str;
		ss << "RewAtt" << i + 1 << endl;
		ss >> str;	
		SetQuestINT32(str, pQuestProto->rew_att[i], xmlNodeMap );
		string tmp = str;
		tmp += "-value";
		SetQuestINT16(tmp, pQuestProto->rew_att_val[i], xmlNodeMap );
	}

	for( int i = 0; i < QUEST_SKILLS_COUNT; ++i )
	{
		stringstream ss;
		string str;
		ss << "RewSkill" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->rew_skill[i], xmlNodeMap );
		string tmp = str;
		tmp += "-value";
		SetQuestINT32(tmp, pQuestProto->rew_skill_val[i], xmlNodeMap );
	}

	for( int i = 0; i < QUEST_REW_BUFF; ++i )
	{
		stringstream ss;
		string str;
		ss << "RewBuff" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->rew_buff[i], xmlNodeMap );
	}
}

// ----------------------------------------------------------------------------
// string ----> DWORD
// ----------------------------------------------------------------------------
void  QuestMgr::SetQuestDword(std::string str, DWORD& dwQuest, XmlNodeMap& xmlNodeMap)
{
	XmlNodeMap::iterator pos = xmlNodeMap.find(str);
	if ( pos != xmlNodeMap.end() )
	{
		dwQuest = atol(pos->second.c_str());
		//xmlNodeMap.erase(pos);
	}
}

// ----------------------------------------------------------------------------
// string ----> DWORD FLAGs
// ----------------------------------------------------------------------------
void  QuestMgr::SetQuestDwordFlags(std::string str, DWORD& dwQuest, INT32 nMask, XmlNodeMap& xmlNodeMap)
{
	XmlNodeMap::iterator pos = xmlNodeMap.find(str);
	if ( pos != xmlNodeMap.end() )
	{
		DWORD n = atol(pos->second.c_str());
		if(n != 0)
			dwQuest = dwQuest | nMask;
		xmlNodeMap.erase(pos);
	}
}

// ----------------------------------------------------------------------------
// string ----> UINT16
// ----------------------------------------------------------------------------
void  QuestMgr::SetQuestUINT16(std::string str, UINT16& unQuest, XmlNodeMap& xmlNodeMap)
{
	XmlNodeMap::iterator pos = xmlNodeMap.find(str);
	if ( pos != xmlNodeMap.end() )
	{
		unQuest = (UINT16)atoi(pos->second.c_str());
		xmlNodeMap.erase(pos);
	}
}

// ----------------------------------------------------------------------------
// string ----> BYTE
// ----------------------------------------------------------------------------
void  QuestMgr::SetQuestBYTE(std::string str, BYTE& byQuest, XmlNodeMap& xmlNodeMap)
{
	XmlNodeMap::iterator pos = xmlNodeMap.find(str);
	if ( pos != xmlNodeMap.end() )
	{
		byQuest = (BYTE)atoi(pos->second.c_str());
		xmlNodeMap.erase(pos);
	}
}

// ----------------------------------------------------------------------------
// string ----> INT16
// ----------------------------------------------------------------------------
void  QuestMgr::SetQuestINT16(std::string str, INT16& n16Quest, XmlNodeMap& xmlNodeMap)
{
	XmlNodeMap::iterator pos = xmlNodeMap.find(str);
	if ( pos != xmlNodeMap.end() )
	{
		n16Quest = (INT16)atoi(pos->second.c_str());
		xmlNodeMap.erase(pos);
	}
}

// ----------------------------------------------------------------------------
// string ----> INT32
// ----------------------------------------------------------------------------
void  QuestMgr::SetQuestINT32(std::string str, INT32& nQuest, XmlNodeMap& xmlNodeMap)
{
	XmlNodeMap::iterator pos = xmlNodeMap.find(str);
	if ( pos != xmlNodeMap.end() )
	{
		nQuest = atoi(pos->second.c_str());
		//xmlNodeMap.erase(pos);
	}
}

// ----------------------------------------------------------------------------
// string ----> wstring
// ----------------------------------------------------------------------------
void QuestMgr::SetQuestString(std::string str,TCHAR* szQuest, INT32 nLen, XmlNodeMap& xmlNodeMap)
{
	XmlNodeMap::iterator pos = xmlNodeMap.find(str);
	if ( pos != xmlNodeMap.end() )
	{
		_tcsncpy( szQuest, ToGBKW( pos->second.c_str() ).c_str(), nLen );
		xmlNodeMap.erase(pos);
	}
}

// ----------------------------------------------------------------------------
// string ----> BOOL
// ----------------------------------------------------------------------------
void  QuestMgr::SetQuestBool(std::string str, BOOL& bQuest, XmlNodeMap& xmlNodeMap)
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

// ----------------------------------------------------------------------------
// wstring ---> string
// ----------------------------------------------------------------------------
string QuestMgr::ToAnsi(const wstring &widestring)
{
	// Determine the length necessary for the string in UTF8
	int utf8_length = WideCharToMultiByte(CP_ACP, 0, widestring.c_str(), -1, NULL, 0, NULL, NULL);

	if (utf8_length == 0) return string(""); 

	vector<char> result(utf8_length);
	int length = WideCharToMultiByte(CP_ACP, 0, widestring.c_str(), -1, &result[0], utf8_length, NULL, NULL);

	if (length != utf8_length) return string(""); 

	return string(&result[0]);
}

// ----------------------------------------------------------------------------
// wstring ---> string utf8
// ----------------------------------------------------------------------------
string QuestMgr::ToUtf8(const wstring &widestring)
{
	// Determine the length necessary for the string in UTF8
	int utf8_length = WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, NULL, 0, NULL, NULL);

	if (utf8_length == 0) return string(""); 

	vector<char> result(utf8_length);
	int length = WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, &result[0], utf8_length, NULL, NULL);

	if (length != utf8_length) return string(""); 

	return string(&result[0]);
}

// ----------------------------------------------------------------------------
// string ---> wstring
// ----------------------------------------------------------------------------
wstring QuestMgr::ToGBKW(const char *utf8)
{
	int len=MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL,0);
	unsigned short * wszGBK = new unsigned short[len+1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, (LPWSTR)wszGBK, len);
	wstring wstr = (LPCWSTR)wszGBK;
	delete[] wszGBK;
	wszGBK = NULL;
	return wstr;
}

void QuestMgr::SaveEntry( TiXmlElement *pEle, LPCSTR szName, LPCTSTR szValue )
{
	TiXmlElement* entry;
	entry = new TiXmlElement( "entry" );
	pEle->LinkEndChild( entry );
	//entry->SetAttribute( "name", szName );
	string str = ToUtf8( szValue );
	entry->SetAttribute( szName, str.c_str() );
}

void QuestMgr::SaveEntry( TiXmlElement *pEle, LPCSTR szName, DWORD dwValue )
{
	TiXmlElement* entry;
	entry = new TiXmlElement( "entry" );
	pEle->LinkEndChild( entry );
	//entry->SetAttribute( "name", szName );
	entry->SetAttribute( szName, dwValue );
}

void QuestMgr::SaveEntry( TiXmlElement* pEle, LPCSTR szName, DWORD dwValue1, DWORD dwValue2 )
{
	TiXmlElement* entry;
	entry = new TiXmlElement( "entry" );
	pEle->LinkEndChild( entry );
	entry->SetAttribute( szName, dwValue1 );
	entry->SetAttribute( "value", dwValue2 );
}

void QuestMgr::SaveItem( TiXmlElement* pEle, LPCSTR szName, DWORD dwValue1, DWORD dwValue2, INT nValue3 )
{
	TiXmlElement* entry;
	entry = new TiXmlElement( "entry" );
	pEle->LinkEndChild( entry );
	entry->SetAttribute( szName, dwValue1 );
	entry->SetAttribute( "value", dwValue2 );
	entry->SetAttribute( "quality", nValue3 );
}

void QuestMgr::SaveMaxEntry( TiXmlElement* pEle, LPCSTR szName, DWORD dwValue, DWORD dwMax, DWORD dwMin )
{
	TiXmlElement* entry;
	entry = new TiXmlElement( "entry" );
	pEle->LinkEndChild( entry );
	entry->SetAttribute( szName, dwValue );
	entry->SetAttribute( "max", dwMax );
	entry->SetAttribute( "min", dwMin );
}

void QuestMgr::SaveCreatureEntry( TiXmlElement* pEle, LPCSTR szName, DWORD dwValue, DWORD dwID2, DWORD dwID3, DWORD dwNum )
{
	TiXmlElement* entry;
	entry = new TiXmlElement( "entry" );
	pEle->LinkEndChild( entry );
	entry->SetAttribute( szName, dwValue );
	entry->SetAttribute( "id2", dwID2 );
	entry->SetAttribute( "id3", dwID3 );
	entry->SetAttribute( "value", dwNum );
}

void QuestMgr::AddQuestToMap()
{
	if (m_pQuestTmp == NULL)
		return;
	if (m_pQuestTmp->pQuestProto == NULL)
		return;

	m_mapQuest.insert(make_pair(m_pQuestTmp->pQuestProto->id, m_pQuestTmp));
	m_pQuestTmp = NULL;
}

void QuestMgr::SaveAcceptQuestRelation(LPCTSTR pPath)
{
	//key�����ظ�
	multimap< DWORD, DWORD > mapQuestAcceptRelation;
	DWORD npcID;

	QuestMap::iterator it = m_mapQuest.begin();
	for (; it != m_mapQuest.end(); ++it)
	{
		tagQuestProto* pQuestProto = it->second->pQuestProto;
		if (pQuestProto == NULL)
			continue;
		for ( int i = 0; i < QUEST_ACCEPT_NPC; ++i )
		{
			npcID = pQuestProto->accept_quest_npc[i];
			if ( npcID != 0 )
			{
				mapQuestAcceptRelation.insert( make_pair( npcID, pQuestProto->id ) );
			}
		}
	}

	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "UTF-8", "" );
	doc.LinkEndChild( decl );

	TiXmlElement* root = new TiXmlElement( "accept-quest-relation" );
	doc.LinkEndChild( root );

	multimap< DWORD, DWORD >::iterator it2 = mapQuestAcceptRelation.begin();
	for ( ; it2 != mapQuestAcceptRelation.end(); ++it2 )
	{
		TiXmlElement* entry;
		entry = new TiXmlElement( "entry" );
		root->LinkEndChild( entry );
		entry->SetAttribute( "NPCID",  it2->first );
		entry->SetAttribute( "QuestID",  it2->second );
	}
	wstring strPath =  pPath;
	strPath += _T("\\");
	strPath += _T("accept_quest_npc.data");
	string str = ToAnsi(strPath.c_str());
	doc.SaveFile( str.c_str() );
}

void QuestMgr::SaveCompleteQuestRelation(LPCTSTR pPath)
{
	//key�����ظ�
	multimap< DWORD, DWORD > mapQuestCompleteRelation;
	DWORD npcID;

	QuestMap::iterator it = m_mapQuest.begin();
	for (; it != m_mapQuest.end(); ++it)
	{
		tagQuestProto* pQuestProto = it->second->pQuestProto;
		if (pQuestProto == NULL)
			continue;
		for ( int i = 0; i < QUEST_COMPLETE_NPC; ++i )
		{
			npcID = pQuestProto->complete_quest_npc[i];
			if ( npcID != 0 )
			{
				mapQuestCompleteRelation.insert( make_pair( npcID, pQuestProto->id ) );
			}
		}
	}

	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "UTF-8", "" );
	doc.LinkEndChild( decl );

	TiXmlElement* root = new TiXmlElement( "complete-quest-relation" );
	doc.LinkEndChild( root );

	multimap< DWORD, DWORD >::iterator it2 = mapQuestCompleteRelation.begin();
	for ( ; it2 != mapQuestCompleteRelation.end(); ++it2 )
	{
		TiXmlElement* entry;
		entry = new TiXmlElement( "entry" );
		root->LinkEndChild( entry );
		entry->SetAttribute( "NPCID",  it2->first );
		entry->SetAttribute( "QuestID",  it2->second );
	}
	wstring strPath =  pPath;
	strPath += _T("\\");
	strPath += _T("complete_quest_npc.data");
	string str = ToAnsi(strPath.c_str());
	doc.SaveFile( str.c_str() );
}

// ----------------------------------------------------------------------------
// �����������ѡ���е�8��NPC
// ----------------------------------------------------------------------------
void QuestMgr::SaveQuestNPCRelation(LPCTSTR pPath)
{
	//key�����ظ�
	multimap< DWORD, DWORD > mapQuestNPCRelation;
	DWORD npcID;

	QuestMap::iterator it = m_mapQuest.begin();
	for (; it != m_mapQuest.end(); ++it)
	{
		tagQuestProto* pQuestProto = it->second->pQuestProto;
		if (pQuestProto == NULL)
			continue;
	
		for ( int i = 0; i < QUEST_NPC_COUNT; ++i )
		{
			npcID = pQuestProto->complete_req_npc[i];
			if ( npcID != 0 )
			{
				mapQuestNPCRelation.insert( make_pair( npcID, pQuestProto->id ) );
			}
		}
	}

	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "UTF-8", "" );
	doc.LinkEndChild( decl );

	TiXmlElement* root = new TiXmlElement( "quest-npc-relation" );
	doc.LinkEndChild( root );

	multimap< DWORD, DWORD >::iterator it2 = mapQuestNPCRelation.begin();
	for ( ; it2 != mapQuestNPCRelation.end(); ++it2 )
	{
		TiXmlElement* entry;
		entry = new TiXmlElement( "entry" );
		root->LinkEndChild( entry );
		entry->SetAttribute( "NPCID",  it2->first );
		entry->SetAttribute( "QuestID",  it2->second );
	}
	wstring strPath =  pPath;
	strPath += _T("\\");
	strPath += _T("quest_npc.data");
	string str = ToAnsi(strPath.c_str());
	doc.SaveFile( str.c_str() );
}


// ----------------------------------------------------------------------------
// //����ļ���չ���Ƿ�ƥ��
// ----------------------------------------------------------------------------
bool QuestMgr::IsExtensionOK(LPCTSTR pFileName, LPCTSTR pExt)
{
	wstring file(pFileName);
	wstring ext(pExt);

	size_t pos = file.rfind(_T("."));
	if (pos != wstring::npos)
	{
		return file.substr(pos + 1) == ext;
	}
	else
	{
		return false;
	}
}