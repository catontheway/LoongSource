#pragma once

#include "Render2D.h"
#include "ClientCamera.h"
#include "..\WorldDefine\msg_combat.h"

struct tagShowHPChangeEvent;
struct tagUpdateLPLevelExp;
struct tagUpdateLPLevel;
struct tagNS_LearnSkill;
struct tagPctNExpAchieve;
struct tagLRoleCombatState;
struct tagAddTeammateEvent;
struct tagLeaveTeamEvent;
struct tagReputeChange;
struct tagReputeLvlChange;
struct tagUpdateLPAttEvent;

enum EChangeType
{
	EChange_NULL = 0,	
	EChange_HPInc,			// HP����
	EChange_HPDec,			// HP����
	EChange_MPInc,			// MP����
	EChange_MPDec,			// MP����
	EChange_Miss,			// MISS
	EChange_Crit,			// ����
	EChange_Blocked,		// ��
	EChange_EnterSafe,		// ���밲ȫ��
	EChange_ExitSafe,		// �뿪��ȫ��
	EChange_Exp,			// ����仯
	EChange_NExp,			// N%������
	EChange_Skill,			// ʹ��XX����
	EChange_StudySkill,		// ѧ��XX����
	EChange_EnterCombat,	// ����ս��
	EChange_ExitCombat,		// �뿪ս��
	EChange_EnterTeam,		// �������
	EChange_ExitTeam,		// �뿪����
	EChange_Reputation,		// �����仯
	EChange_ReputaLel,		// �����ȼ�
	EChange_HPPer10,		// 10%hp��ʾ
	EChange_MPPer10,		// 10%mp��ʾ
};

// Ʈ����Ϣ
struct tagFloatInfo
{
	DWORD		roleID;			// �ĸ���ɫ�ı仯
	EChangeType eTypeChange;	// �仯����
	float		startTime;		// ʲôʱ�����
	tagPoint	ptTxtSize;		// ��Ϣ�Ŀ��
	float		stayTime;		// ͣ��ʱ��
	TCHAR		szInfo[64];		// �仯��ֵ
	POINT		ptOffset;		// ƫ�ƣ���ֹ�ص�)
	Vector3		worldPos;		// ��������
	Vector3		srcPos;
	POINT		pt;				// ��Ļ����λ��
	Color4ub	fontColor;
	int			nWidth;
	int			nHeight;
	bool		bFrist;

	tagFloatInfo()
	{
		memset(this, 0, sizeof(*this));
		eTypeChange = EChange_NULL;
		stayTime = 1.0f;
	}

	tagFloatInfo(DWORD dwRoleID)
	{
		memset(this, 0, sizeof(*this));
		roleID = dwRoleID;
		startTime = Kernel::Inst()->GetAccumTime();		//��¼��ʼʱ��
		stayTime = 1.0f;
	}
};

/**class RoleFloatTips
   brief ��ɫͷ��Ʈ����tips��Ϣ
*/
class RoleFloatTipsFrame :
	public GameFrame
{
public:
	RoleFloatTipsFrame(void);
	~RoleFloatTipsFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual VOID Render3D();
	virtual VOID OnEvent(tagGameEvent* pEvent);
	virtual VOID Update();
	virtual BOOL ReloadUI();

	//--��Ϸ�¼�
	DWORD OnShowHPChangeEvent(tagShowHPChangeEvent* pEvent);
	DWORD OnUpdateLPLevelExp(tagUpdateLPLevelExp* pEvent);
	DWORD OnPctNExpAchieve(tagPctNExpAchieve* pEvent);
	DWORD OnLRoleCombatState(tagLRoleCombatState* pEvent);
	DWORD OnAddTeammateEvent(tagAddTeammateEvent* pEvent);
	DWORD OnLeaveTeamEvent(tagLeaveTeamEvent* pEvent);
	DWORD OnReputeChange(tagReputeChange* pEvent);
	DWORD OnReputeLvlChange(tagReputeLvlChange* pEvent);
	DWORD OnUpdateLPAttEvent(tagUpdateLPAttEvent* pEvent);

	//--������Ϣ
	DWORD OnNS_Skill(tagNS_Skill* pMsg, DWORD dwParam);
	DWORD OnNS_LearnSkill(tagNS_LearnSkill* pMsg, DWORD dwParam);

private:
	// ���ƮHP��Ϣ
	void PushHPInfo(DWORD roleID, int damage, bool bMiss=false, bool bCrit=false, bool bBlocked=false);
	// ���ƮMP��Ϣ
	void PushMPInfo(DWORD roleID, int damage);
	// ���������ȫ��
	void PushEnterOrExitSafe(DWORD roleID, bool bEnter=true);
	// ����仯
	void PushUpdateLPLevelExp(INT nExpInc);
	// ʹ�ü���
	void PushLocalRoleUseSkill(DWORD dwSkill);
	// ѧ�Ἴ��
	void PushStudySkill(DWORD dwSkillID);


	//����Ʈ����Ϣ
	void CreateFloatInfo(tagFloatInfo& info);
	//���ƽ�ɫͷ��Ʈ������Ϣ
	void DrawFloatInfo();
	//�����ֲ�����λ�÷�ֹλ���ص�
	void CalCreateFontPos(tagFloatInfo& info);
	//�����ֵ�λ��
	void CalFontPos(const tagFloatInfo& info,int nChange, POINT& pt);
	//�����ֵ���ɫ
	void CalFontColor(DWORD dwRoleID, EChangeType eType, Color4ub &fontColor);

	//10%hp / 10%mp��ʾ
	void OnHPorMpExcessPer10();

private:
	TSFPTrunk<RoleFloatTipsFrame>		m_Trunk;
	TObjRef<GUIRender>					m_pRender;
	TObjRef<GUISystem>					m_pSystem;
	TObjRef<NetCmdMgr>					m_pCmdMgr;
	TObjRef<Util>						m_pUtil;
	vector<tagFloatInfo>				m_vFloatInfoArray;	

	PicFontEx							m_AttackOtherPicFont;			//ͼƬ��
	PicFontEx							m_AttackOwnPicFont;
	IDepthFont							*m_pFont;
	INT									m_nMP;

	BOOL								m_bHPPer10;
	BOOL								m_bMPPer10;
};
