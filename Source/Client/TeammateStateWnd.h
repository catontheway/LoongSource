#pragma once

class Role;
class Player;
class RoleHeadPicIcon;
class BuffUIList;
struct tagTeammateStateInitEvent;
struct tagTeamLeaderChangeEvent;
struct tagTeammateLevelChangeEvent;
struct tagTeammateNameUpdataEvent;
struct tagTeammateAttChangeEvent;
struct tagRoleBuffChangeEvent;
struct tagUpdataAvatarEvent;
struct tagUpdataAvatarDisplaySetEvent;

/** \class TeammateStateWnd
    \brief һ�����ѵ���ʾ���ڣ�������ʾ���ѵĿؼ���
	       �ⲿ�ɴ������ʵ����ͬʱ��ʾ������ѵ�״̬
*/
class TeammateStateWnd
{
public:
	enum { BUFFUI_NUM = 20 };

	TeammateStateWnd();
	~TeammateStateWnd();

	BOOL Create( XmlElement* pEle, const DWORD dwRoleID );
	BOOL SafeDestroy();
	VOID Update();
	VOID Render3D();
	
	GUIWnd* GetWnd() { return m_pWnd; }
	DWORD   GetRoleID() const { return m_dwRoleID; }
	VOID    InitState();
private:
	BOOL GUIEventHandler(tagGUIEvent* pEvent);
	VOID UpdataBuffs( Role* pRole );
	/** \��Ϸ�¼���Ӧ
	*/
	DWORD OnTeammateStateInitEvent(tagTeammateStateInitEvent* pEvent);
	DWORD OnTeamLeaderChangeEvent(tagTeamLeaderChangeEvent* pEvent);
	DWORD OnTeammateLevelChangeEvent(tagTeammateLevelChangeEvent* pEvent);
	DWORD OnTeammateNameUpdataEvent(tagTeammateNameUpdataEvent* pEvent);
	DWORD OnTeammateAttChangeEvent(tagTeammateAttChangeEvent* pEvent);
	DWORD OnRoleBuffChangeEvent(tagRoleBuffChangeEvent* pEvent);
	DWORD OnUpdataAvatarEvent(tagUpdataAvatarEvent* pEvent);
	DWORD OnUpdataAvatarDisplaySetEvent(tagUpdataAvatarDisplaySetEvent* pEvent);
private:
	TSFPTrunk<TeammateStateWnd>	    m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<GameFrameMgr>           m_pMgr;

	GUIWnd*                         m_pWnd;                    // ������
	RoleHeadPicIcon*				m_pHead;				   // ͷ��ͼ��
	GUIStatic*                      m_pSt_Name;                // ���Ʊ�ǩ
	GUIStatic*                      m_pSt_Level;               // �ȼ���ǩ
	GUIProgress*                    m_pPro_Hp;                 // HP������
	GUIProgress*                    m_pPro_Mp;                 // MP������
	BuffUIList*                     m_pBuffUIList;
	GUIStatic*                      m_pSt_Captain;             // �ӳ�ͼ��

	DWORD                           m_dwRoleID;
	DWORD							m_dwLastFindRoleTime;
	BOOL                            m_bInView;                 // �Ƿ�����Ұ�� 
};

