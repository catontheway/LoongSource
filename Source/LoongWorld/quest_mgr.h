//------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: quest_mgr.h
// author: lh
// actor:
// data: 2008-7-24
// last:
// brief: ���������
// remark: ÿ����ɫ����һ���������������������������ļ��أ����µĲ���
//------------------------------------------------------------------------------------------------------
#pragma once

struct tagQuestProto;
class Creature;

class QuestMgr
{
public:
	typedef TMap<UINT16, tagQuestProto*>	QuestMap;
	typedef TMap<DWORD, TList<UINT16>*>		QuestRelationMap;
	typedef TMap<UINT16, TList<DWORD>*>		NPCRelationMap;
	typedef map<string, string>				XmlNodeMap;

public:
	//--------------------------------------------------------------------
	// ��ʼ��
	//--------------------------------------------------------------------
	BOOL Init();

	//--------------------------------------------------------------------
	// ����
	//--------------------------------------------------------------------
	VOID Destroy();

	//--------------------------------------------------------------------
	// ��ָ��Ŀ¼��ȡ����ṹ
	//--------------------------------------------------------------------
	BOOL LoadQuest(LPCTSTR szPath);

	//--------------------------------------------------------------------
	// �Ƿ���ԴӸ�NPC���õ��������
	//--------------------------------------------------------------------
	BOOL IsNPCAcceptQuest(Creature* pCreature, UINT16 u16QuestID);

	//--------------------------------------------------------------------
	// �Ƿ���ԴӸ�NPC�����������
	//--------------------------------------------------------------------
	BOOL IsNPCCompleteQuest(Creature* pCreature, UINT16 u16QuestID);

	//---------------------------------------------------------------------
	// ĳ�������Ƿ�Ҫͨ��NPC���ܽ�
	//--------------------------------------------------------------------
	BOOL IsNeedNPCCompleteQuest(UINT16 u16QuestID);

	//---------------------------------------------------------------------
	// ĳ�������Ƿ�Ҫͨ��NPC���ܽ�
	//--------------------------------------------------------------------
	BOOL IsNeedNPCAcceptQuest(UINT16 u16QuestID);

	//---------------------------------------------------------------------
	// ����Get
	//---------------------------------------------------------------------
	const tagQuestProto* GetQuestProto(UINT16 u16QuestID) { return m_mapAllQuests.Peek(u16QuestID); }

private:
	DWORD	LoadQuestFromXml(tagQuestProto* pQuest, LPCTSTR szFileName);
	VOID	FindQuestFileInDir(LPCTSTR szPath, vector<tstring>& vecQuestXml);

	BOOL	LoadAcceptQuestNPCRelation(LPCTSTR szFileName);
	BOOL	LoadCompleteQuestNPCRelation(LPCTSTR szFileName);

	BOOL	LoadXmlNode(XmlElement* pNode, XmlNodeMap& xmlNodeMap);
	BOOL	SetQuestProto(tagQuestProto* pQuestProto, XmlNodeMap& xmlNodeMap);
	VOID	SetQuestDword(string str, DWORD& dwQuest,XmlNodeMap& xmlNodeMap);
	VOID	SetQuestDwordFlags(string str, DWORD& dwQuest, INT32 nMask, XmlNodeMap& xmlNodeMap);
	VOID	SetQuestUINT16(string str, UINT16& unQuest,XmlNodeMap& xmlNodeMap);
	VOID	SetQuestBYTE(string str, BYTE& byQuest,XmlNodeMap& xmlNodeMap);
	VOID	SetQuestINT16(string str, INT16& n16Quest,XmlNodeMap& xmlNodeMap);
	VOID	SetQuestINT32(string str, INT32& nQuest,XmlNodeMap& xmlNodeMap);
	VOID	SetQuestBool(string str, BOOL& bQuest,XmlNodeMap& xmlNodeMap);
	VOID	SetQuestFloat(string str, FLOAT& fQuest,XmlNodeMap& xmlNodeMap);	

private:
	TObjRef<VarContainer>	m_pVar;

	QuestMap			m_mapAllQuests;					// �������񼯺�
	QuestRelationMap	m_mapAcceptQuestNPCRelation;	// NPC��ɽ������Ӧ��
	QuestRelationMap	m_mapCompleteQuestNPCRelation;	// NPC��ɽ������Ӧ��
	NPCRelationMap		m_mapCompleteNPCRelation;		// �ɽ�������NPC��Ӧ��
	NPCRelationMap		m_mapAcceptNPCRelation;			// �ɽ�������NPC��Ӧ��

	tstring				m_strQuestPath;					// �����ļ����·��
	tstring				m_strQuestFileList;				// �����ļ��б��ļ�
	tstring				m_strAcceptQuestNPCRelation;	// �������ȡNPC��ϵ�ļ�
	tstring				m_strCompleteQuestNPCRelation;	// ���������NPC��ϵ�ļ�
};

extern QuestMgr g_questMgr;