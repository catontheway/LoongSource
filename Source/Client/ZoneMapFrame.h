#pragma once


#include "MapLogicData.h"

class ZoneMapStatic;
struct tagLeaveMapAreaEvent;
struct tagEnterMapAreaEvent;
class AppliedTree;
struct tagMouseMoveEvent;
struct tagSetFlagEvent;
struct tagOpenCloseMapWndEvent;
struct tagOpenMapWndEvent;
struct tagMouseMoveFinishEvent;

/** \class ZoneMapFrame
    \brief �����ͼ�������ͼ����
*/
class ZoneMapFrame : public GameFrame
{
private:
	struct tagZoneMapShow
	{
		DWORD 				dwMapID;
		tstring             strShowName;
		tagZoneMapShow() :  dwMapID( GT_INVALID ) {}
	};
	struct tagMarkIndex
	{
		int nType;
		int nMask;
		tagMarkIndex() : nType( 0 ), nMask( 0 ){}
		tagMarkIndex( const int type, const int mask ) : nType( type ), nMask( mask ) {}
	};
public:
	ZoneMapFrame();
	~ZoneMapFrame();

	// dwParam 0-�����ͼ 1-�����ͼ
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();

	VOID SwithToWorldMap();
	VOID SwithToZoneMap( const DWORD dwMapID );
private:
	VOID Open( BOOL bWorldMap );
	VOID Close();
	BOOL GUIEventHandler(tagGUIEvent* pEvent);
	VOID SafeDestroyUI();
	VOID ResetMapData( const DWORD dwMapID, const int nAreaID );
	VOID SetShowMap( const DWORD dwMapID );
	VOID ReBuidAreaList( DWORD dwMapID );
	int  GetCurAreaIndex();
	VOID ReBuidMarksTree( const DWORD dwMapID );
	VOID OnWorldMapClick( GUIWnd* pWnd );
	/** \��Ϸ�¼���Ӧ
	*/
	DWORD OnOpenMap(tagGameEvent*);
	DWORD OnLeaveMapArea( tagLeaveMapAreaEvent* pEvent );
	DWORD OnEnterMapArea( tagEnterMapAreaEvent* pEvent );
	DWORD OnSetFlagEvent( tagSetFlagEvent* pEvent );
	DWORD OnMouseMoveEvent( tagMouseMoveEvent* pEvent );
	DWORD OnOpenCloseMapWndEvent( tagOpenCloseMapWndEvent* pEvent );
	DWORD OnOpenMapWndEvent( tagOpenMapWndEvent* pEvent );
	DWORD OnMouseMoveFinishEvent( tagMouseMoveFinishEvent* pEvent );
private:
	TSFPTrunk<ZoneMapFrame>	        m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	GUIWnd*                         m_pWnd;               // ������
	GUIButton*                      m_pBn_Close;          // �رհ�ť
	GUIStatic*                      m_pSt_WoldMap;        // �����ͼ�ؼ�
	ZoneMapStatic*                  m_pSt_ZoneMap;        // �����ͼ�ؼ�
	GUIEditBox*                     m_pEb_ShowName;       // ��ǰ��ʾ�ĵ�ͼ����������
	GUIListBox*                     m_pLb_MapList;        // ��ͼ�����б��
	GUIButton*                      m_pBn_Back;           // ���ذ�ť
	GUIButton*                      m_pBn_Task;           // �ɽ�����ť
	AppliedTree*                    m_pTr_Mark;           // ��ͼ�����

	vector<tagMapShowAreaData>      m_vecZoneMapArea;     // ���е�ͼ�б����������ͼ��
	vector<tagZoneMapShow>          m_vecZoneMapShow;     // �����ͼ�б����������ͼ��
	int                             m_nShowAreaIndex;     // ��ǰ��ʾ������m_vecZoneMapArea�е�����
	map<DWORD, tagMarkIndex>        m_mapMarkIndex;       // KEY:ID in AppliedTree
};

