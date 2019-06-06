#include "StdAfx.h"
#include "PlayerTryDressWnd.h"
#include "LocalPlayer.h"
#include "AvatarEquipNode.h"

PlayerTryDressWnd::PlayerTryDressWnd( void )
{

}

PlayerTryDressWnd::~PlayerTryDressWnd( void )
{

}

BOOL PlayerTryDressWnd::Init( GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement )
{
	if( !PlayerViewWindow::Init(pSystem, pFather, pXmlElement) )
		return FALSE;

	return TRUE;
}

VOID PlayerTryDressWnd::Destroy()
{
	PlayerViewWindow::Destroy();
}

VOID PlayerTryDressWnd::UpdataAvatar( Player* pPlayer )
{
	ASSERT( P_VALID( m_pAvatarNode ) );

	if( !P_VALID( pPlayer ) || !P_VALID( pPlayer->GetSceneNode() ) )
		return;

	m_pSG->DetachDynamicNode( m_pAvatarNode );

	//��ȡ�������
	const tagAvatarAtt& att = pPlayer->GetAvatarAtt();

	//��ȡ�Դ���װ��
	tagAvatarEquip ep;
	for( int i = EAE_Start; i <= EAE_End; i++ )
	{
		ep.AvatarEquip[i] = m_pAvatarNode->GetEquip((EAvatarElement)i); 
	}

	// û�м��ع�����ȫ������
	if( NULL == m_pAvatarNode->GetResource() )
	{
		tagDisplaySet disSet;
		disSet.bFashionDisplay = true;
		m_pAvatarNode->SetDisplaySet( disSet );
		m_pAvatarNode->LoadAvatar( att, ep );
		if( EITE_Blade == m_pAvatarNode->GetRWeaponType() )
			m_pAvatarNode->GetSkeletonAniCtrl()->OpenTrack( _T("std2"), true );
		else
			m_pAvatarNode->GetSkeletonAniCtrl()->OpenTrack( _T("std1"), true );
	}
	//	�Ա����,���¼��ع���
	else if( m_pAvatarNode->GetAtt().bySex != att.bySex )				
	{
		SAFE_DEL(m_pAvatarNode);
		m_pAvatarNode = new AvatarEquipNode;
		tagDisplaySet disSet;
		disSet.bFashionDisplay = true;
		m_pAvatarNode->SetDisplaySet( disSet );
		m_pAvatarNode->LoadAvatar( att, ep );
		if( EITE_Blade == m_pAvatarNode->GetRWeaponType() )
			m_pAvatarNode->GetSkeletonAniCtrl()->OpenTrack( _T("std2"), true );
		else
			m_pAvatarNode->GetSkeletonAniCtrl()->OpenTrack( _T("std1"), true );
		return;
	}
	// ���½�ɫ�������Լ�װ��
	else
	{
		m_pAvatarNode->UpdateAllAtt( att );
		m_pAvatarNode->UpdateAllEquip( ep );
	}

	m_pSG->AttachDynamicNode( m_pAvatarNode );
	//UpdataCamera();
}

VOID PlayerTryDressWnd::RotateYaw( const FLOAT fAngle )
{
	if( !P_VALID( m_pAvatarNode ) )
		return;

	m_fAvatarYaw = fAngle;

	Transform tran;
	tran.LoadIdentity();
	tran.Rotate( m_fAvatarYaw, 0.0f, 0.0f );
	m_pAvatarNode->SetWorldMat( tran.GetMatrix() );
}

VOID PlayerTryDressWnd::ReLoadTryEquip()
{
	tagEquipDisplay ep;
	ep.dwTypeID = GT_INVALID;
	for( int i = EAE_Start; i <= EAE_End; i++ )
	{
		this->SetEquip( (EAvatarElement)i, ep );
	}
}