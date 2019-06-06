//-----------------------------------------------------------------------------
//!\file xui_richeditbox.h
//!\author tidy / bwen / zewu / lyp
//!
//!\date 2005-4-13
//! last 2008-1-17
//!
//!\brief ����ϵͳ richeditbox �ؼ�
//-----------------------------------------------------------------------------
#pragma once

namespace vEngine {

struct tagCharIntro
{
	tagGUIImage		*pPic;			// ��ת���ַ�����Ч	
	INT				nCharSize;		// �ַ����
	DWORD			dwHeight;		// �ַ��߶�
	INT				nByte;			// �ֽ���
	DWORD			dwReserve;		// ����
};	 //�����0x0d�ַ�����ʾ���У���ôdwHeight,nCharSize��ʾ���еĸ߶ȣ������е�����ʾ���.



//-----------------------------------------------------------------------------
// ����ϵͳ RichEditBox �ؼ�
//-----------------------------------------------------------------------------
class VENGINE_API GUIRichEditBox : public GUIStatic
{
public:
	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Destroy();
	virtual VOID Update();
	virtual VOID Render();
	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);
	virtual GUIWnd* OnActive();	
	virtual VOID OnDeactive(GUIWnd* pWnd);
	virtual BOOL PointInWnd(tagPoint& pt);

	virtual VOID SendEvent(tagGUIEvent* pEvent);// ���ⷢ���¼�

	//��Ⱦ����
	virtual VOID RenderText(LPCTSTR szText, INT nLeft, INT nBottom, INT nWidth, INT nHeight,
		DWORD dwColor, tagGUIFont* pFont, BOOL bTransparent);

public:		//��ز����ӿ�

	VOID ClsText();		//ɾ���ı�
	//�����ı�bNexLine=TRUE��ʾ����һ�п�ʼ�����������ӵ����һ�У�
	//dwColor=-1,��ʾĬ����m_Data����ɫ����,dwStyleͬ��, bMatchPic��ʾ�Ƿ���Ҫƥ��ͼƬ
	BOOL AddText(LPCTSTR szText, BOOL bNextLine = FALSE, DWORD dwColor=-1, DWORD dwShade=-1,
		BOOL bMatchPic=TRUE, BOOL bEqualTextSize=FALSE);

	//���ַ������й���,���˵������ڲ���ʽ(������ɫ���õ�)
	BOOL FilterString(tstring& str);  

	//ɾ���ı�ǰͷ���ַ�,���м���,����ʵ��ɾ��������
	INT	DelFrontText(INT nRowNum);	

	//ע��ת���ַ�����Ӧ��ͼƬ,�ַ������Ȳ��ܳ���XUI_MAX_TRANSFER_LEN,�ַ�����ʱ�趨���ܰ���˫�ֽ��ַ�
	//ͼƬ��Ȳ��ܳ���richeditbox�Ŀ��
	BOOL RegisterPic(LPCTSTR szStr, LPCTSTR szPicFileName);
	//���ֹ���(��Ϊ��λ)
	DWORD ScrollText(BOOL bUp=FALSE);

	//��������͸����,=0xff000000��ʾ��͸��
	VOID SetTransDepth(DWORD dwTransDepth) { m_dwTransDepth = dwTransDepth; }
	//�����Ƿ���ʾ���
	VOID SetShowCursor(BOOL bShowCursor=TRUE) { m_bHoldCursor = bShowCursor; }
	//���ù����ɫ
	VOID SetCursorColor(DWORD dwCursorColor) {  m_dwCursorColor = dwCursorColor; }

	//�����ܷ�ѡ���ı�(�������ܷ����ı�)
	VOID SetCanSelect(BOOL bCanSelect=TRUE) { m_bCanSelect = bCanSelect; }

	//�������ɵ��������
	VOID SetMaxTextCount(DWORD dwMaxTextCount);

	//�����Ƿ�ÿ�о�����ʾ
	VOID SetMiddleRender(BOOL bMiddle) { m_bMiddleRender = bMiddle; }

	//�Ƿ���ؼ��ּ��»���
	VOID SetKeyWordDownLine(BOOL bDownLine) { m_bDownLine = bDownLine; }
	//���ó����ӵ���껬��ʱ��������ɫ
	VOID SetLinkColorMouseHover(DWORD dwColor) { m_dwLinkColor = dwColor; }
	//�����ַ���ռ����Ƿ�һ��
	VOID SetEqualTextSize(BOOL bEqual) { m_bEqualTextSize = bEqual; }

public:		//�û���ȡ��Ϣ�ӿ�

	//��ǰ�ж�����
	INT GetRowNum(INT& nVisibleRowNum);
	//��ȡĳһ��ʵ����ʾʱ��ռ���(����-1��ʾ��Ч���к�)
	INT GetRowRenderWidth(INT nRowIndex);

	//��ȡ�ı���Ϣ����ӿ�
	INT GetText(tstring &str);
	LPCTSTR GetText(INT& nTextSize);//�˺�������
	//�õ����λ�ô���key, szKeyWord���ⲿ�ṩ�ռ�(�ؼ��ֲ�����100�ֽ�)
	BOOL GetCursorKey(LPTSTR szKeyWord, INT& nStartPos, INT& nEndPos);
	//����index�õ���������ָ�������(����false��ʾ�޴˳���)
	BOOL GetLinkContent(INT nIndex, tstring &strText);

	VOID SetReadOnly(BOOL bReadOnly) { m_bReadOnly = bReadOnly; }
	BOOL IsCanEdit() { return m_bReadOnly == FALSE; }
	VOID SetScrollLast(BOOL bScrollLast) { m_bScrollLast = bScrollLast; }
	BOOL IsScrollLast() { return m_bScrollLast; }

	BOOL IsShowLastLine();		//��ǰ�Ƿ���ʾ���������һ��
	VOID ScrollForLastLine(BOOL bReset=FALSE);	//�����Ա���������ʾ���һ��
	
	
	GUIRichEditBox();
	//--------------------------------------------------------------------------


protected:
	TMap<tstring, tagGUIImage*>	m_mapPic;	// ת�崮��ӦͼƬ
	TMap<tstring, INT>			m_mapFont;	// ת�崮��������
	TMap<tstring, INT>			m_mapKey;	// ת�崮���ùؼ���
	TMap<tstring, INT>			m_mapLink;	// ת�崮���ó����Ӹ�ʽ

	DWORD			m_dwTransDepth;

	BOOL			m_bScrollLast;			// �����ɱ༭��ʱ����������ı����Ƿ��Զ����������һ��

	BOOL			m_bShowCursor;			// ��ǰ�Ƿ���ʾ���(�������˸��)
	BOOL			m_bHoldCursor;			// �Ƿ�ӵ�й��,��m_bShowCursor����һ����
	
	DWORD			m_dwLinkColor;			// ���ó����ӵ��������ʱ��������ɫ
	INT				m_nMouseLinkIndex;		// ��ǰ��������ĳ����ӵ����(����=-1);

	BOOL			m_bCanSelect;			// �Ƿ��ܹ�ѡ��(�������ܷ���)
	BOOL			m_bMiddleRender;		// �Ƿ������Ⱦ,Ĭ��Ϊ�񣬼��������
	BOOL			m_bDownLine;			// �Ƿ���ؼ��ּ��»���(Ĭ��Ϊ����)

	DWORD			m_dwMaxTextCount;		// ����ֽ���
	
	INT				m_nSelectStart;			// ѡ��ʼ��
	INT				m_nSelectEnd;			// ѡ�������
	INT				m_nCursorPos;			// ���λ��

	INT				m_nStartDisplayLine;	// ��ʾ�ĵ�һ�У���ʵ�ʵĵڼ���

	std::vector<tagCharIntro> m_VecCharIntro; // ���ַ������Ϣ

	INT				m_nTextNormalHeight;	// ����Ĭ�ϵĸ߶�
	tagPoint		m_ptSizeChinese;		// ���ֵ�Ĭ�ϴ�С
	tagPoint		m_ptSizeLetter;			// ��ĸ

	BOOL			m_bEqualTextSize;		// �Ƿ������ַ���Сһ��
	DWORD			m_dwLastMoveTime;		// �ϴ��ƶ���ʱ��


	//-----------------------------------------------------------------------------
	// �ɱ༭����
	//-----------------------------------------------------------------------------
	BOOL			m_bDisable;				// �Ƿ���Ч
	BOOL			m_bReadOnly;			// �Ƿ�ֻ��
	DWORD			m_dwCursorColor;		// �����ɫ
	DWORD			m_dwCursorTimer;		// �����˸���
	INT				m_nRowSpacing;			// �и�
	INT				m_nMaxEditTextCount;	// ���ɱ༭�ֽ���



protected:	
	VOID	DelSelected();	// ɾ��ѡ����ַ�
	VOID	CopyToClipboard(); // ��ѡ�����ݿ��������� 
	VOID	PasteFromClipboard(); // �Ӽ����忽���ı�����ǰλ��

	/**************************�ؼ�����_start********************************/

	//����һ���ַ�,����λ��Ĭ��Ϊ��괦
	VOID	InsertText(LPCTSTR szText, INT& nVirtualPos, DWORD dwColor, 
		DWORD dwShade, BOOL bLinkLastLine=TRUE, BOOL bMatchPic=TRUE) ;
	//��nPosλ�û���
	BOOL	NewLine(INT nPos);
	//ɾ����λ�õ��ַ���,ע����λ�����������ú�
	VOID	DeleteText(INT nBeginPos, INT nEndPos);
	//���¼���ʼĩλ��֮����ַ����е���Ϣ����,�����Ű��ԭ��,nEndPos���ܻ��������
	VOID	RecalCharIntro(INT nStartPos, INT nEndPos, INT& nCursorPos, INT& nVirtualPos, BOOL bMatchPic=FALSE);
	// �ַ���ƥ�䣬��ӦͼƬ
	tagGUIImage* MatchPic(tstring &str, INT& nPos);
	// �ַ���ƥ��ת�������������
	INT MatchFont(tstring &str, INT& nPos, DWORD& nValue);

	/**************************�ؼ�����_end*********************************/

	//˫��ѡ��ĳ���ַ�
	VOID CalSelectForDoubleClick();
	// ��view����õ����λ��
	INT	GetCharIndex(INT nViewX, INT nViewY, BOOL bLeft);
	//�����������ƶ�ʱ�µĹ��λ��
	VOID CalCursonForUpDown(BOOL bUp);
	//�����������ƶ�ʱ�Ĺ��λ��
	VOID CalCursorForLeftRight(BOOL bLeft);
	//���¼���ĳһ�е��и�
	VOID RecalRowHeight(INT nRow);
	//����m_VecCharIntro��ƫ��
	VOID MoveCharIntro(INT nPos, INT IncSize);

	//�ܹ�������ʾ�����һ�еĽ���������
	INT	GetLastDisplayIndex();
	//�õ����к����׵��ַ����к�
	INT GetRowHead(INT nRowIndex);
	//�͸�����λ���ж������ĳ����ӵ�index,����index=-1
	INT GetLinkIndex(INT nPos);
	//����Ƿ����˳�����
	BOOL ClickLink(tagGUIInputMsg* pMsg);


	//���������¼�
	BOOL SendScrollEvent();
	//���õ�ǰ��ʾ������m_nStartDisplayLine
	void SetCurrentTextRow(INT nRowIndex );

	//Ѱ�һ���λ��,Ӣ�ĵ��������廻��
	INT FindBreakLinePos(INT nPos);

protected:
	//�������仯,add by zewu 2005-2-9 20:49
	void AdptiveCursor(bool bDrag, BOOL bLink);

	//������̰��µ���Ϣ(һ���ǹ��ܼ��İ���)
	BOOL OnMsgForKeyDown(INT nKey);



};



}	// namespace vEngine {

