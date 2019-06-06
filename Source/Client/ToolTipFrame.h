#pragma once
#include "render2d.h"

class ToolTipStatic;

/** class	ToolTipFrame
	brief	��ʾ��Ϣ�ؼ�����
*/
class ToolTipFrame :
	public GameFrame
{
public:
	ToolTipFrame(void);
	~ToolTipFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	
	virtual BOOL ReloadUI();

	virtual BOOL EscCancel();

	
	/** \��ʾ��ʾ��Ϣ
		\param szTip: ��ʽ���õ���ʾ��Ϣ,==NULL��ʾ����
		\param szTipsEx: ������ʾ��Ϣ����Ϊ��Ʒ�۸�
		\param pWnd: ָ��Ϊ�ĸ��ؼ�����Tips
		\�ⲿ����Ϣ��ʽ���ú����,�ڲ�����������Ϣ�Ĵ�С�Զ�������ʾ��Ϣ��Ĵ�С
	*/
	VOID ShowTips(GUIWnd* pWnd, LPCTSTR szTip, LPCTSTR szTipEx=NULL);
	/** \param rc: ָ��ΪԴ�ؼ�������
	*/
	VOID ShowTips(GUIWnd* pWnd, LPCTSTR szTip, const RECT& rc);
	/** \param ptPos��ָ�����ĸ�λ����ʾ��ʾ��Ϣ
	*/
	VOID ShowTips(tagPoint ptPos, LPCTSTR szTip);
	/** \��ʾװ���Ƚ���Ϣ
		\param szTip: ��ʽ���õ�װ���Ƚ���Ϣ,==NULL��ʾ����
		\param szTipsEx: װ���Ƚ���Ϣ���䣬Ϊ������װ��ʱ׼��
		\�ⲿ����Ϣ��ʽ���ú����,�ڲ�����������Ϣ�Ĵ�С�Զ�������ʾ��Ϣ��Ĵ�С
	*/
	VOID ShowTipsCompare(LPCTSTR szTip, LPCTSTR szTipEx=NULL);

private:
	/** \������ʾ�ؼ��Ĵ�С���¼���ؼ���λ��
		\param ptSize: ��ʾ�ؼ��Ĵ�С
	*/
	VOID ResetWindowPoint(tagPoint ptSize);
	/** \param ptPos: ��ʾ�ؼ���λ��
	*/
	VOID ResetWindowPoint(tagPoint ptSize, tagPoint ptPos);
	/** \ɾ��UI�������
	*/
	BOOL DestroyUI();
	/** \���������UI�¼�	
		\param pEvent: UI�¼�
	*/
	BOOL EventHandler(tagGUIEvent* pEvent);
	/** \��ʾ�ؼ�����Ϣ��ʾ
		\param pStatic: �ÿؼ���ָ��
	*/
	VOID ShowStaticTip(GUIStatic* pStatic);
	/** \��һ���ؼ�����һ���ؼ�����
		\param pSrcWnd: ��Ҫ����Ŀؼ� 
		\param pDstWnd: ��֮����Ŀؼ�
		\param eAlign: ����ĸ�ʽ
	*/
	VOID AlignWnd(GUIWnd* pSrcWnd, GUIWnd* pDstWnd, EGUIAlign eAlign);

private:

	TSFPTrunk<ToolTipFrame>		m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<GUIRender>			m_pRender;

	GUIWnd*						m_pWnd;			// Tips����
	GUIWnd*						m_pWndEx;		// Tips�������
	GUIWnd*						m_pWndCompare;	// װ���Ƚ�Tips����
	GUIWnd*						m_pWndCompareEx;// װ���Ƚ�Tips�������
	ToolTipStatic*				m_pStcTips;		// Tips������ʾ�ؼ�
	ToolTipStatic*				m_pStcTipsEx;	// Tips�������ݿؼ�
	ToolTipStatic*				m_pStcCompare;	// װ���Ƚ�Tips������ʾ�ؼ�
	ToolTipStatic*				m_pStcCompareEx;// װ���Ƚ�Tips����������ʾ�ؼ�

	GUIWnd*						m_pSrcWnd;		// ��Ҫ��ʾ�Ŀؼ�����
};
/** \function ShowTip: ��ʾ��ʾ��Ϣ�����ݿؼ���λ�ã�
	\param pWnd: ��Ҫ��ʾ��Ϣ�Ŀؼ�
	\param szTip: ��Ҫ��ʾ���ַ���
	\param szTipEx: �����ַ�������Ϊ��Ʒ�ļ۸�
*/
inline void ShowTip(GUIWnd* pWnd, LPCTSTR szTip, LPCTSTR szTipEx=NULL)
{
	ToolTipFrame* pTips = (ToolTipFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("Tooltip"));
	
	if (P_VALID(pTips))
		pTips->ShowTips(pWnd, szTip, szTipEx);	
}
/** \function ShowTip: ��ʾ��ʾ��Ϣ������������λ�ã�
	\param ptPos: ��ʾ��Ϣ�������
	\param szTip: ��Ҫ��ʾ���ַ���
*/
inline void ShowTip(tagPoint ptPos, LPCTSTR szTip)
{
	ToolTipFrame* pTips = (ToolTipFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("Tooltip"));

	if (P_VALID(pTips))
		pTips->ShowTips(ptPos, szTip);
}
/** \function ShowTipCompare: ��ʾװ���Ƚ���Ϣ
	\param szTip: ��Ҫ��ʾ���ַ���
	\param szTipEx: ��Ҫ��ʾ���ַ���
*/
inline void ShowTipCompare(LPCTSTR szTip, LPCTSTR szTipEx=NULL)
{
	ToolTipFrame* pTips = (ToolTipFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("Tooltip"));

	if (P_VALID(pTips))
		pTips->ShowTipsCompare(szTip, szTipEx);
}

/** \function HideTip: ���ؽ������Tip
	\param pWnd����ؽ���ؼ�ָ��
*/
inline void HideTip(GUIWnd* pWnd)
{
	POINT pt;
	::GetCursorPos(&pt);
	if(P_VALID(pWnd) && pWnd->PointInWnd(tagPoint(pt.x, pt.y), false))
	{
		ShowTip(NULL ,NULL);
	}
}