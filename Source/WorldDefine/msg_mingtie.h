//-----------------------------------------------------------------------------
//!\file msg_mingtie.h
//!
//!\date 2008-12-29
//!
//!\brief �ͻ����������֮�������������Ϣ
//-----------------------------------------------------------------------------

#pragma once
#include "msg_common_errorcode.h"
#include "mingtie.h"

#pragma pack(push, 1)

//���󷵻���
enum
{
};

//-----------------------------------------------------------------------------
//	����������Ҫ�������Ϣ
//-----------------------------------------------------------------------------
CMD_START(NC_RoleMingTie)
	DWORD	dwRoleID;		// ��ɫID
CMD_END

CMD_START(NS_RoleMingTie)
	BYTE	    bySex;	                                                    //�Ա�
	EHoroscope  eHoroscope;                                                 //����
	EZodiac     eZodiac;                                                    //��Ф
	ERegion     eRegion;                                                    //����
	EProvince   eProvince;                                                  //ʡ��
	ECity       eCity;                                                      //����
	TCHAR       szIndividuality_Signature[MAX_INDIVIDUALITY_SIGNATURE];     //����ǩ��
	DWORD       dwErrorCode;                                                //���ش�����
	CHAR        buff[1];                                                    //ͼƬ����, �䳤
CMD_END

#pragma pack(pop)
