//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_rt_s.h
// author: 
// actor:
// data: 2008-04-27
// last:
// brief: ��ά���������Ϸ����֮�����Ϣ����
//-----------------------------------------------------------------------------
#pragma once
#include "../ServerDefine/login_define.h"
#include "../ServerDefine/rt_define.h"
#include "../ServerDefine/gm_tool_define.h"
#include "../WorldDefine/base_define.h"

#pragma pack(push, 1)

//-----------------------------------------------------------------------------
// ���ӳ�ʼ��Ϣ ������Ϸ���磬������ά��������GMserver��
//-----------------------------------------------------------------------------
CMD_START(NSC_LoginService)
	DWORD			dwSectionID;					// ����ID
	DWORD			dwWorldID;						// ��Ϸ����ID
	EServerType		eType;							// ����
CMD_END;

//-----------------------------------------------------------------------------
// ��ʼ����ȷ����Ϣ ������ά��������GMserver����������Ϸ����
//-----------------------------------------------------------------------------
CMD_START(NSS_LoginService)
	DWORD			dwErrorCode;					// 0:�ɹ�
CMD_END;

//-----------------------------------------------------------------------------
// ���ӳ�ʼ��Ϣ ������ά����ˣ�GMserver����������Ϸ���籾����ά����RTService��
//-----------------------------------------------------------------------------
CMD_START(NLSS_LoginService)
	EServerType		eType;							// ����
CMD_END;

//-----------------------------------------------------------------------------
// ��ʼ����ȷ����Ϣ ������Ϸ���籾����ά����RTService��,������ά����ˣ�GMserver��
//-----------------------------------------------------------------------------
CMD_START(NLSC_LoginService)
	DWORD			dwErrorCode;					// 0:�ɹ�
CMD_END;

//-----------------------------------------------------------------------------
// �ر���Ϸ�������������Ϣ��������ά����ˣ�GMserver����������Ϸ����
//-----------------------------------------------------------------------------
CMD_START(NSS_Close)

CMD_END

//-----------------------------------------------------------------------------
// �ر���Ϸ�����������ȷ����Ϣ��������Ϸ����,������ά����ˣ�GMserver��
//-----------------------------------------------------------------------------
CMD_START(NSC_Close)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------------
// ǿ�йر���Ϸ�������������Ϣ��������ά����ˣ�GMserver����������Ϸ���籾����ά����RTService��
//-----------------------------------------------------------------------------
CMD_START(NLSS_Close)
	
CMD_END

//-----------------------------------------------------------------------------
// ǿ�йر���Ϸ�����������ȷ����Ϣ��������Ϸ���籾����ά����RTService��,������ά����ˣ�GMserver��
//-----------------------------------------------------------------------------
CMD_START(NLSC_Close)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------------
// ������Ϸ�������������Ϣ��������ά����ˣ�GMserver����������Ϸ���籾����ά����RTService��
//-----------------------------------------------------------------------------
CMD_START(NLSS_Start)

CMD_END

//-----------------------------------------------------------------------------
// ������Ϸ�����������ȷ����Ϣ��������Ϸ���籾����ά����RTService��,������ά����ˣ�GMserver��
//-----------------------------------------------------------------------------
CMD_START(NLSC_Start)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------------
// ��ʱ���ظ�����ά�������ķ�����״̬��Ϣ��������Ϸ���磬������ά����ˣ�GMserver��
//-----------------------------------------------------------------------------
CMD_START(NSC_WorldInfo)
	//TCHAR			szSectionName[X_SHORT_NAME];
	//TCHAR			szWorldName[X_SHORT_NAME];
	//EServerType		eType;
	EWorldStatus	eStatus;
	INT				nOnlineNum;					//��Ϸ������������ 4
	INT				nMaxOnlineNum;				//��Ϸ��������������� 4
	INT				nOnlineNumDisplay;			//��ʾ���������� 4
CMD_END

//-----------------------------------------------------------------------------
// ˫������	��������ά����ˣ�GMserver����������Ϸ����
//-----------------------------------------------------------------------------
CMD_START(NSS_Double)
	DWORD			dwClientID;	
	EDoubleType		eDType;
	DWORD			dwLastTime;						//ʱ�� tagDWORDTime
	DWORD			dwOpenTime;						//����ʱ��
	DWORD			dwRatio;						//����
CMD_END

CMD_START(NSC_Double)
	DWORD			dwClientID;
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------------
// ȡ��˫������	��������ά����ˣ�GMserver����������Ϸ����
//-----------------------------------------------------------------------------
CMD_START(NSS_CancelDouble)
	DWORD			dwClientID;	

CMD_END

CMD_START(NSC_CancelDouble)
	DWORD			dwClientID;
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------------
// �Զ�ѭ������ ��������ά����ˣ�GMserver����������Ϸ����
//-----------------------------------------------------------------------------
CMD_START(NSS_AutoNotice)
	DWORD			dwClientID;
	TCHAR			szContent[Notice_Content];		//����
	EColorType		eType;							//��ɫ
	INT				nCirInterval;					//ѭ�����
	//INT				nCirNum;					//ѭ������
CMD_END

CMD_START(NSC_AutoNotice)
	DWORD			dwClientID;
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// ���½ǹ��� ��������ά����ˣ�GMserver����������Ϸ����
//-----------------------------------------------------------------------
CMD_START(NSS_RightNotice)								
	DWORD			dwClientID;
	TCHAR			szTitle[Notice_Title];			//����
	TCHAR			szContent[Notice_Content];		//����
	TCHAR			szLink[Notice_Link];			//���ӵ���վ
CMD_END

CMD_START(NSC_RightNotice)
	DWORD			dwClientID;
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------------
// ȡ�����½�	��������ά����ˣ�GMserver����������Ϸ����
//-----------------------------------------------------------------------------
//CMD_START(NSS_CancelRightNotice)
//	DWORD			dwClientID;	
//CMD_END
//
//CMD_START(NSC_CancelRightNotice)
//	DWORD			dwClientID;
//	DWORD			dwErrorCode;
//CMD_END

//-----------------------------------------------------------------------
// ϵͳָ����Ϣ ��������ά����ˣ�GMserver����������Ϸ�����RTservice
//-----------------------------------------------------------------------
CMD_START(NLSS_SystemCmd)								
	DWORD			dwClientID;
	TCHAR			szCmd[System_Cmd_Length];
CMD_END

CMD_START(NLSC_SystemCmd)
	DWORD			dwClientID;
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// �޸����������������Ϣ ��������ά����ˣ�GMserver����������Ϸ����
//-----------------------------------------------------------------------
CMD_START(NSS_MaxNum)								
	DWORD			dwClientID;
	INT				nMaxNum;
CMD_END

CMD_START(NSC_MaxNum)
	DWORD			dwClientID;
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// �����̳� ��������ά����ˣ�GMserver����������Ϸ����
//-----------------------------------------------------------------------
CMD_START(NSS_UpdateMall)								
	DWORD			dwClientID;
CMD_END

CMD_START(NSC_UpdateMall)
	DWORD			dwClientID;
	DWORD			dwErrorCode;
CMD_END



//-----------------------------------------------------------------------------
// ��� ����GMServer ����loginserver
//-----------------------------------------------------------------------------
CMD_START(NSS_ForbidAccount)
	DWORD			dwClientID;
	TCHAR			szWorld[X_SHORT_NAME];
	BOOL			bForbid; //0:��� 1:��
	TCHAR           szAccount[1];
CMD_END

//-----------------------------------------------------------------------------
// ��� ����loginserver ����GMServer 
//-----------------------------------------------------------------------------
CMD_START(NSC_ForbidAccount)
	DWORD			dwClientID;
	TCHAR           szAccount[X_SHORT_NAME];
	DWORD			dwErrorCode;
CMD_END


//-----------------------------------------------------------------------------
// ��IP ����GMServer ����loginserver
//-----------------------------------------------------------------------------
CMD_START(NSS_ForbidIP)
	DWORD			dwClientID;
	DWORD			dwIP;
	BOOL			bForbid; //0:��� 1:��
CMD_END

//-----------------------------------------------------------------------------
// ��IP ����loginserver ����GMServer 
//-----------------------------------------------------------------------------
CMD_START(NSC_ForbidIP)
	DWORD			dwClientID;
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------------
// ����Ƶ���Զ����棬������ά����ˣ�GMserver����������Ϸ����
//-----------------------------------------------------------------------------
CMD_START(NSS_AutoChatNotice)
	DWORD			dwClientID;
	TCHAR			szNotice[X_HUGE_STRING];		//����
CMD_END

CMD_START(NSC_AutoChatNotice)
	DWORD			dwClientID;
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------------
// ȡ������Ƶ���Զ�����	��������ά����ˣ�GMserver����������Ϸ����
//-----------------------------------------------------------------------------
//CMD_START(NSS_CancelAutoChatNotice)
//	DWORD			dwClientID;	
//CMD_END
//
//CMD_START(NSC_CancelAutoChatNotice)
//	DWORD			dwClientID;
//	DWORD			dwErrorCode;
//CMD_END

//-----------------------------------------------------------------------------
// ��հٱ�����������ά����ˣ�GMserver����������Ϸ����
//-----------------------------------------------------------------------------
CMD_START(NSS_ClearBaibao)
	DWORD			dwClientID;
	TCHAR			szOwnerName[X_SHORT_NAME];		// ��ɫ����
CMD_END

CMD_START(NSC_ClearBaibao)
	DWORD			dwClientID;
	DWORD			dwErrorCode;
CMD_END

CMD_START(NSS_SetFackNumer)
	float			fFackRate;
CMD_END

#pragma pack(pop)