#pragma once

#include "buttonex.h"
/** class SkillButton
	brief ������ʾ��ť
*/
class SkillButton :
	public ButtonEx
{
public:
	SkillButton(void);
	~SkillButton(void);

	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Render();
	virtual VOID Update();
	virtual VOID Destroy();
	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);
	virtual VOID SendEvent(tagGUIEvent* pEvent);
	virtual VOID OnEvent(tagGUIEvent* pEvent); // �����¼�
	// ˢ�¼��ܰ�ť
	void ReflashSkill(DWORD dwID);

	//���ԣ���ȴʱ��
	void SetCDTime(float time) { m_fCDCount = time; }

	//���ԣ���ת��
	void SetRestoreRatio(float time) { m_fRestoreRatio = time; }

private:

private:

	tagGUIImage*				m_pBtnPic;					//����ʱ���ӵ�һ��͸��ͼƬ


	TObjRef<GameFrameMgr>		m_pFrameMgr;		//frame������

};
