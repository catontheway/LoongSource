#include "StdAfx.h"
#include ".\Carrier.h"
#include "MapView.h"
#include "ClientApp.h"

Carrier::Carrier(void)
{
	m_pSGNode=NULL;
	m_pNavNode=NULL;
}

Carrier::~Carrier(void)
{
	m_pSGNode=NULL;//�Լ���delete,������SceneGraphȡdelete
}

void Carrier::Update(MapView *pMap,NavMap *pNav)
{
	if(m_pNavNode==NULL
		||m_pSGNode==NULL)
		return;

	Vector3 nowPos;
	m_pNavNode->GetPos(Kernel::Inst()->GetAccumTime(),nowPos);
	m_space.SetPos(nowPos);
	
	if(m_pSGNode)
		m_pSGNode->SetWorldMat(m_space.GetMatrix());
}

bool Carrier::Init( const tagMapCarrier& carrier,const vector<tagMapWayPoint>& wayPoints,NavMap *pNav )
{
	m_pNavNode=pNav->GetCollider()->FindCarrier(carrier.dwObjID);
	if(m_pNavNode==NULL)
		return false;

	//������ת
	m_space.SetYaw(carrier.fYaw);

	//����SceneNode
	m_pSGNode=new EntityNode;
	m_pSGNode->LoadRes(carrier.szModelPath);

	//��ӵ�����
	for(size_t i=0;i<wayPoints.size();++i)
	{
		const tagMapWayPoint& wayPoint=wayPoints[i];

		if(_tcscmp(carrier.szWayPoint,wayPoint.szName)==0)
		{
			m_pNavNode->AddWayPoint(wayPoint.vPos,2.0f);
		}
	}

	//�����ٶ�
	m_pNavNode->SetSpeed(500.0f);

	//��ʼ����
	m_pNavNode->Run(Kernel::Inst()->GetAccumTime(),0.0f);

	return true;
}

void Carrier::AttachToSceneGraph( SceneGraph *pSG )
{
	pSG->AttachDynamicNode(m_pSGNode);
}