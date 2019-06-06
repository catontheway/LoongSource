#pragma once

struct tagGroundItem;
struct tagStall;
class GUIRender;

/** \ʰȡ����
*/
enum EWorldPickType
{
	EWPT_None,
	EWPT_LClick,    // �������
	EWPT_RClick,    // �Ҽ�����
};

/** \ѡ�н�ɫ�¼�,dwRoleID == GT_INVALIDΪȡ��ѡ��
*/
struct tagRolePickEvent : public tagGameEvent
{
	EWorldPickType eType;
	DWORD dwRoleID;
	tagRolePickEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : 
	tagGameEvent(szEventName,pSenderFrame), eType( EWPT_None ), dwRoleID( GT_INVALID ) {} 
};

/** \ʰȡ�����¼�
*/
struct tagScenePickEvent : public tagGameEvent
{
	EWorldPickType eType;
	Vector3 vPickPos;
	tagScenePickEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : 
	tagGameEvent(szEventName,pSenderFrame), eType( EWPT_None ), vPickPos( 0.0f, 0.0f, 0.0f ) {}
};

/** \ʰȡ������Ʒ�¼�
*/
struct tagGroundItemPickEvent : public tagGameEvent
{
	EWorldPickType eType;
	tagGroundItem* pGroundItem;
	tagGroundItemPickEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : 
	tagGameEvent(szEventName,pSenderFrame), eType( EWPT_None ), pGroundItem( NULL ) {}
};

/** \��ʾ���������¼�
*/
struct tagShowScenePickDecalEvent : public tagGameEvent
{
	BOOL bShow;
	tagShowScenePickDecalEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : 
	tagGameEvent(szEventName,pSenderFrame), bShow( false ) {}
};


/** \class  WorldPickerFrame
	\brief  ����ʰȡ
	\remark ������ʰȡ��⼰�����Ϸ�¼�����
*/
class WorldPickerFrame : public GameFrame
{
	enum EMousePointType
	{
		EMPT_None = -1,
		EMPT_Role = 0,           // ��ɫ
		EMPT_GroundItem,         // ������Ʒ
		EMPT_Scene,              // ����
		EMPT_Water,              // ˮ��
		EMPT_Stall,				 // ̯λ
		EMPT_Num
	};
	struct tagMousePointObject
	{
		bool            bValid;			// �Ƿ���Ч
		Vector3         vPos;			// ��������
		tagMousePointObject() : bValid( false ), vPos( FLOAT_MAX, FLOAT_MAX, FLOAT_MAX ) {}
	};
public:
	WorldPickerFrame(void);
	virtual ~WorldPickerFrame(void);

	// GameFrame
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual VOID Update();

	/** \��ȡ��ǰ���ָ��Ľ�ɫID,����GT_INVALID��ʾû��ָ���ɫ
	*/
	DWORD GetCurMousePointRoleID() const;
	/** \��ȡ��ǰ���ָ��ĵ�����Ʒ�ṹָ��,����NULL��ʾû��ָ�������Ʒ
	*/
	tagGroundItem* GetCurMousePointGroundItem() const;
	/** \��ȡ��ǰѡ�еĽ�ɫID,����GT_INVALID��ʾû��ѡ�н�ɫ
	*/
	DWORD GetCurSelectedRoleID() const { return m_dwSelectedRoleID; }
private:
	VOID  UpdataMousePoint( const Ray& ray, const BOOL bIgnorePlayer );
	VOID  UpdataCursor(); 
	DWORD OnOpenMap( tagGameEvent* );
	DWORD OnCloseMap( tagGameEvent* );
	DWORD OnShowScenePickDecalEvent( tagShowScenePickDecalEvent* pEvent );
	DWORD OnRolePickEvent( tagRolePickEvent* pEvent );
	VOID  ReBuildScenePickDecal( const Vector3& pickPos, const Ray& ray, const bool bWater = false );
private:
	TSFPTrunk<WorldPickerFrame>		m_trunk;
	TObjRef<InputDX8>				m_pInput;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<GUIRender>				m_pRender;

	EMousePointType                 m_eMousePointType;      // ��ǰ���ָ�������
	DWORD                           m_dwMousePointRoleID;   // ��ǰ���ָ��Ľ�ɫID
	DWORD                           m_dwSelectedRoleID;     // ��ǰѡ�еĽ�ɫID
	Vector3                         m_vMousePointScenePos;  // ��ǰ���ָ��ĳ�������
	Vector3                         m_vMousePointWaterPos;  // ��ǰ���ָ���ˮ������
	tagGroundItem*                  m_pGroundItem;          // ��ǰ���ָ��ĵ�����Ʒ�ṹ��ָ��
	tagStall*						m_pStall;				// ��ǰ���ָ���̯λ�ṹ��ָ��
	SGEffectNode*                   m_pScenePickDecal;		// ��������
	bool                            m_bDecalAttached;		// ������Ч�Ƿ��Ѱ�SG
};

inline WorldPickerFrame* GetWorldPickerFrm()
{
	return (WorldPickerFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("WorldPicker"));
}

inline DWORD GetCurMousePointRoleID()
{
	WorldPickerFrame* pWorldPicker = GetWorldPickerFrm();
	if( P_VALID(pWorldPicker) )
		return pWorldPicker->GetCurMousePointRoleID();
	else
		return GT_INVALID;
}