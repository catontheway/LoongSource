#pragma once

#include "..\WorldDefine\ItemDefine.h"

class Player;
class AvatarEquipNode;

/** \class   PlayerViewWindow
    \brief   ��ʾһ����ɫavatar������ʵʱ��װ
    \remarks �����2������Ϊroletl��roletr��Button�ӿؼ�,���ڲ�������������ת��ɫ����
*/
class PlayerViewWindow : public GUIStatic
{
public:
	PlayerViewWindow();
	virtual ~PlayerViewWindow();

	/** \GUIStatic
	*/
	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Destroy();
	virtual VOID Update();
	virtual VOID Render();
	virtual VOID RefreshViewCoord();
	virtual VOID AddChild(GUIWnd* pChild);
	virtual VOID SetInvisible(BOOL b);

	/** \��Ⱦ3D��ɫ����Ҫ�ⲿ��GameFrame��Render3D�е���
	*/
	VOID Render3D();
	/** \��ת��ɫ
	*/
	VOID RotateYaw( const FLOAT fAngle );
	/** \ʹ��pPlayer�Ľ�ɫ�������Ժ�װ����Ϣ����avtatar
	*/
	VOID UpdataAvatar( Player* pPlayer );
	/** \������װ��
	*/
	void SetEquip( const EAvatarElement eAvaEnt, const tagEquipDisplay& ed );
	/** \������ʾ����
	*/
	VOID UpdataDisplaySet( const tagDisplaySet& ds );
protected:
	VOID UpdataCamera();
protected:
	tagGUIImage*			m_pNewCanvas;     // RenderTarget
	AvatarEquipNode*		m_pAvatarNode;    // SceneNode
	NullSceneGraph*			m_pSG;            // ���ڱ�������������յ���Ϣ,��������Ⱦ
	CameraBase				m_Camera;
	FLOAT                   m_fAvatarYaw;

	GUIButton*              m_pBnRotaLeft;
	GUIButton*              m_pBnRotaRight;
};
