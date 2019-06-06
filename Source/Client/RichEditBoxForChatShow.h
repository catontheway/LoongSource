#pragma once
#define GUI_DEFAULT_ROW_SPACING 20				
enum REBForChatShow
{
	EXUIE_MouseDoubleClick = EGUIE_User + 1030,		//�������˫��richeditbox�¼������ڻ�ȡ�ؼ���
	EXUIE_MouseRClick,
};
class RichEditBoxForChatShow :
	public GUIRichEditBox
{
public:
	RichEditBoxForChatShow(void);
	~RichEditBoxForChatShow(void);
	
	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);

	VOID BeNoFlex()						{ m_bFlex = FALSE; }
	VOID BeFlex()						{ m_bFlex = TRUE; }
	BOOL IsFlex()						{ return m_bFlex; }
	const INT& GetTextSize()			{return m_nTextNormalHeight;}			//�õ��ֵĸ߶�
	const INT& GetRowSpacing()			{return m_nRowSpacing;}					//�õ�ÿ�е��и�

	void ZoomWnd(INT nOffset);

protected:
	BOOL							m_bFlex;		//�Ƿ������
	BYTE							m_byFlexStatus;	//��������״̬,-1��ʾ��,0��ʾ��괦���Ϸ���Ե,1��ʾ�����·���Ե

	void AdptiveCursor(bool bDrag, BOOL bLink);
};
