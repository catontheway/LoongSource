//-----------------------------------------------------------------------------
//!\file gui_tree.h
//!\author Lyp
//!
//!\date 2007-05-17
//! last 2005-05-17
//!
//!\brief ����ϵͳ tree �ؼ�
//-----------------------------------------------------------------------------
#pragma once


namespace vEngine {
//-----------------------------------------------------------------------------
// ����ϵͳtree�ؼ�itemԪ�ض���
//-----------------------------------------------------------------------------
struct VENGINE_API tagGUITreeItem
{
	tstring						strShow;			// ��ʾ�ı�
	tstring						strText;			// �����ı�
	tstring						strName;			// ȫ��
	DWORD						dwColor;			// �ı���ɫ
	std::list<tagGUITreeItem*>	listChildren;		// �ӽڵ��б�
	tagGUITreeItem*				pFather;			// ���ڵ�
	INT							nLayer;				// ����
	tagRect						rcPlus;				// plus��ʾ��λ��
	tagRect						rcText;				// text��ʾ��λ��
	INT							nTextWidth;			// �ַ�����ʾ����
	bool						bExpended;			// �Ƿ�չ��

	tagGUITreeItem() 
	{ pFather = 0; nLayer = 0; nTextWidth = 0; bExpended = true; }
};

//-----------------------------------------------------------------------------
// ����ϵͳtree�ؼ�
//-----------------------------------------------------------------------------
class VENGINE_API GUITree : public GUIStatic
{
public:
	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);
	virtual VOID Destroy();
	virtual VOID Render();
	virtual VOID SendEvent(tagGUIEvent* pEvent);

	// ������Ŀ(���szFaterName==NULL���ʾ��Ӹ��ڵ�)
	// ע�⣡�κνڵ��²�����ͬ���ӽڵ�
	// ���ؽڵ�ID, GT_INVALID�������ʧ��
	DWORD InsterItem(LPCTSTR szFatherName, LPCTSTR szText, LPCTSTR szShow);

	// ɾ����Ŀ
	BOOL RemoveItem(LPCTSTR szFullName);

	// ���õ�ǰ��ʾ�ĵ�һ����ʵ�ʵ���һ��
	BOOL SetCurrentTextRow(INT nY, BOOL bSendEvent=TRUE);

	// ���õ�ǰѡ�е�item,������ItemClick�¼�
	BOOL SetCurrentSelItem(DWORD dwID, BOOL bSendEvent=TRUE);
	// �õ���ǰѡ�нڵ�
	DWORD GetCurrentSelItem();

	// �õ�ָ���ڵ�
	tagGUITreeItem* GetItem(DWORD dwID) {return m_mapItem.Peek(dwID);}


	GUITree();

protected:

	// ��Ⱦ��������
	VOID Render(tagGUITreeItem* pItem, INT& nCurRow, INT& nCurY);
	// ����һ���ڵ㼰���ӽڵ���ռ��
	INT CalItemTakeupRow(tagGUITreeItem* pItem);
	// ����һ���ڵ���ʼ��(root���0��)
	INT CalItemStartRow(tagGUITreeItem* pItem);
	BOOL CalItemStartRowHelp(tagGUITreeItem* pItem, tagGUITreeItem* pTargetItem, INT& nRow); // ����

	// �Ƴ�һ��ָ���ڵ�
	BOOL RemoveItem(tagGUITreeItem* pItem);

	TObjRef<Util>					m_pUtil;
	TMap<DWORD, tagGUITreeItem*>	m_mapItem;
	TList<tagGUITreeItem*>			m_listVisibleItem;
	
	tagGUITreeItem*		m_pRootItem;			// ���ڵ�
	tagGUITreeItem*		m_pCurSelItem;			// ��ǰѡ�еĽڵ�
	INT					m_nCurrentTextRow;		// ��ǰ��ʾ�ĵ�һ����ʵ�ʵĶ�����
	INT					m_nTotalRow;			// ������
	
	tagGUIImage*		m_pPlusPic;				// �Ӻ�ͼƬ
	tagGUIImage*		m_pMinusPic;			// ����ͼƬ
	tagGUIImage*		m_pBagPic;				// ��ͼƬ


	//-----------------------------------------------------------------------------
	// �ɱ༭����
	//-----------------------------------------------------------------------------
	INT				m_nRowHeight;				// �и�
	tstring		m_strSelectStickPic;			// ѡ���ͼƬ�ļ���
	tagRect		m_rcSelectStickPic;				// ѡ���ͼƬʹ������(ͼƬ����)
	DWORD			m_dwSelectStickColor;		// ѡ�����ɫ
	DWORD			m_dwPicInterval;			// ͼƬ���
	DWORD			m_dwPicTextInterval;		// ͼƬ���ּ��
	tstring			m_strPlusPic;				// �Ӻ�ͼƬ�ļ���
	tagRect			m_rcPlusPic;				// �Ӻ�ͼƬʹ������(ͼƬ����)
	DWORD			m_dwPlusPicColor;			// �Ӻ�ͼƬ������ɫ
	tstring			m_strMinusPic;				// ����ͼƬ�ļ���
	tagRect			m_rcMinusPic;				// ����ͼƬʹ������(ͼƬ����)
	DWORD			m_dwMinusPicColor;			// ����ͼƬ������ɫ
	tstring			m_strBagPic;				// ��ͼƬ�ļ���
	tagRect			m_rcBagPic;					// ��ͼƬʹ������(ͼƬ����)
	DWORD			m_dwBagPicColor;			// ��ͼƬ������ɫ
};


}	// namespace vEngine {
