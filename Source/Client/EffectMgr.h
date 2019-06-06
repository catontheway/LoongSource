#pragma once
#include "..\WorldDefine\msg_combat.h"
#include "CombatEvent.h"

namespace Cool3D
{
	class EffectTarget;
};
class Role;

/** \class EffectMgr
	\brief ��Ч������
*/
class EffectMgr
{
public:
	/** \��Ч����
	*/
	enum EEffectType
	{
		EET_Common,         // ��ͨ��Ч
		EET_Role,           // ��ɫ��Ч
		EET_Launch,         // ������Ч
		EET_Buff,           // Buff��Ч
		EET_BeAttack,       // ��������Ч
	};
	/** \��Ч����������
	*/
	enum EEffectScaleType
	{
		EEST_Null,           // ������
		EEST_Y,				 // ���ݸ߶�����
		EEST_RoleScale,      // ʹ�ý�ɫ��������������Ч
		EEST_XYZ,			 // ʹ��X,Y,Z��ƽ��ֵ����
		EEST_YOutOnly,       // ���ݸ߶ȷŴ󣬲���С
		EEST_XZ,			 // ʹ��X,Z��ƽ��ֵ����
	};
private:
	/** \��Ч��������
	*/
	struct tagBaseEffectData
	{
		const EEffectType       eType;                // ��Ч����
		SGAttachableEffectNode*	pSGNode;              // ��ЧScene node
		DWORD				    dwBindRoleID;         // ��Ч�󶨵Ľ�ɫID�����û�а󶨽�ɫ��ID==GT_INVALID��
		bool                    bBindTagNode;		  // �Ƿ���˹��ص�����
		EEffectScaleType		eScaleType;           // ��������
		bool                    bPullOnWater;         // �����ˮ�£�����ˮ������
		tagBaseEffectData( EEffectType type ) : eType( type ), pSGNode( NULL ), dwBindRoleID( GT_INVALID ), bBindTagNode( false ), eScaleType( EEST_Null ), bPullOnWater( false ) {}
	};
	typedef map<DWORD, tagBaseEffectData*> EffecMap;

	/** \��ɫ��Ч
	*/
	struct tagRoleEffectData : public tagBaseEffectData
	{
		tagRoleEffectData() : tagBaseEffectData( EET_Role ) {}
	};

	/** \������Ч
	*/
	struct tagLaunchEffectData : public tagBaseEffectData
	{
		tagHitTargetEvent evtHitTarget;
		bool              bEventSended;
		tagLaunchEffectData() : tagBaseEffectData( EET_Launch ), bEventSended( false ) {}
	};

	/** \Buff��Ч
	*/
	struct tagBuffEffectData : public tagBaseEffectData
	{
		DWORD dwBuffID;								// BUFF ID
		tagBuffEffectData() : tagBaseEffectData( EET_Buff ), dwBuffID( GT_INVALID ) {}
	};

	/** \��������Ч
	*/
	struct tagBeAttackEffectData : public tagBaseEffectData
	{
		tagBeAttackEffectData() : tagBaseEffectData( EET_BeAttack ) {}
	};

	enum { EFFECT_START_ID = 1 };
	enum { MIN_EFFECT_NUM = 30, MAX_EFFECT_NUM = 150 };  // ������Чϸ���Լ���ǰ��Ч��������ĳЩ��Ч����
public:
	EffectMgr(void);
	~EffectMgr(void);

	/** \��ʼ������ս��ϵͳ����
	*/
	VOID Init();
	/** \�ͷ����ݣ���ս��ϵͳ����
	*/
	VOID Destroy();
	/** \������Ч����ս��ϵͳ����
	*/
	VOID Update();

	/** \������Ч��ָ��λ�á����š���ת
	*/
	DWORD PlayEffect(const TCHAR* szPath, const Vector3& vPos, const Vector3& vScale, const Vector3& vRot );
	/** \��ָ��λ�ò���һ����Ч
	    \szFolderName-��Ч�����ļ������ƣ����ΪNULL������ΪszEffectName������Ч��·��
	*/
	DWORD PlayEffect(const Vector3& vPos,float yaw,const TCHAR* szEffectName, const Vector3* pSize=NULL, EEffectScaleType eScaleType = EEST_Null, const TCHAR* szFolderName = _T("combat") );
	/** \����һ����ɫ��Ч,������ЧID,����ʧ�ܷ���GT_INVALID
	*/
	DWORD PlayRoleEffect(DWORD dwRoleID,const TCHAR* szEffectName,const TCHAR* szTagNodeName=NULL, EEffectScaleType eScaleType = EEST_Y, const TCHAR* szFolderName = _T("combat"), const bool bPullOnWater = false );
	/** \���ŷ�����Ч,������ЧID
	*/
	DWORD PlayLaunchEffect(const tagHitTargetEvent& evt,const TCHAR* szEffectName,const TCHAR* szTagNodeName=NULL);
	/** \����nNumEffect��szEffectBaseName�������һ����������Ч��������ЧID
	*/
	DWORD PlayBeAttackEffect(DWORD dwRoleID,const TCHAR* szEffectBaseName,const int nNumEffect, EEffectScaleType eScaleType = EEST_YOutOnly );
	/** \����һ��Buff��Ч,������ЧID
	*/
	DWORD PlayBuffEffect(DWORD dwRoleID,DWORD dwBuffID,const TCHAR* szEffectName, EEffectScaleType eScaleType = EEST_Y );
	/** \ֹͣһ��Buff��Ч
	*/
	VOID StopBuffEffect(DWORD dwRoleID,DWORD dwBuffID);
	/** \ֹͣһ����Ч
	*/
	VOID StopEffect( const DWORD dwEffectID );
	/** \��ʾ\������Ч
	*/
	VOID ShowEffect( const DWORD dwEffectID, const bool bShow );
	/** \��ʾ\���ذ��ڽ�ɫ���ϵ�������Ч
	*/
	VOID ShowBindRoleEffect( const DWORD dwBindRoleID, const bool bShow );
	/** \��ȡ��������Ч������
	*/
	size_t GetNumEffect() const { return m_mapEffect.size(); }
	/** \��Ч�����Ƿ��Ѿ��ﵽ���ޣ���Щ����Ҫ����Ч�Ϳ��Բ����ţ�
	*/
	bool                 IsEffectNumFull();
	static EffectMgr* Inst();
private:
	SGAttachableEffectNode*		LoadSFXFile( const TCHAR* szEffectName, const TCHAR* szFolderName = _T("combat") );
	DWORD						AddEffect( tagBaseEffectData* pData, Role* pBindRole, const TCHAR* szTagNodeName );
	VOID						DeleteEffect( tagBaseEffectData* pData );    // ����ɾ��map�еĽڵ�
	VOID						OnCloseMap( tagGameEvent* );
	BOOL                        UpdataRoleEffect( tagBaseEffectData* pData );
	VOID                        UpdataTargetPath( tagLaunchEffectData* pData, const float fTimeDelta );
	VOID                        PullOnWater( tagBaseEffectData* pData );
	Vector3						GetRoleEffectScale( const EEffectScaleType eScaleType, Role* pRole );
	float 						GetEffectScale( const EEffectScaleType eScaleType, const Vector3& vSize );
	Vector3						GetBeAttackPos( Role* pRole );
	bool                        SFXCanMove( const TCHAR* szFileName );
	VOID                        UpdataEffectMatrix( tagBaseEffectData* pData, Role* pRole );
private:
	TObjRef<GameFrameMgr>		m_pGameFrameMgr;
	TSFPTrunk<EffectMgr>		m_Trunk;
	EffecMap					m_mapEffect;				// ��Ч����
	DWORD						m_dwCurEffectID;            // ��¼��ǰ��ЧID
};
