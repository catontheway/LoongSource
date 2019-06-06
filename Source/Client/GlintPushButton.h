#pragma once

class GlintPushButton : public GUIPushButton
{

public:
	GlintPushButton();
	~GlintPushButton();

	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Update();
	virtual VOID Destroy();
	virtual VOID Render();

	bool IsGlintNow(){ return m_bShowGlint; }
	
	void SetGlintStart(bool bGlint )
	{ 
		m_bShowGlint = bGlint;
		if( m_bShowGlint )
			m_dwLastGlint = timeGetTime();
	}

	void SetGlintPic( LPCTSTR strPath );

	void SetGlintTextColor( DWORD dwColor ){ m_dwGlintColor = dwColor; }

protected:
	void DrawGlint();

protected:
	bool m_bShowGlint;							//�Ƿ���˸
	bool m_bShowInterval;
	tstring			m_strGlintPic;
	tagGUIImage*	m_pGlintPic;				
	DWORD			m_dwLastGlint;				//�ϴμ�ʱ��ʱ��
	DWORD			m_dwGlintColor;				//������˸����ɫ
	DWORD			m_dwOldTextColor;			//ԭ������ɫ
};