#include "stdafx.h"
#include "QuestMgr.h"
#include "..\WorldDefine\Questdef.h"


QuestMgr::QuestMgr()
{
}

QuestMgr::~QuestMgr()
{
}

void QuestMgr::SaveEntry( TiXmlElement *pEle, LPCSTR szName, LPCTSTR szValue )
{
	TiXmlElement* entry;
	entry = new TiXmlElement( "entry" );
	pEle->LinkEndChild( entry );
	//entry->SetAttribute( "name", szName );
	string str = WChar2Ansi( szValue );
	string tmp = ConvertGBKToUtf8( str );
	entry->SetAttribute( szName, tmp.c_str() );
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

void QuestMgr::SaveMaxEntry( TiXmlElement* pEle, LPCSTR szName, DWORD dwValue, DWORD dwMax, DWORD dwMin )
{
	TiXmlElement* entry;
	entry = new TiXmlElement( "entry" );
	pEle->LinkEndChild( entry );
	entry->SetAttribute( szName, dwValue );
	entry->SetAttribute( "max", dwMax );
	entry->SetAttribute( "min", dwMin );
}

void QuestMgr::SaveQuestToXml( tagQuestProto* pQuest, LPCTSTR szFileName )
{
	if ( pQuest == NULL )
	{
		return;
	}

	if ( szFileName == NULL )
	{
		return;
	}

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
		SaveEntry( propertyNode, "ID", pQuest->dwQuestId );
		//��������
		SaveEntry( propertyNode, "Name", pQuest->szName );
		//ǰһ������ID
		SaveEntry( propertyNode, "PrevQuestID", pQuest->dwPrevQuestId );
		//��������ID
		SaveEntry( propertyNode, "NextQuestID", pQuest->dwNextQuestId );
		//��ǰ������ɺ󣬺��������Ƿ��Զ����ص��������
		SaveEntry( propertyNode, "AutoAddQuest", pQuest->bAutoAddQuest );
		//�����ȡ����--���������ȡ
		SaveEntry( propertyNode, "QuestPlayerAccept", pQuest->bQuestPlayerAccept );
		//�����ȡ����--ϵͳ����
		SaveEntry( propertyNode, "QuestSystemRelease", pQuest->bQuestSystemRelease );
		//�����ȡ����--����Ʒ��ȡ
		SaveEntry( propertyNode, "QuestItemAccept", pQuest->bQuestItemAccept );
		//��������
		SaveEntry( propertyNode, "Conditions", pQuest->szConditions );
		//�������
		SaveEntry( propertyNode, "Overview", pQuest->szOverview );
		//��������
		SaveEntry( propertyNode, "QuestType", pQuest->eQuestType );
		//������Ҹ���
		SaveEntry( propertyNode, "SuggestedPlayers", pQuest->dwSuggestedPlayers );
		//������ɵ�����ʱ��
		SaveEntry( propertyNode, "LimitTime", pQuest->dwLimitTime );
		//ÿ����ҽ�ȡ�������
		SaveEntry( propertyNode, "DoQuestTimes", pQuest->dwDoQuestTimes );
		//�Ƿ�����������
		SaveEntry( propertyNode, "Periodic", pQuest->bPeriodic );
		//�����������ÿ�� ÿ�£�
		SaveEntry( propertyNode, "QuestPeriodic", pQuest->eQuestPeriodic );
		//��������
		SaveEntry( propertyNode, "Week", pQuest->eWeek );
		//�����Ƿ�����ظ�
		SaveEntry( propertyNode, "Repeatable", pQuest->bRepeatable );
		//����ʼ�ű�ID
		SaveEntry( propertyNode, "QuestStartScriptID", pQuest->QuestStartScriptID );
		//��������ű�ID
		SaveEntry( propertyNode, "QuestCompleteScriptID", pQuest->QuestCompleteScriptID );
		//�����ȡNPCID
		SaveEntry( propertyNode, "AcceptQuestNPCID1", pQuest->dwAcceptQuestNPCID[0] );
		SaveEntry( propertyNode, "AcceptQuestNPCID2", pQuest->dwAcceptQuestNPCID[1] );
		SaveEntry( propertyNode, "AcceptQuestNPCID3", pQuest->dwAcceptQuestNPCID[2] );
		SaveEntry( propertyNode, "AcceptQuestNPCID4", pQuest->dwAcceptQuestNPCID[3] );
		SaveEntry( propertyNode, "AcceptQuestNPCID5", pQuest->dwAcceptQuestNPCID[4] );
		SaveEntry( propertyNode, "AcceptQuestNPCID6", pQuest->dwAcceptQuestNPCID[5] );

		//�������NPCID
		SaveEntry( propertyNode, "CompleteQuestNPCID1", pQuest->dwCompleteQuestNPCID[0] );
		SaveEntry( propertyNode, "CompleteQuestNPCID2", pQuest->dwCompleteQuestNPCID[1] );
		SaveEntry( propertyNode, "CompleteQuestNPCID3", pQuest->dwCompleteQuestNPCID[2] );
		SaveEntry( propertyNode, "CompleteQuestNPCID4", pQuest->dwCompleteQuestNPCID[3] );
		SaveEntry( propertyNode, "CompleteQuestNPCID5", pQuest->dwCompleteQuestNPCID[4] );
		SaveEntry( propertyNode, "CompleteQuestNPCID6", pQuest->dwCompleteQuestNPCID[5] );

		//������ƷID������
		SaveEntry( propertyNode, "SrcItemID", pQuest->dwSrcItemID, pQuest->dwSrcItemCount );
	}

	//block quest accept-conditions
	{
		TiXmlElement* acceptNode = new TiXmlElement( "accept-conditions" );
		root->LinkEndChild( acceptNode );

		//��ȡ��������Ҫ�����ȼ�
		SaveEntry( acceptNode, "GetQuestRequiredMaxLevel", pQuest->dwGetQuestRequiredMaxLevel );
		//��ȡ��������Ҫ����С�ȼ�
		SaveEntry( acceptNode, "GetQuestRequiredMinLevel", pQuest->dwGetQuestRequiredMinLevel );
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
			SaveMaxEntry( acceptNode, str.c_str(), pQuest->dwGetQuestRequiredRep[i], pQuest->dwGetQuestRequiredMinRepValue[i], 
													pQuest->dwGetQuestRequiredMaxRepValue[i] );
		}
		//��ȡ��������Ҫ���Ա�
		SaveEntry( acceptNode, "GetQuestRequiredSex", pQuest->dwGetQuestRequiredSex );
		//��ȡ��������Ҫ��ְҵ	
		SaveEntry( acceptNode, "GetQuestRequiredClass", pQuest->dwGetQuestRequiredClass );
		//��ȡ��������Ҫ�ĵ�ͼ
		SaveEntry( acceptNode, "GetQuestRequiredMapID", pQuest->dwGetQuestRequiredMapID );
		//��ȡ��������Ҫ�ļ���,����ֵ
		for ( int i = 0; i < QUEST_SKILLS_COUNT; ++i )
		{
			stringstream ss;
			string str;
			ss << "GetQuestRequiredSkill" << i + 1 << endl;
			ss >> str;
			//SaveEntry( acceptNode, str.c_str(), pQuest->dwGetQuestRequiredSkill[i] );
			//SaveEntry( acceptNode, "GetQuestRequiredSkillValue", pQuest->dwGetQuestRequiredSkillValue[i] );
			SaveEntry( acceptNode, str.c_str(), pQuest->dwGetQuestRequiredSkill[i], pQuest->dwGetQuestRequiredSkillValue[i] );
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
			SaveEntry( acceptNode, str.c_str(), pQuest->dwGetQuestRequiredItemID[i], pQuest->dwGetQuestRequiredItemCount[i] );
		}
		//������Ʒ�Ƿ�ɾ��
		SaveEntry( acceptNode, "DelRestrictedItem", pQuest->bDelRestrictedItem );
		
		//��ȡ�����������������,ֵ
		for ( int i = 0; i < QUEST_ATTS_COUNT; ++i )
		{
			stringstream ss;
			string str;
			ss << "GetQuestRequiredAtt" << i + 1 << endl;
			ss >> str;	
			SaveEntry( acceptNode, str.c_str(), pQuest->dwGetQuestRequiredAtt[i], pQuest->dwGetQuestRequiredAttValue[i] );
		}
		
		//��ȡ�������败����
		for ( int i = 0; i < QUEST_TRIGGERS_COUNT; ++i )
		{
			stringstream ss;
			string str;
			ss << "GetQuestReqTriggersID" << i + 1 << endl;
			ss >> str;	
			SaveEntry( acceptNode, str.c_str(), pQuest->dwGetQuestReqTriggersID[i] );
		}

		//�Ƿ��Ƿ���
		SaveEntry( acceptNode, "GetQuestReqMarried", pQuest->bGetQuestReqMarried );
		//���¸���
		SaveEntry( acceptNode, "GetQuestReqLoverNum", pQuest->dwGetQuestReqLoverNum );
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
			SaveEntry( completeNode, str.c_str(), pQuest->dwCompleteQuestRequiredItemID[i], pQuest->dwCompleteQuestRequiredItemIDCount[i] );
		}
		//�Ƿ��Ѽ�������һ�־Ϳ����
		SaveEntry( completeNode, "RequiredOnlyOneItem", pQuest->bRequiredOnlyOneItem );
		//���������Ƿ�ɾ����Ʒ
		SaveEntry( completeNode, "DeleteItem", pQuest->bDeleteItem );

		//�����������Ҫɱ�Ĺ�,����
		for ( int i = 0; i < QUEST_CREATURES_COUNT; ++i )
		{
			stringstream ss;
			string str;
			ss << "CompleteQuestRequiredCreatureID" << i + 1 << endl;
			ss >> str;
			//SaveEntry( completeNode, str.c_str(), pQuest->dwCompleteQuestRequiredCreatureID[i] );
			//SaveEntry( completeNode, "CompleteQuestRequiredCreatureCount", pQuest->dwCompleteQuestRequiredCreatureCount[i] );
			SaveEntry( completeNode, str.c_str(), pQuest->dwCompleteQuestRequiredCreatureID[i], pQuest->dwCompleteQuestRequiredCreatureCount[i] );
		}
		//�Ƿ��ɱ��ָ��������ͬһ�ȼ�����Ϳ����
		SaveEntry( completeNode, "RequiredCreatureLevel", pQuest->bRequiredCreatureLevel );
		//�Ƿ��ɱ��ָ��������һ�ֹ���Ϳ����
		SaveEntry( completeNode, "RequiredOnlyOneCreature", pQuest->bRequiredOnlyOneCreature );
		//�����������Ҫ�Ի���NPC����
		for ( int i = 0; i < QUEST_NPC_COUNT; ++i )
		{
			stringstream ss;
			string str;
			ss << "CompleteQuestRequiredNPC" << i + 1 << endl;
			ss >> str;
			SaveEntry( completeNode, str.c_str(), pQuest->dwCompleteQuestRequiredNPC[i] );
		}
		//�Ƿ���밴˳��Ի�
		SaveEntry( completeNode, "RequiredOnlyInOrder", pQuest->bRequiredOnlyInOrder );
		//�Ƿ�ֻ��һ��NPC�Ի���Ϳ����
		SaveEntry( completeNode, "RequiredOnlyOneNPC", pQuest->bRequiredOnlyOneNPC );
		//�����������Ҫ�Ĵ�����ID
		for ( int i = 0; i < QUEST_TRIGGERS_COUNT; ++i )
		{
			stringstream ss;
			string str;
			ss << "CompleteQuestReqTriggersID" << i + 1 << endl;
			ss >> str;
			SaveEntry( completeNode, str.c_str(), pQuest->dwCompleteQuestReqTriggersID[i] );
		}
		//�Ƿ�ֻ����һ���Ϳ����
		SaveEntry( completeNode, "RequiredOnlyOneTrigger", pQuest->bRequiredOnlyOneTrigger );
		//�����������Ҫ�Ľ�Ǯ
		SaveEntry( completeNode, "CompleteQuestRequiredMoney", pQuest->dwCompleteQuestRequiredMoney );
		//�����������Ҫ�ĵ�ͼID
		SaveEntry( completeNode, "CompleteQuestRequiredMapID", pQuest->dwCompleteQuestRequiredMapID );
		//�����������Ҫ�ĵ�ͼX����
		//��float ת�����ַ���
		wstringstream ss;
		wstring wstr;
		ss << pQuest->fCompleteQuestRequiredMap_X << endl;
		ss >> wstr;
		SaveEntry( completeNode, "CompleteQuestRequiredMap_X", wstr.c_str() );
		//�����������Ҫ�ĵ�ͼY����
		ss.clear();
		ss << pQuest->fCompleteQuestRequiredMap_Y << endl;
		ss >> wstr;
		SaveEntry( completeNode, "CompleteQuestRequiredMap_Y", wstr.c_str() );
		//�����������Ҫ�ĵ�ͼZ����
		ss.clear();
		ss << pQuest->fCompleteQuestRequiredMap_Z << endl;
		ss >> wstr;
		SaveEntry( completeNode, "CompleteQuestRequiredMap_Z", wstr.c_str() );
		//�����������Ҫ�ĵ�ͼ�뾶
		ss.clear();
		ss << pQuest->fCompleteQuestRequiredMap_Radius << endl;
		ss >> wstr;
		SaveEntry( completeNode, "CompleteQuestRequiredMap_Radius", wstr.c_str() );
		//��������Ƿ��������
		SaveEntry( completeNode, "CompleteQuestRequiredMap_Random", pQuest->bCompleteQuestRequiredMap_Random );
		//�¼�
		SaveEntry( completeNode, "CompleteQuestEventType", pQuest->eCompleteQuestEventType );
		SaveEntry( completeNode, "UseItemEvent_Item_ID", pQuest->dwUseItemEvent_Item_ID );
		SaveEntry( completeNode, "UseSkillEvent_Skill_ID", pQuest->dwUseSkillEvent_Skill_ID );

		//�Ƿ����������������һ��������������
		SaveEntry( completeNode, "RequiredOnlyOneCondition", pQuest->bRequiredOnlyOneCondition );

		//�Ƿ��Ƿ���
		SaveEntry( completeNode, "CompleteQuestReqMarried", pQuest->bCompleteQuestReqMarried );

		//���¸���
		SaveEntry( completeNode, "CompleteQuestReqLoverNum", pQuest->dwCompleteQuestReqLoverNum );

		//�ȼ�
		SaveEntry( completeNode, "CompleteQuestReqLevel", pQuest->dwCompleteQuestReqLevel );
	}

	//block quest reward
	{
		TiXmlElement* rewardNode = new TiXmlElement( "reward" );
		root->LinkEndChild( rewardNode );
		
		//������ɺ󣬽����Ľ�Ǯ
		SaveEntry( rewardNode, "RewMoney", pQuest->dwRewMoney );
		//������ɺ󣬽����ľ���ֵ
		SaveEntry( rewardNode, "RewXP", pQuest->dwRewXP );
		//������������,����ֵ
		for ( int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i )
		{
			stringstream ss;
			string str;
			ss << "RewRequiredRep" << i + 1 << endl;
			ss >> str;
			//SaveEntry( rewardNode, str.c_str(), pQuest->dwRewRequiredRep[i] );
			//SaveEntry( rewardNode, "RewRequiredRepValue", pQuest->dwRewRequiredRepValue[i] );
			SaveEntry( rewardNode, str.c_str(), pQuest->dwRewRequiredRep[i], pQuest->dwRewRequiredRepValue[i] );
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
			SaveEntry( rewardNode, str.c_str(), pQuest->dwRewItemID[i], pQuest->dwRewItemCount[i] );
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
			SaveEntry( rewardNode, str.c_str(), pQuest->dwRewChoicesItemID[i], pQuest->dwRewChoicesItemCount[i] );
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
			SaveEntry( rewardNode, str.c_str(), pQuest->dwRewAtt[i], pQuest->dwRewAttValue[i] );
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
			SaveEntry( rewardNode, str.c_str(), pQuest->dwRewSkill[i], pQuest->dwRewSkillValue[i] );
		}
		//��������buff
		for ( int i = 0; i < QUEST_REW_BUFF; ++i )
		{
			stringstream ss;
			string str;
			ss << "RewBuff" << i + 1 << endl;
			ss >> str;
			SaveEntry( rewardNode, str.c_str(), pQuest->dwRewBuff[i] );
		}
	}

	string strFileName = WChar2Ansi( szFileName );
	doc.SaveFile( strFileName.c_str() );
}

BOOL QuestMgr::LoadQuestFromXml( tagQuestProto* pQuest, LPCTSTR szFileName )
{
	if (!pQuest) return FALSE;

	if (!szFileName) return FALSE;

	string strFileName = WChar2Ansi(szFileName);

	//����XML
	TiXmlDocument doc(strFileName.c_str());
	if (doc.LoadFile() == false) return FALSE;

	TiXmlHandle hDoc(&doc);
	TiXmlHandle hRoot(0);

	//�Ƿ��������ļ�
	TiXmlElement *pElem = hDoc.FirstChildElement("quest").Element();
	if (!pElem) return FALSE;

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

	SetQuestProto(pQuest, xmlNode);
	return TRUE;	
}

BOOL QuestMgr::LoadXmlNode(TiXmlElement* pNode, XmlNodeMap& xmlNodeMap)
{
	//�Ƿ���ڸýڵ�
	if (! pNode) return FALSE;

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
	return TRUE;
}

BOOL QuestMgr::SetQuestProto(tagQuestProto* pQuestProto, XmlNodeMap& xmlNodeMap)
{
	//property
	SetQuestDword("ID", pQuestProto->dwQuestId, xmlNodeMap);
	SetQuestString("Name", pQuestProto->szName, xmlNodeMap );
	SetQuestDword("PrevQuestID", pQuestProto->dwPrevQuestId, xmlNodeMap );
	SetQuestDword("NextQuestID", pQuestProto->dwNextQuestId, xmlNodeMap );
	SetQuestBool("AutoAddQuest", pQuestProto->bAutoAddQuest, xmlNodeMap );
	SetQuestBool("QuestPlayerAccept", pQuestProto->bQuestPlayerAccept, xmlNodeMap );
	SetQuestBool("QuestSystemRelease", pQuestProto->bQuestSystemRelease, xmlNodeMap );
	SetQuestBool("QuestItemAccept", pQuestProto->bQuestItemAccept, xmlNodeMap );
	SetQuestString("Conditions", pQuestProto->szConditions, xmlNodeMap );
	SetQuestString("Overview", pQuestProto->szOverview, xmlNodeMap );
	SetQuestDword("QuestType", (DWORD&)pQuestProto->eQuestType, xmlNodeMap );
	SetQuestDword("SuggestedPlayers", pQuestProto->dwSuggestedPlayers, xmlNodeMap );
	SetQuestDword("LimitTime", pQuestProto->dwLimitTime, xmlNodeMap );
	SetQuestDword("DoQuestTimes", pQuestProto->dwDoQuestTimes, xmlNodeMap );
	SetQuestBool("Periodic", pQuestProto->bPeriodic, xmlNodeMap );
	SetQuestDword("QuestPeriodic", (DWORD&)pQuestProto->eQuestPeriodic, xmlNodeMap );
	SetQuestDword("Week", (DWORD&)pQuestProto->eWeek, xmlNodeMap );
	SetQuestBool("Repeatable", pQuestProto->bRepeatable, xmlNodeMap );
	SetQuestDword("QuestStartScriptID", pQuestProto->QuestStartScriptID, xmlNodeMap );
	SetQuestDword("QuestCompleteScriptID", pQuestProto->QuestCompleteScriptID, xmlNodeMap );
	for( int i = 0; i < 6 ; ++i )
	{
		stringstream ss;
		string str;
		ss << "AcceptQuestNPCID" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->dwAcceptQuestNPCID[i], xmlNodeMap );
	}
	
	for( int i = 0; i < 6 ; ++i )
	{
		stringstream ss;
		string str;
		ss << "CompleteQuestNPCID" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->dwCompleteQuestNPCID[i], xmlNodeMap );
	}
	
	SetQuestDword("SrcItemID", pQuestProto->dwSrcItemID, xmlNodeMap );
	SetQuestDword("SrcItemID-value", pQuestProto->dwSrcItemCount, xmlNodeMap );

	//accept-conditions
	SetQuestDword("GetQuestRequiredMaxLevel", pQuestProto->dwGetQuestRequiredMaxLevel, xmlNodeMap);
	SetQuestDword("GetQuestRequiredMinLevel", pQuestProto->dwGetQuestRequiredMinLevel, xmlNodeMap);
	for( int i = 0; i < 4 ; ++i )
	{
		stringstream ss;
		string str;
		ss << "GetQuestRequiredRep" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->dwGetQuestRequiredRep[i], xmlNodeMap );
		string tmp = str;
		tmp += "-max";
		SetQuestDword(str, pQuestProto->dwGetQuestRequiredMaxRepValue[i], xmlNodeMap );
		tmp = str;
		tmp += "-min";
		SetQuestDword(str, pQuestProto->dwGetQuestRequiredMinRepValue[i], xmlNodeMap );
	}
	for( int i = 0; i < 6 ; ++i )
	{
		stringstream ss;
		string str;
		ss << "GetQuestRequiredSkill" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->dwGetQuestRequiredSkill[i], xmlNodeMap );
		string tmp = str;
		tmp += "-value";
		SetQuestDword(tmp, pQuestProto->dwGetQuestRequiredSkillValue[i], xmlNodeMap );
	}
	SetQuestDword("GetQuestRequiredSex", pQuestProto->dwGetQuestRequiredSex, xmlNodeMap);
	SetQuestDword("GetQuestRequiredClass", pQuestProto->dwGetQuestRequiredClass, xmlNodeMap);
	SetQuestDword("GetQuestRequiredMapID", pQuestProto->dwGetQuestRequiredMapID, xmlNodeMap);
	for( int i = 0; i < QUEST_ITEMS_COUNT ; ++i )
	{
		stringstream ss;
		string str;
		ss << "GetQuestRequiredItemID" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->dwGetQuestRequiredItemID[i], xmlNodeMap );
		string tmp = str;
		tmp += "-value";
		SetQuestDword(tmp, pQuestProto->dwGetQuestRequiredItemCount[i], xmlNodeMap );
	}
	for( int i = 0; i < QUEST_ATTS_COUNT ; ++i )
	{
		stringstream ss;
		string str;
		ss << "GetQuestRequiredAtt" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->dwGetQuestRequiredAtt[i], xmlNodeMap );
		string tmp = str;
		tmp += "-value";
		SetQuestDword(tmp, pQuestProto->dwGetQuestRequiredAttValue[i], xmlNodeMap );
	}
	SetQuestBool("DelRestrictedItem", pQuestProto->bDelRestrictedItem, xmlNodeMap);
	for( int i = 0; i < QUEST_TRIGGERS_COUNT ; ++i )
	{
		stringstream ss;
		string str;
		ss << "GetQuestReqTriggersID" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->dwGetQuestReqTriggersID[i], xmlNodeMap );
	}
	SetQuestBool("GetQuestReqMarried", pQuestProto->bGetQuestReqMarried, xmlNodeMap);
	SetQuestDword("GetQuestReqLoverNum", pQuestProto->dwGetQuestReqLoverNum, xmlNodeMap);
	
	//complete-conditions
	for( int i = 0; i < QUEST_ITEMS_COUNT; ++i )
	{
		stringstream ss;
		string str;
		ss << "CompleteQuestRequiredItemID" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->dwCompleteQuestRequiredItemID[i], xmlNodeMap );
		string tmp = str;
		tmp += "-value";
		SetQuestDword(tmp, pQuestProto->dwCompleteQuestRequiredItemIDCount[i], xmlNodeMap );
	}
	SetQuestBool("RequiredOnlyOneItem", pQuestProto->bRequiredOnlyOneItem, xmlNodeMap);
	SetQuestBool("DeleteItem", pQuestProto->bDeleteItem, xmlNodeMap);

	for( int i = 0; i < QUEST_CREATURES_COUNT; ++i )
	{
		stringstream ss;
		string str;
		ss << "CompleteQuestRequiredCreatureID" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->dwCompleteQuestRequiredCreatureID[i], xmlNodeMap );
		string tmp = str;
		tmp += "-value";
		SetQuestDword(tmp, pQuestProto->dwCompleteQuestRequiredCreatureCount[i], xmlNodeMap );
	}
	SetQuestBool("RequiredCreatureLevel", pQuestProto->bRequiredCreatureLevel, xmlNodeMap);
	SetQuestBool("RequiredOnlyOneCreature", pQuestProto->bRequiredOnlyOneCreature, xmlNodeMap);

	for( int i = 0; i < QUEST_NPC_COUNT ; ++i )
	{
		stringstream ss;
		string str;
		ss << "CompleteQuestRequiredNPC" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->dwCompleteQuestRequiredNPC[i], xmlNodeMap );
	}

	SetQuestBool("RequiredOnlyInOrder", pQuestProto->bRequiredOnlyInOrder, xmlNodeMap);
	SetQuestBool("RequiredOnlyOneNPC", pQuestProto->bRequiredOnlyOneNPC, xmlNodeMap);

	for( int i = 0; i < QUEST_TRIGGERS_COUNT; ++i )
	{
		stringstream ss;
		string str;
		ss << "CompleteQuestReqTriggersID" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->dwCompleteQuestReqTriggersID[i], xmlNodeMap );
	}
	SetQuestBool("RequiredOnlyOneTrigger", pQuestProto->bRequiredOnlyOneTrigger, xmlNodeMap);

	SetQuestDword("CompleteQuestRequiredMoney", pQuestProto->dwCompleteQuestRequiredMoney, xmlNodeMap);
	SetQuestDword("CompleteQuestRequiredMapID", pQuestProto->dwCompleteQuestRequiredMapID, xmlNodeMap);
	SetQuestFloat("CompleteQuestRequiredMap_X", pQuestProto->fCompleteQuestRequiredMap_X, xmlNodeMap);
	SetQuestFloat("CompleteQuestRequiredMap_Y", pQuestProto->fCompleteQuestRequiredMap_Y, xmlNodeMap);
	SetQuestFloat("CompleteQuestRequiredMap_Z", pQuestProto->fCompleteQuestRequiredMap_Z, xmlNodeMap);
	SetQuestFloat("CompleteQuestRequiredMap_Radius", pQuestProto->fCompleteQuestRequiredMap_Radius, xmlNodeMap);

	SetQuestBool("CompleteQuestRequiredMap_Random", pQuestProto->bCompleteQuestRequiredMap_Random, xmlNodeMap);
	SetQuestBool("RequiredOnlyOneCondition", pQuestProto->bRequiredOnlyOneCondition, xmlNodeMap);

	SetQuestBool("CompleteQuestReqMarried", pQuestProto->bCompleteQuestReqMarried, xmlNodeMap);
	SetQuestDword("CompleteQuestReqLoverNum", pQuestProto->dwCompleteQuestReqLoverNum, xmlNodeMap);
	SetQuestDword("CompleteQuestReqLevel", pQuestProto->dwCompleteQuestReqLevel, xmlNodeMap);

	SetQuestDword("CompleteQuestEventType", (DWORD&)pQuestProto->eCompleteQuestEventType, xmlNodeMap );
	SetQuestDword("UseItemEvent_Item_ID", pQuestProto->dwUseItemEvent_Item_ID, xmlNodeMap);
	SetQuestDword("UseSkillEvent_Skill_ID", pQuestProto->dwUseSkillEvent_Skill_ID, xmlNodeMap);
	//Reword
	SetQuestDword("RewMoney", pQuestProto->dwRewMoney, xmlNodeMap);
	SetQuestDword("RewXP", pQuestProto->dwRewXP, xmlNodeMap);
	
	for( int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i )
	{
		stringstream ss;
		string str;
		ss << "RewRequiredRep" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->dwRewRequiredRep[i], xmlNodeMap );
		string tmp = str;
		tmp += "-value";
		SetQuestDword(tmp, pQuestProto->dwRewRequiredRepValue[i], xmlNodeMap );
	}	

	for( int i = 0; i < QUEST_REW_ITEM; ++i )
	{
		stringstream ss;
		string str;
		ss << "RewItemID" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->dwRewItemID[i], xmlNodeMap );
		string tmp = str;
		tmp += "-value";
		SetQuestDword(tmp, pQuestProto->dwRewItemCount[i], xmlNodeMap );
	}

	for( int i = 0; i < QUEST_REW_ITEM; ++i )
	{
		stringstream ss;
		string str;
		ss << "RewChoicesItemID" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->dwRewChoicesItemID[i], xmlNodeMap );
		string tmp = str;
		tmp += "-value";
		SetQuestDword(tmp, pQuestProto->dwRewChoicesItemCount[i], xmlNodeMap );
	}

	for( int i = 0; i < QUEST_ATTS_COUNT; ++i )
	{
		stringstream ss;
		string str;
		ss << "RewAtt" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->dwRewAtt[i], xmlNodeMap );
		string tmp = str;
		tmp += "-value";
		SetQuestDword(tmp, pQuestProto->dwRewAttValue[i], xmlNodeMap );
	}

	for( int i = 0; i < QUEST_SKILLS_COUNT; ++i )
	{
		stringstream ss;
		string str;
		ss << "RewSkill" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->dwRewSkill[i], xmlNodeMap );
		string tmp = str;
		tmp += "-value";
		SetQuestDword(tmp, pQuestProto->dwRewSkillValue[i], xmlNodeMap );
	}

	for( int i = 0; i < QUEST_REW_BUFF; ++i )
	{
		stringstream ss;
		string str;
		ss << "RewBuff" << i + 1 << endl;
		ss >> str;	
		SetQuestDword(str, pQuestProto->dwRewBuff[i], xmlNodeMap );
	}

	return TRUE;
}

void  QuestMgr::SetQuestDword(std::string str, DWORD& dwQuest, XmlNodeMap& xmlNodeMap)
{
	XmlNodeMap::iterator pos = xmlNodeMap.find(str);
	if ( pos != xmlNodeMap.end() )
	{
		dwQuest = atol(pos->second.c_str());
	}
}

void QuestMgr::SetQuestString(std::string str,TCHAR* szQuest, XmlNodeMap& xmlNodeMap)
{
	XmlNodeMap::iterator pos = xmlNodeMap.find(str);
	if ( pos != xmlNodeMap.end() )
	{
		_tcsncpy( szQuest, ConvertUtf8ToGBKW( pos->second.c_str() ).c_str(), 36 );
	}
}

void  QuestMgr::SetQuestBool(std::string str, BOOL& bQuest, XmlNodeMap& xmlNodeMap)
{
	XmlNodeMap::iterator pos = xmlNodeMap.find(str);
	if ( pos != xmlNodeMap.end() )
	{
		bQuest = atol(pos->second.c_str());
	}
}

void  QuestMgr::SetQuestFloat(std::string str, FLOAT& fQuest, XmlNodeMap& xmlNodeMap)
{
	XmlNodeMap::iterator pos = xmlNodeMap.find(str);
	if ( pos != xmlNodeMap.end() )
	{
		fQuest = (FLOAT)atof(pos->second.c_str());
	}
}
