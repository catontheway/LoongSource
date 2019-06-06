#pragma once
#include "role.h"

class FSM_NPC;
struct tagNS_GetRemoteCreatureState;
struct tagCreatureMdlData;

/** \class NPC
	\brief ����ҽ�ɫ
*/
class NPC :	public Role
{
public:
	NPC(void);
	virtual ~NPC(void);

	//--Role
	virtual void InitFSM();
	virtual void AttachSceneNodeToSG(SceneGraph* pSG);
	virtual void DetachSceneNodeFromSG(SceneGraph* pSG);
	virtual void Update();
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pGameEvent);
	virtual SceneNode* GetSceneNode() { return m_pEntityNode; }
	virtual void GetHeadPos(Vector3& out);
	virtual Vector3 GetRoleSize();
	virtual bool IsDead();
	virtual bool IsAttributeInited() const;
	virtual void Hide( const bool bHide );
	virtual bool IsHide() const { return m_bHide; }
	virtual VOID FaceTo(const Vector3& pos,BOOL bFaceRound=TRUE);
	virtual VOID FaceBack(void);
	virtual void SetTransparent(float alpha);
	virtual	AnimationCtrl* GetAniCtrl();

	//--
	/** \����SceneNode
	*/
	void LoadEntity( const TCHAR* szFileName );
	/**	\������Ƥ
	*/
	void LoadSkin( const TCHAR* szFileName );
	void UpdateBoundingBox(void);
	/** \�������Գ�ʼ��
	*/
	virtual void InitAttribute( tagNS_GetRemoteCreatureState* pCmd );
	/** \������Խṹָ��
	*/
	const tagCreatureProto* GetProto() const { return m_pProto; }
	/** \���ģ���������
	*/
	const tagCreatureMdlData* GetMdlData() const { return m_pMdlData; }
	/** \��������ID
	*/
	void SetTaggedOwnerID( const DWORD dwID ) { m_dwTaggedOwnerID = dwID; }
	/** \�Ƿ����ڱ������
	*/
	bool IsBelongToLP() const;
	/** \�����ж�
	*/
	bool IsNPC();
	bool IsGather();
	bool IsMonster();
	bool IsChest();
	bool IsNormalGameObject();
	bool IsCommonInvesGO();
	bool IsQuestInvesGO();
	bool IsQuestTalkGameObject();
    bool IsPet();
	/** \���TypeID
	*/
	DWORD GetTypeID() { return m_dwTypeID; }

	//--��������
	void PlayTrack(const TCHAR* szTrackName,bool loop,bool replay=false,float timePos=0.0f);
	bool IsTrackEnd(const TCHAR* szTrackName);

	//--Ч�����
	/** \���������Ч
	*/
	void UpdateGameObjectQuestEffect();
protected:
	void UpdataPitch();
	virtual void OnMsgCode( const DWORD dwMsgCode );
protected:
	EntityNode*				m_pEntityNode; 
	tstring					m_curTrack;			// ��ǰ���ŵ�track
	FSM_NPC*				m_pFSM;

	tagCreatureProto*		m_pProto;			// ��̬���Խṹָ��
	tagCreatureMdlData*		m_pMdlData;			// ģ���������

	DWORD					m_dwTypeID;
	bool					m_bGetAttribute;	// �Ƿ��Ѵӷ������������

	bool					m_bHide;			// �Ƿ�����
	DWORD					m_dwQueryEffectID;	// ���������ЧID

	float					m_originYaw;

	float					m_lastYaw;
	float					m_lastPitch;

	DWORD                   m_dwTaggedOwnerID;  // ��������ID

	DECL_RTTI(NPC);
};
