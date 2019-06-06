#include "StdAfx.h"
#include "clientcamera.h"
#include "Role.h"

#include "MapMgr.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
ClientCamera::ClientCamera(void)
{
	m_bQuaking = false;
	m_fZFar = 25600.0f;
	m_fOccultDist = FLOAT_MAX;
	m_bFly = false;
	m_dwLastOccultTime = timeGetTime();
	m_bDecTransparent = FALSE;		
	m_fTransparent = 1.0f;
    m_HeightScaleRate = 1.0f;
}


CONST FLOAT CCamera_ResetSpeed = FLOAT_PI/4;//��ͷ�ظ��ٶȣ�����/�룩
CONST FLOAT CCamera_ZNear = 20;
CONST FLOAT MIN_DIST = 200.0f;	// ��������˵��������
CONST FLOAT MIN_EXP_DIST = 220.0f;	// �ֶ����������ʱ���˵��������

// �𶯶���
CONST DWORD QUAKE_TIME = 300; // ����ʱ��
CONST int QUAKE_FRM_NUM = 10; // ֡��
POINT QUAKE_FRMS[QUAKE_FRM_NUM] = // ��ͷƫ��
{
	{0,5},	{0,-5},	{0,5},	{0,-5},	{0,5},	{0,-5},	{0,4},	
	{0,-4},	{0,4},	{0,-4},
};


void ClientCamera::SetDefault(float zFar)
{
	SetPerspective((FLOAT)Device()->GetClientWidth(),
		(FLOAT)Device()->GetClientHeight(),_DegToRad(45),CCamera_ZNear,zFar);
	Set(Vector3(0,500,0),0.39793599f,-0.18762225f,0,660);
	m_fExpectDist = m_fDist;
}

void ClientCamera::SetFov(float deg)
{
	SetPerspective((FLOAT)Device()->GetClientWidth(),
		(FLOAT)Device()->GetClientHeight(),_DegToRad(deg),CCamera_ZNear,m_fZFar);
	m_fExpectDist = m_fDist;
}


void ClientCamera::Update(Role* pLookRole, NavMap* pNavMap)
{
	// m_pConsole->Watch(_T("CameraDist"), (DWORD)GetDist());

	// ������
	TObjRef<GUISystem> pGUI;
	if( m_pInput->GetRelativeZ()!=0 && pGUI->GetMousePoint()==pGUI->GetDesktop() )
	{
		float delta = -m_pInput->GetRelativeZ() / 2.0f;
		m_fExpectDist += delta;

		if( m_fExpectDist > 1800 )
			m_fExpectDist = 1800;

		if( m_fExpectDist < MIN_EXP_DIST )
			m_fExpectDist = MIN_EXP_DIST;

		// ���������������������������2����ʱ
		m_dwLastOccultTime = 0;
	}

	// ��������Ҽ�˲��
	if( m_pInput->IsMouseDownOnce(1) )
	{
		HWND hWnd = (HWND)TObjRef<VarContainer>()->GetDword(_T("HWND"));
		::SetCapture(hWnd);
		::GetCursorPos(&m_ptLast);
		::ShowCursor(FALSE);
	}

	// �ɿ�����Ҽ�˲��
	if( m_pInput->IsMouseUpOnce(1) )
	{
		::ReleaseCapture();
		::ShowCursor(TRUE);
	}

	//--��������Ҽ����϶�
	if( m_pInput->IsMouseDown(1) && m_pInput->IsMouseMove() )
	{
		float yaw = GetYaw() - (-m_pInput->GetRelativeX()*1.0f) * 0.6f * 0.006545f;
		SetYaw(yaw);

		float pitch = GetPitch() + (-m_pInput->GetRelativeY()*1.0f) * 0.006545f;
		if( pitch < -FLOAT_PI*0.45f )
			pitch = -FLOAT_PI*0.45f;
		if( pitch>_DegToRad(80) )
			pitch=_DegToRad(80);

		
		if( GetDist() > MIN_DIST || m_pInput->GetRelativeY() > 0 )
		{
			SetPitch(pitch);	// ����̫��������ͷ����
		}
	
		::SetCursorPos(m_ptLast.x, m_ptLast.y);

		if(m_pInput->GetRelativeX()!=0)
		{
			tagRotateCameraEvent event(_T("tagRotateCameraEvent"),NULL);
			m_pFrmMgr->SendEvent(&event);
		}
	}

	//--����
	if(m_bFly)
	{
		if(m_pInput->IsKeyDown(DIK_UP))
		{
			Vector3 newPos=GetLookat()+GetForward()*40.0f;
			SetLookAtPos(newPos);
		}
		if(m_pInput->IsKeyDown(DIK_DOWN))
		{
			Vector3 newPos=GetLookat()-GetForward()*40.0f;
			SetLookAtPos(newPos);
		}
		if(m_pInput->IsKeyDown(DIK_LEFT))
		{
			Vector3 newPos=GetLookat()+GetSide()*40.0f;
			SetLookAtPos(newPos);
		}
		if(m_pInput->IsKeyDown(DIK_RIGHT))
		{
			Vector3 newPos=GetLookat()-GetSide()*40.0f;
			SetLookAtPos(newPos);
		}

		CameraEuler::UpdateViewMatrix();
		return;
	}

	if( NULL == pLookRole )
		return;

	// ����m_vLookAt
	Vector3 nextLookAt = pLookRole->GetPos();
	float height = 130.0f * m_HeightScaleRate;

	nextLookAt.y += height;

	// ��ͷ��Ӱ��m_vLookAt
	UpdateQuake(nextLookAt);
	m_vLookat = nextLookAt;

	// m_vLookAt���ܵ���ˮ��
	float waterH=0.0f;
	if( pNavMap->GetCollider()->GetWaterHeight(m_vLookat, waterH) )
	{
		waterH += 10.0f;
		if( m_vLookat.y < waterH )
			m_vLookat.y = waterH;
	}

	// �����ڵ���ʵ���Ǽ���dist(from)
	Occult(pNavMap);
	CameraEuler::UpdateViewMatrix();	// Ԥ�ȸ��£�����m_vLookFrom

	// ���� dist ���� m_vLookAt
	FLOAT fUp = (GetDist() - MIN_DIST) / 10;
	if( fUp < 140 )	// ��������
		m_vLookat.y += fUp;
	else
		m_vLookat.y += 140;

	// �����¾���
//	Occult(pNavMap);
//	CameraEuler::UpdateViewMatrix();

	// m_vLookFrom���ܵ���ˮ��
	if( pNavMap->GetCollider()->GetWaterHeight(m_vFrom, waterH) )
	{
		waterH += 10.0f;
		if( m_vFrom.y < waterH )
			m_vFrom.y = waterH;
	}
	// m_vFrom���ܵ��ڵ���
	float fTrnHeight = pNavMap->GetCollider()->SafeGetGroundHeightSlerp(m_vFrom.x, m_vFrom.z);
	if (m_vFrom.y < fTrnHeight + 10)
	{
		m_vFrom.y = fTrnHeight + 10;
	}

	CameraBase::SetLookAt(m_vFrom,m_vLookat,m_vUp);
	//UpdateRoleTransparent();
}


void ClientCamera::PlayQuake()
{
	if(!m_bQuaking)
	{
		m_dwQuakeStartTime=Kernel::Inst()->GetAccumTimeDW();
		m_bQuaking=true;
	}
}


void ClientCamera::UpdateQuake(Vector3& nextLookAt)
{
	if(!m_bQuaking)
		return;

	DWORD time=Kernel::Inst()->GetAccumTimeDW()-m_dwQuakeStartTime;
	if(time<=QUAKE_TIME)
	{
		int frm=int(QUAKE_FRM_NUM*time/QUAKE_TIME);
		if(frm<0)
			frm=0;
		if(frm>=QUAKE_FRM_NUM)
			frm=QUAKE_FRM_NUM-1;
		nextLookAt.x+=QUAKE_FRMS[frm].x;
		nextLookAt.y+=QUAKE_FRMS[frm].y;
	}
	else
	{
		m_bQuaking=false;
	}
}


void ClientCamera::Occult(NavMap *pNavMap)
{
	if( !pNavMap )
		return;

	// �õ���ǰ����;���
	Vector3 dir = m_vFrom - m_vLookat;
	float fCurrentLen = Vec3Len(&dir);
	dir /= fCurrentLen;

	// �Ƿ���Ҫ������Զ
	if( fCurrentLen < (m_fExpectDist - 1) )// && (timeGetTime() - m_dwLastOccultTime > 2000) )	// 2���ʼ�ظ�
	{
		const float SpringSpeed = 1000.0f;//ÿ��ָ�����
		float deltaTime=Kernel::Inst()->GetDeltaTime();

		fCurrentLen += SpringSpeed * deltaTime;
		if( fCurrentLen > m_fExpectDist )
			fCurrentLen = m_fExpectDist;
	}

	if( fCurrentLen > m_fExpectDist )
		fCurrentLen = m_fExpectDist;

	// ��ʼ����赲
	// ��ͷ��Ŀ�������߲����䵽��������, ����ʹ��NavMap�Դ�����ײ��⺯��
	float f = 0;
	for(f=0; f<=fCurrentLen; f+=10.0f)
	{
		Vector3 current = m_vLookat + dir * f;
		float h = pNavMap->GetCollider()->SafeGetGroundHeightSlerp(current.x, current.z) + 30.0f;
		if( current.y < h )
		{
			m_dwLastOccultTime = timeGetTime();
			break;
		}
	}
	if( f > fCurrentLen )
		f = fCurrentLen;

	//--��ͷ��ˮ����ײ
	Vector3 intersectPos;
	if(pNavMap->GetCollider()->RayCollideWater(Ray(dir, m_vLookat, f),intersectPos))
	{
		intersectPos.y+=30.0f;//��ˮ���Ը�һ��
		Vector3 temp = intersectPos - m_vLookat;
		f = Vec3Len(&temp);
		m_dwLastOccultTime = timeGetTime();
	}

	// ��ͷ��Ŀ�������߲������ϰ���
	HugeQuadTreeSG* pHQT = (HugeQuadTreeSG*)MapMgr::Inst()->GetSceneGraph();
	TResult result = pHQT->RayCollide(Ray(dir, m_vLookat));
	if( result.first )
	{
		if( result.second - 80.0f < f )
		{
			f = result.second - 80.0f;
			m_dwLastOccultTime = timeGetTime();
		}
	}

	if( f < MIN_DIST )	// ������С����
	{
		f = MIN_DIST;
		m_bDecTransparent = TRUE;
	}
	else
	{
		m_bDecTransparent = FALSE;
	}

	SetDist(f);
}


void ClientCamera::UpdateRoleTransparent()
{
	if( !m_bDecTransparent )
	{
		if( m_fTransparent < 1.0f )
		{
			m_fTransparent += Kernel::Inst()->GetDeltaTime()*4.0f;
			if( m_fTransparent > 1.0f )
				m_fTransparent = 1.0f;
			RoleMgr::Inst()->GetLocalPlayer()->SetTransparent(m_fTransparent);
		}
	}
	else
	{
		if( m_fTransparent > 0.0f )
		{
			m_fTransparent -= Kernel::Inst()->GetDeltaTime()*4.0f;
			if( m_fTransparent < 0.0f )
				m_fTransparent = 0.0f;
			RoleMgr::Inst()->GetLocalPlayer()->SetTransparent(m_fTransparent);
		}
	}

}