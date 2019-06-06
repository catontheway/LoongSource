//-----------------------------------------------------------------------------
//!\file gui_listbox.h
//!\author Lyp
//!
//!\date 2004-12-08
//! last 2007-04-23
//!
//!\brief ����ϵͳ list box �ؼ�
//-----------------------------------------------------------------------------
#pragma once


namespace vEngine {
//-----------------------------------------------------------------------------
// ����ϵͳlist�ؼ�itemԪ�ض���
//-----------------------------------------------------------------------------
struct VENGINE_API tagGUIListItem
{
	tstring					strText;		// �ı�
	DWORD					dwColor;		// �ı���ɫ
	std::list<tstring>		listOptionText;	// ��ѡ�ı�
	tagGUIImage*			pPic;			// ͼƬ
	DWORD					dwPicColor;		// ͼƬ��ɫ
	BOOL					bEditable;		// �Ƿ�ɱ༭
	
	tagGUIListItem() 
	{ pPic = NULL; bEditable = false; }
};


//-----------------------------------------------------------------------------
// ����ϵͳ list box �ؼ�
//-----------------------------------------------------------------------------
class VENGINE_API GUIListBox : public GUIStatic
{
public:
	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Destroy();
	virtual VOID Render();
	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);
	virtual VOID SendEvent(tagGUIEvent* pEvent);
	virtual VOID AddChild(GUIWnd* pChild);	// ����Ӵ���
	virtual VOID DelChild(GUIWnd* pChild);

	// ����������Ĭ���п�
	VOID	SetColNum(INT nColNum, INT nDefaultColWidth);
	// ����ָ���еĿ��,�Ҳ���ָ���з���FALSE
	BOOL	SetColWidth(INT nColIndex, INT nColWidth);

	// ���szText == NULL,����յ�Ԫ��
	VOID	SetText(INT nRow, INT nCol, LPCTSTR szText, DWORD dwColor=GT_INVALID);
	// �õ�ָ����Ԫ����ַ�������,ָ����Ԫ��������,����GT_INVALID,
	LPCTSTR	GetText(INT nRow, INT nCol);
	// ���õ�Ԫ����Ϣ,
	VOID SetItemPic(INT nRow, INT nCol, LPCTSTR szPic, const tagRect& rc, DWORD dwColor=0xffffffff);
	VOID SetItemEditable(INT nRow, INT nCol, BOOL bEditable);
	VOID SetItemOptionText(INT nRow, INT nCol, std::list<tstring>& listOptionText);

	// ��ӿհ���
	VOID InsertRow(INT nRow);
	// ɾ��ָ����
	VOID DelRow(INT nRow);
	// ɾ����������
	VOID Clear();

	// �õ���ǰ������
	INT	GetTotalRowNum() { return m_nTotalTextRow; }
	// �õ���ǰ��ʾ�ĵ�һ����ʵ�ʵ���һ��
	BOOL GetCurrentTextRow() { return m_nCurrentTextRow; }
	// ���õ�ǰ��ʾ�ĵ�һ����ʵ�ʵ���һ��
	BOOL SetCurrentTextRow(INT nY, BOOL bSendEvent=TRUE);
	// �õ���ǰѡ����
	INT	 GetCurSelectedRow() { return m_nCurSelectedRow; }
	// ���õ�ǰѡ����
	VOID SetCurSelectedRow(INT n) { m_nCurSelectedRow = n; }
	// �õ��и�
	INT GetRowHeight() { return m_nRowHeight; }

	GUIListBox();

protected:

	// �����ڲ��ı༭��
	VOID ActiveEditor(INT nRow, INT nCol, tagPoint& ptPos,tagPoint& ptSize);
	DWORD SwapColor(DWORD dwColor32)
	{ return (dwColor32 & 0x00ff00)|((dwColor32&0xff0000)>>16)|((dwColor32 & 0xff)<<16); }


	TObjRef<VarContainer>	m_pGlobeVar;

	TMap<DWORD, tagGUIListItem*>  m_mapItem;	// �����ı�
	TMap<DWORD, INT> m_mapWidth;				// ÿ�еĿ��
	
	GUIEditBox*		m_pEditBox;					// �༭��
	GUIComboBox*	m_pComboBox;				// ��Ͽ�
	tagGUIImage*	m_pSelectStickPic;			// ѡ���ͼƬ

	INT m_nCurrentTextRow;						// ��ǰ��ʾ�ĵ�һ����ʵ�ʵ��ı��Ķ�����
	INT m_nTotalRow;							// �ܹ������ж�����(�������Ŀհ���)
	INT m_nTotalTextRow;						// ʵ���Ͼ������ݵ�����
	INT	m_nCurSelectedSegment;					// ��ǰ���ѡ�е����ָ���
	INT	m_nCurSelectedRow;						// ��ǰѡ�����
	INT	m_nCurEditRow;							// ��ǰ�༭����
	INT	m_nCurEditCol;							// ��ǰ�༭����
	INT	m_nCurSelectedRowWhenMouseDown;			// �����������µ�ʱ��ѡ�����

	//-----------------------------------------------------------------------------
	// �ɱ༭����
	//-----------------------------------------------------------------------------
	BOOL		m_bCanMoveCol;					// �зָ����Ƿ���ƶ�
	BOOL		m_bMoveSelect;					// �Ƿ��ƶ�������ѡ��
	BOOL		m_bOpenFileDialog;				// �Ƿ��Զ����ļ��Ի���
	BOOL		m_bOpenColorDialog;				// �Ƿ��Զ���ɫ�ʶԻ���
	INT			m_nRowHeight;					// �и�

	tstring		m_strSelectStickPic;			// ѡ���ͼƬ�ļ���
	tagRect		m_rcSelectStickPic;				// ѡ���ͼƬʹ������(ͼƬ����)
	DWORD		m_dwSelectStickColor;			// ѡ�����ɫ
	DWORD		m_dwSelectStickActiveColor;		// ѡ���������ɫ
	DWORD		m_dwRowLineColor;				// �ָ�����ɫ
	DWORD		m_dwSelectTextColor;			// ѡ�е��ı���ɫ
	DWORD		m_dwSelectShadeColor;			// ѡ�е��ı���Ӱ��ɫ
	tagPoint	m_ptPicOffset;					// ��ĿͼƬƫ��
	tagPoint	m_ptPicSize;					// ��ĿͼƬ��ʾ��С
};


}	// namespace vEngine {
