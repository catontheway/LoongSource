#include "stdafx.h"
#include "Strength.h"
#include "WorldRankings.h"
#include "db_session.h"
#include "../ServerDefine/msg_strength.h"

WorldRankingMgr g_WorldRankingMgr;

VOID WorldRankingMgr::Update()
{
	tagDWORDTime NowTime= GetCurrentDWORDTime();

	// ÿ�����ĸ���
	if ( NowTime.hour == 0 && NowTime.min == 0 && (NowTime.sec < 1) )
	{
		// ʵ�����а�
		tagNDBC_UpdateRankings senda;
		senda.eType = ERT_Strength;
		g_dbSession.Send(&senda, senda.dwSize);

		// �ʽ����а�
		tagNDBC_UpdateRankings sendb;
		sendb.eType = ERT_Money;
		g_dbSession.Send(&sendb, sendb.dwSize);

		// �������а�
		tagNDBC_UpdateRankings sendc;
		sendc.eType = ERT_Fame;
		g_dbSession.Send(&sendc, sendc.dwSize);

		// �ȼ����а�
		tagNDBC_UpdateRankings sendd;
		sendd.eType = ERT_Level;
		g_dbSession.Send(&sendd, sendd.dwSize);
	}

	return;
}