#pragma once
#include ".\NetSession.h"
#include "..\WorldDefine\role_info.h"
#include "..\WorldDefine\msg_role_title.h"
#include "..\WorldDefine\msg_role_vcard.h"
/**	\class PlayerNameTab
	\brief �洢���������õ�����������ֺ�id
*/
class GameFrame;
struct tagRoleGetIDEvent
	:public tagGameEvent
{
	DWORD	dwRoleNameCrc;	// ��ɫ����crc32
	DWORD	dwRoleID;		// ��ɫ����crc32
	DWORD	dwParam;
	tagRoleGetIDEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		:tagGameEvent(szEventName, pSenderFrame)
	{}
};
struct tagRoleGetNameEvent
	: public tagGameEvent
{
	BOOL	bResult;		// TRUE = �ɹ�
	DWORD	dwRoleID;		// ����Ŀ����ҵ�roleid
	tstring	strRoleName;	// ��ɫname
	DWORD	dwParam;
	tagRoleGetNameEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		:tagGameEvent(szEventName, pSenderFrame)
	{}
};

struct tagRoleGetSomeNameEvent
	: public tagGameEvent
{
	std::vector<DWORD> IDs;	// ȡ������ЩID��������
	tagRoleGetSomeNameEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName, pSenderFrame)
	{}
};

struct tagGetNameByNameID : public tagGameEvent
{
	BOOL	bResult;		//!< TRUE = �ɹ�
	DWORD	dwNameID;		//!< �����NameID
	tstring	strName;		//!< Name
	tagGetNameByNameID(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName, pSenderFrame) {}
};

struct tagGetHeadpicByID : public tagGameEvent
{
    DWORD       dwRoleID;
    tstring     strImageURL;
    tagGetHeadpicByID(LPCTSTR szEventName, GameFrame* pSenderFrame)
        : tagGameEvent(szEventName, pSenderFrame) {}
};

struct tagRoleGetTitleEvent
	: public tagGameEvent
{
	DWORD		dwRoleID;		// ��ɫID
	DWORD		dwTitleID;		// ��ΪGT_INVALID,��Ч
	DWORD		dwParam;
	tagRoleGetTitleEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		:tagGameEvent(szEventName, pSenderFrame)
	{}
};

class PlayerNameTab
{
public:
	PlayerNameTab(void);
	~PlayerNameTab(void);

	static PlayerNameTab* Inst();

	void Init();
	void Destroy();

	// ÿ֡����
	void Update();

	//!	ʧ���򷵻�GT_INVALID
	DWORD FindIDByName(const TCHAR* szName);

	//!	ʧ���򷵻�NULL,���Ϸ���ID����GFC_NULL
	const TCHAR* FindNameByID(DWORD id);

	//! ʧ�ܷ��ؿ��ַ�
	const TCHAR* FindNameByNameID(DWORD dwNameID);

    //! ʧ�ܷ��ؿ��ַ�
    LPCTSTR FindHeadPicURLbyID(DWORD dwRoleID);
	
	//���Ƿ��ܹ��õ������һ������.
	//��ȫ��ȡ�÷���true
	//����ȫ��ȡ�÷���false, �ҽ�����ȡ�õ�����ͨ��Cantfinds������
	bool CanGetBundleOfNames(const std::vector<DWORD>& IDs, std::vector<DWORD>& Cantfinds);

	bool IsIDExist(DWORD dwID)
	{
		if(m_id2name.find(dwID) != m_id2name.end()) 
			return true;
		else
			return false;
	}

	// ��Ӧ������Ϣ
	void OnNetRoleGetID(tagNS_RoleGetID* pNetCmd, DWORD dwParam);
	
	//! ͨ��NameIDȡ����
	void OnNS_GetNamebyNameID(tagNS_GetNamebyNameID* pNetCmd, DWORD dwParam);	//by hyu

    //! ͨ��idȡheadpic
    void OnNS_GetRoleHeadPicURL(tagNS_GetRoleHeadPicURL* pNetCmd, DWORD dwParam);

	// ��Ӧ���������ֵ���Ϣ
	void OnNS_RoleGetSomeName(tagNS_RoleGetSomeName *pCmd, DWORD);

protected:
	//��ӱ����¼
	void AddRecord(DWORD id,const TCHAR* szName);

private:
	TSFPTrunk<PlayerNameTab>	m_Trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<NetSession>			m_pSession;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetCmdMgr>			m_pCmdMgr;

	map<DWORD,tstring>	m_id2name;
	map<tstring,DWORD>	m_name2id;				// Role��Name��ID��һһ��Ӧ
	map<DWORD, tstring> m_NameID2Name;			// NmaeID��Name��Ӧ����� added by hyu
    map<DWORD, tstring> m_RoleID2HeadPicUrl;	// RoleID��ͷ��URL��Ӧ�����

	// ����ͨ����������ID�Ļ���<NameCrc,Name>
	map<DWORD,tstring>	m_mapName2IDReq;

	pair<DWORD, list<DWORD> >	m_paReqRole;		 // ͨ��IDȡ����
    //ͨ��IDȡͷ��URL�Ļ���
    set<DWORD>          m_setRoleID2HeadPic;
	//! ͨ��NameIDȡName�Ļ��� added by hyu
	set<DWORD>			m_setNameID2NameReq;
};