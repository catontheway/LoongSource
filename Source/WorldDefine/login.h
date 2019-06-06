#pragma once

#include "msg_common_errorcode.h"
#define MAX_MD5_ARRAY	36
#define MAX_MIBAO		6

#pragma pack(push, 1)

//--------------------------------------------------------------------------------------------
// Login �������
//--------------------------------------------------------------------------------------------
enum
{
	E_ProofResult_Account_No_Match				= 1,	//���ʺŷǷ�(�ʻ������ڻ������벻��,�ܱ�����)
	E_ProofResult_Account_In_Use				= 2,	//���ʺŵ�ǰ�Ѿ���ʹ��
	E_ProofResult_Account_Freezed				= 3,	//���ʺű�ͣ��
	E_ProofResult_Wrong_Build_Number			= 4,	//�汾�Ų���
	E_ProofResult_Disabled						= 5,	//���ʺ��Ѿ�����ͣ
	E_ProofResult_Queue							= 6,	//��ʼ�Ŷ�	
	E_ProofResult_Mibao_Error					= 7,	//�ܱ�����
	E_ProofResult_Wrong_Type					= 8,	//�Ƿ���֤����
	E_ProofResult_Proofing						= 9,	//������֤���Ѿ���֤���

	E_SelectWorld_Server_Full					= 11,	//�÷�������ǰӵ��
	E_SelectWorld_Server_Suspended				= 12,	//�÷�������ǰ����
	E_SelectWorld_Server_Maintenance			= 13,	//�÷�������ǰά��
	E_SelectWorld_Failed						= 14,	//�����쳣��ѡ��ʧ��
	E_SelectWorld_Short_Time					= 15,	//���ʱ��̫��,���Ժ�����
	E_SelectWorld_No_Select						= 16,	//���Ѿ�ѡ��ɹ�,������ѡ
	E_SelectWorld_GameWorldName_Wrong			= 17,	//�Ƿ�����Ϸ������


	E_ProofResult_Forbid_MemberCenter			= 18,	//���Ļ�Ա�����˺��ѱ���ͣ������ϵ�ͷ�
	E_ProofResult_Forbid_GMTool					= 19,	//������Ϸ�˺��ѱ���ͣ������ϵ�ͷ�	
	E_ProofResult_Forbid_CellPhone				= 20,	//�����˺��ѱ�����������ϵ�ͷ�
	E_ProofResult_Forbid_MiBao					= 21,	//�����˺��Ѿ���ʧ�ܱ���������ɽ����¼
};

//---------------------------------------------------------------------------------------------
// �Ŷ�ʱ��10����
//---------------------------------------------------------------------------------------------
#define QUEUE_TIME	10*60


//-----------------------------------------------------------------------------
// ��֤��Ϣ
//-----------------------------------------------------------------------------
CMD_START(NLC_SafeProof)
	DWORD	dwProofKey;						// ��ȫ��֤key
	CHAR	szUserName[X_SHORT_NAME];		// �û���
	DWORD	dwType;							// ��֤��ʽ
	DWORD	dwCurVersionID;					// ��ǰ�汾ID
	DWORD	dwWorldNameCrc;					// ��Ϸ��������crc
	CHAR	szPsd[MAX_MD5_ARRAY];			// ����
	CHAR	szGUID[X_LONG_NAME];			// GUIDֵ
CMD_END;

CMD_START(NLS_ProofResult)
	DWORD	dwErrorCode;					// ������
	BOOL	bGuard;							// ������
	DWORD	dwIndex;						// �Ⱥ����к�
	DWORD	dwVerifyCode;					// ��½���������͸��ͻ��˵���֤�룬����ZoneServerʱҪ�õ�
	DWORD	dwAccountID;					// �ʺ�ID
	DWORD   dwIP;							// ZoneServer IP ��ַ
	DWORD   dwPort;							// ZoneServer �˿ں�
CMD_END;

//------------------------------------------------------------------------------
// �ܱ�
//------------------------------------------------------------------------------
CMD_START(NLS_MiBao)
	CHAR	szMiBao[MAX_MIBAO];				// �ܱ�������ʾ��
CMD_END

CMD_START(NLC_MiBao)
	DWORD	dwMBCrc;						// �ܱ�crc
CMD_END

//-------------------------------------------------------------------------------
// �Ŷ�
//-------------------------------------------------------------------------------
CMD_START(NLS_Queue)
	DWORD	dwPosition;						// �Ŷӵ�λ��
	DWORD	dwTime;							// �Ŷӵ�ʣ��ʱ��
CMD_END;



//-------------------------------------------------------------------------------
//	��¼Э����֤��			by: Jampe
//-------------------------------------------------------------------------------

#define PK_StrToID(n, s)		do{													\
									n = (DWORD)strlen(s);							\
									DWORD step = (n >> 5) + 1;						\
									for(DWORD i = n; i >= step; i -= step)			\
										n = n ^ ((n << 5) + (n >> 2) + s[i - 1]);	\
								}while(0)

#define ProofKey(k, p)			do{												\
									DWORD n[3];									\
									PK_StrToID(n[0], p->szUserName);			\
									PK_StrToID(n[1], p->szPsd);					\
									PK_StrToID(n[2], p->szGUID);				\
									k = p->dwWorldNameCrc + p->dwCurVersionID;	\
									k = (k << 2) + p->dwType;					\
									k = k + n[0] + n[1] + n[2];					\
								}while(0)


#pragma pack(pop)

