#pragma once

#include "msg_common_errorcode.h"

#define MAX_NOTICE_TITLE	8
#define MAX_NOTICE_MSG_LEN	1024


#pragma pack(push, 1)

//������Ƶ�� ���� չʾ��Ʒ��װ�� ������
#define WORLD_CHANNEL_DEC_SLIVER 500

//chat�������
enum
{
	E_RoleChat_ForbidChat				= 1,				//��ֹ����
	E_RoleChat_ServerNotReady			= 2,				//������û��׼����
	E_RoleChat_TooLength				= 3,				//��Ϣ���ȹ���(����100����)

	E_RoleChat_CastTalk_NoIMItem		= 10,				//����,û��IM����

	E_RoleChat_World_NoItem				= 20,				//����,û�е���
	E_RoleChat_World_Frequently			= 21,				//����,���Թ���
	E_RoleChat_World_NoMoney			= 22,				//����,��Ǯ����
	E_RoleChat_World_RoleNo10			= 23,				//����,����û��10��

	E_RoleChat_Pvt_RemoteRoleLeave		= 30,				//˽��,�Է�������
	E_RoleChat_Pvt_NoRoleName			= 31,				//˽��,û��Ŀ����� 
	E_RoleChat_Pvt_NoSelf				= 32,				//˽��,�Լ��������Լ�˽��

	E_RoleChat_Guild_NoJoin				= 40,				//����,û������

	E_RoleChat_Troop_NoJoin				= 50,				//����,û�ж���

	E_RoleChat_Current_Frequently		= 60,				//��ǰ,���Թ���
	E_RoleChat_Quest_Bag_Full			= 70,				//������Ʒ��������
	E_RoleChat_Not_GM					= 80,				//�����˲���GM

};

const DWORD MAX_CHAT_LEN = 200;						//�ͻ���������������
CMD_START(NC_RoleChat)		// ��ĳ��˵��
	DWORD	dwDestRoleID;	// ��˭˵��
	BYTE	byChannel;		// ����Ƶ��
	BYTE	byAutoReply;	// �Զ��ظ� 0:���� 1:��
	TCHAR	szMsg[1];		// ��������   
CMD_END;

CMD_START(NS_RoleChat)		// ĳ�˶���˵��
	DWORD	dwErrorCode;	
	DWORD	dwSrcRoleID;	// ��˭˵�Ļ�GT_INVALID����ϵͳ����
	DWORD	dwDestRoleID;	// �Ƕ�˭˵
	BYTE	byChannel;		// ����Ƶ��
	BYTE	byAutoReply;	// �Զ��ظ� 0:���� 1:��
	TCHAR	szMsg[1];		// ��������
CMD_END;

//--------------------------------------------------------------------------------
//����Ϊ�ڱ�������,�����͵Ĺ㲥��Ϣ
//--------------------------------------------------------------------------------

//��ȡ����
CMD_START(NC_LoadLeftMsg)
CMD_END


//�����·��㲥 �������㲥����XX�ڱ����˵����������·���
CMD_START(NS_ChatBillike)
	DWORD			dwRoleID;		//���ID
CMD_END

//�ڻ��Ϲŷ�ӡ�㲥 �������㲥����XX�ڻ����Ϲŷ�ӡ���Ϲŵ�������XXX�����˼䣬���·Ӣ����ȥն����ħ����
CMD_START(NS_ChatBadOldSeal)
	DWORD			dwRoleID;		//���ID
	DWORD			dwMapCrc;		//��ͼ����CRCֵ
CMD_END

//�����·��㲥 �������㲥����XX�ڱ����˵����������·���
CMD_START(NS_Plutus)
	DWORD			dwRoleID;		//���ID
CMD_END

//�����Ϲ�֮�� �������㲥��XXX�ڱ��������Ϲ����飬���������˼俼�������������
CMD_START(NS_AwakenGods)
	DWORD			dwRoleID;		//���ID
CMD_END

//��֮���� ����BOSS��ɱ����ȫ�����棺��XXX���ڱ��������ң���ս###��BOSS���ƣ��ɹ��������XXX��
CMD_START(NS_GodsOfPractice)
	DWORD			dwRoleID;		//��� ID
	DWORD			dwNpcTypeID;	//NPC type id
	DWORD			dwItemTypeID;	//��Ʒ type id
CMD_END

//���Ź��﹥�ǻ ϵͳ������ʾ����###������������Ĺ������5���Ӻ��##�ǽ��б���������Э��������
CMD_START(NS_MonsterAttackCity)
	DWORD			dwRoleID;		//���ID
	DWORD			dwMapCrc;		//��ͼ����CRCֵ
CMD_END

// �㲥���俪���ĵ���Ʒ
CMD_START(NS_RoleGetChestItem)
	DWORD			dwRoleID;
	DWORD			dwItemTypeID;
CMD_END

// �Զ�ѭ������
CMD_START(NS_AutoNotice)
	INT				nType;							//��ɫ
	INT				nCirInterval;					//ѭ�����
	TCHAR			szContent[1];					//����
CMD_END

// ���½ǹ���
CMD_START(NS_RightNotice)
	INT				nTitleLen;						// ����
	INT				nLinkLen;						// ����
	INT				nContentLen;					// ����
	TCHAR			szContent[1];
CMD_END

#pragma pack(pop)