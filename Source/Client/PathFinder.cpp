#include "StdAfx.h"
#include "PathFinder.h"
#include "LocalPlayer.h"

PathFinder::PathFinder(void)
{}

PathFinder::~PathFinder(void)
{}

bool PathFinder::Init(const Vector3& startPos,const Vector3& endPos,NavMap* pNav,LocalPlayer* pRole,bool bOnLand)
{
	//--��鵱ǰλ���Ƿ����Ѱ·
	if(!pRole->IsSwimState()
		&&!pRole->IsOnWaterState()
		&&!pNav->GetCollider()->IfCanFindPath(pRole->GetPos(),pRole->GetRoleSize()))
	{
		return false;
	}

	//--�����յ�
	m_endPos=endPos;

	m_bOnLand=bOnLand;

	m_patchPath.clear();
	m_tilePath.clear();

	//--���ڵؿ�Ѱ·
	bool bOnRoad=pNav->GetPlayerNavMap()->IfCanGo(startPos.x,startPos.z,true);
	if(bOnRoad)//��·����
	{
		m_step=EStep_OnRoad;

		m_resultByPatch=pNav->GetPlayerNavMap()->FindPathByPatch(startPos,endPos,true,m_patchPath);
		if(m_resultByPatch==PlayerNavMap::ER_Failed)
			return false;
		if(m_patchPath.empty())
			return false;

	}
	else//�Զ�����
	{
		m_step=EStep_EnterRoad;

		m_resultByPatch=pNav->GetPlayerNavMap()->FindPathByPatch(startPos,endPos,false,m_patchPath);
		if(m_resultByPatch==PlayerNavMap::ER_Failed)
			return false;
		if(m_patchPath.empty())
			return false;
	}
	
	return true;
}

bool PathFinder::GetPathPoint( NavMap* pNav,LocalPlayer* pRole,Vector3& out )
{
	if(m_tilePath.empty())//����·��������
	{
		switch(m_step)
		{
		case EStep_EnterRoad:
			{
				if(!PreparePath_EnterRoad(pNav,pRole))
					return false;
			}
			break;
		case EStep_OnRoad:
			{
				if(!PreparePath_OnRoad(pNav,pRole))
					return false;
			}
			break;
		case EStep_LeaveRoad:
			{
				if(!PreparePath_LeaveRoad(pNav,pRole))
					return false;
			}
			break;
		}
	}

	out=m_tilePath.front();
	m_tilePath.pop_front();
	return true;
}

bool PathFinder::PreparePath_EnterRoad(NavMap* pNav,LocalPlayer* pRole)
{
	PlayerNavMap::EResult result;

	if(m_patchPath.size()==0)//size==0
	{
		return false;
	}
	else if(m_patchPath.size()==1)//size==1
	{
		Point tile=m_patchPath.front();
		m_patchPath.pop_front();

		result=pNav->GetPlayerNavMap()->FindPathByTile(pRole->GetPos(),tile,pRole->GetRoleSize(),false,true,false,m_bOnLand,pNav->GetCollider(),m_tilePath);
	}
	else//size>1
	{
		m_patchPath.pop_front();
		Point tile=m_patchPath.front();

		result=pNav->GetPlayerNavMap()->FindPathByTile(pRole->GetPos(),tile,pRole->GetRoleSize(),false,true,true,m_bOnLand,pNav->GetCollider(),m_tilePath);
	}

	switch(result)
	{
	//--�����յ�--------------------------------
	case PlayerNavMap::ER_Arrive:		
		{
			if(m_tilePath.empty())
				return false;
		}
		break;
	//--�����·����----------------------------
	case PlayerNavMap::ER_EnterRoad:	
		{
			if(m_tilePath.empty())
				return false;

			//--�ڵ�·���򰴵ؿ�Ѱ·
			m_resultByPatch=pNav->GetPlayerNavMap()->FindPathByPatch(m_tilePath.back(),m_endPos,true,m_patchPath);
			if(m_resultByPatch==PlayerNavMap::ER_Failed)
				return false;
			if(m_patchPath.empty())
				return false;

			m_step=EStep_OnRoad;
		}
		break;
	//--ʧ��-------------------------------------
	case PlayerNavMap::ER_Failed:		
	//--���������-------------------------------
	case PlayerNavMap::ER_GetNearPos:
	//--����-------------------------------------
	default:
		return false;
	}

	return true;
}

bool PathFinder::PreparePath_OnRoad(NavMap* pNav,LocalPlayer* pRole)
{
	PlayerNavMap::EResult result;

	if(m_patchPath.size()==0)//size==0
	{
		if(m_resultByPatch==PlayerNavMap::ER_Arrive)//�ѵ����յ�
		{
			return false;
		}
		else//δ�����յ�
		{
			m_step=EStep_LeaveRoad;

			m_resultByPatch=pNav->GetPlayerNavMap()->FindPathByPatch(pRole->GetPos(),m_endPos,false,m_patchPath);
			if(m_resultByPatch==PlayerNavMap::ER_Failed)
				return false;
			if(m_patchPath.empty())
				return false;

			return PreparePath_LeaveRoad(pNav,pRole);
		}
	}
	else if(m_patchPath.size()==1)//size==1
	{
		Point tile=m_patchPath.front();
		m_patchPath.pop_front();

		result=pNav->GetPlayerNavMap()->FindPathByTile(pRole->GetPos(),tile,pRole->GetRoleSize(),true,false,false,m_bOnLand,pNav->GetCollider(),m_tilePath);
	}
	else//size>1
	{
		m_patchPath.pop_front();
		Point tile=m_patchPath.front();

		result=pNav->GetPlayerNavMap()->FindPathByTile(pRole->GetPos(),tile,pRole->GetRoleSize(),true,false,true,m_bOnLand,pNav->GetCollider(),m_tilePath);
	}

	switch(result)
	{
	case PlayerNavMap::ER_Arrive://�����յ�	
		{
			if(m_tilePath.empty())
				return false;
		}
		break;
	case PlayerNavMap::ER_Failed://ʧ��	
	case PlayerNavMap::ER_GetNearPos://���������
	default://����
		return false;
	}

	return true;
}

bool PathFinder::PreparePath_LeaveRoad(NavMap* pNav,LocalPlayer* pRole)
{
	PlayerNavMap::EResult result;

	if(m_patchPath.size()==0)//size==0
	{
		return false;
	}
	else if(m_patchPath.size()==1)//size==1
	{
		Point tile=m_patchPath.front();
		m_patchPath.pop_front();

		result=pNav->GetPlayerNavMap()->FindPathByTile(pRole->GetPos(),tile,pRole->GetRoleSize(),false,false,false,m_bOnLand,pNav->GetCollider(),m_tilePath);
	}
	else//size>1
	{
		m_patchPath.pop_front();
		Point tile=m_patchPath.front();

		result=pNav->GetPlayerNavMap()->FindPathByTile(pRole->GetPos(),tile,pRole->GetRoleSize(),false,false,true,m_bOnLand,pNav->GetCollider(),m_tilePath);
	}

	switch(result)
	{
	case PlayerNavMap::ER_Arrive://�����յ�	
		{
			if(m_tilePath.empty())
				return false;
		}
		break;
	case PlayerNavMap::ER_Failed://ʧ��	
	case PlayerNavMap::ER_GetNearPos://���������
	default://����
		return false;
	}

	return true;
}