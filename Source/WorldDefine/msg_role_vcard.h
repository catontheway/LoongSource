//-----------------------------------------------------------------------------
// filename: msg_role_info.h
// author: client/server hyu/xlguo
// actor:
// data: 2009-02-02
// last: 2009-02-03
// brief: ��ɫ�����ͻ��˷������Ϣ����
//-----------------------------------------------------------------------------
#pragma once
#pragma pack(push, 1)

#include "vcard_define.h"


//----------------------------------------------------------------------------
// ��ȡ��ɫ������Ϣ
//----------------------------------------------------------------------------
CMD_START(NC_GetRoleVCard)
	DWORD			dwRoleID;			// ��ɫID
CMD_END

CMD_START(NS_GetRoleVCard)
	DWORD			dwErrorCode;		// ������
	DWORD			dwRoleID;			// ��ɫID
//	tagVCardData	VCardData;			// ��������
	BYTE			pData[1];				// ��������
CMD_END

//----------------------------------------------------------------------------
// ���ý�ɫ������Ϣ
//----------------------------------------------------------------------------
CMD_START(NC_SetRoleVCard)
	DWORD				dwRoleID;			// ��ɫID
	tagCustomVCardData	customVCardData;	// �Զ�����Ϣ
CMD_END

CMD_START(NS_SetRoleVCard)
	DWORD			dwErrorCode;		// ������
	DWORD			dwRoleID;			// ��ɫID
CMD_END

//----------------------------------------------------------------------------
// ��ý�ɫͷ��URL��Ϣ
//----------------------------------------------------------------------------
CMD_START(NC_GetRoleHeadPicURL)
    DWORD           dwRoleID;           //!< ��ɫID
CMD_END

CMD_START(NS_GetRoleHeadPicURL)
    DWORD           dwRoleID;
    BYTE            byHeadUrlSize;      //!< ͷ��ߴ�(byte����)
    BYTE            byData[1];          //!< ͷ��URL������'\0'����
CMD_END

#pragma pack(pop)