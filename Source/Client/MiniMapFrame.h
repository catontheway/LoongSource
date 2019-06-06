#pragma once

class MiniMapStatic;
struct tagLeaveMapAreaEvent;
struct tagEnterMapAreaEvent;
struct tagSetFlagEvent;
struct tagMouseMoveFinishEvent;

/** \class MiniMapFrame
    \brief С��ͼ����
*/
class MiniMapFrame : public GameFrame
{
public:
	MiniMapFrame();
	~MiniMapFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
private:
	VOID SafeDestroyUI();
	BOOL OnWnd_MaxSize(tagGUIEvent* pEvent);				      //��󻯴����¼���Ӧ����
	BOOL OnWnd_MiniSize(tagGUIEvent* pEvent);				      //��С�������¼���Ӧ����
	BOOL OnWnd_Set(tagGUIEvent* pEvent);				          //С��ͼ���ô����¼���Ӧ����
	VOID ResetMapData();
	/** \��Ϸ�¼���Ӧ
	*/
	DWORD OnOpenMap(tagGameEvent*);
	DWORD OnLeaveMapArea( tagLeaveMapAreaEvent* pEvent );
	DWORD OnEnterMapArea( tagEnterMapAreaEvent* pEvent );
	DWORD OnSetFlagEvent( tagSetFlagEvent* pEvent );
	DWORD OnMouseMoveFinishEvent( tagMouseMoveFinishEvent* pEvent );
	/** \�ȼ���Ӧ����
	*/
	DWORD OnZoomIn_HotKey( DWORD dwID, BOOL bDown );
	DWORD OnZoomOut_HotKey( DWORD dwID, BOOL bDown );

	INT EGStoEMME( const INT nEGS ) const;
private:
	TSFPTrunk<MiniMapFrame>	        m_Trunk;
	TObjRef<GameInputMap>			m_pKeyMap;
	TObjRef<GUISystem>				m_pGUI;

	GUIWnd*                         m_pWnd_MaxSize;               //С��ͼ��󻯴���
	MiniMapStatic*                  m_pMSt_MiniMap;               //С��ͼ�ؼ�
	GUIStatic*                      m_pSt_MapName;                //��ͼ���ֱ�ǩ
	GUIStatic*                      m_pSt_PosX;                   //���X�����ǩ
	GUIStatic*                      m_pSt_PosZ;                   //���Z�����ǩ
	GUIButton*                      m_pBn_MiniSize;               //��С����ť
	GUIButton*                      m_pBn_Setting;                //С��ͼ���ð�ť
	GUIButton*                      m_pBn_AreaMap;                //�����ͼ��ť
	GUIButton*                      m_pBn_TaskQuery;              //������ѯ��ť
	GUIButton*                      m_pBn_TaskTrace;              //����׷�ٽ��水ť
	GUIButton*                      m_pBn_ZoomIn;                 //�Ŵ�ť
	GUIButton*                      m_pBn_ZoomOut;                //��С��ť
	GUIButton*						m_pBn_VipStall;				  //VIP̯λ
	GUIButton*						m_pBn_OnlineTip;			  //������ʾ
	GUIPushButton*					m_pPBtn_ContinuteSkill;		  //���Ӽ�����
	GUIButton*                      m_pBn_StyleAction;            //���Զ�����ť
	GUIButton*						m_pBtnQuestHelp;			  // �������

	GUIWnd*                         m_pWnd_MiniSize;              //С��ͼ��С������
	GUIStatic*                      m_pSt_MapName_MiniSize;       //��С�������еĵ�ͼ���ֱ�ǩ
	GUIStatic*                      m_pSt_PosX_MiniSize;          //��С�������е�X�����ǩ
	GUIStatic*                      m_pSt_PosZ_MiniSize;          //��С�������е�Z�����ǩ
	GUIButton*                      m_pBn_MaxSize;                //��󻯰�ť

	GUIWnd*                         m_pWnd_Set;                   //С��ͼ���ô���
	GUIPushButton*                  m_pBn_EleShow[EMME_Num];      //�Ƿ���ʾ��ť
	GUIButton*                      m_pBn_Ok_Set;                 //ȷ��
	GUIButton*                      m_pBn_Cancel_Set;             //ȡ��
	GUIButton*                      m_pBn_Close_Set;              //�ر�



	tagPoint                        m_ptLocalPlayerPos;
};

