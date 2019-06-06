//-----------------------------------------------------------------------------
//!\file quest.h
//!
//!\date 2008-08-19
//!
//!\brief �ͻ����������֮�䷢������ϵͳ��ص���Ϣ
//-----------------------------------------------------------------------------

#pragma once

#include "msg_common_errorcode.h"

#pragma pack(push, 1)

//�����ȡʧ��ԭ��
enum
{
	E_QUEST_NOT_EXIST								= 1,		//���񲻴���
	E_QUEST_NPC_NOT_EXIST							= 2,		//����NPC������
	E_QUEST_NPC_TOO_FAR								= 3,		//����NPC����Ҿ���̫Զ
	E_QUEST_FAILED_ADD 								= 4,        //��������ʧ��
	E_QUEST_FATIGUEGUARD							= 5,		//�����Ա���
	
	E_CanTakeQuest_FAILED_LOW_LEVEL					= 11,		//û���㹻�ĵȼ�
	E_CanTakeQuest_FAILED_LOW_REP					= 12,		//û���㹻������
	E_CanTakeQuest_ALREADY_DONE						= 13,		//�Ѿ������
	E_CanTakeQuest_ALREADY_TAKE						= 14,		//�Ѿ���ȡ��
	E_CanTakeQuest_FAILED_MISSING_ITEMS				= 15,		//û����������Ҫ����Ʒ
	E_CanTakeQuest_FAILED_MISSING_SKILLS			= 16,		//û����������Ҫ�ļ���
	E_CanTakeQuest_FAILED_MISSING_Att				= 17,		//û����������Ҫ����������
	E_CanTakeQuest_FAILED_WRONG_SEX					= 18,       //��������Ҫ���Ա�
	E_CanTakeQuest_FAILED_WRONG_CLASS				= 19,       //��������Ҫ��ְҵ
	E_CanTakeQuest_FAILED_NOT_ENOUGH_MONEY			= 20,       //û����������Ҫ�Ľ�Ǯ
	E_CanTakeQuest_FAILED_QUEST_NUM_FULL			= 21,		//�������ˣ������ٽ�
	E_CanTakeQuest_FAILED_WRONG_MAP 				= 22, 		//��ͼ����
	E_CanTakeQuest_FAILED_PREV 						= 23,      	//����ǰ�򲻶�
	E_CanTakeQuest_FAILED_NEXT 						= 24,      	//�����������
	E_CanTakeQuest_FAILED_WRONG_TIME 				= 25,       //����ʱ�䲻��
	E_CanTakeQuest_FAILED_WRONG_TIMES 				= 26,       //�����ȡ��������
	E_CanTakeQuest_FAILED_MISS_ITEM_OF_QUEST 		= 27,       //û��������Ʒ
	E_CanTakeQuest_FAILED_WRONG_TRIGGER 			= 28,       //���񴥷�������
	E_CanTakeQuest_FAILED_SCRIPT					= 29,		//�ű��ж�ʧ�ܣ��ͻ��˲��ṩ������ʾ��������ֱ�ӷ���
	E_CanTakeQuest_FAILED_BAG_FULL					= 30,		//����û�еط�������������Ʒ

	E_CanCompleteQuest_FAILED_MISSING_Creature		= 31,		//û�������������Ҫ������
	E_CanCompleteQuest_FAILED_MISSING_NPC			= 32,		//û�������������Ҫ��NPC
	E_CanCompleteQuest_FAILED_MISSING_ITEMS			= 33,		//û�������������Ҫ����Ʒ
	E_CanCompleteQuest_FAILEDENOUGH_MONEY			= 34,		//û�������������Ҫ�Ľ�Ǯ
	E_CanCompleteQuest_BAG_FULL						= 35,		//����û�еط������ý�����Ʒ
	E_CanCompleteQuest_FALIED_SCRIPT				= 36,		//�ű��ж�ʧ�ܣ��ͻ��˲��ṩ������ʾ��������ֱ�ӷ���

	E_CanTakeQuest_Tomorrow							= 37,		//ÿ��ֻ�����һ��
	E_CanTakeQuest_Week								= 38,		//ֻ�������ڡ����
	E_CanTakeQuest_Class							= 39,		//ְҵ������

	E_CanUpdateQuest_Fail							= 40,		//���ܸ���
};


//-----------------------------------------------------------------------------
//	�Ƿ���Խ�ȡ������
//-----------------------------------------------------------------------------
CMD_START(NC_NPCAcceptQuest)
	DWORD		dwNPCID;		// ��ȡ����NPCΨһID 
	UINT16		u16QuestID;		// ����ID
CMD_END

//-----------------------------------------------------------------------------
//	��������ȡ����
//-----------------------------------------------------------------------------
CMD_START(NC_TriggerAcceptQuest)
	UINT16		u16QuestID;		// ����ID
	DWORD		dwTriggerID;	// ������ID
CMD_END

CMD_START(NS_AcceptQuest)
	UINT16		u16QuestID;		// ����ID
	DWORD		dwErrorCode;	// ������
CMD_END

CMD_START(NS_AddQuest)
	UINT16		u16QuestID;							// ����ID
	INT16		n16ItemCount[QUEST_ITEMS_COUNT];	// ��Ʒ����׷������
	tagQuestDynamicTarget	DynamicTarget;			// ��̬Ŀ������Żᷢ���ͻ���
CMD_END

//-----------------------------------------------------------------------------
//	�Ƿ������ɴ�����
//-----------------------------------------------------------------------------
CMD_START(NC_CompleteQuest)
	DWORD		dwNPCID;					// �������NPCΨһID 
	UINT16		u16QuestID;					// ����ID
	INT32		nRewChoicesItemIndex;		// ��ѡ������Ʒ���
CMD_END

CMD_START(NS_CompleteQuest)
	UINT16		u16QuestID;					// ����ID
	DWORD		dwErrorCode;				// ������
CMD_END

//-----------------------------------------------------------------------------
//	�Ƿ����ɾ������
//-----------------------------------------------------------------------------
CMD_START(NC_DeleteQuest)
	UINT16		u16QuestID;					// ɾ������ID
CMD_END

CMD_START(NS_DeleteQuest)
	UINT16		u16QuestID;					// ����ID
	DWORD		dwErrorCode;				// ������
CMD_END

//-----------------------------------------------------------------------------
//	��������NPC�Ի�����
//-----------------------------------------------------------------------------
CMD_START(NC_UpdateQuestNPCTalk)
	DWORD		dwNPCID;						// ��������NPCΨһID 
	UINT16		u16QuestID;						// ��������ID
CMD_END

//-----------------------------------------------------------------------------
//	����ɱ����������
//-----------------------------------------------------------------------------
CMD_START(NS_QuestUpdateKillCreature)
	UINT16		u16QuestID;						// ����ID
	INT			nCreatureIndex;					// �������
	INT16		n16NewKillNum;					// ���¹����ɱ�ּ���
CMD_END

//-----------------------------------------------------------------------------
//	������Ʒ��������
//-----------------------------------------------------------------------------
CMD_START(NS_QuestUpdateItem)
	UINT16		u16QuestID;						// ����ID
	INT			nItemIndex;						// ��Ʒ���
	INT16		n16NewItemNum;					// ���¹������Ʒ����
CMD_END

//-----------------------------------------------------------------------------
// ����NPC�Ի���������
//-----------------------------------------------------------------------------
CMD_START(NS_QuestUpdateNPCTalk)
	UINT16		u16QuestID;						// ����ID
	DWORD		dwErrorCode;					// ������
	INT			nIndex;							// ĳ������
CMD_END

//-----------------------------------------------------------------------------
// ����Trigger��������
//-----------------------------------------------------------------------------
CMD_START(NS_QuestUpdateTrigger)
	UINT16		u16QuestID;						// ����ID
	INT			nIndex;							// ĳ������
CMD_END

//-----------------------------------------------------------------------------
// ���µ��������������
//-----------------------------------------------------------------------------
CMD_START(NS_QuestUpdateInveset)
	UINT16		u16QuestID;
	INT			nIndex;
CMD_END

//-----------------------------------------------------------------------------
// �������������Ϣ
//-----------------------------------------------------------------------------
CMD_START(NS_QuestFailedMsg)
	UINT16		u16QuestID;						// ����ID
	TCHAR		szMsg[1];						// ����������Ϣ
CMD_END

#pragma pack(pop)