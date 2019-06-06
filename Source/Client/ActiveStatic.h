#pragma once

#include "ComposeStatic.h"

/** \class   ActiveStatic
    \brief   ActiveStatic���̳�GUIStatic
    \remarks �ṩ���๦�ܽӿ�
*/
class ActiveStatic : public GUIStatic
{
public:
	ActiveStatic();
	virtual ~ActiveStatic();
	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);
	virtual VOID OnEvent(tagGUIEvent* pEvent);
	virtual VOID Update();

	virtual BOOL SetPic( tstring strFilename);

	// ʹ�ؼ���˸��ͨ������pic color��alphaͨ��ʵ�֣�
	virtual VOID Glint( const BOOL bGlint, const BYTE byGlintEndAlpha = 0 );
	virtual BOOL IsGlint() { return m_bGlint; }
	virtual VOID SetGlintTime( DWORD dwCycleTime ) { m_dwGlintCycleTime = dwCycleTime; }
	// �û�
	virtual VOID SetColorGray( const BOOL bGray );
protected:
	BOOL       m_bHaveMouseKeyDown;
	DWORD      m_dwInitPicColor;
	DWORD      m_dwGlintStartTime;
	DWORD      m_dwGlintCycleTime;
	DWORD      m_dwLastUpdataTime;
	BOOL       m_bGlint;
	BYTE       m_byGlintEndAlpha;
	BYTE       GetAlphaARGB( DWORD dwColor ) { return BYTE( dwColor>>24 ); }
	VOID       SetAlphaARGB( DWORD& dwColor, BYTE nAlpha ) { dwColor = ( nAlpha << 24 ) | ( dwColor & 0x00ffffff ); }
};
