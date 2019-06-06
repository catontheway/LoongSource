#pragma once
#include ".\GameClientDefine.h"

enum EOnEventSendDropGUI
{
	EOnEventSendDropGUI_Item = EGUIE_User + 1030,		
};


/*
	class ComposeStatic
	brief ����ǿ��static��ֻ�����϶���ť�ͷţ������϶�
*/
class ComposeStatic:
	public GUIStatic
{
public:
	ComposeStatic(void);
	~ComposeStatic(void);

	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Render();
	virtual VOID Update();
	virtual VOID Destroy();
	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);
	virtual VOID OnEvent(tagGUIEvent* pEvent);

	//���ÿؼ�ͼƬ
	void SetPic(const tstring& strPicName);

protected:

	VOID DrawText();
	VOID DrawImg();

private:
	
};
