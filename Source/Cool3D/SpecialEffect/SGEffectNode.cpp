#include "StdAfx.h"
#include ".\sgeffectnode.h"
#include "EffectBase.h"
#include "EffectPathCtrl.h"
#include "..\Math\CameraBase.h"
#include "..\SceneGraph\SceneGraph.h"
#include "..\Math\Transform.h"
#include "..\Math\Frustum.h"
#include "..\ResSys\ResSpecialEffect.h"
#include "..\ResSys\ResMgr.h"
#include "EffectTarget.h"
#include "EffectParticleEmitter.h"

namespace Cool3D
{
	IMP_RTTI_DYNC(SGEffectNode,SceneNode);

	bool SortEffByLOD(const EffectBase* pEff1,const EffectBase* pEff2)
	{
		return pEff1->GetLOD() < pEff2->GetLOD();
	}

	int SGEffectNode::sUserLod = 0;
	class SGEffectNode::Member
	{
	public:
		vector<tagEffectRenderNode>		m_renderNodes;	//effect array������������RenderNode,Ϊ�˷������
		vector<EffectBase*>				m_effectArray;
		map<tstring,EffectPathCtrl*>	m_pathArray;	//·������

		Matrix4	m_toWorld;
		Vector3 m_boxCenter;

		vector<EffectMsg::tagState>	m_frmMsgArray;
		float			m_timeScale;
		float			m_runTime;//�����˶���ʱ��(�����time scale)
		bool			m_bPlaying;//�Ƿ����ڲ���
		bool			m_bUserUpdate;
		bool            m_bUserPlay;//�Ƿ��ֶ�����

		bool			m_bBuild;
		ResSpecialEffect* m_pRes;//��Դ
		bool			m_bUserWorldMat;//�Ƿ����ⲿָ��WorldMat������sfx�ļ��е�

		int				m_lod;
		Vector3         m_vTargetPathStart;     // ����TargetPath���ݣ������첽��ȡ��ʱ��������Ч
		Vector3         m_vTargetPathTarget;

		const tstring   m_strCurVersion;        // ��ǰ�汾
		AABBox			m_boxLocal;				// �ֲ��ռ�İ�Χ��
		AABBox          m_boxWorld;             // ����ռ�İ�Χ��
	public:
		Member() : m_strCurVersion( _T("v1") )
		{
			D3DXMatrixIdentity(&m_toWorld);
			m_runTime=0;
			m_timeScale=1.0f;
			m_bPlaying=false;
			m_bUserUpdate=false;
			m_boxCenter=MathConst::Zero3;
			m_lod=0;
			m_bBuild=false;
			m_pRes=NULL;
			m_bUserWorldMat=false;
			m_bUserPlay=false;

			m_vTargetPathStart = MathConst::Zero3;
			m_vTargetPathTarget = MathConst::Zero3;

			m_boxLocal=MathConst::SmallBox;
			m_boxWorld=MathConst::SmallBox;
		}
		~Member()	
		{
			size_t i;
			for(i=0;i<m_effectArray.size();i++)
			{
				EffectBase *pEft=m_effectArray[i];
				delete pEft;
			}
			m_effectArray.clear();

			for(map<tstring,EffectPathCtrl*>::iterator iter=m_pathArray.begin();
				iter!=m_pathArray.end();++iter)
			{
				EffectPathCtrl *pPath=iter->second;
				delete pPath;
			}
			m_pathArray.clear();

			m_renderNodes.clear();

			if(m_pRes!=NULL)
			{
				ResMgr::Inst()->ReleaseSpecialEffect(m_pRes);
				m_pRes=NULL;
			}
		}
	};

	SGEffectNode::SGEffectNode(void)
	{
		m_p=new Member;
		m_bShow=true;
	}

	SGEffectNode::~SGEffectNode(void)
	{
		delete m_p;
	}

	void SGEffectNode::Serialize(FArchive& ar)
	{
		vector<EffectPathCtrl*>	pathArray;
		GetEffectPathArray(pathArray);

		ar<<m_p->m_toWorld;
		SerialObjPtrVector(ar,m_p->m_effectArray);
		SerialObjPtrVector(ar,pathArray);

		// ��Ч��㲻�����ӽ��
		const int numChildren = GetNumChildren();
		for( int i = 0; i < numChildren; i++ )
		{
			const SceneNode* pChild = GetChild( 0 );
			DetachChild( pChild );
		}
		
		SetUserData( m_p->m_strCurVersion.c_str() );
		SceneNode::Serialize(ar);

		// �����Χ��
		bool bBoxBuild = ReBuildLocalBox();
		ASSERT(bBoxBuild);
		ar<<m_p->m_boxLocal.min;
		ar<<m_p->m_boxLocal.max;
	}

	void SGEffectNode::Deserialize(FArchive& ar)
	{
		ASSERT(m_p->m_effectArray.size()==0);
		ASSERT(m_p->m_renderNodes.size()==0);
		ASSERT(m_p->m_pathArray.size()==0);

		vector<EffectPathCtrl*>	pathArray;
		if(!m_p->m_bUserWorldMat)
			ar>>m_p->m_toWorld;
		else
		{
			Matrix4 tmp;
			ar>>tmp;
		}
		DeserialObjPtrVector(ar,m_p->m_effectArray);
		DeserialObjPtrVector(ar,pathArray);
		
		SceneNode::Deserialize(ar);
		
		// ��ȡ��Χ��
		const tstring strVersion = GetUserData();
		if( strVersion == m_p->m_strCurVersion )
		{
			ar>>m_p->m_boxLocal.min;
			ar>>m_p->m_boxLocal.max;
		}
		else
		{
			m_p->m_boxLocal.min = Vector3( -50.0f, -50.0f, -50.0f );
			m_p->m_boxLocal.max = Vector3(  50.0f,  50.0f,  50.0f );
		}
		UpdataWorldBox();

		for(vector<EffectPathCtrl*>::iterator iter=pathArray.begin();
			iter!=pathArray.end();++iter)
		{
			EffectPathCtrl *pPath=(*iter);
			m_p->m_pathArray[pPath->GetName()]=pPath;
		}
		RebuildEffectRenderNodes();

		//--������ʼ����
		if(!m_p->m_bUserPlay)
			Play();
	}

	bool SGEffectNode::IsInFrustum(const Frustum *pFrumstum) const
	{
		if(!m_p->m_bBuild)
			return false;

		return pFrumstum->BoxInFrustum(m_p->m_boxWorld);
	}

	void SGEffectNode::LoadRes(const TCHAR* szName)
	{
		ASSERT(m_p->m_pRes==NULL);
		ASSERT(m_p->m_bBuild==false);

		m_p->m_pRes=ResMgr::Inst()->NewSpecialEffect(szName);
	}

	UINT SGEffectNode::GetNumRenderNode() const
	{
		if(!m_p->m_bBuild)
			return 0;

		if(m_bShow)
		{
			for( size_t i = 0; i < m_p->m_renderNodes.size(); i++ )
			{
				if( m_p->m_renderNodes[i].nLOD > MAX_LOD - m_p->m_lod )
					return i;
			}
			return m_p->m_renderNodes.size();
		}
		else
			return 0;
	}

	RenderNode *SGEffectNode::GetRenderNode(UINT i) const
	{
		ASSERT(i>=0 && i<GetNumRenderNode());
		return m_p->m_renderNodes[i].pRenderNode;
	}

	const Matrix4& SGEffectNode::GetWorldMat() const
	{
		return m_p->m_toWorld;
	}

	void SGEffectNode::SetWorldMat(const Matrix4 *pMat)
	{
		m_p->m_toWorld=*pMat;

		// ������Ч��Ҫ���⴦������
		for(vector<EffectBase*>::iterator iter=m_p->m_effectArray.begin();
			iter!=m_p->m_effectArray.end();
			++iter)
		{
			EffectBase *pEffect=(*iter);
			if( pEffect->IS_STATIC_CLASS(EffectParticleEmitter) )
			{
				EffectParticleEmitter* pParEm = (EffectParticleEmitter*)pEffect;
				pParEm->OnSetWorldMat( m_p->m_toWorld );
			}
		}

		UpdataWorldBox();
	}

	void SGEffectNode::GetBox(AABBox& out) const
	{
		out = m_p->m_boxWorld;
	}

	void SGEffectNode::SetBoxCenter(const Vector3& center)
	{
		m_p->m_boxCenter=center;
		UpdataWorldBox();
	}

	IMaterial*	SGEffectNode::GetMtl(int mtlID)
	{
		int numEffect=m_p->m_effectArray.size();
		if(mtlID>=0 && mtlID<numEffect)
		{
			EffectBase *pEffect=m_p->m_effectArray[mtlID];
			return pEffect->GetMtl();
		}
		return EffectBase::GetMtlFallback();
	}

	void SGEffectNode::Update(float deltaTime,SceneGraph *pSG)
	{
		if(m_p->m_bUserUpdate)
			return;

		MainUpdate(deltaTime,pSG);
	}

	void SGEffectNode::ForceUpdate(float deltaTime,SceneGraph *pSG)
	{
		//--�����Դ�������
		if(!m_p->m_bBuild)
		{
			if(m_p->m_pRes!=NULL
				&& m_p->m_pRes->IsCreated())
			{
				m_p->m_pRes->DeserializeEffectNode(this);
				m_p->m_bBuild=true;

				// ������ֶ����ţ�����������²���/ֹͣһ��
				if( m_p->m_bUserPlay )
				{
					if( m_p->m_bPlaying )
						Play();
					else
						Close();
				}

				// �������������һ��·��
				ResetTargetPath( m_p->m_vTargetPathStart, m_p->m_vTargetPathTarget );
			}
		}

		//--����lod
		const int lod = pSG->CalcDetailLevel(m_p->m_boxWorld);
		m_p->m_lod = max( lod, sUserLod );

		//--update children
		SceneNode::ForceUpdate(deltaTime,pSG);
	}

	void SGEffectNode::SetUserUpdate(bool bUserUpdate)
	{
		m_p->m_bUserUpdate=bUserUpdate;
	}

	void SGEffectNode::UserUpdate(float deltaTime,SceneGraph *pSG)
	{
		if(m_p->m_bUserUpdate)
			MainUpdate(deltaTime,pSG);
	}

	void SGEffectNode::MainUpdate(float deltaTime,SceneGraph *pSG)
	{
		if(!m_p->m_bBuild)
			return;

		if(!m_p->m_bPlaying)//û���ڲ���,�Ͳ�update
			return;

		//--ֻ���汾֡����Ϣ,���������һ֡����Ϣ
		m_p->m_frmMsgArray.clear();

		//--����ʱ��
		if(deltaTime > 1.0f/10)//����ֹFPS���͵�ʱ����֡���Ч��ը������
			deltaTime=1.0f/10;
		float dt=deltaTime*m_p->m_timeScale;
		m_p->m_runTime+=dt;

		CameraBase *pCamera=pSG->GetCamera();
		//--��Update����path
		for(map<tstring,EffectPathCtrl*>::iterator iter=m_p->m_pathArray.begin();
			iter!=m_p->m_pathArray.end();
			++iter)
		{
			EffectPathCtrl *pPath=(*iter).second;
			pPath->Update(pSG,this,pCamera,dt,m_p->m_runTime);
		}
		//--��Update����effect(����Msg)
		for(vector<EffectBase*>::iterator iter=m_p->m_effectArray.begin();
			iter!=m_p->m_effectArray.end();
			++iter)
		{
			EffectBase *pEffect=(*iter);
			pEffect->Update(pSG,this,pCamera,dt,m_p->m_runTime);
		}

		// ����Ƿ�ر�
		m_p->m_bPlaying = false;
		for(size_t j=0;j<m_p->m_effectArray.size();j++)
		{
			EffectBase *pEffect=m_p->m_effectArray[j];
			if(!pEffect->IsClosed())
			{
				m_p->m_bPlaying = true;
				break;
			}
		}
		
		//--
		//if(GetParent())
		//{
		//	SetWorldMat(&GetParent()->GetWorldMat());//
		//}

		SceneNode::Update(deltaTime,pSG);
	}

	void SGEffectNode::AddEffect(EffectBase *pEffect)
	{
		if(pEffect==NULL)
			return;
		m_p->m_effectArray.push_back(pEffect);
		
		RebuildEffectRenderNodes();
	}

	void SGEffectNode::RebuildEffectRenderNodes()
	{
		// ����LOD���򣬱����޳�
		sort( m_p->m_effectArray.begin(), m_p->m_effectArray.end(), SortEffByLOD );

		//��RenderNode������������,ͳһ����
		m_p->m_renderNodes.clear();
		for(size_t j=0;j<m_p->m_effectArray.size();j++)
		{
			EffectBase *pEffect=m_p->m_effectArray[j];
			pEffect->SetGroupID(j);
			if( pEffect->GetMtlDesc().bHeatHaze )
				continue;
			for(int i=0;i<pEffect->GetNumRenderNodes();i++)
			{
				m_p->m_renderNodes.push_back( tagEffectRenderNode( pEffect->GetLOD(), pEffect->GetRenderNode(i) ) );
			}
			pEffect->GetMtl()->InvalidateRenderCache();
		}

		m_p->m_bBuild=true;
	}

	void SGEffectNode::Play()
	{
		m_p->m_bPlaying=true;

		if(!m_p->m_bBuild)
			return;
		
		m_p->m_runTime=0;

		for(size_t j=0;j<m_p->m_effectArray.size();j++)
		{
			EffectBase *pEffect=m_p->m_effectArray[j];
			pEffect->ResetPlayState();
		}

		for(map<tstring,EffectPathCtrl*>::iterator iter=m_p->m_pathArray.begin();
			iter!=m_p->m_pathArray.end();
			++iter)
		{
			EffectPathCtrl *pPath=(*iter).second;
			pPath->ResetPlayState();
		}
	}

	void SGEffectNode::Close()
	{
		if( !m_p->m_bBuild )
		{
			m_p->m_bPlaying = false;
			return;
		}
		for(size_t j=0;j<m_p->m_effectArray.size();j++)
		{
			EffectBase *pEffect=m_p->m_effectArray[j];
			pEffect->Close(m_p->m_runTime);
		}
	}

	bool SGEffectNode::IsClosed()
	{
		return !m_p->m_bPlaying;
	}

	void SGEffectNode::Pause()
	{
		m_p->m_bPlaying=false;
	}

	void SGEffectNode::Resume()
	{
		m_p->m_bPlaying=true;
	}

	void SGEffectNode::SetTimeScale(float timeScale)
	{
		m_p->m_timeScale=timeScale;
	}

	void SGEffectNode::AddEffectPath(EffectPathCtrl *pPath)
	{
		if(pPath)
			m_p->m_pathArray[pPath->GetName()]=pPath;
	}

	EffectPathCtrl* SGEffectNode::GetEffectPath(const TCHAR* szName)
	{
		map<tstring,EffectPathCtrl*>::iterator fIter
			=m_p->m_pathArray.find(szName);
		if(fIter!=m_p->m_pathArray.end())
			return (*fIter).second;
		else
			return NULL;
	}

	float SGEffectNode::GetRunTime() const
	{
		return m_p->m_runTime;
	}

	void SGEffectNode::GetEffectArray(vector<EffectBase*>& out)
	{
		out=m_p->m_effectArray;
	}

	void SGEffectNode::GetEffectPathArray(vector<EffectPathCtrl*>& out)
	{
		out.clear();
		for(map<tstring,EffectPathCtrl*>::iterator iter=m_p->m_pathArray.begin();
			iter!=m_p->m_pathArray.end();++iter)
		{
			out.push_back(iter->second);
		}
	}

	void SGEffectNode::RemoveEffect(const TCHAR* szName,bool bAutoDel)
	{
		for(vector<EffectBase*>::iterator iter=m_p->m_effectArray.begin();
			iter!=m_p->m_effectArray.end();)
		{
			EffectBase *pEft=(*iter);
			if(_tcscmp(szName,pEft->GetName())==0)
			{
				iter=m_p->m_effectArray.erase(iter);
				if(bAutoDel)
					delete pEft;
				else
					pEft->OnDetachFromSGNode(this);
				break;
			}
			else
				++iter;
		}
		RebuildEffectRenderNodes();
	}

	void SGEffectNode::RemovePath(const TCHAR* szName,bool bAutoDel)
	{
		map<tstring,EffectPathCtrl*>::iterator iter=m_p->m_pathArray.find(szName);
		if(iter!=m_p->m_pathArray.end())
		{
			EffectPathCtrl *pPath=iter->second;
			m_p->m_pathArray.erase(iter);
			if(bAutoDel)
				delete pPath;
		}
	}

	EffectBase *SGEffectNode::FindEffect(const TCHAR* szName)
	{
		for(vector<EffectBase*>::iterator iter=m_p->m_effectArray.begin();
			iter!=m_p->m_effectArray.end();++iter)
		{
			EffectBase *pEft=(*iter);
			if(_tcscmp(szName,pEft->GetName())==0)
			{
				return pEft;
			}
		}

		return NULL;
	}

	EffectPathCtrl *SGEffectNode::FindPath(const TCHAR* szName)
	{
		map<tstring,EffectPathCtrl*>::iterator iter=m_p->m_pathArray.find(szName);
		if(iter!=m_p->m_pathArray.end())
		{
			return iter->second;
		}
		else
			return NULL;
	}

	const vector<EffectMsg::tagState>& SGEffectNode::GetFrmMsgArray() const
	{
		return m_p->m_frmMsgArray;
	}
	
	void SGEffectNode::PushFrmMsg(EffectMsg::tagState  msg)
	{
		m_p->m_frmMsgArray.push_back(msg);
#ifdef _DEBUG
		TRACE(_T("SGEffectNode Frame msg=%s\r\n"),msg.szMsg);
#endif
	}

	float SGEffectNode::GetPlayTime()
	{
		float time=-1;
		for(int i=0;i<(int)m_p->m_effectArray.size();i++)
		{
			float t=m_p->m_effectArray[i]->GetPlayTime();
			if(t>time)
				time=t;
		}
		return time;
	}

	bool SGEffectNode::IsBuild()
	{
		return m_p->m_bBuild;
	}

	void SGEffectNode::SetUserWorldMat(bool bUserWorldMat)
	{
		m_p->m_bUserWorldMat=true;
	}

	void SGEffectNode::ResetTargetPath( const Vector3& vStart, const Vector3& vTarget )
	{
		if( !m_p->m_bBuild )
		{
			m_p->m_vTargetPathStart = vStart;
			m_p->m_vTargetPathTarget = vTarget;
			return;
		}
		for(map<tstring,EffectPathCtrl*>::iterator iter=m_p->m_pathArray.begin();
			iter!=m_p->m_pathArray.end();
			++iter)
		{
			EffectPathCtrl *pPath=(*iter).second;
			if( pPath->IS_STATIC_CLASS(EffectTarget) )
			{
				EffectTarget* pTargetPath = (EffectTarget*)pPath;
				pTargetPath->ResetTarget( vStart, vTarget );
			}
		}
	}

	void SGEffectNode::ChangeTargetPathTarget( const Vector3& vTarget )
	{
		if( !m_p->m_bBuild )
		{
			m_p->m_vTargetPathTarget = vTarget;
			return;
		}
		for(map<tstring,EffectPathCtrl*>::iterator iter=m_p->m_pathArray.begin();
			iter!=m_p->m_pathArray.end();
			++iter)
		{
			EffectPathCtrl *pPath=(*iter).second;
			if( pPath->IS_STATIC_CLASS(EffectTarget) )
			{
				EffectTarget* pTargetPath = (EffectTarget*)pPath;
				pTargetPath->ChangeTargetPt( vTarget );
			}
		}
	}

	bool SGEffectNode::IsTargetPathArrive()
	{
		if( !m_p->m_bBuild )
		{
			return false;
		}
		for(map<tstring,EffectPathCtrl*>::iterator iter=m_p->m_pathArray.begin();
			iter!=m_p->m_pathArray.end();
			++iter)
		{
			EffectPathCtrl *pPath=(*iter).second;
			if( pPath->IS_STATIC_CLASS(EffectTarget) )
			{
				EffectTarget* pTargetPath = (EffectTarget*)pPath;
				if( !pTargetPath->IsArrive() )
					return false;
			}
		}
		return true;
	}

	void SGEffectNode::SetUserPlay(bool bUserPlay)
	{
		m_p->m_bUserPlay=bUserPlay;
	}

	bool SGEffectNode::ReBuildLocalBox()
	{
		m_p->m_boxLocal.min=Vector3(FLOAT_MAX,FLOAT_MAX,FLOAT_MAX);
		m_p->m_boxLocal.max=Vector3(-FLOAT_MAX,-FLOAT_MAX,-FLOAT_MAX);
		for(map<tstring,EffectPathCtrl*>::iterator iter=m_p->m_pathArray.begin();
			iter!=m_p->m_pathArray.end();
			++iter)
		{
			EffectPathCtrl *pPath=(*iter).second;
			AABBox box;
			if( !pPath->GetBox( box ) )
				return false;
			m_p->m_boxLocal.Merge( box );
		}
		for(vector<EffectBase*>::iterator iter=m_p->m_effectArray.begin();
			iter!=m_p->m_effectArray.end();
			++iter)
		{
			EffectBase *pEffect=(*iter);
			AABBox box;
			if( !pEffect->GetBox( box ) )
				return false;
			m_p->m_boxLocal.Merge( box );
		}
		UpdataWorldBox();
		return true;
	}

	void SGEffectNode::UpdataWorldBox()
	{
		Transform trans;
		trans.LoadMatrix(&GetWorldMat());
		trans.Translate(m_p->m_boxCenter);
		m_p->m_boxWorld = m_p->m_boxLocal;
		m_p->m_boxWorld.Transform( *trans.GetMatrix() );
	}
	
	int SGEffectNode::GetCurLOD()
	{
		return m_p->m_lod;
	}
}//namespace Cool3D