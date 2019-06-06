#pragma once
#include "ButtonEx.h"
class Item;
/** class ItemButton
	brief ��Ʒ��ʾ��ť
*/
class ItemButton :
	public ButtonEx
{
public:
	ItemButton();

	// ButtonEx
	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Render();
	virtual VOID Update();
	virtual VOID Destroy();
	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);
	virtual VOID OnEvent(tagGUIEvent* pEvent);
	
	// ������Ʒ��ʾ
	VOID RefreshItem(DWORD dwTypeID = GT_INVALID, INT nCount = 0, BYTE byQuality = 0);
	//��ȴʱ��
	void SetCDTime(float time) { m_fCDCount = time; }
	//��ת��
	void SetRestoreRatio(float time) { m_fRestoreRatio = time; }

	//����Ʒ��
	VOID SetQuality(BYTE byQuality);

protected:
	
	VOID DrawText();

	VOID DrawQuality();

	VOID SetQualityPic(tstring strPath);

protected:
	tagGUIImage*		m_pQuailityPic;

	VOID PlaySound(DWORD dwItemTypeID);
};

class GoodsButton : 
	public ItemButton
{
public:
	GoodsButton();

	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Render();
	virtual VOID Destroy();
	
	VOID SetShangjia(bool bShangjia);

protected:
	VOID DrawShangjia();
	
	VOID SetShangjiaPic();
protected:
	tagGUIImage*		m_pShangjiaPic;

};

class MallButton:
	public ItemButton
{
public:
	MallButton();

	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Render();
	virtual VOID Destroy();

	VOID SetHotMallItem(bool bHotItem);
	VOID SetNewMallItem(bool bNewItem);

protected:
	VOID DrawMallItemImg();

	VOID SetHotPic();
	VOID SetNewPic();

protected:
	tagGUIImage*		m_pHotPic;
	tagGUIImage*		m_pNewPic;

};


class ChestButton : public ItemButton
{
public:
	ChestButton();
	~ChestButton();
	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Destroy();
	virtual VOID Render();

	void EnableCursor(bool bCursor);
private:
	void DrawCursor();
	tagGUIImage *m_pCursorPic;
	bool m_bDrawCursor;
};