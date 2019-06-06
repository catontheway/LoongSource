#pragma once
#include "buttonex.h"
class ShortcutBarFrame;
/**	brief: ��ݰ�ť
*  �¼�˵��		EGUISCB_DropCheck	EGUISCB_Drop����ʱ����
*						   dwParam1    ��������
*						   dwParam2    �϶�����1
*						   dwParam3    �϶�����2
*						   dwParam4    ���λ��
*
*	m_BtnData��Ա�������壺
*					 	m_eType;		��ť����
						m_dwParam1;		��Ӧ����1(skill:id,	item:��Ŀ+λ��)
						m_dwParam2;		��Ӧ����2(skill:NULL,	item:typeid)
						m_n64Param3;	skill:NULL, item: 64id)	
*
*	��Ʒ�ڿ�����еıȽ����⣬��Ҫ��¼�����ݣ�
*		1��m_nItemNum������ʾItem��������
*/
class ShortcutButton :
	public ButtonEx
{
public:
	ShortcutButton(void);
	~ShortcutButton(void);

	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Render();
	virtual VOID Update();
	virtual VOID Destroy();
	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);
	virtual VOID SendEvent(tagGUIEvent* pEvent);
	virtual VOID OnEvent(tagGUIEvent* pEvent); // �����¼�

	//���ÿ�ݰ�ť�Ŀ�ݼ�ֵ
	void SetKeyBar(const TCHAR*	strKey)	{ m_strKeyBar = strKey;	 }
	tstring& GetKeyBar() { return m_strKeyBar; }

	//���ÿ�ݰ�ťDIK��ֵ
	void SetDIKBar(DWORD key) { m_dwKeyID = key; }
	DWORD GetDIKBar()const { return m_dwKeyID; }

	//���ÿ�ݰ�ť����Ʒ����
	void SetItemNum(const TCHAR* strNum) { m_strItemNum = strNum;  m_nItemNum = _ttoi(strNum); }
	
	//��Ʒ����
	void SetItemNum(int itemNum);
	int GetItemNum()const { return m_nItemNum; }	

	//��ȴʱ��
	void SetCDTime(float time) { m_fCDCount = time; }

	//��ת��
	void SetRestoreRatio(float time) { m_fRestoreRatio = time; }

	//��������ݰ�ťʱ��˸ͼ��ʱ��
	BOOL BeginPressBtn();
	void EndPressBtn();
			
	//--����/��ÿ�ݰ�ť������
	void SetCanUse(bool bVal) { m_bCanUse = bVal; }
	bool GetCanUse() { return m_bCanUse; }

	//���ò�����
	void SetInvalite(bool bVal) { m_bIsInvalite = bVal; }

	//���尴���Ŀ�ݼ�
	void SetCacheImage(BOOL bVal) { m_bCacheImg = bVal; }

	//���ÿ������Ԫ�����ϽǵĿ�ݼ���ʾ�ַ�����Ϊ��Ŀ��ɫ��
	void SetKeyBarEnable(BOOL bVal){ m_bKeyEnable = bVal; }

    //������Ʒ���͵�����
    void SetItemData( DWORD dwParam1, DWORD dwParam2);	

protected:

	//����϶���־
	void ClearDropFlag()	{ m_nDragFlag = 0; }	

	//���ƿ�ݰ�ť�ϵ�����
	void DrawBarText();	

	//���Ƶ���������ť��ʱ��ʾ��˸��ͼƬ
	void DrawPressFrame();


private:

	tstring				m_strKeyBar;				//��ݰ�ť������ʾ�Ŀ�ݼ�ֵ
	tstring				m_strItemNum;				//��ݰ�ť������ʾ����Ʒ����
	int					m_nItemNum;					//��Ʒ����
	BOOL				m_bIsInvalite;					

	tagGUIImage*		m_pBtnInvalitePic;			//������ʱ��һ��͸��ͼƬ
	tagGUIImage*		m_pBtnGlintPic[4];			//����ʱ,����������ͼ
	tagGUIImage*		m_pBtnCachePic;				//���弼��ʱ���Ƶ���ͼ
	int					m_curFrame;
	float				m_frameTime;
	BOOL				m_bCacheImg;				//�Ƿ���ƻ��弼�ܵ���ͼ
	BOOL				m_bKeyEnable;				//�Ƿ�����Ŀ�����ʾ
	DWORD				m_dwKeyID;
	

	int					m_nDragFlag;				// ��¼�Ϸ�״̬ 0: ����,1: ��drag,δ��drop
	TObjRef<GameFrameMgr>		m_pFrameMgr;		//frame������
	ShortcutBarFrame*	m_pFrame;

};

class SpecialSkillButton
	: public ShortcutButton
{
public:
	SpecialSkillButton(void);
	~SpecialSkillButton(void);

	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Destroy();
	virtual VOID Render();
	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);
	virtual VOID OnEvent(tagGUIEvent* pEvent); 
};
