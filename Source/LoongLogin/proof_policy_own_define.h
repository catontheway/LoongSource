//-----------------------------------------------------------------------------
//!\file proof_policy_own_define.h
//!\author Aslan
//!
//!\date 2009-06-07
//! last 2008-06-07
//!
//!\brief ���뼤����ӿڶ���
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)

#define X_STRING_LEN	50			// �ַ�������

//-----------------------------------------------------------------------------
// ��ʼ��½ID
//-----------------------------------------------------------------------------
CMD_START(NC_LOGIN)
	DWORD	dwLoginID;				// login ID
CMD_END

CMD_START(NS_LOGIN)
	BYTE	byResult;				// ���ؽ��
CMD_END

//------------------------------------------------------------------------------
// ������Ϣ
//------------------------------------------------------------------------------
CMD_START(NC_HEARTBEAT)
	DWORD	dwLoginID;					// login ID
CMD_END

//------------------------------------------------------------------------------
// ��ҵ���
//------------------------------------------------------------------------------
CMD_START(NC_USERLOGIN)
	CHAR	szGUID[X_STRING_LEN];			// GUID
	DWORD	dwLoginID;						// login ID
	CHAR	szAccountName[X_STRING_LEN];	// �ʺ���
	CHAR	szPsd[X_STRING_LEN];			// ����
	DWORD	dwClientID;						// ���к�
CMD_END

CMD_START(NS_USERLOGIN)
	CHAR	szGUID[X_STRING_LEN];			// GUID
	CHAR	szAccountName[X_STRING_LEN];	// �û���
	CHAR	szPsd[X_STRING_LEN];			// �µ�����
	BYTE	byResult;						// ��֤���
	BYTE	byState;						// 0 �û�״̬����   1�û��˺ű���ͣ
	BYTE	byBlocktype;					// 1����Ա���ķ�ͣ4���ֻ����˺���Ϸ��ͣ 8���ܱ����˺ŷ�ͣ
	BYTE	byGuard;						// ������
	DWORD	dwClientID;						// ���к�
	CHAR	szMibao[MIBAO_LEN];				// �ܱ�
CMD_END

//-------------------------------------------------------------------------------
// �û���������
//-------------------------------------------------------------------------------
CMD_START(NS_USERUPDATEPWD)
	CHAR	szGUID[X_STRING_LEN];			// GUID
	DWORD	dwLoginID;						// login ID
	CHAR	szAccountName[X_STRING_LEN];	// �û���
	CHAR	szPsd[X_STRING_LEN];			// ������
CMD_END

CMD_START(NC_USERUPDATEPWD)
	CHAR	szGUID[X_STRING_LEN];			// GUID
	CHAR	szAccountName[X_STRING_LEN];	// �û���
	DWORD	dwLoginID;						// login ID
	BYTE	byResult;						// ���
CMD_END

//---------------------------------------------------------------------------------
// �û����ܱ�
//---------------------------------------------------------------------------------
CMD_START(NS_USERBINDMIBAO)
	CHAR	szGUID[X_STRING_LEN];			// GUID
	DWORD	dwLoginID;						// login ID
	CHAR	szAccountName[X_STRING_LEN];	// �û���
	CHAR	szMibao[MIBAO_LEN];				// �ܱ�
CMD_END

CMD_START(NC_USERBINDMIBAO)
	CHAR	szGUID[X_STRING_LEN];			// GUID
	CHAR	szAccountName[X_STRING_LEN];	// �û���
	DWORD	dwLoginID;						// login ID
	BYTE	byResult;						// ���
CMD_END

//---------------------------------------------------------------------------------
// �û�ȡ���ܱ�
//---------------------------------------------------------------------------------
CMD_START(NS_USERUNBINDMIBAO)
	CHAR	szGUID[X_STRING_LEN];			// GUID
	DWORD	dwLoginID;						// login ID
	CHAR	szAccountName[X_STRING_LEN];	// �û���
	CHAR	szMibao[MIBAO_LEN];				// �ܱ�
CMD_END

CMD_START(NC_USERUNBINDMIBAO)
	CHAR	szGUID[X_STRING_LEN];			// GUID
	CHAR	szAccountName[X_STRING_LEN];	// �û���
	DWORD	dwLoginID;						// login ID
	BYTE	byResult;						// ���
CMD_END


//---------------------------------------------------------------------------------
// �˺ŷ�ͣ
//---------------------------------------------------------------------------------
CMD_START(NS_BLOCKACCOUNT)
	CHAR	szGUID[X_STRING_LEN];			// GUID
	DWORD	dwLoginID;						// login ID
	CHAR	szAccountName[X_STRING_LEN];	// �û���
	BYTE    byBlockType;					//1����Ա���ķ�ͣ4���ֻ����˺���Ϸ��ͣ 8���ܱ����˺ŷ�ͣ
CMD_END

CMD_START(NC_BLOCKACCOUNT)
	CHAR	szGUID[X_STRING_LEN];			// GUID
	CHAR	szAccountName[X_STRING_LEN];	// �û���
	DWORD	dwLoginID;						// login ID
	BYTE	byResult;						// ���  0 ��ȷ  1ʧ�� 
CMD_END


//---------------------------------------------------------------------------------
// �˺Ž��
//---------------------------------------------------------------------------------
CMD_START(NS_UNBLOCKACCOUNT)
	CHAR	szGUID[X_STRING_LEN];			// GUID
	DWORD	dwLoginID;						// login ID
	CHAR	szAccountName[X_STRING_LEN];	// �û���
	BYTE    byUnBlockType;					//1����Ա���Ľ��  4���ֻ����˺���Ϸ���  8���ܱ����˺Ž��
CMD_END

CMD_START(NC_UNBLOCKACCOUNT)
	CHAR	szGUID[X_STRING_LEN];			// GUID
	CHAR	szAccountName[X_STRING_LEN];	// �û���
	DWORD	dwLoginID;						// login ID
	BYTE	byResult;						// ���
CMD_END

//---------------------------------------------------------------------------------
// �˺����÷�����
//---------------------------------------------------------------------------------
CMD_START(NS_USERUPDATECHENMI)
	CHAR	szGUID[X_STRING_LEN];			// GUID
	DWORD	dwLoginID;						// login ID
	CHAR	szAccountName[X_STRING_LEN];	// �û���
	BYTE    byChenMi;						//0�Ƿ����� 1������
CMD_END

CMD_START(NC_USERUPDATECHENMI)
	CHAR	szGUID[X_STRING_LEN];			// GUID
	CHAR	szAccountName[X_STRING_LEN];	// �û���
	DWORD	dwLoginID;						// login ID
	BYTE	byResult;						// ���
CMD_END



#pragma pack(pop)