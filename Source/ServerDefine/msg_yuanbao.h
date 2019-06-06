//-----------------------------------------------------------------------------
//!\file msg_yuanbao.h
//!
//!\date 2009-3-11
//!
//!\brief Ԫ������
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push)
#pragma pack(1)

#include "../WorldDefine/currency_define.h"
#include "../WorldDefine/mall_define.h"

CMD_START(NDBC_LoadAllYBAccount)				// ��ȡ������ҵ�Ԫ�������˻�
CMD_END

CMD_START(NDBS_LoadAllYBAccount)
	INT				nNum;
	tagYBAccount	Account[1];
CMD_END

CMD_START(NDBC_InsertAccount)					// ����һ��Ԫ�������˻�
	DWORD			dwRoleID;
CMD_END;

CMD_START(NDBC_SynAccountYB)					// ͬ��Ԫ�������˻�Ԫ����
	DWORD			dwRoleID;
	INT				nYuanBao;
CMD_END

CMD_START(NDBC_SynAccountSilver)				// ͬ��Ԫ�������˻���Ǯ��
	DWORD			dwRoleID;
	tagMoney		Money;
CMD_END

CMD_START(NDBC_SynSubmitSell)					// ͬ��Ԫ�������˻����۶�����״̬
	DWORD			dwRoleID;
	bool			bSellOrder;
CMD_END

CMD_START(NDBC_SynSubmitBuy)					// ͬ��Ԫ�������˻��չ�������״̬
	DWORD			dwRoleID;
	bool			bBuyOrder;					
CMD_END

CMD_START(NDBC_LoadAllOrder)					// ��ȡ������ҵ�Ԫ�������˵�
CMD_END

CMD_START(NDBS_LoadAllOrder)
	INT				nNum;
	DWORD			dwMaxOrderID;
	tagYuanBaoOrder YBOrder[1];
CMD_END

CMD_START(NDBC_InsertOrder)						// ����Ԫ�����׶���
	tagYBOrder		YBOrder;					
CMD_END

CMD_START(NDBC_CompleteOrder)
	tagCompleteOrder	CompleteOrder;
CMD_END

CMD_START(NDBC_RefreshOrder)
	tagRefresh			RefreshOrder;
CMD_END

CMD_START(NDBC_GetRoleYBOrder)
	DWORD			dwRoleID;
CMD_END

CMD_START(NDBS_GetRoleYBOrder)
	INT				nNum;
	tagYuanBaoOrder YBOrder[1];
CMD_END

CMD_START(NDBC_DeleteYBOrder)					// ɾ�����ݿ���ʱ�䳬����������ж���
	DWORD			dwDeleteTime;
CMD_END

#pragma pack(pop)