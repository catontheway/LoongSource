#pragma once

/** \����״̬
*/
enum ECursorState
{
	ECS_Idle,		// ����
	ECS_Active,		// ����
	ECS_Sum
};

/** \��꼤�������
*/
enum ECursorActiveName {
	ECAN_Null,

	ECAN_ItemStart	= 20,	// ������Ʒʹ����ʼ��Ϊ����������������λ�÷����ж�
	ECAN_Identify,			// ������
	ECAN_Gem,				// ��ʯ
	ECAN_Brand,				// ��ӡ
	ECAN_Longfu,			// ����
	ECAN_Chisel,			// ����
	ECAN_Grind,				// ĥʯ
	ECAN_Key,				// ������ by xtian
    ECAN_Dye,               // Ⱦɫ
	ECAN_AddPot,			// ����װ��Ǳ��ֵ
	ECAN_BeGoldStone		// ���ʯ
};

/** \�����е�����
*/
enum ECursorIdleName
{
	ECIN_Default,           // Ĭ��
	ECIN_Attack,            // ����
	ECIN_NPCTalk,           // NPC�Ի�
	ECIN_NPCShop,           // �̵�NPC
	ECIN_PickGroundItem,    // ʰȡ������Ʒ
	ECIN_Gather,            // �ɼ�
	ECIN_Hyperlink,         // ������
	ECIN_DigBao,			// �ڱ�
};

/** \�����ʾ��������ʽ
*/
enum ECursorDisplayType
{
	ECDT_Default,           // Ĭ��
	ECDT_Attack,            // ����
	ECDT_NPCTalk,           // NPC�Ի�
	ECDT_PickGroundItem,    // ʰȡ������Ʒ
	ECDT_Gather,            // �ɼ�
	ECDT_Hyperlink,         // ������
	ECDT_Identify,          // ����
	ECDT_UseSpecilItem,     // ������Ʒʹ��
	ECDT_NPCShop,           // �̵�NPC
	ECDT_DigBao,			// �ڱ�
	ECDT_Num,
};

/** \
*/
struct tagActiveCursorEvent : public tagGameEvent
{
	ECursorActiveName	eCursor;
	DWORD				dwParam;

	tagActiveCursorEvent(LPCTSTR szEventName, GameFrame* pSenderFrame):tagGameEvent(szEventName, pSenderFrame)
	{
		eCursor = ECAN_Null;
		dwParam = GT_INVALID;
	}
};

/** \class CursorMgr, ��Ϸ��������
    \brief ���ڼ��ء����桢������Ϸ�еĹ�꣬���������״̬
	\todo  ��ʱ����֧�ֹ�궯��
*/
class CursorMgr
{
public:
	CursorMgr();
	~CursorMgr();

	static CursorMgr* Inst();

	void Init(HINSTANCE hInst);
	void Destroy();

	void Active(tagActiveCursorEvent* pGameEvent);
	void OnWindowsMsg(DWORD dwMsg, DWORD dwParam1, DWORD dwParam2);
	void SetIdleCursor( const ECursorIdleName eIn );

	ECursorState GetCursorState(){return m_eCursorState;}
	ECursorActiveName GetCursorActive(){return m_eActiveCursor;}
	ECursorIdleName GetCursorIdle(){return m_eIdleCursor;}
	DWORD GetCursorActiveParam(){return m_dwParam;}

	void Change2IdleState();
	const tagPoint& GetCursorPos() const { return m_ptCursor; }
protected:
	ECursorDisplayType GetCursorDisplay();
private:
	TSFPTrunk<CursorMgr>	m_Trunk;
	ECursorState			m_eCursorState;		// ��ǰ���״̬
	ECursorActiveName		m_eActiveCursor;	// ��ǰ��꼤��״̬������
	ECursorIdleName         m_eIdleCursor;      // ��ǰ������״̬������
	DWORD					m_dwParam;			// ����m_dwParam�Ǹ��ݵ�ǰ��꼤���������ȷ����
												// ECAN_Identify��ECAN_Gem��ECAN_Brand��ECAN_Longfu��m_dwParamΪ��Ʒ�ڱ����е�λ��

	HCURSOR                 m_hCursor[ECDT_Num];
	ECursorDisplayType      m_eCursorDisplay;   // ��ǰ������ʾ��ʽ
	tagPoint				m_ptCursor;         // ��ǰ���Ĵ�������
};



