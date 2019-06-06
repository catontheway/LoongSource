#pragma once


// CMapObjDlg �Ի���
#include "resource.h"
#include "..\Cool3D\EditorSupport\SimpleView.h"
#include ".\editdlg.h"
#include ".\MapPropertyList.h"
#include "afxwin.h"
#include "afxcmn.h"
#include ".\AdaptiveDlg.h"

struct tagResGather;
class ResourceGather;
#define LOCK_COL 2
#define FLAG_COL 3
//�������Ͷ���
enum GMAPOBJ_TYPE
{
	GMAPOBJ_Npc ,
	GMAPOBJ_StaticObj,
	GMAPOBJ_DynamicBlock,	//by add xtian 2008-8-12
	GMAPOBJ_WAYPOINT,
	GMAPOBJ_Trigger, 
	GMAPOBJ_Sound,
	GMAPOBJ_PointLight,
	GMAPOBJ_MapRect,		//by add xtian 2008-5-12
	GMAPOBJ_PathPoint,		//by add xtian 2008-8-6
	GMAPOBJ_SpawnPoint,		//by add xtian 2008-8-11
	GMAPOBJ_EventTrigger,	//by add xtian 2008-9-10
	GMAPOBJ_NPCNavStartPoint, //by add xtian 2008-10-20
	GMAPOBJ_PlayerNavStartPoint, //by add xtian 2008-10-20
	GMAPOBJ_MapDoor,
	GMAPOBJ_MapCarrier,		//by add xtian 2009-3-13
	GMAPOBJ_MapTriggerEffect,//by add xtian 2009-7-21

	GMAPOBJ_NUM,
};

enum Img_Type				//����ͼƬ����
{
	EImg_Null = 0,
	EImg_Lock,
	EImg_Flag,
};

class CMapObjDlg : public CDialog
{
	DECLARE_DYNAMIC(CMapObjDlg)

public:
	CMapObjDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMapObjDlg();

    // �Ի�������
	enum { IDD = IDD_MAPOBJDLG };

	CListCtrl		 m_objList;     //�����б��
	CComboBox		 m_objTypeList; //��������������
	CMapPropertyList m_propList;    //���Կ�
	CButton          m_NEWMAP;      //�����µ�ͼ
	HACCEL			m_hAccelTable;	//��ݼ�

	void AttachParent(CWnd* pWnd)
	{
		ASSERT(m_pParent == NULL);
		m_pParent = pWnd;
	}
	MapObjEdit* GetPreCreate() const    { return this->m_pPreCreated;}
	void SetPreCreate(MapObjEdit* pEdit){ m_pPreCreated = pEdit;     } 
	virtual BOOL PreTranslateMessage(MSG* pMsg);		//��ݼ���Ϣ������

protected:
	
	friend class     CPlugMgr;
	SimpleView		*m_Cool3DView;
	CWnd			*m_pParent;
	MapObjEdit     *m_pPreCreated;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DWORD m_dwMapIndex;      //�༭��ͼID
	bool m_bIsBuildWayPoint; //�Ƿ���������
	bool m_bIsBuildTrigger;  //�Ƿ���������
	bool m_bIsBuildSound;    //�Ƿ�����Ч
	CString m_szStaticFileOpenType;//�ļ�������
	int m_PreObjListSelectedIndex;//���ڿ��Ƽ���ѡ��objlist
	bool m_bIsBuildPointLight;	//�Ƿ������Դ
	bool m_bCopyBuildPointLgt;	//�Ƿ��ƽ������Դ
	bool m_bIsBuildMapRect;		//�Ƿ�����ͼ����
	bool m_bIsBuildPathPoint;	//�Ƿ���Ѱ·����
	bool m_bIsBuildSpawnPoint;	//�Ƿ���ˢ�ֵ�
	bool m_bIsBuildNpcNav;		//�Ƿ���NPC���ӵ���ͼ�������
	bool m_bIsBuildPlayerNav;	//�Ƿ������Ѱ·����ͼ�������


	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);

	//������
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

	//��ť����
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedDel();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedCommit();
	afx_msg void OnBnClickedButtonNewmap();
	

    //������
	afx_msg void OnCbnSelchangeobjtypelist();
	afx_msg void OnNMClickobjlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkobjlist(NMHDR *pNMHDR, LRESULT *pResult);	//˫��ʹ������ҵ���ͼ����
	afx_msg void OnOK();		//�ڶ��������޸ĺ󰴻س�,�ᴥ��Ӧ�ð�ť�¼�	
	//ɸѡ��ť
	afx_msg void OnBnClickedButtonFilter();	

	//function
	void ResetAllInfo();

    //--����/����Ƿ����������ʶ
	bool GetIsBuildWayPoint()                       {   return  m_bIsBuildWayPoint;          }  
	void SetIsBuildWayPoint(bool bIsBuildWayPoint)  {   m_bIsBuildWayPoint=bIsBuildWayPoint; }
    //--����/����Ƿ�����������ʶ
	bool GetIsBuildTrigger()                        {   return  m_bIsBuildTrigger;           }  
	void SetIsBuildTrigger(bool bIsBuildTrigger)    {   m_bIsBuildTrigger=bIsBuildTrigger;   }
	//--���ã�����Ƿ�����Ч
    bool GetIsBuildSound()                          {   return  m_bIsBuildSound;             }  
	void SetIsBuildSound(bool bIsBuildSound)        {   m_bIsBuildSound=bIsBuildSound;       }         
	//--�õ���ǰ�Ƿ���������
	bool GetIsLockOnObject();
	//--����/����Ƿ������Դ
	bool GetIsBuildPointLight()						{	return m_bIsBuildPointLight;		}
	void SetIsBuildPointLight(bool bIsBuildPointLight){	m_bIsBuildPointLight = bIsBuildPointLight;}
	//--����/����Ƿ�����ͼ����
	bool GetIsBuildMapRect()						{	return m_bIsBuildMapRect; }
	void SetIsBuildMapRect(bool bIsBuildMapRect)	{	m_bIsBuildMapRect = bIsBuildMapRect; }
	//--����/����Ƿ񿽱����Դ
	bool GetCopyBuildPointLgt()						{	return m_bCopyBuildPointLgt; }
	void SetCopyBuildPointLgt(bool bVal)			{	m_bCopyBuildPointLgt = bVal; }
	//--����/����Ƿ���Ѱ·������
	bool GetIsBuildPathPoint()						{	return m_bIsBuildPathPoint; }
	void SetIsBuildPathPoint(bool bVal)				{	m_bIsBuildPathPoint = bVal; }
	//--����/����Ƿ���ˢ�ֵ�
	bool GetIsBuildSpawnPoint()						{	return m_bIsBuildSpawnPoint; }
	void SetIsBuildSpawnPoint(bool bVal)			{	m_bIsBuildSpawnPoint = bVal; }
	//--
	bool GetIsBuildNPCNavStartPoint()				{	return m_bIsBuildNpcNav; }
	void SetIsBuildNPCNavStartPoint(bool bVal)		{	m_bIsBuildNpcNav = bVal; }
	//--
	bool GetIsBuildPlayerNavStartPoint()			{	return m_bIsBuildPlayerNav; }
	void SetIsBuildPlayerNavStartPoint(bool bVal)	{	m_bIsBuildPlayerNav = bVal; }

	//--��ʾ�����б�
	void DisplayList(MapObjEdit::EType type);
	//--��ʾ�������б�
	void DisplayWayPointList();
	//--��ʾ�������б�
	void DisplayTriggerList();
	//--��ʾ��Ч�б�
	void DisplaySound();
	//--��ʾ���Դ�б�
	void DisplayPointLightList();
	//--��ʾ��ͼ�����б�
	void DisplayMapRectList();
	//--��ʾѰ·������
	void DisplayPathPoint();
	//--��ʾˢ�ֵ�
	void DisplaySpawnPoint();
	//--��ʾ�¼�������
	void DisplayEventTrigger();
	//--
	void DisplayNPCNav();
	//--
	void DisplayPlayerNav();

	//--�������������б�
	void UpdateList(MapObjEdit* pSelected);
	//--���µ����������б�
	void UpdatePropListForWayPoint(tagMapWayPoint* pWayPoint);
	//--���´����������б�
	void UpdatePropListForTrigger(tagMapTrigger* pTrigger);
	//--������Ч�б�
	void UpdatePropListForSound(tagMapSound* pSound);
	//--���µ��Դ�б�
	void UpdatePropListForPointLight(MapObjEdit* pEdit);
	//--���µ�ͼ�����б�
	void UpdatePropListForMapRect(MapObjEdit* pEdit);
	//--����Ѱ·������
	void UpdatePropListForPathPoint(MapObjEdit* pEdit);
	//--����ˢ�ֵ�
	void UpdatePropListForSpawnPoint(MapObjEdit* pEdit);
	//--�����¼�������
	void UpdatePropListForEventTrigger(MapObjEdit* pEdit);
	//--
	void UpdateProListForNPCNav(tagNPCNavMapBuildStartPoint* pNpcNav);
	//--
	void UpdateProListForPlayerNav(tagPlayerNavMapBuildStartPoint* pPlayerNav);
    
	//--��������ID��obj�б�
	int AddObjIDToObjList(CString szBuffer);
  
	//--�޸ĵ����������
	void ModifyWayPointProp(CString szID);
	//--�޸Ĵ�����������
	void ModifyTriggerProp(CString szID);
	//--�޸���Ч������
	void ModifySoundProp(CString szID);
	//--�޸ĵ��Դ������
	void ModifyPointLightProp(CString szID);
	//--�޸ĵ�ͼ���������
	void ModifyMapRectProp(CString szID);
	//--�޸�Ѱ·������
	void ModifyPathPointProp(CString szID);
	//--�޸�ˢ�ֵ�
	void ModifySpawnPoint(CString szID);
	//--�޸��¼�������
	void ModifyEventTrigger(CString szID);
	//--
	void ModifyNPCNav(CString szID);
	//--
	void ModifyPlayerNav(CString szID);

	//--new a waypoint
	bool NewWayPoint();
	//--new a trigger
	bool NewTrigger();
	//--new a object
	bool NewObject();
	//--new a sound
	bool NewSound();
	//--
	bool NewNPCNavStartPoint();
	//--
	bool NewPlayerNavStartPoint();

	//--��ʾ/����NPC
	void ShowNPC(bool bVal);
	//--��ʾ/���ض�̬�ϰ���
	void ShowDynamicBlock(bool bVal);
	//--��ʾ/���ؾ�̬����
	void ShowStaticObj(bool bVal);
	//--��ʾ/������������ͼ����
	void ShowLockOjb(bool bVal);
	//--��ʾ/�����ѱ�ǵ�ͼ����
	void ShowFlagObj(bool bVal);
	//--��ʾ/������
	void ShowMapDoor(bool bVal);
	//--��ʾ/���ش�����Ч
	void ShowMapTriggerEffect(bool bVal);

	//--�Ƿ���ʾ/����NPC
	bool IsShowNPC();
	//--�Ƿ���ʾ/���ض�̬�ϰ���
	bool IsShowDynamicBlock();
	//--�Ƿ���ʾ/���ؾ�̬����
	bool IsShowStaticObj();
	//--�Ƿ���ʾ/���ص��﷽��
	bool IsShowQuad();
	//--�Ƿ���ʾ/�����Թ�����
	bool IsShowMazeZone();
	//--�Ƿ���ʾ/������������ͼ����
	bool IsShowLockObj();
	//--�Ƿ���ʾ/�����ѱ�ǵ�ͼ����
	bool IsShowFlagObj();
	//--�Ƿ���ʾ/���ص�ͼ����
	bool IsShowMapRect();
	//--�Ƿ���ʾ/������
	bool IsShowMapDoor();
	//--�Ƿ���ʾ/���ش�����Ч
	bool IsShowMapTriggerEffect();

	//�õ���ǰ��ͼ��������
	GMAPOBJ_TYPE  GetCurGMAPOBJTYPE();
	
	// ������Ӱ�ť
	CButton m_CopyAdd;
	afx_msg void OnBnClickedButtonCopyadd();		//������Ӱ�ť�¼�������
	// ���浼������
	CString m_strWayPtName;
	// ��ʾɸѡ
	bool m_IsNpc;
	bool m_IsDynaBlock;
	bool m_IsStaticObj;
	bool m_IsQuad;
	bool m_IsMazeZone;
	bool m_IsLock;
	bool m_IsFlag;
	bool m_IsMapRect;
	bool m_IsTrigger;
	bool m_IsWayPoint;
	bool m_IsMapDoor;
	bool m_IsMapTriggerEffect;
	
	// ����/�����ļ�����
	CString m_szMapID;						// �����ͼID
	tagLight m_SunLight;					// ̫����
	Color4f m_SkyCol;						//���
	DWORD   m_dwSkyShadeCol;				//���������ɫ
	bool    m_bRenderSkyShade;              //�Ƿ���Ⱦ�������
	DWORD   m_dwDynamicDiffCol;             // ��̬ƽ�й�diffuse
	DWORD   m_dwDynamicAmbCol;				// ��̬ƽ�й�Ambient
	DWORD   m_dwDynamicSpecCol;             // ��̬ƽ�й�Specular
	CString m_szSkyBoxTopTex;				//��պ�������
	CString m_szSkyBoxFrontRightTex;		//��պ�ǰ������
	CString m_szSkyBoxBackLeftTex;			//��պк�������
	Vector3 m_vecSkyBoxSize;				//��պд�С
	Vector3 m_vecSkyBoxOff;					//��պ�ƫ��
	FLOAT	m_fSunModulus;					//̫������ǿϵ��
	FLOAT	m_fSkyYaw;						//��պ���ת�Ƕ�
	BOOL	m_bMapAttOk;					//�Ƿ�ȷ����ͼ����
	tagDistFog m_Fog;						//��
	INT		m_dist;							//��Ұ
	
	afx_msg void OnBnClickedMapatt();// ��ͼ���԰�ť�¼�

	//--adaptive
	afx_msg void OnBnClickedBnadative();
	AdaptiveDlg* m_adaptiveDlg;
	afx_msg LONG OnDelAdativeDlgMsg(WPARAM wP,LPARAM lP);//����AdaptiveͶ�ݵ�ɾ��������Ϣ
	CButton m_BtnAdative;		

	// ͼƬ
	CImageList m_imgList;								//ͼƬ��Դ�б�(���ͱ�־)

	//--��Դ����
	ResourceGather*	m_pResGatherDlg;			
	CButton				m_BtnResGather;					//��Դ���ܰ�ť
	static vector<tagResGather>	m_vecRes;				//��������ӵĵ�ͼ����
	afx_msg void OnBnClickedButtonResgather();			//��Դ���ܰ�ť�¼�����
	afx_msg LONG OnDelRGDlgMsg(WPARAM wP,LPARAM lP);	//������Դ����Ͷ�ݵ�ɾ��������Ϣ
	afx_msg LONG OnNewObjMsg(WPARAM wP,LPARAM lP);		//������Դ����Ͷ�ݵ����ӵ�ͼ������Ϣ
	afx_msg LONG OnShowModel(WPARAM wP, LPARAM lP);		//������Դ����Ͷ�ݵĲ鿴ģ����Ϣ
	
	//--ֻ��ʾ����/��ʶ�ĵ�ͼ����
	BYTE m_byColumn;		//��������һ���е�CListCtrl��ť	
	bool m_bColEqual;
	afx_msg void OnColumnclickobjlist(NMHDR *pNMHDR, LRESULT *pResult);	//����CListCtrl�а�ť�¼�
	void hideNpcStaObjByLock(MapObjEdit::EType type);
	void hideNpcStaObjByFlag(MapObjEdit::EType type);

	//--
	afx_msg void OnBnClickedShowstaticpl();
	afx_msg void OnBnClickedShowdynamicpl();
	BOOL m_bShowStaticPL;
	BOOL m_bShowDynamicPL;

	//��ʾ��
	BOOL m_bShowFog;
	afx_msg void OnBnClickedShowfog();
	
	//�Ƿ�������ƶ�
	BOOL m_bFollowMove;
	afx_msg void OnBnClickedCheckFollowmove();
	BOOL GetFollowMove(){ return m_bFollowMove; }
	
	//ȫ��id
	DWORD	m_dwGlobalID;
	vector<tagMapNPC*>		m_vecSortNPC;
}; 


