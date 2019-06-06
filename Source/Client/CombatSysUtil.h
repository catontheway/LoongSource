#pragma once
#include "..\WorldDefine\move_define.h"
#include "..\WorldDefine\ItemDefine.h"
#include "AvatarAniPlayer.h"

class ChatFrame;
class SceneCenterInfoFrame;
class Role;
class NPC;
class AudioSys;
class Player;
/** \class CombatSysUtil
	\brief ս��ϵͳ������
*/
class CombatSysUtil
{
public:
	/** \��ʼ����ս��ϵͳ��ʼ��ʱ����
	*/
	void Init();
	/** \�ͷ�����
	*/
	void Destroy();
	/** \��������ƶ���������
	*/
	Vector3 CalcKeyMoveDir(EWalkDir dir,float cameraYaw);
	/** \������ϵͳս��Ƶ����ʾս����Ϣ
	*/
	void ShowCombatMsg(const TCHAR* szFormat,...);
	/** \������Ƶ����ϵͳƵ����ʾ��Ϣ
	*/
	void ShowSystemMsg(const TCHAR* szFormat,...);
	/** \����Ļ������ʾ��Ϣ
	*/
	void ShowScreenCenterMsg(const TCHAR* szFormat,...);
	/** \����Ļ������ʾ��Ϣ(����)
	*/
	void ShowScreenCenterMsgEx(const TCHAR* szFormat,...);
	/** \��ʾPK��ش�����Ϣ
	*/
	void ShowPKErrorMsg(DWORD dwErrorCode);
	/** \��ʾ��Ϊ��������Ϣ
	*/
	void ShowActionCheckMsg(DWORD dwErrorCode);
	/** \��ʾʹ�ü���ʱ���������صĴ�����Ϣ
	*/
	void ShowSkillSvrErrMsg(DWORD dwErrorCode);
	/** \��ʾʹ����Ʒʱ���������صĴ�����Ϣ
	*/
	void ShowItemSvrErrMsg(DWORD dwErrorCode);
	/** \��ʾ���Ÿ��Զ���ʱ�Ĵ�����Ϣ
	*/
	void ShowPlayStyleActionErrMsg(DWORD dwErrorCode);
	/** \��������������ͣ����ر�ս����
	*/
	const TCHAR* GetReadyTrackName(EItemTypeEx weapon);
	/** \��������������͵ȣ��������ܶ���
	*/
	const TCHAR* GetMoveTrackName(EItemTypeEx weapon,bool bInCombat,float speed);
	/** \��������������ͣ��������߶���
	*/
	const TCHAR* GetWalkTrackName(EItemTypeEx weapon,bool bInCombat);
	/** \��������������ͣ����ر��ܶ���
	*/
	const TCHAR* GetRunTrackName(EItemTypeEx weapon,bool bInCombat);
	/** \��������������ͣ����س�̶���
	*/
	const TCHAR* GetRushTrackName(EItemTypeEx weapon,bool bInCombat);
	/** \��������������ͣ����ظ񵵶���
	*/
	const TCHAR* GetBlockTrackName(EItemTypeEx weapon);
	/** \��������������ͣ�����վ������
	*/
	const TCHAR* GetStandTrackName(EItemTypeEx RWeapon);
	/** \��������������ͣ����ؽ��뱸ս����
	*/
	const TCHAR* GetEnterCombatTrackName(EItemTypeEx RWeapon);
	/** \��������������ͣ������뿪��ս����
	*/
	const TCHAR* GetLeaveCombatTrackName(EItemTypeEx RWeapon);
	/** \���ɱ���������
	*/
	const TCHAR* GetBeattackTrackName();
	/** \����NPC�ƶ��ٶȣ��������ܵȶ���
	*/
	const TCHAR* GetNPCMoveTrackName(float moveSpeed);
	/**	\������ﶯ��
	*/
	const TCHAR* GetMountTrackName(AvatarAniPlayer::EAniType aniType,float moveSpeed);
	/** \���ɼ��ܺ�ʹ����Ʒ��Ϣ���к�
	*/
	static DWORD MakeSkillMsgSerial();
	/** \���ŽŲ���Ч
	*/
	void PlayFootSound(const Vector3& rolePos);
	void PlayNPCFootSound(NPC* pNPC);
	/** \����ˤ����Ч
	*/
	void PlayFallSound(const Vector3& rolePos);
	/** \���Ž�ɫ������Ч
	*/
	void PlayRoleDeadSound(Role* pRole);
	/** \���Ź������ս����Ч
	*/
	void PlayMonsterCombatSound(NPC* pMonster);
	/** \����������Ч
	*/
	void PlayWeaponSound(Player* pPlayer);
	/** \������ҽ���������Ч
	*/
	void PlayPlayerRoarSound(Player* pPlayer);
	/** \������Ұ��򺰽���Ч
	*/
	void PlayPlayerHowlSound(Player* pPlayer);
	/** \���Ź������������Ч
	*/
	void PlayMonsterRoarSound(NPC* pRole);
	/** \���Ź��ﰤ�򺰽���Ч
	*/
	void PlayMonsterHowlSound(NPC* pRole);
	/** \�������˲��ʹ����Ʒ
	*/
	void SelfUseItemImmediate(INT64 n64ItemID,DWORD dwItemTypeID,DWORD dwSrcRoleID,DWORD dwTargetRoleID,const Vector3& srcRolePos);
	/** \��������Ҵӿ�����ק����
	*/
	void PullLocalPlayer();
	/** \��ͨ�����Ƿ���ƶ�
	*/
	bool NormalAttackCanMove();

	CombatSysUtil(void);
	~CombatSysUtil(void);
	static CombatSysUtil* Inst();
private:
	TObjRef<GameFrameMgr>	m_pGameFrameMgr;
	TObjRef<AudioSys>		m_pAudioSys;
	ChatFrame*				m_pChatFrame;
	SceneCenterInfoFrame*	m_pSceneCenterInfoFrame;
};
