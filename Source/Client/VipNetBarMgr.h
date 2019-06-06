#pragma once

struct tagNS_GetRoleInitState_Att;
struct tagNS_GetRemoteRoleState;
struct tagNS_GetVipNetBarName;
struct tagNS_VipNetBarGift;

/** \class VipNetBarMgr
    \brief �����������ӹ���
*/
class VipNetBarMgr
{
public:
	static VipNetBarMgr* Inst() { static VipNetBarMgr v; return &v; }

	void	Init();
	void	Destroy();
	void	Update();
	tstring GetVipNetBarName( DWORD dwRoleID ) ;
private:
	VipNetBarMgr();
	~VipNetBarMgr();

	DWORD OnNS_GetVipNetBarName( tagNS_GetVipNetBarName* pCmd, DWORD );
	DWORD OnNS_VipNetBarGift( tagNS_VipNetBarGift* pCmd, DWORD );
private:
	TSFPTrunk<VipNetBarMgr>			m_trunk;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<NetSession>				m_pSession;

	std::map<DWORD, tstring>		m_mapNetBarName;	// �������������б�
	pair<DWORD, list<DWORD> >		m_paReqRole;		// ͨ��IDȡ
};