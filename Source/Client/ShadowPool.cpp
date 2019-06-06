#include "StdAfx.h"
#include "shadowpool.h"
#include "SGDynamicProjector.h"
#include "RoleMgr.h"
#include "Role.h"
#include "NPC.h"
#include "Player.h"
#include "LocalPlayer.h"
#include "MapMgr.h"

int ShadowPool::OP_MAX_SHADOW = 10;
int ShadowPool::OP_SHADOW_SIZE = 128;
float ShadowPool::OP_MAX_DIST = 64.0f * TILE_SCALE;

ShadowPool::ShadowPool(void)
{
}

ShadowPool::~ShadowPool(void)
{
	Destroy();
}

void ShadowPool::CreateData()
{
	// �Ѿ���ȫ������������Դ
	if(m_shadows.size()==OP_MAX_SHADOW)
		return;

	// ����ʣ���(�����һ�λ��ߵڶ��˽�����Ϸ�����)
	for(int i=m_shadows.size();i<OP_MAX_SHADOW;i++)
	{
		SGDynamicProjector *pShadow=new SGDynamicProjector;
		pShadow->CreateData(OP_SHADOW_SIZE,OP_SHADOW_SIZE);
		m_shadows.push_back(pShadow);
	}
}

void ShadowPool::Destroy()
{
	for(vector<SGDynamicProjector*>::iterator iter=m_shadows.begin();
		iter!=m_shadows.end();iter++)
	{
		SGDynamicProjector *pShadow=(*iter);
		delete pShadow;
	}
	m_shadows.clear();
}


class CmpRoles
{
public:
	Vector3 refPos;
	bool operator()( Role *pRole1,Role *pRole2 )
	{
		const float dist1 = Vec3DistSq( refPos, pRole1->GetPos() );
		const float dist2 = Vec3DistSq( refPos, pRole2->GetPos() );
		return dist1 < dist2;
	}
};

void ShadowPool::AssignToRoles()
{
	if( 0 == m_shadows.size() )
		return;

	// �õ���׶
	CameraBase* pCam = MapMgr::Inst()->GetSceneGraph()->GetCamera();
	if( NULL == pCam )
		return;
	Frustum frustum;
	frustum.Extract( pCam->GetView(), pCam->GetPrj() );

	Role* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
	if( !P_VALID( pLocalPlayer ) || !P_VALID( pLocalPlayer->GetSceneNode() ) )
		return;

	// �ѿ��԰���Ӱ�Ľ�ɫ�����б�
	list<Role*> listRoles;
	pLocalPlayer->DetachShadow();
	if( CanAttach( pLocalPlayer, frustum ) )
		listRoles.push_back( pLocalPlayer );

	const map<DWORD,Role*>& mapRoles = RoleMgr::Inst()->GetRoleMap();
	for( map<DWORD,Role*>::const_iterator cItr = mapRoles.begin(); cItr != mapRoles.end(); cItr++ )
	{
		Role* pRole = cItr->second;
		if( !P_VALID( pRole ) )
			continue;

		// ȡ����
		pRole->DetachShadow();

		if( !CanAttach( pRole, frustum ) )
			continue;

		listRoles.push_back( cItr->second );
	}

	// ����
	CmpRoles CmpFuc;
	CmpFuc.refPos = pLocalPlayer->GetPos();
	listRoles.sort( CmpFuc );

	// ����ɫ����Ӱ
	int c = 0;
	for( list<Role*>::iterator iter = listRoles.begin(); iter != listRoles.end(); iter++ )
	{
		if( c >= OP_MAX_SHADOW )
			break;
		Role *pRole=(*iter);
		pRole->AttachShadow( m_shadows[c++] );
	}
}

bool ShadowPool::CanAttach( Role* pRole, const Frustum& frustum )
{
	NavMap* pNav = MapMgr::Inst()->GetNavMap();
	NavCollider* pCollider = pNav->GetCollider();
	if( NULL == pNav || NULL == pCollider )
		return false;

	// ��ɫ������
	if( pRole->IsHide() )
		return false;

	// ��ɫ��͸��
	if( pRole->GetTransparent() < 1.0f )
		return false;

	// ��ɫ��ˮ�������Ӱ�����кܶ����⣩
	float fWaterHeight = 0.0f;
	if( pCollider->GetWaterHeight( pRole->GetPos(), fWaterHeight ) && pRole->GetPos().y <= fWaterHeight )
		return false;

	// ��Դû�м�����ɣ�������Ӱ
	SceneNode* pSGNode = pRole->GetSceneNode();
	if( !P_VALID( pSGNode ) || !pSGNode->IsLoaded() )
		return false;

	// �������ˣ�������Ӱ
	if( pRole->IS_KIND_OF(Player) )
	{
		Player* pPlayer = (Player*)pRole;
		if( pPlayer->GetMount() != NULL )
			return false;
	}

	// ��׶֮��
	if( !pSGNode->IsInFrustum( &frustum ) )
		return false;

	// ������������
	const float fDistToLPSq = Vec3DistSq( RoleMgr::Inst()->GetLocalPlayer()->GetPos(), pRole->GetPos() );
	if( fDistToLPSq > OP_MAX_DIST * OP_MAX_DIST )
		return false;

	return true;
}







