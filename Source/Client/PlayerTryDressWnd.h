#pragma once

#include "PlayerViewWindow.h"
#include "AvatarEquipElement.h"

class Player;
class AvatarEquipNode;
/**	class PlayerTryDressWnd
	brief �Դ�����
*/
class PlayerTryDressWnd : public PlayerViewWindow
{
public:
	PlayerTryDressWnd(void);
	virtual ~PlayerTryDressWnd(void);

	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Destroy();

	/** \ʹ��pPlayer�Ľ�ɫavtatar���Դ���װ��
	*/
	VOID UpdataAvatar( Player* pPlayer );
	/** \��ת��ɫ
	*/
	VOID RotateYaw( const FLOAT fAngle );
	/** \������б��ش��ڱ�����Դ�װ��
	*/
	VOID ReLoadTryEquip();

};