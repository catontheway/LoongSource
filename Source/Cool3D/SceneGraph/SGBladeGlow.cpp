#include "StdAfx.h"
#include ".\sgbladeglow.h"
#include "..\RenderSys\DynamicMesh.h"
#include "..\RenderMtl\MGeneral.h"
#include "..\RenderMtl\MConstColor.h"
#include "..\RenderMtl\MMAlphaMask.h"
#include "..\RenderMtl\MMFrameBlend.h"
#include "..\RenderMtl\MMDepthFrame.h"
#include "..\RenderMtl\MMUVPage.h"
#include "..\ResSys\ResStaticMesh.h"
#include "..\Public\TagNodeDefine.h"
#include "..\Math\Transform.h"
#include "..\Math\CameraBase.h"
#include "..\SceneGraph\SceneGraph.h"
#include "..\Device\DrawX.h"

#include "EntityNode.h"

namespace Cool3D
{
	IMP_RTTI_DYNC(SGBladeGlow,SceneNode);

	/////////////////////////////////////////////////////////////////////////////////
	// class BladeGlow
	// brief ���λ���
	//
	class BladeGlow
	{
	public:
		BladeGlow(){}
		virtual ~BladeGlow(){}
		virtual void Update(SceneGraph *pSG, EntityNode *pFatherNode, Color4ub color) =0;
		virtual RenderNode* GetRenderNode() =0;
	};

	/////////////////////////////////////////////////////////////////////////////////
	// class BladeGlowCircle
	// brief Բ�ι���
	//
	class BladeGowCircle : public BladeGlow
	{
	public:
		BladeGowCircle()
		{
			m_pMesh = NULL;
			m_fOffset = 0;
		}

		virtual ~BladeGowCircle()
		{
			SAFE_DELETE(m_pMesh);
		}

		void Init(EntityNode *pFatherNode,tagTagNode& tagNode)
		{
			ASSERT(m_pMesh==NULL);
			ASSERT(pFatherNode!=NULL);

			//--������ص�
			m_tagNode = tagNode;

			//--����Ĭ�ϰ뾶
			if( m_tagNode.rotat.x <= 0 )
				m_tagNode.rotat.x = 20.0f;

			//--��������
			/*
			2--4
			|  |
			1--3
			*/
			m_pMesh = new DynamicMesh;
			m_pMesh->Create( 0, EDraw_TriangleStrip, EVType_PTD, 4 );

			VertStream *pVStream = m_pMesh->GetVertStream();
			ASSERT( pVStream && pVStream->GetNumVert() == 4 );
			Vert_PTD *pVB = (Vert_PTD *)pVStream->GetRawBuffer();
			ASSERT( pVB );

			pVB[0].uv = Vector2( 0.0f, 1.0f );
			pVB[1].uv = Vector2( 0.0f, 0.0f );
			pVB[2].uv = Vector2( 1.0f, 1.0f );
			pVB[3].uv = Vector2( 1.0f, 0.0f );

			//--����ƫ�ƾ���
			AABBox box;
			pFatherNode->GetBox(box);
			m_fOffset = (box.max.y - box.min.y)*0.5f;

		}

		virtual RenderNode* GetRenderNode()
		{
			return m_pMesh;
		}

		virtual void Update(SceneGraph *pSG, EntityNode *pFatherNode, Color4ub color)
		{
			CameraBase *pCamera = pSG->GetCamera();
			Vector3 vEye = pCamera->GetPos();

			//--�任�е�λ��
			Vector3 vMid = m_tagNode.trans;
			{
				Transform tran;
				tran.LoadMatrix( &pFatherNode->GetWorldMat() );
				tran.TransVec3( vMid );

				Vector3 vOffset = vEye - vMid;
				D3DXVec3Normalize( &vOffset, &vOffset );
				vOffset *= m_fOffset;
				vMid += vOffset;

			}

			//--����������,������
			Vector3 vRight, vTop;
			{
				Vector3 vMidToEye = vEye - vMid;
				D3DXVec3Normalize( &vMidToEye, &vMidToEye );

				Vector3 vUp( 0.0f, 1.0f, 0.0f );
				D3DXVec3Cross( &vRight, &vUp, &vMidToEye );
				D3DXVec3Normalize( &vRight, &vRight );

				D3DXVec3Cross( &vTop, &vRight, &vMidToEye );
				D3DXVec3Normalize( &vTop, &vTop );

				vRight	*= m_tagNode.rotat.x;
				vTop	*= m_tagNode.rotat.x;
			}


			//--��������
			{
				VertStream *pVStream = m_pMesh->GetVertStream();
				ASSERT( pVStream->GetNumVert() == 4 );
				Vert_PTD *pVB = (Vert_PTD *)pVStream->GetRawBuffer();

				pVB[0].pos = vMid + vTop + vRight;
				pVB[1].pos = vMid + vTop - vRight;
				pVB[2].pos = vMid - vTop + vRight;
				pVB[3].pos = vMid - vTop - vRight;
				
				pVB[0].diffuse = color;
				pVB[1].diffuse = color;
				pVB[2].diffuse = color;
				pVB[3].diffuse = color;
			}
		}

	private:
		tagTagNode				m_tagNode;		// ���ص�
		DynamicMesh				*m_pMesh;		// ����
		float					m_fOffset;		// ����Ƭ��ͷƫ�Ƶľ���
	};

	/////////////////////////////////////////////////////////////////////////////////
	// class BladeGlowEllipse
	// brief ��Բ�ι���
	//
	class BladeGlowEllipse : public BladeGlow
	{
	public:
		BladeGlowEllipse()
		{
			m_pMesh = NULL;
			m_fOffset = 0;
		}

		virtual ~BladeGlowEllipse()
		{
			SAFE_DELETE(m_pMesh);
		}

		void Init(EntityNode *pFatherNode,tagTagNode& tagNode1,tagTagNode& tagNode2)
		{
			ASSERT(m_pMesh==NULL);
			ASSERT(pFatherNode!=NULL);

			//--������ص�
			m_tagNode1 = tagNode1;
			m_tagNode2 = tagNode2;

			//--����Ĭ�ϰ뾶
			if( m_tagNode1.rotat.x <= 0 )
				m_tagNode1.rotat.x = 20.0f;

			//--��������
			/*
			2--4
			|  |
			1--3
			*/
			m_pMesh = new DynamicMesh;
			m_pMesh->Create( 0, EDraw_TriangleStrip, EVType_PTD, 4 );

			VertStream *pVStream = m_pMesh->GetVertStream();
			ASSERT( pVStream && pVStream->GetNumVert() == 4 );
			Vert_PTD *pVB = (Vert_PTD *)pVStream->GetRawBuffer();
			ASSERT( pVB );

			pVB[0].uv = Vector2( 0.0f, 1.0f );
			pVB[1].uv = Vector2( 0.0f, 0.0f );
			pVB[2].uv = Vector2( 1.0f, 1.0f );
			pVB[3].uv = Vector2( 1.0f, 0.0f );

			//--����ƫ�ƾ���
			AABBox box;
			pFatherNode->GetBox(box);
			m_fOffset = (box.max.y - box.min.y)*0.5f;
		}

		virtual RenderNode* GetRenderNode()
		{
			return m_pMesh;
		}

		virtual void Update(SceneGraph *pSG, EntityNode *pFatherNode, Color4ub color)
		{
			CameraBase *pCamera = pSG->GetCamera();
			Vector3 vEye = pCamera->GetPos();

			//--�����Χ��
			Vector3 Box[8];
			{

				// ���㵶���뵶�Ѷ���������
				Vector3 vStart, vEnd, vAxis;
				{
					vStart = m_tagNode1.trans;
					vEnd = m_tagNode2.trans;

					Transform tran;
					tran.LoadMatrix( &pFatherNode->GetWorldMat() );
					tran.TransVec3( vStart );
					tran.TransVec3( vEnd );

					vAxis = vStart - vEnd;
					D3DXVec3Normalize( &vAxis, &vAxis );
				}

				// ����������
				Vector3 vRight;
				{
					Vector3 vEndToEye = vEye - vEnd;
					D3DXVec3Cross( &vRight,&vEndToEye, &vAxis );
					D3DXVec3Normalize( &vRight, &vRight );
				}

				// ���㴹ֱ����
				Vector3 vVertical;
				D3DXVec3Cross( &vVertical, &vAxis, &vRight );
				D3DXVec3Normalize( &vVertical, &vVertical );

				// ����ƫ������
				Vector3 vOffset;
				{
					Vector3 vMid = vStart + (vEnd - vStart) * 0.5f;
					vOffset = vEye - vMid;
					D3DXVec3Normalize( &vOffset, &vOffset );
					vOffset *= m_fOffset;
				}

				//// �ӳ������뵶��
				//{
				//	Vector3 vExtend = vAxis * m_fExtend;
				//	vStart += vExtend;
				//	vEnd   -= vExtend;
				//}

				// ����Box
				{

					vStart		+= vOffset;
					vEnd		+= vOffset;
					vVertical	*= m_tagNode1.rotat.x;
					vRight		*= m_tagNode1.rotat.x;

					Box[0] = vEnd + vRight + vVertical;
					Box[1] = vEnd - vRight + vVertical;
					Box[2] = vEnd + vRight - vVertical;
					Box[3] = vEnd - vRight - vVertical;
					Box[4] = vStart + vRight + vVertical;
					Box[5] = vStart - vRight + vVertical;
					Box[6] = vStart + vRight - vVertical;
					Box[7] = vStart - vRight - vVertical;

				}
			}

			//--��Box�ҳ���������ΪƬ����������
			int edge1 = 0, edge2 = 1;
			{
				Plane planeFront;
				BuildPlane( Box[0], Box[4], Box[5], planeFront );
				bool bFrontVisible = planeFront.ClassifyPoint( vEye ) == POSITIVE;

				Plane planeTop;
				BuildPlane( Box[0], Box[1], Box[3], planeTop );
				bool bTopVisible = planeTop.ClassifyPoint( vEye ) == POSITIVE;

				Plane planeBottom;
				BuildPlane( Box[4], Box[6], Box[7], planeBottom );
				bool bBottomVisible = planeBottom.ClassifyPoint( vEye ) == POSITIVE;
				if( bFrontVisible && bTopVisible )
				{
					edge1 = 2;
					edge2 = 1;
				}

				if( bFrontVisible && bBottomVisible )
				{
					edge1 = 3;
					edge2 = 0;
				}

				if( bFrontVisible && !bTopVisible && !bBottomVisible )
				{
					edge1 = 2;
					edge2 = 0;
				}

				if( !bFrontVisible && bTopVisible )
				{
					//edge1 = 1;
					//edge2 = 0;
					edge1 = 2;
					edge2 = 1;
				}

				if( !bFrontVisible && bBottomVisible )
				{
					//edge1 = 3;
					//edge2 = 2;
					edge1 = 3;
					edge2 = 0;
				}

			}

			//--��������
			{
				VertStream *pVStream = m_pMesh->GetVertStream();
				ASSERT( pVStream->GetNumVert() == 4 );
				Vert_PTD *pVB = (Vert_PTD *)pVStream->GetRawBuffer();

				pVB[0].pos = Box[edge1*2];
				pVB[1].pos = Box[edge1*2+1];
				pVB[2].pos = Box[edge2*2];
				pVB[3].pos = Box[edge2*2+1];

				pVB[0].diffuse = color;
				pVB[1].diffuse = color;
				pVB[2].diffuse = color;
				pVB[3].diffuse = color;
			}
		}

	private:
		tagTagNode				m_tagNode1,		// ���ص�
								m_tagNode2;
		DynamicMesh				*m_pMesh;		// ����
		float					m_fOffset;		// ����Ƭ��ͷƫ�Ƶľ���
	};

	/////////////////////////////////////////////////////////////////////////////////
	// class SGBladeGlow
	SGBladeGlow::SGBladeGlow(void)
	{
		m_bEnable = false;
		m_color = Color4ub(255,0,0,255);
		m_blendOne = true;
	}

	SGBladeGlow::~SGBladeGlow(void)
	{
		vector<BladeGlow*>::iterator it=m_vecBladeGlow.begin();
		while(it!=m_vecBladeGlow.end())
		{
			BladeGlow* pBG = *it;
			delete pBG;
			++it;
		}
		m_vecBladeGlow.clear();
	}

	bool BGESortFun(const tagTagNode& tagNode1,const tagTagNode& tagNode2)
	{
		TCHAR str1[4] = {0};
		TCHAR str2[4] = {0};
		_tcsncpy(str1,_FourCC2Str(tagNode1.name).c_str(),sizeof(str1)/sizeof(TCHAR));
		_tcsncpy(str2,_FourCC2Str(tagNode2.name).c_str(),sizeof(str2)/sizeof(TCHAR));
		int num1 = _ttoi(&str1[2]);
		int num2 = _ttoi(&str2[2]);
		return num1 < num2;
	}

	void SGBladeGlow::Init( EntityNode *pFatherNode )
	{
		//--��ֹ�ظ���ʼ��
		ASSERT(m_vecBladeGlow.empty());

		ResRenderable *pRes = pFatherNode->GetResObj();
		if(pRes==NULL
			||!pRes->IsCreated())
			return;

		ASSERT( pRes->IS_STATIC_CLASS(ResStaticMesh) );
		ResStaticMesh *pStaticMesh = (ResStaticMesh *)pRes;
		if( pStaticMesh==NULL || !pStaticMesh->IsCreated() )
			return;

		const vector<tagTagNode>& tagNodes = pStaticMesh->GetTagNodes().nodes;
		if( tagNodes.size() < 1 )
			return;

		//--���ص����
		vector<tagTagNode> tagNodesEllipse;
		vector<tagTagNode>	tagNodesCircle;
		size_t i;
		for( i=0; i<tagNodes.size(); i++ )
		{
			DWORD name = tagNodes[i].name & 0xFFFF;
			if( name == _MakeFourCC(_T("GC")) )
				tagNodesCircle.push_back(tagNodes[i]);
			if( name == _MakeFourCC(_T("GE")) )
				tagNodesEllipse.push_back(tagNodes[i]);
		}

		//--����Բ�ι���
		for( i=0; i<tagNodesCircle.size(); i++ )
		{
			BladeGowCircle* pGC = new BladeGowCircle;
			pGC->Init( pFatherNode, tagNodesCircle[i] );
			m_vecBladeGlow.push_back(pGC);
		}

		//--������Բ�ι���
		if( tagNodesEllipse.size()>0 )
		{
			if( tagNodesEllipse.size() % 2 == 0 )
			{
				sort( tagNodesEllipse.begin(), tagNodesEllipse.end(), BGESortFun );

				for( i=0; i<tagNodesEllipse.size(); i+=2 )
				{
					BladeGlowEllipse* pGE = new BladeGlowEllipse;
					pGE->Init( pFatherNode, tagNodesEllipse[i], tagNodesEllipse[i+1] );
					m_vecBladeGlow.push_back(pGE);
				}
			}
			else
			{
				ASSERT( 0 && _T("��Բ�ι�����Ҫ�������ص�") );
			}
		}
	}

	bool SGBladeGlow::IsInFrustum(const Frustum *pFrumstum) const
	{
		return true;
	}

	void SGBladeGlow::LoadRes(const TCHAR* szName)
	{
		m_mtl.SetTexture( 0, szName );
		m_mtl.m_bTwoSide=true;
		m_mtl.m_bEnableLight=false;
		m_mtl.m_mipFilter=ETFilter_LINEAR;
		m_mtl.m_minFilter=m_mtl.m_magFilter=ETFilter_LINEAR;
		m_mtl.m_addressU=m_mtl.m_addressV=ETAddress_Clamp;

		MMAlphaMask *pMask=new MMAlphaMask;
		pMask->m_alphaTestFunc=ECMP_Greater;
		pMask->m_alphaRef=0;
		m_mtl.AttachModifier(pMask);

		MMFrameBlend *pBlend = new MMFrameBlend;
		pBlend->m_blendEnable = true;
		pBlend->m_src = EBlend_SrcAlpha;
		pBlend->m_dst = m_blendOne? EBlend_One : EBlend_InvSrcAlpha;
		m_mtl.AttachModifier( pBlend );

		MMUVPage *pPage = new MMUVPage;
		pPage->SetParam(4,4,10.0f,false);
		m_mtl.AttachModifier(pPage);
	}

	UINT SGBladeGlow::GetNumRenderNode() const
	{
		if( !m_bEnable )
			return 0;

		return m_vecBladeGlow.size();
	}

	RenderNode *SGBladeGlow::GetRenderNode(UINT i) const
	{
		if( !m_bEnable )
			return NULL;

		ASSERT( i>=0 && i<m_vecBladeGlow.size() );
		return m_vecBladeGlow[i]->GetRenderNode();
	}

	const Matrix4& SGBladeGlow::GetWorldMat() const
	{
		return MathConst::IdentityMat;
	}

	void SGBladeGlow::SetWorldMat(const Matrix4 *pMat)
	{
		ASSERT( 0 && _T("can not set BladeGlow world matrix") );
	}

	void SGBladeGlow::GetBox(AABBox& out) const
	{
		out = MathConst::SmallBox;
	}

	IMaterial*	SGBladeGlow::GetMtl(int mtlID)
	{
		ASSERT( mtlID == 0 );
		return &m_mtl;
	}

	void SGBladeGlow::Update(float deltaTime, SceneGraph *pSG)
	{
		const SceneNode *pNode = GetParent();
		ASSERT( pNode->IS_KIND_OF( EntityNode ) );
		EntityNode *pFather = ( EntityNode * )pNode;

		if( m_vecBladeGlow.empty() )
		{
			Init( pFather );
		}

		if( !m_vecBladeGlow.empty() )
		{
			for( size_t i=0; i<m_vecBladeGlow.size(); i++ )
				m_vecBladeGlow[i]->Update( pSG, pFather, m_color );
		}

		//--
		m_mtl.UpdateModifiers(deltaTime);
	}

	void SGBladeGlow::ForceUpdate(float deltaTime,SceneGraph *pSG)
	{
		//have no child,do nothing
	}

	void SGBladeGlow::SetBlendOne(bool bBlendOne)
	{
		if(m_blendOne==bBlendOne)
			return;

		m_blendOne=bBlendOne;

		for(UINT i=0;i<m_mtl.GetNumMods();i++)
		{
			const IMtlModifier* pMod=m_mtl.GetModifier(i);
			if(pMod->IS_STATIC_CLASS(MMFrameBlend))
			{
				MMFrameBlend* pBlend=(MMFrameBlend*)pMod;
				pBlend->m_dst=m_blendOne ? EBlend_One : EBlend_InvSrcAlpha;
			}
		}

		m_mtl.InvalidateRenderCache();
	}

}//namespace Cool3D