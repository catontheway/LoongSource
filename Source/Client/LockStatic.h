#pragma once
#include "..\vEngine\gui\gui_static.h"

/** \class LockStatic
	\brief ���׽��������ؼ��������������׿��еĵ���¼��������赲��꾭���¼�
*/
class LockStatic :
	public GUIStatic
{
public:
	LockStatic(void);
	~LockStatic(void);

	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);

private:

	GUIWnd*			m_pLastWnd;//��һ������Ŀؼ�
};
