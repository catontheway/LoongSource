#pragma once

class RoleFloatWnd:
	public GUIStatic
{
public:
	RoleFloatWnd(void);
	//--
	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Destroy();
	virtual VOID Render();
	
	//�����ַ����Ŀ��
	tagPoint ChageFontSize(const TCHAR* szInfo, INT nWidth, INT nHeight, INT nWeight=0);

};