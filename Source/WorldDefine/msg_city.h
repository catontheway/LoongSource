//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_city.h
// author: 
// actor:
// data: 2009-11-09
// last:
// brief: ����ϵͳ��Ϣ
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/city_define.h"
#include "../WorldDefine/msg_common_errorcode.h"

#pragma pack(push, 1)

//----------------------------------------------------------------------------
// ��ȡ��������
//----------------------------------------------------------------------------
CMD_START(NC_GetCityInfo)
	DWORD					dwNPCID;
CMD_END

CMD_START(NC_GetCityInfoByCityID)
	DWORD					dwCityID;
CMD_END

CMD_START(NS_GetCityInfo)
	DWORD					dwErrorCode;
	tagCityAtt2Client		sCityAtt;
CMD_END

CMD_START(NC_GetAppointedCityAtt)
	DWORD					dwNPCID;
	ECityAttType			eCityAttType;
CMD_END

CMD_START(NS_GetAppointedCityAtt)
	tagCityAppointedAtt		sCityAtt;
CMD_END

//----------------------------------------------------------------------------
// ��ȡ��ս������Ϣ
//----------------------------------------------------------------------------
CMD_START(NC_GetSigupInfo)
	DWORD					dwNPCID;
CMD_END

CMD_START(NS_GetSigupInfo)
	DWORD					dwErrorCode;
	DWORD					dwCityID;	   //����id
	DWORD					dwAvgAckMoney; //����������ƽ������
	DWORD					dwGuildID;	   //��ǰռ��ó��еİ���id �����npcռ����ΪGT_INVALID
	tagWarTime				WarTime;	   //��սʱ��
	INT32					nAckNum;
	INT32					nDefNum;
	INT32					nConfirmNum;   //��ȷ�Ϸ��ص�
	DWORD					dwData[1];
CMD_END

//----------------------------------------------------------------------------
// �ط�ȷ��
//----------------------------------------------------------------------------
CMD_START(NC_ConfirmDefenceForce)
	DWORD					dwNPCID;
	DWORD					dwGuildID[MAX_CITY_DEFENCE_FORCE];
CMD_END

CMD_START(NS_ConfirmDefenceForce)
	DWORD					dwErrorCode;
CMD_END

//----------------------------------------------------------------------------
// ����˰��
//----------------------------------------------------------------------------
CMD_START(NC_SetTaxRate)
	DWORD					dwNPCID;
	INT32					nTaxRate;
CMD_END

CMD_START(NS_SetTaxRate)	// �㲥
	DWORD					dwErrorCode;
	DWORD					dwCityID;
	INT32					nTaxRate;
CMD_END

//----------------------------------------------------------------------------
// ��ȡ˰��
//----------------------------------------------------------------------------
CMD_START(NC_TransformTaxation)
	DWORD					dwNPCID;
	ETaxTransformType		eTransType;
	INT32					nTaxation;
CMD_END

CMD_START(NS_TransformTaxation)
	DWORD					dwErrorCode;
	INT32					nTaxation;		//ʣ��˰��
CMD_END

//----------------------------------------------------------------------------
// ��ս����
//----------------------------------------------------------------------------

//������ͳ�ΰ��ɵ�ʱ��
CMD_START(NC_SignupForCitywar)
	ECityForceType			eForce;
	INT32					nFare; //��������ʱ�ĳ���
CMD_END

CMD_START(NS_SignupForCitywar)
	DWORD					dwErrorCode;
CMD_END


//----------------------------------------------------------------------------
// �鿴��ǰ��ս��Ϣ

CMD_START(NC_GetCityWarCity)
CMD_END

CMD_START(NS_GetCityWarCity)
	INT32 nNum;
	DWORD dwCityID[1];
CMD_END


CMD_START(NC_GetCityWarInfo)	
	DWORD dwCityID;
CMD_END

CMD_START(NS_GetCityWarInfo)
	DWORD dwErrorCode;
	DWORD dwAttacks[MAX_CITY_ATTACK_FORCE];			// ������
	DWORD dwDefenses[MAX_CITY_DEFENCE_FORCE + 1];	// ������
CMD_END


//----------------------------------------------------------------------------
// ����������
//----------------------------------------------------------------------------

CMD_START(NC_UpgradeProlificacy)
	DWORD dwNPCID;
	EProlificacyUpgrade eType;
CMD_END

CMD_START(NS_UpgradeProlificacy)	
	DWORD dwErrorCode;
CMD_END

#pragma pack(pop)

