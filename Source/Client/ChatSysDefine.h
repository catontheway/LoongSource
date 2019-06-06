#pragma once
#include "..\WorldDefine\chat_define.h"
#include "..\WorldDefine\ItemDefine.h"
//----------------------------------------------------------------
//���͸��������ķ������ݽṹ(����)
//----------------------------------------------------------------
struct tagSendSpeak
{
	DWORD				dwDestID;					//���͸�˭ID
	tstring				strDestName;				//���͸�˭Name
	float				fSendTime;					//���͵�ʱ��
	ESendChatChannel	eChannel;					//����Ƶ��
	tstring				strMsg;						//��������
	BYTE				byAutoReply;		//�Ƿ��Զ��ظ� 0:���� 1:��

	tagSendSpeak()
	{	
		dwDestID	= GT_INVALID;
		eChannel	= ESCC_NULL;
		fSendTime	= 0.0f;
		byAutoReply = 0;
	}
	tagSendSpeak(const tagSendSpeak& other)
	{
		Copy(other);
	}

	const tagSendSpeak& operator = (const tagSendSpeak& other)
	{
		if(this!=&other) 
			Copy(other);
		return *this;
	}
private:
	void Copy(const tagSendSpeak&  other)
	{
		dwDestID	= other.dwDestID;
		eChannel	= other.eChannel;
		fSendTime	= other.fSendTime;
		strDestName	= other.strDestName;
		strMsg		= other.strMsg;
		byAutoReply = other.byAutoReply;
	}
};

//-----------------------------------------------------------------
//�ӷ��������ܵ���������Ϣ�����ݽṹ�����ܷ��������ԣ�
//-----------------------------------------------------------------
struct tagRecvSpeak
{
	DWORD				dwSendRoleID;		//������ GT_INVALID����ϵͳ����	
	DWORD				dwRecvRoleID;		//������
	ESendChatChannel	eChannel;			//����Ƶ��
	tstring				strSendRoleName;	//�����ߵ�����
	tstring				strRecvRoleName;	//����������
	tstring				strText;			//��������
	BYTE				byAutoReply;		//�Ƿ��Զ��ظ� 0:���� 1:��

	tagRecvSpeak()
	{		
		dwSendRoleID	= GT_INVALID;
		dwRecvRoleID	= GT_INVALID;
		eChannel		= ESCC_NULL;
		byAutoReply		= FALSE;
	}
	tagRecvSpeak(const tagRecvSpeak& other)
	{
		Copy(other);
	}

	const tagRecvSpeak& operator = (const tagRecvSpeak& other)
	{
		if(this!=&other)
			Copy(other);
		return *this;
	}
private:
	void Copy(const tagRecvSpeak&  other)
	{
		dwSendRoleID	= other.dwSendRoleID;
		dwRecvRoleID	= other.dwRecvRoleID;
		eChannel		= other.eChannel;
		strSendRoleName	= other.strSendRoleName;
		strRecvRoleName	= other.strRecvRoleName;
		strText			= other.strText;
		byAutoReply		= other.byAutoReply;
	}
};

//-------------------------------------------------------
//-------------------------------------------------------
struct tagChatShowEquip
{
	DWORD				dwSrcID;
	tstring				strSrcName;
	DWORD				dwDstID;
	tstring				strDstName;
	ESendChatChannel	eChannel;
	tagEquip			equip;
};
struct tagChatShowItem
{
	DWORD				dwSrcID;
	tstring				strSrcName;
	DWORD				dwDstID;
	tstring				strDstName;
	ESendChatChannel	eChannel;
	DWORD				item;
};

//-------------------------------------------------------
//��������
//-------------------------------------------------------
const byte NUM_SEND_CHAT = 6;				//�ͻ��˷���Ƶ��������

const byte NUM_CHAT_CHANNEL = 8;			//�ͻ��˽���Ƶ��������

const INT MAX_EMOTION = 60;					//����������

const int MIN_REPEAT_SPEAK_TIME = 30;		//��ǰƵ���ظ����Ե�ʱ����

const int MIN_WORLD_RESPEAK_TIME = 30;		//����Ƶ���ظ����Ե��ж�ʱ��

const int MIN_RUNHORSE_RESPEAK_TIME = 30;	//����Ʒ��Ե��ж�ʱ��

const int MAX_PVT_SPEAK_VALIDETIME = 120;	//˽�ķ��Ե���Чʱ�䣬���������ʱ�仹û�в鵽����ҵ�ID����ӻ�����ɾ��

const int MAX_ROW = 300;					//���촰���������

const DWORD RICHEDITBOX_MAX_TEXT = 20480*2;	//richeditbox����ַ���

const byte NUM_CHAT_CAST_WND = 3;			//����

const byte NUM_CHAT_ZOOM_MAX = 4;

const byte NUM_CHAT_ZOOM_MIN = 0;





//����������Ƶ��
enum ERecvChatChannel
{
	ERCC_NULL		= 0x00,
	ERCC_CastTalk	= 0x01,		//����
	ERCC_World		= 0x02,		//����
	ERCC_Pvt		= 0x04,		//˽��
	ERCC_Guild		= 0x08,		//����
	ERCC_Troop		= 0x10,		//����
	ERCC_Current	= 0x20,		//��ǰ
	ERCC_Combat		= 0x40,		//ս��
	ERCC_Affiche	= 0x80,		//����
	ERCC_GM			= 0x100,	//GM
	ERCC_System		= 0x200,	//ϵͳ
	ERCC_End		= 0x7FFF,	//ȫ��
	ERCC_Num		= 10,
};

const DWORD g_dwRecvChannel[] = 
{
	ERCC_World | ERCC_Pvt | ERCC_Guild | ERCC_Troop | ERCC_Current | ERCC_Affiche | ERCC_GM | ERCC_System,
	ERCC_Troop | ERCC_System | ERCC_Affiche,
	ERCC_Guild | ERCC_System | ERCC_Affiche,
	ERCC_Pvt | ERCC_System | ERCC_Affiche,
	ERCC_World | ERCC_System | ERCC_Affiche,
	ERCC_Affiche | ERCC_System | ERCC_Affiche,
	ERCC_Combat  | ERCC_System | ERCC_Affiche,
};

//���ڻ��洫��
struct tagScroText					
{
	tstring szInfo;
	tstring szName;
	tstring url;
	DWORD	dwRoleID;
	float	fTime;
};

struct ShowEvent
{
	DWORD			dwParam1;	
	DWORD			dwParam2;
	DWORD			dwParam3;
	DWORD			dwParam4;
};
