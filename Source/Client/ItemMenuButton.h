#pragma once
#include "..\vEngine\gui\gui_pushbutton.h"

/** \class ItemMenuButton
	\brief ֧����Ʒ�Ϸŵı����ֿ�ҳ��ť
*/
class ItemMenuButton :
	public GUIPushButton
{
public:
	ItemMenuButton(void);
	~ItemMenuButton(void);

	virtual VOID OnEvent(tagGUIEvent* pEvent);
};
