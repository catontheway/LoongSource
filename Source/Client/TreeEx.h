#pragma once
/**
*class TreeEx
*brief �Զ���tree�ؼ�
*author hyu
*remark
*base GUITree
*/
class TreeEx :
	public GUITree
{
public:
	TreeEx(void);
	~TreeEx(void);

	//������Ŀ�ı���ɫ
	BOOL SetItemColor(DWORD dwID, DWORD dwColor);
	//����showtext
	BOOL SetShowText(DWORD dwID, LPCTSTR szShowText);
};
