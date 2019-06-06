#pragma once
#include ".\GameClientDefine.h"

class DragBtn :
	public GUIStatic
{
public:
	DragBtn(void);
	~DragBtn(void);
	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);
	virtual VOID OnEvent(tagGUIEvent* pEvent);					
	virtual VOID Render();
	
	//��ʹ��һ��ʵ��
	static DragBtn* NewInstance(GUISystem *pGUISys);
	static void DestroyDragBtn();

	//�ƶ��϶���ť
	VOID MoveToPos(INT x,INT y);

	//��ʼ�϶�
	VOID BeginDrage();

	//�����϶�
	VOID EndDrag();
	
	//�Ƿ�����Ϸ����
	bool PointInScene(tagPoint& pt);

	//����϶���ť��¼������
	void Clear(){ m_pSrcWnd = NULL; m_eDragType = EDT_Null; m_dwParam1 = m_dwParam2 = m_dwParam3 = m_dwParam4 = 0; }

private:	

	EDragType			m_eDragType;							//�϶�����
	GUIWnd*				m_pSrcWnd;								//ԭ�ؼ�
	DWORD				m_dwSrcWndID;							//ԭ�ؼ���ID

	DWORD				m_dwParam1;								//����1
	DWORD				m_dwParam2;								//����2
	DWORD				m_dwParam3;								//����3
	DWORD				m_dwParam4;								//����4

	int					m_nClickUp; //	MouseLeftKeyUp״̬�����ڿ����϶�����
									// 0:ת����ʾ���ձ�����;  
									// 1:�յ���һ��clickup�¼�;
									// 2:�յ��ڶ���clickup�¼�;
};
