#pragma once

class ScrollRichEditBox :
	public GUIRichEditBox
{
public:
	ScrollRichEditBox(void);
	~ScrollRichEditBox(void);
	
	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Render();
	virtual VOID Update();

	//�����ı�
	BOOL PushScrollText(LPCTSTR szText, float fTime = 60.0f);

	//�����ܼ�����Ļ�Ϲ����೤��ʱ��
	void SetScrollTime(float fTime) { m_fSclTimeCount = fTime; }
	const float& GetScrollTime()const { return m_fSclTimeCount; }

	//��������ƿ�ʼ��λ��
	void SetStartPos(LONG posX, LONG posY) { m_lnSlcFatherWndX = posX; m_lnSlcFatherWndY = posY;}
	bool GetContinueScrText() { return m_bNextText; }
	void SetContinueScrText(bool bVal = false) { m_bNextText = bVal; } 

	void SetClient(const tagPoint& ptClient)	{ m_ptClient = ptClient; }

	void SetWidth(const LONG& width) { m_ptSize.x = width; m_rcText.right = width;}

private:
	float						m_fSclSpeed;				// �������ʾλ���ٶ�
	float						m_fSclTimeStart;			// ��ʼ������ʱ��,2���ʼ����
	float						m_fSclTimeCount;			// ��������ʱ�䣬�ܼƹ����೤��ʱ����ʧ
	LONG						m_lnSlcFatherWndX;			// ����Ƹ��ؼ������Ҷ�����
	LONG						m_lnSlcFatherWndY;			
	bool						m_bNextText;				// ��ǰ����Ƶ���Ϣ�Ƿ��Ѿ����
};
