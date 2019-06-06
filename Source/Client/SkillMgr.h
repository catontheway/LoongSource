#pragma once

#include "..\WorldDefine\skill_define.h"
#include "..\WorldDefine\talent_define.h"
#include "..\WorldDefine\role_att.h"
#include "..\WorldDefine\msg_talent.h"

#define MTransSkillTypeID(skill,lv) (DWORD)(skill*100+lv)
#define MTransSkillID(id) (DWORD)(id/100)
#define MTransSkillLevel(id) (INT)(id%100)
#define MTransProduceSkill(id) (DWORD)(id/10)

const int MAX_BATTLE_TALENT_COUNT = 2;

/** \struct tagSkillData
	\brief �������ݽṹ��δ�꣩
*/
struct tagSkillData
{
	DWORD					dwSkillID;		// ����ID
	DWORD					dwTypeID;		// ����ģ��ID
	INT						nCurCD;			// ��ǰ��ȴֵ�����룩
	INT						nLevel;			// ��ǰ�ȼ�
	INT						nLearnLevel;	// ��ǰѧϰ�ȼ�
	INT						nExp;			// ��ǰ����

	//--��������ӵ�е�����
	INT						nSkillDmg;		// ��������
	FLOAT					fOPDist;		// ���þ��루���絥λ��
	INT						nPrepareTime;	// ����ʱ�䣨���룩
	INT						nCooldown;		// ��ȴʱ�䣨���룩
	DWORD					dwStartTime;	// ��ȴ������ʼʱ��
	INT						nCost[ESCT_End];// ����
						
	tagSkillProtoClient*	pProto;
	
	tagSkillData(){ZeroMemory(this, sizeof(tagSkillData));}
	tagSkillData(DWORD dwID){ZeroMemory(this, sizeof(tagSkillData)); dwSkillID = dwID;}
};

class NetSession;
/** \class SkillMgr
	\brief ���ܹ�����
	\remarks ����ͻ��˱��������ѧ��ļ���
*/
class SkillMgr
{
public:
	SkillMgr(void);
	~SkillMgr(void);

	static SkillMgr* Inst();
	
	void Init();
	void Destroy();
	
	/** \���ݼ���ID��ü�������
	*/
	const tagSkillData* GetSkillData(DWORD dwID);
	/** \���ݼ���TypeID��ü�������
	*/
	const tagSkillData* GetSkillDataByTypeID(DWORD dwTypeID);

	/** \��ȡ��ͨ����ID
	*/
	DWORD GetNormalAttackID();
	/** \��ȡ��ͨ������TypeID
	*/
	DWORD GetNormalAttackTypeID();
	/** \��ȡ�����༼�ܵ�TypeID
	*/
	DWORD GetGatherSkillTypeID(INT nType);

	/** \ʹ�ü���
	*/
	VOID OnUseSkill(DWORD dwTypeID);
	
	
	/** \��ȡ�����б�
	*/
	void GetSkillList(TList<DWORD>& List);
	
	/** \��ȡ����ͼƬ
	*/
	tstring GetSkillIconPath(DWORD dwTypeID);
	/** \�Ƿ��ڿ�Ͷ���ʷ���
	*/
	BOOL IsInsideOfMaxTalent(ETalentType eTalent);
	/** \��ȡ���ʵ���
	*/
	INT GetTalentPoint(ETalentType eTalent);

	/** \�Ƿ��иü���
	*/
	BOOL IsHaveSkillByTypeID(DWORD dwTypeID);
	BOOL IsHaveProduceSkillByTypeID(DWORD dwTypeID);

	/** \����ģ��ID��ȡ�����Ƿ���ȴ
	*/
	BOOL IsSkillCDByTypeID(DWORD dwTypeID);
	/** \��ȡ����CD
	*/
	VOID GetSkillCDTime(DWORD dwID, FLOAT& fCDCount, FLOAT& fRestoreRatio);
	/** \�����Ƿ��ѧϰ
		\param dwTypeIDΪ�ü����ܵ�ԭ��
		\param n8Typeѧϰ����
	*/
	DWORD IsSkillCanLearn(DWORD dwTypeID);

	/** \������Ϣ������ʾ
	*/
	void ShowNetError(INT nErrorCode);

    /** \��õ�ǰ���ս��ϵ���ʸ���
    */
    int GetBattleTalentCount();
    /** \�Ƿ��ô�����
    */
    BOOL IsTalentAquired(ETalentType eType);

private:

	
	/** \�����¼���
	*/
	void AddSkill(tagSkillMsgInfo data);
	/** \ɾ��һ�����м���
	*/
	void RemoveSkill(DWORD dwID);
	/** \����һ������
	*/
	void UpdateSkill(tagSkillMsgInfo data);
	
	/** \�������ʵ���
	*/
	void SetTalentPoint(tagRoleTalent Talent);

	/** \��ʼ����Ҽ���
	*/
	DWORD NetInitLocalPlayerSkill(tagNS_GetRoleInitState_Skill* pNetCmd, DWORD dwParam);

	/** \������Ϣ�Ĵ���
	*/
	DWORD NetAddSkill(tagNS_AddSkill* pNetCmd, DWORD dwParam);
	DWORD NetUpdateSkill(tagNS_UpdateSkill* pNetCmd, DWORD dwParam);
	DWORD NetRemoveSkill(tagNS_RemoveSkill* pNetCmd, DWORD dwParam);
	DWORD NetLearnSkill(tagNS_LearnSkill* pNetCmd, DWORD dwParam);
	DWORD NetLevelUpSkill(tagNS_LevelUpSkill* pNetCmd, DWORD dwParam);
	DWORD NetForgetSkill(tagNS_ForgetSkill* pNetCmd, DWORD dwParam);
	DWORD NetClearTalent(tagNS_ClearTalent* pNetCmd, DWORD dwParam);
	DWORD NetAddTalent(tagNS_AddTalent* pNetCmd, DWORD dwParam);
	DWORD NetRemoveTalent(tagNS_RemoveTalent* pNetCmd, DWORD dwParam);
	DWORD NetUpdateTalent(tagNS_UpdateTalent* pNetCmd, DWORD dwParam);
	DWORD NetUpdateSkillCD(tagNS_UpdateSkillCoolDown* pNetCmd, DWORD dwParam);

	/** \�ͷż���Map
	*/
	void FreeMap(); 
private:
	TSFPTrunk<SkillMgr>			m_Trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<NetSession>		m_pSession;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	
	TMap<DWORD, tagSkillData*>	m_skillMap;
	tagRoleTalent				m_talentPoint[X_MAX_TALENT_PER_ROLE];
};
