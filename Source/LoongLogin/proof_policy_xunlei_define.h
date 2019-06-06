//-----------------------------------------------------------------------------
//!\file proof_policy_xunlei_define.h
//!\author songg
//!
//!\date 2009-06-22
//! last 2008-06-22
//!
//!\brief xunlei��½�����ӿڶ���
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)

//char������ ����룬�Ҳ��ո�
//byte������ ����룬�Ҳ�0��

#define XL_GAMEID		"00025"

#define XL_CMD_LOGIN	"LOGIN"

#define XL_DECRYPT_REQUIRE_DATA (112) //���������ݳ���

#define XL_ENCRYPT_REQUIRE_DATA (116) //���� �������ݳ���
#define XL_MAX_LEN	(500)			// ��󳤶�
//-----------------------------------------------------------------------------
// ��½����֡�ṹ
//-----------------------------------------------------------------------------
typedef struct tagXunLeiLoginFrame_t{
	
	//DWORD	dwFrameSize;			//size ���������ֶα����С
	CHAR	szGameID[5];			//��ʾ��Ϸ������
	CHAR	szProtocolVersion[5];	//Э��汾
	CHAR	szEncryptData[XL_MAX_LEN];		//��������

	tagXunLeiLoginFrame_t()
	{
		//dwFrameSize = 0;
		memset(szGameID,' ',sizeof(szGameID));
		memset(szProtocolVersion,' ',sizeof(szProtocolVersion));
		memset(szEncryptData,' ',sizeof(szEncryptData));
	}
	
}stXunLeiLoginFrame;

//-----------------------------------------------------------------------------
// 112��byte��δ������������   ���ܺ����stXunLeiLoginFrame.szEncryptData��
//-----------------------------------------------------------------------------
typedef struct tagXunLeiRequireData_t{

	CHAR	szCommand[10];			//����
	USHORT	usSerialNum;				//���к�
	BYTE	byAccountData[100];		//�˺�������ܰ�	��Ѹ�׹�˾�ṩ�ĵ�¼������


	tagXunLeiRequireData_t()
	{
		memset(szCommand,' ',sizeof(szCommand));		
		usSerialNum = 0;
		memset(byAccountData,' ',sizeof(byAccountData));
	}

}stXunLeiRequireData;


//-----------------------------------------------------------------------------
// δ���� �䳤�ķ�������
//-----------------------------------------------------------------------------
typedef struct tagXunLeiReturnData_t{

	CHAR	szReturnCode[4];		//������
	USHORT	usSerialNum;			//���к� ��Ӧ���������е����к����

	CHAR	szAccountName[20];			//�˺�/�˺�ID�����ش������0000ʱ���и��ֶΡ�

	CHAR	szGuard[1];				//������

	CHAR	szReverse[10];			//����Ҫ�в�Ȼ���ܻ����

	tagXunLeiReturnData_t()
	{
		memset(szReturnCode,' ',sizeof(szReturnCode));		
		usSerialNum = 0;
		memset(szAccountName,' ',sizeof(szAccountName));
		memset(szGuard,' ',sizeof(szGuard));
	}

}stXunLeiReturnData;



#pragma pack(pop)