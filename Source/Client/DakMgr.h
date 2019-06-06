#pragma once

class NetSession;
class DakFrame;
/**
	class DakMgr
	brief ������վ���ڵĴ���
*/
class DakMgr
{
public:
	DakMgr(void);
	~DakMgr(void);

	static DakMgr* Inst();

	BOOL Init();

	VOID Destroy();

private:
	DWORD OnEvent(tagGameEvent* pEvent);
	BOOL CreateDakFrame();

private:
	TSFPTrunk<DakMgr>				m_Trunk;
	DakFrame*						m_pDakFrame;
};