//-----------------------------------------------------------------------------
//!\file gui_editboxex.h
//!\author Lyp
//!
//!\date 2004-12-08
//! last 2008-01-21
//!
//!\brief ����ϵͳ ���� edit box �ؼ�
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine {

enum EGUIEditChar
{
	EGEC_Char		= 0,	// ��ͨ�ַ�
	EGEC_Normal		= 1000,
	EGEC_Return		= 1001,	// Ӳ�س�
	EGEC_VReturn	= 1002,	// ��س�
};

//-----------------------------------------------------------------------------
// ÿ���ַ�����Ϣ
//-----------------------------------------------------------------------------
struct tagGUIEditCharInfo	// editbox char info
{
	EGUIEditChar	eType;		// ����
	INT				nWidth;		// �ֿ�
	tagGUIEditCharInfo() { eType = EGEC_Char; nWidth = 0; }
};

//-----------------------------------------------------------------------------
// ����ϵͳ edit box �ؼ�
//-----------------------------------------------------------------------------
class VENGINE_API GUIEditBoxEx : public GUIStatic
{
public:
	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Destroy();
	virtual VOID Update();
	virtual VOID Render();
	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);
	virtual VOID SendEvent(tagGUIEvent* pEvent);
	virtual VOID AddChild(GUIWnd* pChild);	// ����Ӵ���
	virtual GUIWnd* OnActive();	
	virtual VOID OnDeactive(GUIWnd* pWnd);
	virtual BOOL Resize(tagPoint& ptSize); // �ı�ؼ���С,�����µĴ�С

	// ��������ִ���ӿ�
	VOID		Clear();	// ɾ�������ı�
	VOID		SetText(LPCTSTR szText);	// ֱ�������ı�������
	VOID		AddText(LPCTSTR szText) { AddString(m_strText.size()-1, szText); }	// ����ı�
	tstring&	GetText(BOOL bVirtualReturn);	// bVirtualReturn:�Ƿ���Ҫ����س�

	// ���������Լ���Ϣ��ȡ
	INT			GetLineNum() { return m_nLine; }	// �õ��ı�������
	VOID		DelFrontLine(INT nLine);	// ɾ��ǰ�������
	INT			GetCurLine() { return m_nCurFirstLine; }	// �õ���ǰ��ʾ�ĵ�һ����ʵ�ʵ���һ��
	VOID		SetCurLine(INT n, BOOL bSendEvent=TRUE);	// ���õ�ǰ��
	INT			GetCursorLine() { return this->GetLine(m_nCursorPos); }	// �õ����������
	BOOL		IsShowLastLine() { return (m_nCurFirstLine+m_nVisibleLine >= m_nLine-2); }
	VOID		SelectAll(); // ȫ��ѡ��
	
	// ��ǩ
	VOID		SetBookmark(INT nLine, DWORD dwColor);
	BOOL		IsSetBookmark(INT nLine) { return m_mapBookmark.IsExist(nLine); }

	VOID		EnterLock() { EnterCriticalSection(&m_Lock); }
	VOID		LeaveLock() { LeaveCriticalSection(&m_Lock); }

	GUIEditBoxEx();

protected:
	// --������Ҫ����ı���
	std::vector<tagGUIEditCharInfo> m_vecChar;	// ÿ����Ϣ
	INT		m_nSelStart;						// ѡ��ʼ��
	INT		m_nSelEnd;							// ѡ�������
	INT		m_nCursorPos;						// ���λ��
	INT		m_nDispStart;						// ��ǰ����һ�ֿ�ʼ��ʾ
	INT		m_nLine;							// һ��������
	INT		m_nCurFirstLine;					// ��ǰ��ʾ�ĵ�һ����ʵ����һ��
	INT		m_nVisibleLine;						// һ������ʾ������

	// --��������Ҫ����ı���
	bool	m_bShowCursor;						// ��ǰ�Ƿ���ʾ���(�������˸��)
	BOOL	m_bFixWidth;						// �Ƿ�ȿ�
	INT		m_nEngWidth;						// Ӣ�Ŀ��
	INT		m_nChnWidth;						// ���Ŀ��
	tstring	m_strOut;							// ����õ��ַ���

	TMap<INT, DWORD>	m_mapBookmark;			// ��ǩ
	CRITICAL_SECTION	m_Lock;					// Ϊ��֤IMSG��Ķ��߳�ͬ��
	

	// ��ָ��λ�ã�ָ��������ַ��������������ַ���
	INT		AddString(INT nPos, tstring str);
	INT		FindFirstCharThisLine(INT nPos);	// �õ�ָ��λ�������еĵ�һ����
	INT		FindFirstChar(INT nLine);			// �õ�ָ���еĵ�һ����
	INT		FindReturnThisLine(INT nPos);		// �õ�ָ��λ�������еĻ��з�λ��
	INT		GetLine(INT nPos);					// ��ָ��λ�õõ��к�,����ȷ��m_nDispStart�Լ�m_nCurFirstLine��ȷ
	INT		ReMakeup(INT nPos, INT nEndPos);	// �ӵڼ����ֿ�ʼ�����Ű�,���������ַ���
	VOID	ChangeDisplayLineStart(INT nLine, BOOL bSendEvent=TRUE);	// �޸Ĵ���һ�п�ʼ��ʾ(nLineΪ���ֵ)
	VOID	ChangeCursorPos(INT nPos);			// �޸������λ��(nPosΪ���ֵ)
	VOID	DelString(INT nPos, INT nEndPos);	// ɾ��ָ���ַ�
	INT		GetCharIndex(tagPoint pt);			// ��view����õ���ӽ��ַ������к�
	VOID	CopyToClipboard();					// ��ѡ�����ݿ��������� 
	VOID	PasteFromClipboard();				// �Ӽ����忽���ı�����ǰλ��
	LPCTSTR	GetText(INT nStart, INT& nLen);		// �õ��ı���ʵ������(ȥ������س�), nLen:[in]��Ҫȡ�����ȣ�[out]ʵ��ȡ������


	//-----------------------------------------------------------------------------
	// �ɱ༭����
	//-----------------------------------------------------------------------------
	BOOL		m_bReadOnly;		// �Ƿ�ֻ��
	DWORD		m_dwSelectColor;	// ѡ�ֱ�����ɫ
	DWORD		m_dwCursorColor;	// �����ɫ
	DWORD		m_dwCursorTimer;	// �����˸���
	INT			m_nLineHeight;		// �и�
    INT			m_nMaxTextCount;	// ��������ֽ���
};



}	// namespace vEngine {

