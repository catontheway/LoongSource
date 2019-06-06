#pragma once
#include "role.h"
#include "..\WorldDefine\ItemDefine.h"
#include "AvatarEquipNode.h"
#include "AvatarAniPlayer.h"
#include "RoleTitleProtoData.h"
#include "ActionState.h"

class FSM_RP;
class Pet;
struct tagNS_GetRemoteRoleState;

/** \class Player
	\brief ��ҽ�ɫ
*/
class Player :	public Role
{
public:
	Player(void);
	virtual ~Player(void);

	//--Role-------------------------------------------------------------------
	virtual void InitFSM();
	virtual void AttachSceneNodeToSG(SceneGraph* pSG);
	virtual void DetachSceneNodeFromSG(SceneGraph* pSG);
	virtual void Update();
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pGameEvent);
	virtual SceneNode* GetSceneNode() { return m_pAvatarNode; }
	virtual void GetHeadPos(Vector3& out);
	virtual Vector3 GetRoleSize();
	virtual bool IsDead();
	virtual bool IsAttributeInited() const;
	virtual void Hide( const bool bHide );
	virtual bool IsHide() const { return m_bHide; }
	virtual VOID FaceTo(const Vector3& pos,BOOL bFaceRound=TRUE);
	virtual VOID FaceBack(void);
	virtual void ReplayLowerAni();
	virtual void SetTransparent(float alpha);

	//--��������----------------------------------------------------------------
	AvatarAniPlayer* GetAniPlayer()	{ return &m_aniPlayer;}
	NodeAnimationCtrl* GetAniCtrl();
	void  SetMount(Pet *pPet);
	void  SetPos(const Vector3& pos);
	void  SetYaw(float angle,bool bShade=true);
	float GetMoveSpeed();
	float GetMoveYaw(){ return m_moveYaw;}
	void SetMoveYaw(float yaw){ m_moveYaw=yaw;}

	//--��ɫ�������Լ�װ��----------------------------------------------------
	/** \����Ĭ�ϵ�avatar
	*/
	void LoadDefaultAvatar();
	/** \����ָ����avatar��pEpuip=NULL��ֻ����att
	*/
	void LoadAvatar(const tagAvatarAtt& att, const tagAvatarEquip& ep, const tagDisplaySet& displaySet);
	/** \����avtatar��pEpuip=NULL��ֻ����att
	*/
	void UpdateAvatar(const tagAvatarAtt& att, const tagAvatarEquip& ep, SceneGraph* pSG);
	/** \������װ��
	*/
	void SetAvatarEquip( const EAvatarElement eAvaEnt, const tagEquipDisplay& ed );
	/** \��õ���װ��
	*/
	const tagEquipDisplay& GetAvatarEquip( const EAvatarElement eAvaEnt ) const;
	/** \��ý�ɫ������������
	*/
	const tagAvatarAtt& GetAvatarAtt() const;
	/** \�ó�/�ջ�����
	*/
	void HandUpWeapon( const bool bHandUp );
	/** \��ȡ������������
	*/
	EItemTypeEx GetRWeaponType() const;
	/** \��ȡ������������
	*/
	EItemTypeEx GetLWeaponType() const;
	/** \չ��/�ر�����װ��(���ű��ζ���)
	*/
	void OpenAllEquip( const bool bOpen );
	/** \����װ���Ƿ��Ѵ�
	*/
	bool IsAllEquipOpened();
	/** \����װ���Ƿ��ѹر�
	*/
	bool IsAllEquipClosed();
	/** \����������Ч
	*/
	void UpdataLongHunEffect( const EAvatarElement eAvaEnt, const BYTE byEquipEffect );
	/** \������װ��Ч
	*/
	void UpdataSuitEffect( const DWORD dwSuitID );

	//--����������-----------------------------------------------------------
	/** \��ʼ���������
	*/
	void InitAttribute(tagNS_GetRemoteRoleState* pCmd);
	/** \�����������
	*/
	void SetPlayerName(LPCTSTR szName, DWORD dwNameCrc = GT_INVALID);

    //! ������ҳƺ�
    void SetPlayerTitle(DWORD dwTitleID);

	// ���ý�ɫ̯λ�ȼ�
	void SetStallLevel(BYTE byLevel);

    //! ������Ұ󶨵ĸ����
    void SetRebornPoint(DWORD dwMapID, Vector3& dwRebornPoint);

    //! ������ҿɼ�¼���͵�
    void SetTransportPoint(DWORD dwMapID, Vector3& dwPoint);
    //! �õ���ǰ������õĿɼ�¼���͵�ĵ�ͼ��
    tstring GetTransportPointName();
    //! �õ���ǰ������õĿɼ�¼���͵������
    Vector3 GetTransportPoint();

	/** \�������ְҵ
	*/
	void SetPlayerClass(EClassType eClass);
    void SetPlayerClassEx(EClassTypeEx eClassEx);
    /** \������ְҵ
    */
    EClassType GetPlayerClass() { return m_eClass; }
    EClassTypeEx GetPlayerClassEx() { return m_eClassEx; }

	// ���̯λ�ȼ�
	BYTE GetPlayerStallLevel() { return m_byStallLevel; }

    /** \���������ż
    */
    void SetPlayerLoverID(DWORD dwLoverID);
    void SetPlayerIsHasWedding(BOOL bWedding) { m_bHaveWedding = bWedding; }
    /** \��������ż,���򷵻�GT_INVALID
    */
    DWORD GetPlayerLoverID() { return m_dwLoverID; }
    BOOL IsHaveWedding() { return m_bHaveWedding; }

	/** \��ȡ����Ա�
		\����Ա�δȥ���򷵻�0xFF��ʾδ֪
	*/
	BYTE GetPlayerSex(){if(P_VALID(m_pAvatarNode))return m_pAvatarNode->GetAtt().bySex; return 0xFF;}
	/** \���ý�ɫ״̬
	*/
	void SetRoleState(DWORD dwValue);
	/** \���ý�ɫ״̬
	*/
	void SetRoleState(ERoleState eType, bool bFlag);
	/** \��ȡ��ɫ״̬
	*/
	DWORD GetRoleState(){ return m_roleState;}
	/** \��ȡ��ɫ�Ƿ����ĳ״̬
	*/
	bool GetRoleState(ERoleState eType){return (m_roleState&eType) != 0;}

    //! ��ȡ�ƺ�
    DWORD GetTitleID() { return m_dwTitleID; }
    tstring GetTitleName() {return m_strTitleName; }
    
    //! ��ȡ��ǰ�󶨵ĸ��������
    tstring GetRebornPoinName();

	/** \��ȡPK״̬
	*/
	ERolePKState GetPKState(){ return m_ePKState;}
	/** \�Ƿ����ˮ������
	*/
	bool CanWaterWalk() { return 0!=(m_roleState&ERS_WaterWalk); }
	/** \�Ƿ��ڰ�
	*/
	bool IsInGuild() const {return P_VALID(m_dwGuildID);}

    /** \��ʾ����
    */
	const tagDisplaySet& GetDisplaySet();
	void SetDisplaySet( const tagDisplaySet& ds );

	/** \������Ұ���ID
	*/
	void SetRoleGuildID( DWORD dwID ){m_dwGuildID = dwID;}
	/** \������Ұ���ְλ
	*/
	void SetRoleGuildPos( INT8 n8Pos ){m_n8GuildPos = n8Pos;}
	/** \��ȡ��Ұ���ID
	*/
	DWORD GetRoleGuildID(){return m_dwGuildID;}
	/** \��ȡ��Ұ���ְλ
	*/
	INT8 GetRoleGuildPos(){return m_n8GuildPos;}

	//--Ч�����-----------------------------------------------------------
	/** \��ʾ/��������
	*/
	void HideWeapon(bool bHide);

	// ��ʾ/������������ģ��
	void HideWeaponPertain(bool bHide);

	// ����/ֹͣ������װ��Ч
	void PlaySuitEffect(bool bPlay);

	//--
	virtual bool IsReadyState();
	virtual bool IsStandState();
	virtual bool IsStyleActionState();
	virtual bool IsIdleState();
	virtual EActionState GetActionState();

	Pet*	GetMount(void)	{ return m_pMount; }
    void SwitchMountPitch(bool bPitchEnabled);

	/**	\�������ʱƨ�ɾ������ĸ߶�
	*/
	float GetBunsHeight(void);

	/** \������ʾ/����̯λ
	*/
	void SendEventStallAppear(bool bAppear, BYTE byStallLevel = 1);

protected:
	/** \����״̬�ı���Ϸ�¼�
	*/
	void SendEventRoleStateChange(const DWORD dwOldState);
	/**	\�������ʱ���������
	*/
	bool CalRideMat(Pet *pMount,Matrix4& outRide,UINT index);
	/** \������Ϣ��
	*/
	virtual void OnMsgCode( const DWORD dwMsgCode );
protected:
	AvatarEquipNode*	m_pAvatarNode;		//�����޽��
	AvatarAniPlayer		m_aniPlayer;		//����������

	float				m_moveYaw;			//��ɫ�ƶ�����

	DWORD				m_dwRoleNameCrc;	//�������Crcֵ
	EClassType			m_eClass;			//ר��ְҵ
	EClassTypeEx		m_eClassEx;			//Ӣ��ְҵ
	DWORD				m_roleState;		//�����������������״̬
	ERolePKState		m_ePKState;			//PK״̬

	bool                m_bAttributeInited;	//�Ƿ��Ѵӷ������������
	bool                m_bHide;

	Pet*				m_pMount;			//����
	int					m_bunsBoneIndex;	//�β���������

	DWORD				m_dwGuildID;		//��������ID
	INT8				m_n8GuildPos;		//��������ְλ

    DWORD               m_dwTitleID;        //��ǰ�ƺ�id(��ЧΪGT_INVALID)
    tstring             m_strTitleName;     //��ǰ�ƺ���,��ЧΪ���ַ���

    DWORD               m_dwRebornMapID;    //��ǰ�󶨵ĸ����mapId(��ЧΪGT_INVALID)
    Vector3             m_vRebornPoint;     //���������

    DWORD               m_dwTransportID;    // ��ǰ��¼���ͷ��󶨵�ͼID
    Vector3             m_vTransportPoint;  // ��ǰ��¼���ͷ��󶨵�ͼ����

    DWORD			    m_dwLoverID;        // ���˶�Ӧ��roleid û�����ΪGT_INVALID
    BOOL			    m_bHaveWedding;     // �Ƿ���й�����
	
	BYTE				m_byStallLevel;		// ̯λ�ȼ�
private:
	FSM_RP*				m_pFSM;

	float				m_lastYaw;
	float				m_lastPitch;

	DECL_RTTI(Player);
};
