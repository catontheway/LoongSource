/********************************************************************
	created:	2008/08/26
	created:	26:8:2008   15:04
	filename: 	d:\loong_client\Client\QuestQuery.h
	file path:	d:\loong_client\Client
	file base:	QuestQuery
	file ext:	h
	author:		leili
	
	purpose:	���������ѯ���ɽ��������NPC�б�����������б���ҵ�ǰ�����б�
*********************************************************************/
#pragma once
#include "ServerTime.h"

class QuestQuery
{
	friend class QuestMgr;

public:
	QuestQuery(void);
	~QuestQuery(void);

	VOID	Reset(void);

	struct tagQuestTrack 
	{
		bool	isTracked;
		float	lastUpdateTime;
		INT		itemCount[QUEST_ITEMS_COUNT];
		INT		creatureCount[QUEST_CREATURES_COUNT];
		bool	invesObj[QUEST_ITEMS_COUNT];

		tagQuestTrack(void)
		{
			memset(this,0,sizeof(tagQuestTrack));
		}
	};

	struct tagActiveQuest
	{
		EWeek	week;			// ���ڼ�
		UINT16	questID;		// ����ID
		tstring	questName;		// ��������
		INT32	minLevel;		// ��С�ȼ�
		INT32	maxLevel;		// ���ȼ�
		DWORD	acceptNPCID;	// ��ȡNPCID
		tstring	acceptNPCName;	// ��ȡNPC����
		INT		complete;		// ����ɴ���
		INT		all;			// ����ɴ���
	};

	enum ESortType
	{
		EST_None	= 0,		// ������
		EST_Time	= 1,		// ��ʱ������
		EST_Quest,				// ��������������
		EST_Level,				// ���ȼ���������
		EST_AcceptNPC,			// ����ȡ������
		EST_Complete			// ����ɴ�������
	};

	struct tagMapNPCInfo
	{
		DWORD	dwTypeID;		// ����ID
		vector<Vector3>	vPos;	// ��ʼ����
	};

	typedef std::set<DWORD> NPCSet;
	typedef std::map<UINT16,tagQuestTrack> QuestsMap;
	typedef std::pair<UINT16,tagQuestTrack> Quest_Pair;
	typedef std::map<DWORD,tagMapNPCInfo> MapNPCMap;
	typedef std::pair<DWORD,tagMapNPCInfo> MapNPC_Pair;
	typedef std::vector<tagActiveQuest> ActiveQuestArray;

	VOID			BuildMapQuestNPCs(void);
	VOID			BuildDynamicQuestNPCs(list<tstring>& strNPCs);
	VOID			BuildAcceptQuestNPCs(void);
	VOID			BuildCompleteQuestNPCs(void);
	VOID			BuildUpdateQuestNPCs(void);
	VOID			AddInvesObject(UINT16 questID);
	VOID			DeleteInvesObject(UINT16 questID);
	VOID			DeleteInvesObject( UINT16 questID,INT index );
	VOID			UpdateInvesObject(UINT16 questID);
	VOID			AddTriggerEffect(UINT16 questID);
	VOID			DeleteTriggerEffect(UINT16 questID);
	VOID			UpdateTriggerEffect(UINT16 questID);
	VOID			UpdateTriggerEffect(void);

	BOOL			IsInvesObject(DWORD typeID);

	const NPCSet&	GetAcceptQuestNPCs(void)		const				{ return m_acceptQuestNPCs; }
	const NPCSet&	GetCompleteQuestNPCs(void)		const				{ return m_completeQuestNPCs; }
	const NPCSet&	GetUpdateQuestNPCs(void)		const				{ return m_updateQuestNPCs; }
	const QuestsMap&GetCurrentQuests(void)			const				{ return m_currentQuests; }
	VOID			GetNPCAcceptQuests(DWORD npcID,vector<UINT16>& quests)	const;
	VOID			GetNPCCompleteQuests(DWORD npcID,vector<UINT16>& quests)const;
	VOID			GetNPCUpdateQuests(DWORD npcID,vector<UINT16>& quests)	const;
	BOOL			QuestCanAccept(UINT16 questID)	const;
	BOOL			QuestCanComplete(UINT16 questID)	const;
	BOOL			NPCCanUpdateQuest(DWORD npcID,UINT16 questID)	const;
	BOOL			QuestCanTrack(UINT16 questID)	const;
	BOOL			QuestIsTracked(UINT16 questID)	const;
	BOOL			GetQuestItemCount(UINT16 questID,DWORD num,UINT16& count)		const;
	BOOL			GetQuestCreatureCount(UINT16 questID,DWORD num,UINT16& count)	const;
	BOOL			ObjBeInves( UINT16 questID,DWORD num ) const;
	BOOL			SetQuestItemCount(UINT16 questID,DWORD index,UINT16 count);
	BOOL			SetQuestCreatureCount(UINT16 questID,DWORD index,UINT16 count);
	BOOL			SetQuestInvesObj(UINT16 questID,INT index);
	BOOL			UpdateQuestState(UINT16 questID);
	BOOL			GetNPCPosition(DWORD npcID,Vector3& pos,bool bRandom=true)		const;
	BOOL			GetNPCPosition(DWORD npcID,vector<Vector3>& posArray)			const;
	BOOL			IsAcceptNPC(DWORD npcID)		const;
	BOOL			IsCompleteNPC(DWORD npcID)		const;
	BOOL			IsUpdateNPC(DWORD npcID)		const;

	BOOL			QuestIsCompleted(UINT16 questID)	const;
	BOOL			QuestIsAccepted(UINT16 questID)		const;

	VOID			SortActiveQuest(ESortType eSort);
	const ActiveQuestArray& GetActiveQuestArray(void)	const	{ return m_activeQuests; }
	UINT16			GetActiveQuestID(DWORD index)		const;
	DWORD			GetActiveQuestNPCID(DWORD index)	const;

private:
	NPCSet				m_acceptQuestNPCs;			// �ɽ������npc�б�
	NPCSet				m_dynAcceptQuestNPCs;		// ��̬�ɽ������npc�б�
	NPCSet				m_completeQuestNPCs;		// �ɽ������npc�б�
	NPCSet				m_updateQuestNPCs;			// �ɸ��������npc�б�
	QuestsMap			m_currentQuests;			// ��ҵ�ǰ�����б�
	MapNPCMap			m_mapNPCMap;				// ��ǰ��ͼ��NPC�б�
	NPCSet				m_dynamicNPCs;				// ��̬NPC�б�
	ActiveQuestArray	m_activeQuests;				// �̶�������б�

	typedef std::map<DWORD,INT> InvesObjMap;
	InvesObjMap			m_invesObjects;				// �ɵ�������б�

	struct tagTriggerEffect 
	{
		INT		refNum;
		DWORD	effectID;
	};
	typedef std::map<DWORD,tagTriggerEffect> TriggerEffectMap;
	TriggerEffectMap	m_triggers;					// ���񴥷����б�

	typedef std::map<UINT16,INT> DoneQuestsMap;
	DoneQuestsMap		m_doneQuests;				// �Ѿ���ɵ��������񣬺���Ϊ��ɴ���

	typedef std::map<UINT16,tagDWORDTime> DoneTimeMap;
	DoneTimeMap			m_doneTimes;				// ��������ʱ���
};