//-----------------------------------------------------------------------------
//!\file gui_editbox.h
//!\author Lyp
//!
//!\date 2004-12-08
//! last 2007-12-31
//!
//!\brief ����ϵͳ edit box �ؼ�
//-----------------------------------------------------------------------------
#pragma once


namespace vEngine {
//-----------------------------------------------------------------------------
// ����ϵͳ edit box �ؼ�
//-----------------------------------------------------------------------------
class VENGINE_API GUIEditBox : public GUIStatic
{
public:
	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Update();
	virtual VOID Render();
	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);

	virtual GUIWnd* OnActive();	
	virtual VOID OnDeactive(GUIWnd* pWnd);

	virtual BOOL Resize(tagPoint& ptSize); // �ı�ؼ���С,�����µĴ�С

	LPCTSTR	GetText() { return m_strText.c_str(); }
	VOID	SetText(LPCTSTR szText);	//ֱ�������ı�������
	VOID	Deselect() { m_nSelStart = m_nSelEnd = m_nCursorPos; SetRefresh(); }	// ȡ��ѡ��

	VOID	SetMaxValue(INT nValue) { m_nMaxNumberValue = nValue; }
	VOID	SetMaxTextCount(INT nValue) { m_nMaxTextCount = nValue; }

	GUIEditBox();

protected:
	bool	m_bShowCursor;					// ��ǰ�Ƿ���ʾ���(�������˸��)
	INT		m_nSelStart;					// ѡ��ʼ��
	INT		m_nSelEnd;						// ѡ�������
	INT		m_nCursorPos;					// ���λ��
	INT		m_nDispStart;					// ��ʾ�ĵ�һ���ַ�����ʵ�ʵĵڼ����ַ�
	INT		m_nDispEnd;						// ��ʾ�����һ���ַ�����ʵ�ʵĵڼ����ַ�
	INT		m_nMaxNumberValue;				// �ܹ�����������ֵ��ֻ�����ֵ��
	
	std::vector<INT> m_vecCharSize;			// ÿ���ֵĿ��

	VOID	CalCharWidth();					// ����ַ������޸ģ���Ҫ���ô˺�������ÿ���ַ����
	VOID	ChangeDisplayStart(INT n);		// �޸Ĵ��Ǹ��ַ���ʼ��ʾ(nΪ���ֵ���ڲ��ᴦ��˫�ֽ�)

	VOID	DelSelected();					// ɾ��ѡ����ַ�
	INT		GetCharIndex(INT nViewX);		// ��view����õ���ӽ��ַ������к�
	VOID	CopyToClipboard();				// ��ѡ�����ݿ��������� 
	VOID	PasteFromClipboard();			// �Ӽ����忽���ı�����ǰλ��

	//-----------------------------------------------------------------------------
	// �ɱ༭����
	//-----------------------------------------------------------------------------
	BOOL		m_bReadOnly;		// �Ƿ�ֻ��
	BOOL		m_bPassword;		// �Ƿ������
	BOOL		m_bNumerical;		// �Ƿ����ֿ�
	DWORD		m_dwCursorColor;	// �����ɫ
	DWORD		m_dwCursorTimer;	// �����˸���
	INT			m_nMaxTextCount;	// ��������ֽ���
};



}	// namespace vEngine {

