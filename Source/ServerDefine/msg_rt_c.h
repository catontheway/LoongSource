//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_rt_c.h
// author: 
// actor:
// data: 2009-04-27
// last:
// brief: ��ά�������ͻ���֮�����Ϣ����
//-----------------------------------------------------------------------------
#pragma once
#include "../ServerDefine/login_define.h"
#include "../WorldDefine/base_define.h"
#include "../ServerDefine/rt_define.h"
#include "../ServerDefine/gm_tool_define.h"

#pragma pack(push, 1)

enum EMsgSet
{
	NRTC_LoginService,
	NRTC_Challenge,
	NRTC_Close,
	NRTC_Start,
};

//-----------------------------------------------------------------------------
// ���ӳ�ʼ��Ϣ��������ά�ͻ��ˣ�RtClient����������ά������(GMserver)
//-----------------------------------------------------------------------------
CMD_START(NRTC_LoginService)				
	
CMD_END;

//-----------------------------------------------------------------------------
// ȷ��������Ϣ��������ά������(GMserver)��������ά��������Rtclient��
//-----------------------------------------------------------------------------
CMD_START(NRTS_LoginService)				
	DWORD			dwErrorCode;				// 0:�ɹ�
CMD_END;

//-----------------------------------------------------------------------------
// �ر���Ϸ�������������Ϣ��������ά�ͻ��ˣ�RtClient����������ά������(GMserver)
//-----------------------------------------------------------------------------
CMD_START(NRTC_Close)
	DWORD			dwSectionID;				//����ID
	EServerType		eType;						//����������
	DWORD			dwWorldID;					//��Ϸ����ID�����ڹر�LoginServer����Ϣ������
	BOOL			bForce;						//FALSE:�����ر� 
												//TRUE:ǿ�ƹر�					
CMD_END

//-----------------------------------------------------------------------------
// �ر���Ϸ�����������ȷ����Ϣ��������ά������(GMserver)��������ά�ͻ��ˣ�RtClient��
//-----------------------------------------------------------------------------
CMD_START(NRTS_Close)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------------
// ������Ϸ�������������Ϣ��������ά�ͻ��ˣ�RtClient����������ά������(GMserver)
//-----------------------------------------------------------------------------
CMD_START(NRTC_Start)
	DWORD			dwSectionID;				//����ID
	EServerType		eType;						//����������
	DWORD			dwWorldID;					//��Ϸ����ID�����ڿ���LoginServer����Ϣ������
CMD_END

//-----------------------------------------------------------------------------
// ������Ϸ�����������ȷ����Ϣ��������ά������(GMserver)��������ά�ͻ��ˣ�RtClient��
//-----------------------------------------------------------------------------
CMD_START(NRTS_Start)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------------
// ����˺Ű󶨵�IP��ַ
//-----------------------------------------------------------------------------
CMD_START(NRTC_BindIP)
	TCHAR			szAccountName[X_SHORT_NAME];
	DWORD			dwIP;
CMD_END

CMD_START(NRTS_BindIP)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------------
// ��������̬��Ϣ����Ϣ��������ά������(GMserver)��������ά�ͻ��ˣ�RtClient��
//-----------------------------------------------------------------------------
struct tagServerInfo
{
	BOOL			bValid;
	EWorldStatus	eStatus;
	INT				nMaxPlayerNum;
	INT				nPlayerOnline;	
	INT				nRealOnline;
};

CMD_START(NRTS_AllServerDynaInfo)
	INT				nSectionNum;				//��������
	BYTE			byInfo[1];					//����ID-login״̬-world����-world[n]ID-World[n]LoongWorld״̬-World[n]LoongDB״̬
	
CMD_END

//-----------------------------------------------------------------------------
// ��������̬��Ϣ����Ϣ��������ά������(GMserver)��������ά�ͻ��ˣ�RtClient��
//-----------------------------------------------------------------------------
CMD_START(NRTS_AllServerStatInfo)
	INT				nSectionNum;				//��������
	BYTE			byInfo[1];					//������-����ID-login IP-world����-world[n]����-world[n]ID-LoongWorldIP-LoongDBIP
CMD_END

//-----------------------------------------------------------------------------
// �˺���֤��Ϣ��������ά�ͻ���RtClient��������ά������(GMserver)
//-----------------------------------------------------------------------------
CMD_START(NRTC_Challenge)
	TCHAR			szAccountName[X_SHORT_NAME];
	DWORD			dwPsdCrc;
CMD_END

//-----------------------------------------------------------------------------
// �˺���֤��ȷ����Ϣ��������ά������(GMserver)��������ά�ͻ��ˣ�RtClient��
//-----------------------------------------------------------------------------
CMD_START(NRTS_Challenge)
	DWORD			dwErrorCode;
	EPrivilege		ePrivilege;
CMD_END

//-----------------------------------------------------------------------------
// Ȩ����Ϣ��������ά������(GMserver)��������ά�ͻ��ˣ�RtClient��
//-----------------------------------------------------------------------------
CMD_START(NRTS_PrivilegeInfo)
	INT				nNum;						// Ȩ�޶�����
	DWORD			dwInfo[1];					// �����ֵĶ�������ʽ��ÿһλ�ֱ�������Ϣ��ִ��Ȩ��
CMD_END

//-----------------------------------------------------------------------------
// ����ϵͳָ�����Ϣ��������ά�ͻ���RtClient��������ά������(GMserver)
//-----------------------------------------------------------------------------
CMD_START(NRTC_SystemCmd)
	DWORD			dwSectionID;				//����ID
	EServerType		eType;						//����������
	DWORD			dwWorldID;					//��Ϸ����ID,db������ָ��
	TCHAR			szCmd[System_Cmd_Length];	//ϵͳָ��
CMD_END

CMD_START(NRTS_SystemCmd)
	DWORD			dwErrorCode;
	EPrivilege		ePrivilege;
CMD_END

//-----------------------------------------------------------------------------
// �����������������������ά�ͻ���RtClient��������ά������(GMserver)
//-----------------------------------------------------------------------------
CMD_START(NRTC_MaxNum)
	DWORD			dwSectionID;				//����ID
	EServerType		eType;						//����������
	DWORD			dwWorldID;					//��Ϸ����ID,db������ָ��
	INT				nMaxNum;					//�����õ������������
CMD_END

CMD_START(NRTS_MaxNum)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------------
// �����̳ǣ�������ά�ͻ���RtClient��������ά������(GMserver)
//-----------------------------------------------------------------------------
CMD_START(NRTC_UpdateMall)
	DWORD			dwSectionID;				//����ID
	EServerType		eType;						//����������
	DWORD			dwWorldID;					//��Ϸ����ID,db������ָ��
CMD_END

CMD_START(NRTS_UpdateMall)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
CMD_START(NRTC_GMQuery)
	
CMD_END

CMD_START(NRTS_GMQuery)
	DWORD		dwErrorCode;
CMD_END

#pragma pack(pop)
