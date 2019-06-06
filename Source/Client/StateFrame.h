#pragma once
#include "..\WorldDefine\RoleDefine.h"
struct tagFashionEvent;

const DWORD BUTTON_DELAY_MS = 3000;	/*< ��ť�����ӳ�,��λ:���� */
const int MAX_PLAYER_RESIST = 1000;
const int MIN_PLAYER_RESIST = -1000;

enum EDisplayProperty
{
	EDP_Physique,	// ���
	EDP_Strength,	// ����
	EDP_Pneuma,		// Ԫ��
	EDP_InnerForce,	// ����
	EDP_Agility,	// ��
	EDP_Technique,  // ����
	
	EDP_End,
};

enum EDisplayPropertyEx
{
	EDPE_WeaponDamage,		// �����˺���Сֵ
	EDPE_WuHun,				// ���
	EDPE_ArmDefine,			// ���׷���
	EDPE_MoveSpeed,			// �ƶ��ٶ�
	EDPE_Morale,			// ʿ��
	EDPE_InternalInjury,	// ����
	EDPE_WeaponDamage0,		// �����˺����ֵ
	EDPE_WuHun0,			// �������ֵ
	EDPE_ArmDefine0,		// ���׷�������ֵ
	EDPE_MoveSpeed0,		// �ƶ��ٶ�����ֵ
	EDPE_Morale0,			// ʿ������ֵ
	EDPE_InternalInjury0,	// ��������ֵ
	EDPE_Luck,				// ��Ե
	EDPE_Feature,			// ����
	EDPE_Command,			// ͳ��
	EDPE_Energy,			// ����
	EDPE_Morality,			// ����
	EDPE_Cultivate,			// ��Ϊ
	EDPE_Luck0,				// ��Ե����ֵ
	EDPE_Feature0,			// ��������ֵ
	EDPE_Command0,			// ͳ������ֵ
	EDPE_Energy0,			// �������ֵ
	EDPE_Morality0,			// ��������ֵ
	EDPE_Cultivate0,		// ��Ϊ����ֵ
	EDPE_Health,			// ����
	EDPE_Power,				// ����
	EDPE_Toughness,			// �־���
	EDPE_MeleeDamage,		// �⹦����
	EDPE_MeleeDefine,		// �⹦����
	EDPE_MagicDamage,		// �ڹ�����
	EDPE_MagicDefine,		// �ڹ�����
	EDPE_DamageSkill,		// ��������
	EDPE_DefineSkill,		// ��������
	EDPE_Hit,				// ����
	EDPE_Dodge,				// ����
	EDPE_Health0,			// �������ֵ
	EDPE_Power0,			// �������ֵ
	EDPE_Toughness0,		// �־�������ֵ
	EDPE_MeleeDamage0,		// �⹦��������ֵ
	EDPE_MeleeDefine0,		// �⹦��������ֵ
	EDPE_MagicDamage0,		// �ڹ���������ֵ
	EDPE_MagicDefine0,		// �ڹ���������ֵ
	EDPE_DamageSkill0,		// ������������ֵ
	EDPE_DefineSkill0,		// ������������ֵ
	EDPE_Hit0,				// ��������ֵ
	EDPE_Dodge0,			// ��������ֵ
	
	EDPE_End,
};

enum EDisplayResist
{
	EDR_BleedResist,		// ��Ѫ�˺�����
	EDR_BruntResist,		// ����˺�����
	EDR_BangResist,			// �ػ��˺�����
	EDR_PoisonResist,		// �����˺�����
	EDR_ThinkerResist,		// �����˺�����
	EDR_InjuryResist,		// �����˺�����
	EDR_OrdinaryResist,		// ��ͨ�˺�����

	EDR_End,
};

enum EDisplayTemporary
{
	EDT_Origin_Health,		// ԭʼ����
	EDT_Origin_Power,		// ԭʼ����
	EDT_Origin_ExDamge,		// ԭʼ�⹦����
	EDT_Origin_ExDefine,	// ԭʼ�⹦����
	EDT_Origin_InDamage,	// ԭʼ�ڹ�����
	EDT_Origin_InDefine,	// ԭʼ�ڹ�����
	EDT_Origin_DmgSkill,	// ԭʼ��������
	EDT_Origin_DefSkill,	// ԭʼ��������
	EDT_Origin_Hit,			// ԭʼ����
	EDT_Origin_Dodge,		// ԭʼ����
	EDT_Origin_Toughness,	// ԭʼ�־���

	EDT_Origin_End,
};

const DWORD ORDINARY_COLOR = Color4ub(255, 243, 236, 164);// ԭʼ��ɫ
const DWORD MINUS_RED_COLOR = Color4ub(255, 247, 3, 15);// ����ʱ�ĺ�ɫ
const DWORD PLUS_GREEN_COLOR = Color4ub(255, 194, 246, 58);// ����ʱ����ɫ
#define GETTRUECOLOR(n) (0 > n ? MINUS_RED_COLOR : \
	( 0 < n ? PLUS_GREEN_COLOR : ORDINARY_COLOR))

class NetSession; 

/** class	StateFrame
	brief	״̬����ģ��
	remarks ��ʾ�͹�����ҵ�ǰ����������Ϣ
*/ 
class StateFrame :
	public GameFrame
{
public:
	StateFrame(void);
	~StateFrame(void);
	
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();

	VOID UpdatePalyerAtt();
	
private:

	DWORD OnGameEvent(tagGameEvent* pGameEvent);

	DWORD EventHandler(tagGUIEvent* pEvent);
	

	VOID UpdateAttribute(ERoleAttribute eType);
	
	VOID SetPlayerName(LPCTSTR szName);
	VOID SetPlayerLevel(INT nLevel);
	VOID SetPlayerClass(LPCTSTR szClass);
	VOID SetPlayerGuild(LPCTSTR szGuild);

	VOID SetPlayerOverplus(INT nValue);
	VOID SetBiddenPoint(ERoleAttribute eType, INT nValue);
	VOID AddPreBidPoint(ERoleAttribute eType, INT nInc);
	VOID RemoveBidPoint();
	VOID UpdateBidAttribute();

	VOID SetPlayerAtt(EDisplayProperty eType, INT nValue, DWORD dwColor = ORDINARY_COLOR);
	VOID SetPlayerAttEx(EDisplayPropertyEx eType, INT nValue, DWORD dwColor = ORDINARY_COLOR);
	VOID SetPlayerResist(EDisplayResist eType, INT nValue);

	VOID SendNetBidAtt();

	ERoleAttribute TransBtn2Att(EDisplayProperty eType);

    // �⹦�˺�����ӳ�
    int CalExAttackResist(int src);
    // �ڹ��˺�����ӳ�
    int CalInAttackResist(int src);
private:
	TSFPTrunk<StateFrame>		m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>		m_pSession;

	// GUI�ؼ�
    GUIWnd*                     m_pWndState;
	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndFather;
	GUIStatic*					m_pStcAtt[EDP_End];
	GUIStatic*					m_pStcAttEx[EDPE_End];
	GUIStatic*					m_pStcResPic[EDR_End];
	GUIStatic*					m_pStcResValue[EDR_End];
	GUIStatic*					m_pStcOverplus;
	GUIButton*					m_pBtnAtt[EDP_End];
	GUIFrameMutex*				m_pFmSuitOpt;
	GUIPushButton*				m_pPbnDisplayAtt;
	GUIPushButton*				m_pPbnDisplayEquip;
	GUIPushButton*				m_pPbnDisplayFashion;
	GUIStatic*					m_pStcPlayerName;
	GUIStatic*					m_pStcPlayerGuild;
	GUIStatic*					m_pStcPlayerLevel;
	GUIStatic*					m_pStcPlayerClass;
	GUIButton*					m_pBtnSure;
	GUIButton*					m_pBtnCancel;
	GUIWnd*						m_pWndAtt;
	GUIWnd*						m_pWndAttEx;
	GUIWnd*						m_pWndAttIn;
	GUIWnd*						m_pWndRes;

	bool						m_isSwappingSuitPattern;
	
	// ��ʱ����
	INT							m_Temporary[EDT_Origin_End];
	INT							m_nPreBid[X_ERA_ATTA_NUM];// ԤͶ��
	INT							m_nBidden[X_ERA_ATTA_NUM];// ��Ͷ��
	INT							m_nOgnAtt[X_ERA_ATTA_NUM];// ����ɳ���
	INT							m_nOverplus;			  // ��ǰ��Ͷ����
	INT							m_nInternalInjury;		//����ֵ
};
