/********************************************************************
	created:	2009/02/02
	created:	2:2:2009   11:09
	filename: 	d:\loong_client\Client\DungeonSys.h
	file path:	d:\loong_client\Client
	file base:	DungeonSys
	file ext:	h
	author:		leili
	
	purpose:	����ϵͳ����
*********************************************************************/

#pragma once

struct tagNS_SendToPrison;
struct tagRoleGetNameEvent;

class DungeonSys : public GameFrame
{
public:
	DungeonSys(void);
	virtual ~DungeonSys(void);

	// GameFrame
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();

	DWORD		 NetRecvMessage_SendToPrison(tagNS_SendToPrison* pMsg, DWORD pParam);

private:
	DWORD		 OnEvent_GetRoleName(tagRoleGetNameEvent* pEvent);
	VOID		 ShowSendToPrisonInfo(void) const;

private:
	TSFPTrunk<DungeonSys>	m_Trunk;
	TObjRef<NetCmdMgr>		m_pCmdMgr;

	enum EGetRoleNameState
	{
		EGRN_None	= 0x0000,
		EGRN_GetSrc	= 0x0001,
		EGRN_GetDst	= 0x0002
	};
	WORD					m_flag;		// ���ڱ���Ƿ����������
	DWORD					m_srcID;
	DWORD					m_dstID;
	tstring					m_srcName;
	tstring					m_dstName;
};