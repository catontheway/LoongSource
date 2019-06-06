//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_rt_c.h
// author: 
// actor:
// data: 2009-04-27
// last:
// brief: GM�������GM����֮�����Ϣ����
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/base_define.h"
#include "gm_tool_define.h"
#include "rt_define.h"

#pragma pack(push, 1)
//-----------------------------------------------------------------------
//GM����
//-----------------------------------------------------------------------
CMD_START(NGMTC_GMCreate)
	TCHAR			szName[X_SHORT_NAME];			//�ʺ�
	DWORD			dwPswCrc;						//����CRC
	EPrivilege		ePrivilege;						//Ȩ��
CMD_END

CMD_START(NGMTS_GMCreate)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
//GM�޸�
//-----------------------------------------------------------------------
CMD_START(NGMTC_GMModify)
	TCHAR			szName[X_SHORT_NAME];			//�ʺ�
	EPrivilege		ePrivilege;						//Ȩ��
CMD_END

CMD_START(NGMTS_GMModify)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
//GMɾ��
//-----------------------------------------------------------------------
CMD_START(NGMTC_GMDelete)
	TCHAR			szName[X_SHORT_NAME];			//�ʺ�
CMD_END

CMD_START(NGMTS_GMDelete)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
//						��¼�鿴msg_rt_c.h 
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
//��ɫ�ָ�
//-----------------------------------------------------------------------
CMD_START(NGMTC_RoleResume)
	INT				nOldID;							//��ɫԭ����ID
	TCHAR			szNewName[X_SHORT_NAME];		//��ɫ���������
	DWORD			dwSectionID;					//����ID
	DWORD			dwWorldID;						//��Ϸ����ID
CMD_END

CMD_START(NGMTS_RoleResume)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
//��ɫ�����޸�
//-----------------------------------------------------------------------
CMD_START(NGMTC_ProModify)
	TCHAR			szName[X_SHORT_NAME];			//��ɫ����
	INT				nLel;							//�ȼ�
	INT				nSkill;							//���ʵ�
	INT				nYuanbao;						//Ԫ��
	INT64			n64Exp;							//����
	INT64			n64Money;						//Ǯ	
	DWORD			dwSectionID;					//����ID
	DWORD			dwWorldID;						//��Ϸ����ID
CMD_END

CMD_START(NGMTS_ProModify)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
//��Ʒ����
//-----------------------------------------------------------------------
CMD_START(NGMTC_ItemCreate)
	TCHAR			szName[X_SHORT_NAME];			//��ɫ
	DWORD			dwItemTypeID;					//��ƷTypeID
	INT				nItemNum;						//����
	BYTE			byQuality;						//Ʒ��
	DWORD			dwSectionID;					//����ID
	DWORD			dwWorldID;						//��Ϸ����ID
CMD_END

CMD_START(NGMTS_ItemCreate)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
//��Ʒɾ��
//-----------------------------------------------------------------------
CMD_START(NGMTC_ItemDelete)
	TCHAR			szName[X_SHORT_NAME];			//��ɫ
	//INT64			n64ItemID;						//��Ʒȫ��ID
	DWORD			dwSectionID;					//����ID
	DWORD			dwWorldID;						//��Ϸ����ID
	TCHAR			sz64IDString[1];				//Ҫɾ����Ʒ��64λID��ɵĴ�		
CMD_END

CMD_START(NGMTS_ItemDelete)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
//��Ʒ�ָ�
//-----------------------------------------------------------------------
CMD_START(NGMTC_ItemResume)
	TCHAR			szTargetRole[X_SHORT_NAME];		//Ŀ���ɫ��
	DWORD			dwSectionID;					//����ID
	DWORD			dwWorldID;						//��Ϸ����ID
	TCHAR			sz64IDString[1];				//��Ʒȫ��ID
CMD_END

CMD_START(NGMTS_ItemResume)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// �˺ŷ�ͣ
//-----------------------------------------------------------------------
CMD_START(NGMTC_AccountForbid)
	TCHAR			szSectionName[X_SHORT_NAME];		//����
	TCHAR			szWorldName[X_SHORT_NAME];			//��Ϸ����
	INT				nAction;							//0:��� 1����
	TCHAR           szAccountStr[1];					//�˺Ŵ�,���˺�֮�䶺�ŷָ�
CMD_END

CMD_START(NGMTS_AccountForbid)
	//BOOL			bLast;
	TCHAR           szAccount[X_SHORT_NAME];
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
//--˫��
//-----------------------------------------------------------------------
CMD_START(NGMTC_Double)
	EDoubleType		eDType;
	DWORD			dwOpenTime;						//����ʱ��
	DWORD			dwLastTime;						//����ʱ��
	DWORD			dwRatio;						//����
	DWORD			dwSectionID;					//����ID,
	DWORD			dwWorldID[1];					//��Ϸ����ID[n]     ����ô�����������Ϸ����		
CMD_END

//-----------------------------------------------------------------------
//--���½ǹ���
//-----------------------------------------------------------------------
CMD_START(NGMTC_RightNotice)								
	DWORD			dwNoticeTime;					//����ʱ�� tagDWORDTime 0Ϊ��ʱ
	TCHAR			szTitle[Notice_Title];			//����
	TCHAR			szContent[Notice_Content];		//����
	TCHAR			szLink[Notice_Link];			//���ӵ���վ
	DWORD			dwSectionID;					//����ID,
	DWORD			dwWorldID[1];					//��Ϸ����ID[n]      ��Ϸ����ID:GT_INVALID����ô�����������Ϸ����											
CMD_END

//-----------------------------------------------------------------------
//--�Զ�ѭ������
//-----------------------------------------------------------------------
CMD_START(NGMTC_AutoNotice)
	TCHAR			szContent[Notice_Content];		//����
	EColorType		eType;							//��ɫ
	INT				nTimeInterval;					//ʱ����
	INT				nCirInterval;					//ѭ�����
	INT				nCirNum;						//ѭ������
	DWORD			dwSectionID;					//����ID,
	DWORD			dwWorldID[1];					//��Ϸ����ID[n]      ��Ϸ����ID:GT_INVALID����ô�����������Ϸ����	
CMD_END

//-----------------------------------------------------------------------
//--��ʱ���йصķ�����Ϣ
//-----------------------------------------------------------------------
CMD_START(NGMTS_TimeLimitedMsg)
	ETimeLimitedMsg	eType;	
	DWORD			dwErrorCode;					// 0-�ɹ�
	DWORD			dwWorldID;
CMD_END

//-----------------------------------------------------------------------
// ���/ɾ��������IP
//-----------------------------------------------------------------------
CMD_START(NGMTC_Blacklist)
	TCHAR			szSectionName[X_SHORT_NAME];		// ����
	DWORD			dwIP;								// IP
	INT				nAction;							// 0:��� 1����
CMD_END

CMD_START(NGMTS_Blacklist)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// ����Ƶ���Զ�����
//-----------------------------------------------------------------------
CMD_START(NGMTC_AutoChatNotice)
	TCHAR			szNotice[X_HUGE_STRING];			// ��������
	BYTE			byHour;								// ����: ʱ
	BYTE			byMinute;							// ����: ��
	DWORD			dwSectionID;						// ����ID
	DWORD			dwWorldID[1];						// ��Ϸ����ID[n]      ��Ϸ����ID:GT_INVALID����ô�����������Ϸ����	// ����ʱ��
CMD_END

CMD_START(NGMTS_AutoChatNotice)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// ȡ���������趨����Ƶ����Ϣ
//-----------------------------------------------------------------------
CMD_START(NGMTC_CancelAutoChatNotice)
CMD_END

CMD_START(NGMTS_CancelAutoChatNotice)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// ȡ�����м�ʱ��Ϣ
//-----------------------------------------------------------------------
CMD_START(NGMTC_CancelTimeLimitedMsg)
CMD_END

CMD_START(NGMTS_CancelTimeLimitedMsg)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// ȡ������˫������
//-----------------------------------------------------------------------
CMD_START(NGMTC_CancelDoubleMsg)
	//DWORD			dwSectionID;						// ����ID
	//DWORD			dwWorldID[1];						// ��Ϸ����ID[n]      ��Ϸ����ID:GT_INVALID����ô�����������Ϸ����	// ����ʱ��
CMD_END

CMD_START(NGMTS_CancelDoubleMsg)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// ȡ���������趨���½�
//-----------------------------------------------------------------------
CMD_START(NGMTC_CancelRightNotice)
CMD_END

CMD_START(NGMTS_CancelRightNotice)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// ���ָ����ҵİٱ���
//-----------------------------------------------------------------------
CMD_START(NGMTC_ClearBaibao)
	TCHAR	szOwnerName[X_SHORT_NAME];
	DWORD	dwSectionID;
	DWORD	dwWorldID;
CMD_END

CMD_START(NGMTS_ClearBaibao)
	DWORD			dwErrorCode;
CMD_END

CMD_START(NGMTS_GMSetFackNumer)
	float			fFackRate;
CMD_END

#pragma pack(pop)
