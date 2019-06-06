#pragma once
#include "npc.h"

/** \class Door
	\brief ��
*/
class Door : public NPC
{
public:
	Door(void);
	virtual ~Door(void);

	//--Role
	virtual void InitFSM();
	
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pGameEvent);
	virtual bool IsDead();

	//--NPC
	virtual void InitAttribute( tagNS_GetRemoteCreatureState* pCmd );

	//--
	
	bool GetDoorState() { return m_bState; }
protected:
	void OpenDoor(bool bPlayAction);
	void CloseDoor(bool bPlayAction);

protected:
	DWORD m_dwMapObjID;	//��¼��ͼ����ID
	bool m_bState;		//����

	DECL_RTTI(Door);
};
