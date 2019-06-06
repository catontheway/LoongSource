#include "StdAfx.h"
#include ".\effectquad.h"
#include "..\RenderSys\DynamicMesh.h"
#include "..\RenderSys\RBillboards.h"
#include "..\Math\Transform.h"
#include "EffectPathCtrl.h"
#include "SGeffectNode.h"
#include "EftQuadDef.h"

namespace F3D
{
	IMP_RTTI_DYNC(EffectQuad,EffectBase);

	class EffectQuad::Member
	{
	public:
		tagQuadProp			m_prop;
		DynamicMesh			*m_pRMesh;
		vector<tagState>	m_keyStates;	//ÿ���ؼ�֡��״̬
		int					m_curState;		//��ǰ�Ĺؼ�֡״̬index
		float				m_stateTime;	//��ǰ״̬�����˶���ʱ��
		float				m_tailFadeSpeed;//β���ϵ�Quad(��һ�����������)alpha���͵��ٶ�,
											//�Ǹ���β�͵ĳ��Ⱥ�β�ͼ��ʱ����������
		float				m_tailDely;		//�ϴ�β�����ӵ����ڵ�ʱ��
		tagEftQuad			*m_quads;
		int					m_tailIndex;	//���һ��β��quad�������е��±�

		bool				m_bActive;//�����Ƿ��Ѿ�����
		
		Vector3 curRot;	//��ǰ��ת
	public:
		Member():curRot(0,0,0)
		{
			m_curState=0;
			m_stateTime=0;
			m_pRMesh=NULL;
			m_tailFadeSpeed=1.0f;
			m_tailDely=0;
			m_quads=NULL;
			m_tailIndex=1;
			m_bActive=true;
		}
		~Member()	
		{
			Destroy();
		}

		void Destroy()
		{
			SAFE_DELETE(m_pRMesh);
			SAFE_FREE(m_quads);
		}

		//����һ����β��quad
		void AddTailQuad()
		{
			if(m_prop.tailSize==1)
				return;
			m_tailDely=0;//���ʱ��״̬

			m_tailIndex++;
			if(m_tailIndex>=m_prop.tailSize)
				m_tailIndex=1;
			m_quads[m_tailIndex]=m_quads[0];//����head quad��״̬
		}
		
		//����β�͵�Fade״̬
		void UpdateTailFade(float deltaTime)
		{
			if(m_prop.tailSize==1)
				return;
			Color4f c;
			for(int i=1;i<m_prop.tailSize;i++)
			{
				c=m_quads[i].color;
				c.A-=m_tailFadeSpeed*deltaTime;
				if(c.A<0.0f)
					c.A=0.0f;
				Color4ub cc=c;
				DWORD dc=cc;
				for(int j=0;j<4;j++)
					m_quads[i].color=dc;
			}
		}

		void SetClosingFade(float alpha)
		{
			Color4f c;
			for(int i=0;i<m_prop.tailSize;i++)
			{
				c=m_quads[i].color;
				if(c.A>alpha)
					c.A=alpha;
				Color4ub cc=c;
				DWORD dc=cc;
				for(int j=0;j<4;j++)
					m_quads[i].color=dc;
			}
		}

		void UpdateQuad(const Color4f &color,const Vector2 &size,
			const Vector3 &pos,const Vector3 &rot,tagEftQuad& quad)
		{
			Color4ub tmpC(color);
			quad.color=tmpC;
			quad.pos=pos;
			quad.rot=rot;
			quad.halfSize=size*0.5f;
		}

		void ResetQuadBuffer()
		{
			ASSERT(m_quads!=NULL);
			int bufSize=m_prop.tailSize*sizeof(tagEftQuad);
			memset(m_quads,0,bufSize);
			for(int i=0;i<m_prop.tailSize;i++)
			{
				m_quads[i].eUV.x=1.0f;
				m_quads[i].eUV.y=1.0f;
			}
		}
	};

	EffectQuad::EffectQuad(void)	  
	{								  
		m_p=new Member;
	}

	EffectQuad::~EffectQuad(void)
	{
		delete m_p;
	}

	void EffectQuad::Serialize(FArchive& ar)
	{
		ar.ValueWrite(m_p->m_prop);
		SerialValVector(ar,m_p->m_keyStates);

		EffectBase::Serialize(ar);
	}

	void EffectQuad::Deserialize(FArchive& ar)
	{
		tagQuadProp prop;
		ar.ValueRead(prop);
		vector<tagState> keyStates;
		DeserialValVector(ar,keyStates);

		//build
		BeginBuild(prop);
		for(size_t i=0;i<keyStates.size();i++)
			AddKeyState(keyStates[i]);
		EndBuild();

		EffectBase::Deserialize(ar);
	}

	RenderNode* EffectQuad::GetRenderNode(int i)
	{
		return m_p->m_pRMesh;
	}

	void EffectQuad::Update(SceneGraph* pSG,SGEffectNode *pSGNode,CameraBase *pCamera,float deltaTime,float runTime)
	{
		if(m_p->m_pRMesh==NULL || m_p->m_quads==NULL)
			return;
		//--�Ƿ�����
		if(!m_p->m_bActive)
		{
			if(runTime > m_p->m_prop.m_startTime)
				m_p->m_bActive=true;
		}
		if(!m_p->m_bActive)
			return;

		m_p->m_stateTime+=deltaTime;
		//--��ֵ����ǰ��״̬
		m_p->m_curState=AdvanceState(m_p->m_curState,m_p->m_keyStates.size(),
			m_p->m_keyStates[m_p->m_curState].lastTime,m_p->m_stateTime,
			m_p->m_prop.m_loop);
		ASSERT(m_p->m_curState>=0 
			&& m_p->m_curState<(int)m_p->m_keyStates.size());
		tagState curState=m_p->m_keyStates[m_p->m_curState];
		
		if(m_p->m_keyStates.size()>1)//��������֡,�Ž��в�ֵ
		{
			int nextState=m_p->m_curState+1;
			bool bLerp=true;
			if(nextState>=(int)m_p->m_keyStates.size())
			{
				if(!m_p->m_prop.m_loop)//�Ѿ����ŵ����һ��״̬,���Ҳ���ѭ������,�����ֵ
					bLerp=false;
				nextState=0;
			}

			if(bLerp)
			{
				tagState& ts=m_p->m_keyStates[nextState];
				float r=m_p->m_stateTime/curState.lastTime;

				D3DXVec4Lerp((Vector4*)&curState.color,(Vector4*)&curState.color,(Vector4*)&ts.color,r);
				D3DXVec2Lerp(&curState.size,&curState.size,&ts.size,r);
				D3DXVec3Lerp(&curState.rotVel,&curState.rotVel,&ts.rotVel,r);
			}
		}

		//--update tail
		if(m_p->m_prop.tailSize>1)
		{
			if(!m_bClosing)
			{
				m_p->m_tailDely+=deltaTime;
				if(m_p->m_tailDely>m_p->m_prop.tailInterval)
				{
					m_p->AddTailQuad();
					m_p->m_tailDely-=m_p->m_prop.tailInterval;
				}
				m_p->UpdateTailFade(deltaTime);
			}//endof if
		}
		
		//--update head quad		
		m_p->curRot+=curState.rotVel*deltaTime;
		
		Vector3 pos=m_p->m_prop.offset;
		EffectPathCtrl* pPath=pSGNode->GetEffectPath(m_p->m_prop.szPathName);
		if(pPath)
			pos+=pPath->GetCurPos();

		m_p->UpdateQuad(curState.color,curState.size,
			pos,m_p->curRot,m_p->m_quads[0]);

		if(m_bClosing)//������ڹر�,������quad��alpha����
		{
			EffectBase::UpdateClosingFade(runTime,m_p->m_prop.m_closeTime);
			m_p->SetClosingFade(m_closingFade);
		}

		//--update render mesh
		CopyQuadArrayToMesh(m_p->m_pRMesh,m_p->m_quads,m_p->m_prop.tailSize,
			pCamera,pSGNode->GetWorldMat(),m_p->m_prop.faceType);
		

		//--update mtl
		EffectBase::Update(pSG,pSGNode,pCamera,deltaTime,runTime);

	}

	void EffectQuad::BeginBuild(const tagQuadProp& prop)
	{
		m_p->m_keyStates.clear();
		m_p->m_curState=0;
		
		m_p->m_prop=prop;
	}

	void EffectQuad::AddKeyState(const tagState& state)
	{
		m_p->m_keyStates.push_back(state);
	}

	void EffectQuad::EndBuild()
	{
		ETransparentType transType=ETrans_Total;//todo:��Mtlȡ��

		m_p->Destroy();
		//-- ����һ��Quad��Ҫ��Mesh
		m_p->m_pRMesh=new DynamicMesh;
		m_p->m_pRMesh->Create(m_p->m_prop.m_groupID,
			EDraw_TriangleList,transType,EVtype_PTD,
			m_p->m_prop.tailSize*6);//ÿ��Quad������������(6���������)
		VertStream *pVStream=m_p->m_pRMesh->GetVertStream();
		memset(pVStream->GetRawBuffer(),0,pVStream->GetBufferSize());
		m_p->m_pRMesh->SetDrawLimit(0);


		//--����β��alpha���͵��ٶ�
		if(m_p->m_prop.tailLife==0.0f)
		{
			if(m_p->m_prop.tailSize>1)
				m_p->m_tailFadeSpeed=1.0f/((m_p->m_prop.tailSize-1)*m_p->m_prop.tailInterval);
			else
				m_p->m_tailFadeSpeed=1.0f;
		}
		else
			m_p->m_tailFadeSpeed=1.0f/m_p->m_prop.tailLife;

		//--����quad buffer
		int bufSize=m_p->m_prop.tailSize*sizeof(tagEftQuad);
		m_p->m_quads=(tagEftQuad*)malloc(bufSize);
		m_p->ResetQuadBuffer();

		//--��ʼ��head quad
		ResetPlayState();

		ASSERT(m_p->m_keyStates.size()>0);
		tagState& initState=m_p->m_keyStates[0];
		m_p->UpdateQuad(initState.color,initState.size,
			m_p->m_prop.offset,m_p->curRot,m_p->m_quads[0]);
	}

	void EffectQuad::ResetPlayState()
	{
		m_p->m_curState=0;
		m_p->m_stateTime=0;
		
		m_p->curRot=Deg2Rad3f(m_p->m_prop.initRot);

		//--resetβ��
		m_p->ResetQuadBuffer();
		m_p->m_tailIndex=1;

		m_p->m_bActive=m_p->m_prop.m_bActive;
		if(!m_p->m_bActive)
			m_p->m_pRMesh->SetDrawLimit(0);

		EffectBase::ResetPlayState();
	}

	const char* EffectQuad::GetName() const
	{
		return m_p->m_prop.m_szName;
	}

	EffectQuad::tagQuadProp EffectQuad::GetProp() const
	{
		return m_p->m_prop;
	}

	int EffectQuad::GetNumStates() const
	{
		return m_p->m_keyStates.size();
	}

	EffectQuad::tagState EffectQuad::GetState(int i) const
	{
		ASSERT(i>=0 && i<GetNumStates());
		return m_p->m_keyStates[i];
	}

	void EffectQuad::Rename(const char* szNewName)
	{
		strncpy(m_p->m_prop.m_szName,szNewName,sizeof(m_p->m_prop.m_szName));
	}

	void EffectQuad::SetGroupID(int id)
	{
		m_p->m_prop.m_groupID=id;
		if(m_p->m_pRMesh)
			m_p->m_pRMesh->SetMtlID(id);
	}

	void EffectQuad::SetOffset( const Vector3& pos )
	{
		m_p->m_prop.offset = pos;
	}

}//namespace F3D