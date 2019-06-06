#pragma once
#include "..\Cool3D\Util\RTTIObj.h"
#include "RoleRotater.h"
#include "UpperRotater.h"
#include "HeadRotater.h"
#include "RoleScaler.h"
#include "..\WorldDefine\RoleDefine.h"
#include "..\WorldDefine\buff_define.h"
#include "..\WorldDefine\dword_flag.h"

struct tagRoleBuff
{
	DWORD				dwBuffTypeID;				// Buff����ID
	INT					nPersistTimeLeft;			// Buff��ʧʱ�䵹��ʱ�����룩
	INT					nMaxPersistTime;			// Buff�ܳ���ʱ�䣨���룩
	DWORD				dwStartTime;				// ����ʱ��ʼʱ�䣨tagDWORDTime�� 
	INT					nWarpTimes;					// ��ǰ���Ӵ���
	tagBuffProtoClient*	pBuffProto;					// ָ��Buffģ��ָ��
};

class SGDynamicProjector;

/** \class Role
	\brief ��Ϸ�и��ֽ�ɫ�Ļ���
*/
class Role : public RTTIObj
{
public:
	Role(void);
	virtual ~Role(void);

	//--��������
	//��ʼ��״̬��
	virtual void InitFSM() =0;
	//��SceneNode��ӵ�SceneGraph����RoleMgr����
	virtual void AttachSceneNodeToSG(SceneGraph* pSG) =0;
	//��SceneNode��SceneGraph���Ƴ�����RoleMgr����
	virtual void DetachSceneNodeFromSG(SceneGraph* pSG) =0;
	//���½�ɫ��Ϊ
	virtual void Update() =0;
	//����������Ϣ
	virtual void OnNetCmd(tagNetCmd* pNetCmd){}
	//������Ϸ�¼�
	virtual void OnGameEvent(tagGameEvent* pGameEvent){}
	//�õ�SceneNode
	virtual SceneNode* GetSceneNode() =0;
	//��Ӱ
	virtual void AttachShadow( SGDynamicProjector* pShadow );
	virtual void DetachShadow();
	//����/��ʾ
	virtual void Hide( const bool bHide ) =0;
	virtual bool IsHide() const =0;
	//�Ƿ���Ҫ�ͷ�
	virtual bool IsNeedToFree() const { return m_bNeedToFree; }
	virtual void Free() { m_bNeedToFree=true; }
	
	//--�������
	//���ý�ɫID
	void SetID(DWORD id)	{ m_id=id;		}
	//���ؽ�ɫID
	DWORD GetID()			{ return m_id;	}
	//���ý�ɫ����
	void SetPos(const Vector3& pos);
	//���ؽ�ɫ����
	const Vector3& GetPos();
	//���ý�ɫƫת��
	void SetYaw(float angle,bool bShade=true);
	void SetPitch(float angle,bool bShade=true);
	//���ؽ�ɫƫת��
	float GetYaw() const;
	//��������ƫת��
	float GetDestYaw();
	//���ý�ɫ����
	void SetScale(const Vector3& scale,bool bShade=true);
	//���ؽ�ɫ��������
	const Vector3& GetDesScale() const;
	//���ؽ�ɫ������
	float GetPitch() const;
	//�õ���ɫ�ߴ�
	virtual Vector3 GetRoleSize() =0;
	//�õ��ƶ��ٶ�
	float GetMoveSpeed(BOOL bIsMount=FALSE);
	//�õ�����ƽ���ٶ�
	float GetDropXzSpeed();
	//�õ���Ծ������ʼ�ٶ�
	float GetJumpYSpeed();
	//�õ�����ƽ���ٶ�
	float GetHitFlyXzSpeed();
	//�õ�����������ʼ�ٶ�
	float GetHitFlyYSpeed();
	//�õ���Ӿ�ٶ�
	float GetSwimSpeed();
	//�õ�ͷ����λ��
	virtual void GetHeadPos(Vector3& out)=0;
	//�õ�����ռ�
	const ObjectSpace& GetObjectSpace() const { return m_space; }

	//��ȡ��ɫ����
	INT GetAttribute(ERoleAttribute eType){return m_roledata[eType];}
	//д���ɫ����
	void SetAttribute(ERoleAttribute eType, INT nValue, bool bInit);
	//д��Զ�̽�ɫ����
	void SetRemoteRoleAtt(ERemoteRoleAtt eType, INT nValue, bool bInit);
	//�����Ƿ��Ѿ���ʼ��
	virtual bool IsAttributeInited() const =0;

	//��ȡ��ɫ�ȼ�
	INT GetRoleLevel(){return m_nRoleLevel;}
	//д���ɫ�ȼ�
	void SetRoleLevel(INT nLevel, BOOL bLevelUp = FALSE);
	
	//��ȡ��ɫ����
	tstring GetRoleName(){return m_strRoleName;}
	//д���ɫ����
	void SetRoleName(tstring strName);
	
	//��ý�ɫ״̬
	const tagDWORDFlagArray<ES_End>& GetState(){ return m_state;}
	//����״̬
	void SetState(EState state);
	//���״̬
	void UnSetState(EState state);
	//�ж�״̬
	bool IsInState(EState eState){ return m_state.IsSet(eState) != FALSE;}
	//���ؽ�ɫ�Ƿ�����
	virtual bool IsDead() =0;
	
	//--ս�����
	//����buff
	VOID AddRoleBuff(tagBuffMsgInfo data);
	//ɾ��buff
	VOID RemoveRoleBuff(DWORD dwBuffID);
	//����buff
	VOID UpdateRoleBuff(tagBuffMsgInfo data);
	//���buff
	VOID FreeRoleBuff();
	//����buff�ı����Ϸ�¼�
	VOID SendBuffChangeEvent();
	//���ݴ�ID����BUFF
	tagRoleBuff* FindBuff(DWORD dwBuffID);
	//��ȡbufflist
	TList<tagRoleBuff*>& GetBuffList(){return m_listRoleBuff;}

	//--�������
	//����ĳ��λ��
	const vector<DWORD>& GetMsgCodes() const { return m_vecMsgCode; }
	virtual VOID FaceTo(const Vector3& pos,BOOL bFaceRound=TRUE) =0;
	virtual VOID FaceBack(void) =0;
	BOOL GetBoneSpace( const TCHAR* szBoneName, Matrix4& matOut );

	//--Ч�����
	//͸��Ч��
	virtual void SetTransparent(float alpha) =0;
	virtual float GetTransparent() const { return m_fTransparent; }
	//��ɫЧ��
	virtual void SetMulColor(const float r, const float g, const float b);
protected:
	/** \������Ϣ��
	*/
    void HandleMsgCode(AnimationCtrl* pAniCtrl);
	virtual void OnMsgCode( const DWORD dwMsgCode ) =0;
protected:
	SGDynamicProjector*		m_pShadow;				// ��¼һ����Ӱָ��,�Ա�Detach
	ObjectSpace				m_space;				// ��ɫ�ռ䶨��

	RoleRotater				m_rotaterYaw;			// ��ת������
	RoleRotater				m_rotaterPitch;			// ��ת������

	RoleScaler				m_scaler;				// ���ſ�����
	vector<DWORD>			m_vecMsgCode;			// ������Ϣ����
	UpperRotater			m_upperRotater;
	HeadRotater				m_headRotater;
	DWORD					m_id;					// ��ɫID
	
	
	tstring					m_strRoleName;			// ��ɫ����
	INT						m_nRoleLevel;			// ��ɫ�ȼ�
	INT						m_roledata[ERA_End];	// ��ɫ��Ϣ

	tagDWORDFlagArray<ES_End>	m_state;			// �������������Ľ�ɫ״̬

	TList<tagRoleBuff*>		m_listRoleBuff;			// ��ɫbuff�б�

	bool					m_bNeedToFree;			// �Ƿ���Ҫ�ͷ�
	float					m_fTransparent;			// ��ǰ͸����
	Vector3					m_vMulColor;            // ��˵���ɫ
	DECL_RTTI(Role);
};
