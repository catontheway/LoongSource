#pragma once
#include "..\Cool3D\Cool3DEngine.h"
using namespace Cool3D;

#include "..\Cool3D\public\iplugeditor.h"
#include "Resource.h"
#include "TEditorSheet.h"
#include "BrushDlg.h"

class PlugTerrainEditor :	public IPlugEditor
{
public:
	PlugTerrainEditor(void);
	virtual ~PlugTerrainEditor(void);

	//--IPlugEditor
	virtual void AttachEngineInterface(void *pInterface,void *pExInterface=NULL);
	virtual void SetCamera(CameraBase *pCamera) { m_pCamera=pCamera;}

	virtual void Update(bool bActive);
	virtual void Render(bool bActive);

	virtual const TCHAR* GetUserName();
	virtual const TCHAR* GetClassName();
	virtual void Create(DWORD pParentWnd);
	virtual void Destroy();
	virtual void ShowWindow(bool bShow);

	virtual bool Build(const TCHAR* szMapName,const TCHAR* szSaveBasePath,DWORD dwItems,tagBuildOptions& options,IUICallBack* pUI);

	virtual void OnLButtonDown(UINT nFlags, UINT x,UINT y);
	virtual void OnLButtonUp(UINT nFlags, UINT x,UINT y);
	virtual void OnMouseMove(UINT nFlags, UINT x,UINT y);
	virtual void OnMouseWheel(UINT nFlags, short zDelta, UINT x,UINT y);
	virtual void OnRButtonDown(UINT nFlags, UINT x,UINT y);
	virtual void OnRButtonUp(UINT nFlags, UINT x,UINT y);

	//--
	TerrainEditor*	GetEngine()	{ return m_pEngine; }
	CameraBase* GetCamera()		{ return m_pCamera;}
	TEditorSheet*	GetSheet()	{ return &m_sheet; }
	CBrushDlg*		GetBushDlg(){ return &m_brushDlg; }
	void AlignBrushDlg();

private:
	bool _Build(const TCHAR* szMapName,const TCHAR* szSaveBasePath,DWORD dwItems,tagBuildOptions& options,IUICallBack* pUI);
	void CreateBrush(tagTEditBrush& brush,int inversVal=1);
	void DoCurrentTools(tagTEditBrush& brush);
	
	POINT			m_cursorScrPos;		//�������Ļ����
	Vector3			m_cursorPos;		//ѡ�еĵ������������
	Vector3			m_cursorPosRiver;	//ѡ�еĺ�������������
	CameraBase		*m_pCamera;
	TerrainEditor	*m_pEngine;
	//--����
	TEditorSheet	m_sheet;
	CBrushDlg		m_brushDlg;
	POINT			m_lastDoPt;//��һ�β���������,��ֹ�ظ�����
	CString			m_szHmapInfo;

	//���״̬����
	typedef enum _MOUSESTATE
	{
		MS_INVALID				=-1,	//��Ч״̬
		MS_NONE					=0,		//����״̬
		MS_LDOWN,						//�������
		MS_LMOVE,						//����ƶ�
		MS_RDOWN,						//�Ҽ�����
		MS_RMOVE,						//�Ҽ��ƶ�
		MS_LRDOWN,						//���Ҽ�����
		MS_LRMOVE,						//���Ҽ��ƶ�
		MS_LMOVE_RDOWN,					//����ƶ��Ҽ�����
		MS_RMOVE_LDOWN					//�Ҽ��ƶ��������
	} MOUSESTATE;
	MOUSESTATE		m_mouseState;		//���״̬
	//����������
	typedef enum _MOUSEOPERATION
	{
		MO_INVALID				=-1,	//��Ч����
		MO_ONLDOWN				=0,		//�������
		MO_ONLUP,						//�������
		MO_ONRDOWN,						//�Ҽ�����
		MO_ONRUP,						//�Ҽ�����
		MO_ONMOVE						//����ƶ�
	} MOUSEOPERATION;
	/** �ı����״̬
		\param	������
	*/
	void ChangeMouseState(MOUSEOPERATION mouseOp);
};

extern PlugTerrainEditor	*g_editor;
void BeginMFCRes();
void EndMFCRes();