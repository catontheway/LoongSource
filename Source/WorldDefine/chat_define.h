#pragma once

#pragma pack(push, 1)

//������췢��Ƶ��
enum ESendChatChannel
{
	ESCC_NULL = -1,
	ESCC_CastTalk = 0,		//����
	ESCC_World,				//����
	ESCC_Pvt,				//˽��
	ESCC_Guild,				//����
	ESCC_Troop,				//����
	ESCC_Current,			//��ǰ
	ESCC_Leave,				//����
	ESCC_Combat,			//ս��
	ESCC_Affiche,			//����
	ESCC_GM,				//GM
	ESCC_System,			//ϵͳ
	ESCC_End,		
};

//����Ƶ��ʱ����
const INT	WORLD_CHANNEL_INTERVAL	= TICK_PER_SECOND * 30;

//����Ƶ����Ϣ��������
const INT	WORLD_CHANNEL_MSG_LEN	= 200;

//����Ƶ����Ϣ��ҵȼ�����
const INT	WORLD_CHANNEL_ROLE_LVL	= 10;

//����IM��ƷID1360001
const DWORD TYPEID_IM_CASTTALK		= 1360001;

#pragma pack(pop)