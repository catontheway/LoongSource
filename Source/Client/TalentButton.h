#pragma once

/** \class TalentButton
	\brief ���ʰ�ť
*/
class TalentButton :
	public GUIStatic
{
public:
	TalentButton();
	~TalentButton();

	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);
	virtual VOID Render();
	virtual VOID Destroy();

	// ��ֵ����ͼ��
	VOID SetIcon(const tstring str);
	// ��������ͼ���Alphaͨ��
	VOID SetAlphaTex(const tstring str);

private:
	tagGUIImage*		m_pIcon;				// ���ʵ�ͼ��
	tagGUIImage*		m_pAlphaTex;			// ���ʵ�ͼ���Alphaͨ��
};