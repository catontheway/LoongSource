#include "StdAfx.h"
#include "PetShowWindow.h"
#include "PetProtoData.h"

const float g_fPetRotateSpeed = 2.0f;
const Vector3 g_vPetLookAt( 0.0f, 85.0f, 0.0f );
const Vector3 g_vPetFrom( 0.0f, 120.0f, -230.0f );

PetShowWindow::PetShowWindow(void) :
    m_pNewCanvas(0),
    m_pPetNode(0),
    m_pSG(0),
    m_fModelYaw(0.0f)
{
}

PetShowWindow::~PetShowWindow(void)
{
}

BOOL PetShowWindow::Init( GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement )
{
    if( !GUIStatic::Init(pSystem, pFather, pXmlElement) )
        return FALSE;

    // ��Ϊ��Ҫ��Ȼ��壬�����޷�ʹ��Render2D����
    m_pNewCanvas = new tagGUIImage;
    if( !P_VALID(m_pNewCanvas) )
        return FALSE;

    m_pNewCanvas->rc = tagRect( 0, 0, m_ptSize.x, m_ptSize.y );
    m_pNewCanvas->ptSize = m_ptSize;

    // ��Ⱦ������
    IRenderTexture* pRenderTexture = Device()->NewRenderTexture();
    bool bResult = pRenderTexture->Create(m_pNewCanvas->ptSize.x, m_pNewCanvas->ptSize.y, EPF_A8R8G8B8, true, false, 1, false);
    if( !bResult )	// ����RenderTargetʧ��
    {
        SAFE_DEL(pRenderTexture);
        SAFE_DEL(m_pNewCanvas);
        return FALSE;
    }
    m_pNewCanvas->dwHandle = (DWORD)pRenderTexture;
    m_pNewCanvas->bRenderTarget = TRUE;

    m_pNewCanvas->ptOffset.x = m_ptView.x;
    m_pNewCanvas->ptOffset.y = m_ptView.y;
    m_pNewCanvas->bRefresh = TRUE;

    // ���ó�ʼ�������λ��
    m_Camera.SetLookAt( g_vPetFrom, g_vPetLookAt, MathConst::AxisY );
    // ����͸��ͶӰ
    m_Camera.SetPerspective( (float)m_ptSize.x, (float)m_ptSize.y, _DegToRad(45.0f), 1.0f, 20000.0f);

    // �����ճ���ͼ
    m_pSG = new NullSceneGraph;
    // �������
    m_pSG->BindCamera( &m_Camera );

    return TRUE;
}

VOID PetShowWindow::Destroy()
{
    GUIStatic::Destroy();

    m_pSG->DetachDynamicNode( m_pPetNode );
    SAFE_DEL( m_pPetNode );
    SAFE_DEL( m_pSG );
    m_pRender->ReleaseRenderTarget(m_pNewCanvas->dwHandle);
    m_pRender->DestroyImage(m_pNewCanvas);
    m_pNewCanvas = 0;
}

VOID PetShowWindow::Update()
{

    GUIStatic::Update();
}

VOID PetShowWindow::Render()
{
    if( m_bInvisible )
        return GUIStatic::Render();

    if( !P_VALID( m_pPetNode ) )
        return;

    // ��Ⱦ���ּ��ӿؼ�
    m_pRender->PushRenderTarget(m_pNewCanvas);
    m_pNewCanvas->rc = m_rcText + m_ptView;
    m_pRender->Text(m_pNewCanvas, m_strText, m_pFont, m_dwTextColor, m_dwShadeColor, m_eTextAlign, m_bTextWriteAlpha);
    GUIWnd::Render();
    m_pRender->PopRenderTarget(m_pNewCanvas);

    // ��Ⱦ��������
    m_pCanvas->rc.SetLTWH(m_ptView, m_ptSize);
    m_pNewCanvas->rc.SetLTWH(0, 0, m_ptSize.x, m_ptSize.y);
    m_pRender->Draw(m_pCanvas, m_pNewCanvas, 0xFFFFFFFF, m_eAlphaWrite);
}

VOID PetShowWindow::RefreshViewCoord()
{
    GUIStatic::RefreshViewCoord();
    m_pNewCanvas->ptOffset.x = m_ptView.x;
    m_pNewCanvas->ptOffset.y = m_ptView.y;
}

VOID PetShowWindow::SetInvisible( BOOL b )
{

    if( m_bInvisible == b )	// ��ֹ�ظ�����ɾ��
        return;

    GUIStatic::SetInvisible(b);

    if( b )
    {
        SAFE_DEL( m_pPetNode );                                          // ɾ��PetNode
        m_pRender->ReleaseRenderTarget(m_pNewCanvas->dwHandle);          // ɾ������,�ͷ�render target
        m_pNewCanvas->dwHandle = GT_INVALID;

    }
    else
    {
        tagRect rc;
        rc = m_ptSize;
        m_pNewCanvas->dwHandle = m_pRender->CreateRenderTarget(rc);      // ����render target
        if( !GT_VALID(m_pNewCanvas->dwHandle) )
        {
            // ����RenderTargetʧ��
            GUIStatic::SetInvisible( TRUE );
        }
        if( !P_VALID( m_pPetNode ) )                                   // ����AvatarNode
            m_pPetNode = new EntityNode();
    }
}

void PetShowWindow::Render3D()
{
    if( m_bInvisible )
        return GUIStatic::Render();

    if( !P_VALID( m_pPetNode ) )
        return;

    UpdateCamera();

    IDevice* pDev = Device();
    IRenderTexture* pRT = (IRenderTexture*)m_pNewCanvas->dwHandle;
    float fTimeDelta = Kernel::Inst()->GetDeltaTime();

    m_pNewCanvas->bRefresh = TRUE;
    m_bRefresh = TRUE;

    pRT->Begin();
    pDev->Clear( true, true, false, 0xff000000, 1.0f, 0 );
    StateBlock block;
    block.SaveState(ERS_ColorWriteEnable);
    pDev->SetRenderState(ERS_ColorWriteEnable, D3DCOLORWRITEENABLE_BLUE|D3DCOLORWRITEENABLE_GREEN|D3DCOLORWRITEENABLE_RED);

    // �رպ��ڴ���
    const bool bEnablePostProcess = Kernel::Inst()->GetRenderSys()->IsEnablePostProcess();
    Kernel::Inst()->GetRenderSys()->EnablePostProcess( false );
    m_pSG->Render( fTimeDelta );
    Kernel::Inst()->GetRenderSys()->EnablePostProcess( bEnablePostProcess );

    block.Apply();
    pRT->End();
}

void PetShowWindow::RotateYaw( FLOAT fAngle )
{
    if( !P_VALID( m_pPetNode ) )
        return;

    m_fModelYaw += fAngle;

    Transform tran;
    tran.LoadIdentity();
    tran.Rotate( m_fModelYaw, 0.0f, 0.0f );
    m_pPetNode->SetWorldMat( tran.GetMatrix() );
}

void PetShowWindow::SetPetShow( DWORD dwPetTypeID, EPetModelType eModelType /*= EPMT_Follow*/ )
{
    // ж�ؽڵ�
    m_pSG->DetachDynamicNode(m_pPetNode);
    SAFE_DEL(m_pPetNode);

    // ��ó���ԭ��
    const tagPetProto* petProto = PetProtoData::Inst()->FindPetProto(dwPetTypeID);
    if (!P_VALID(petProto))
        return;

    // �ؽ��ڵ�
    m_pPetNode = LoadEntity(petProto->strModelPath[eModelType].c_str());
    if (EAT_Skeleton == petProto->eAniType[eModelType])
    {
        for (list<tstring>::const_iterator it = petProto->lstSkinPath[eModelType].begin();
            it != petProto->lstSkinPath[eModelType].end(); ++it)
        {
            LoadSkin(it->c_str(), m_pPetNode);
        }
        m_pPetNode->UpdateBoundingBox();
    }

    // ���ؽڵ�
    m_pSG->AttachDynamicNode(m_pPetNode);

    // ���������
    UpdateCamera();
}
    
void PetShowWindow::UpdateCamera()
{
    if( !P_VALID(m_pPetNode ) )
        return;

    // ����m_pPetNode�İ�Χ��
    AABBox boxPet;
    const int nNumChild = m_pPetNode->GetNumChildren();
    for( int i = 0; i < nNumChild; i++ )
    {
        const SceneNode *pChild = m_pPetNode->GetChild( i );
        if(pChild->IS_STATIC_CLASS(SGSkinNode))
        {
            const ResSkin *pRes=((const SGSkinNode*)pChild)->GetResSkin();
            boxPet.Merge(*(pRes->GetAABBox()));
        }
    }
    boxPet.Transform( m_pPetNode->GetWorldMat() );

    Vector3 vLookAt = boxPet.GetCenter();
    Vector3 vFrom = vLookAt;
    //vFrom.z -= 220.0f;
    vFrom.z -= (boxPet.max.z - boxPet.min.z + 120.0f) ;
    m_Camera.SetLookAt( vFrom, vLookAt, MathConst::AxisY );
}

EntityNode* PetShowWindow::LoadEntity( LPCTSTR szFileName )
{
    EntityNode* pEntityNode = new EntityNode;

    pEntityNode->LoadRes( szFileName );

    pEntityNode->Hide( FALSE );

    return pEntityNode;
}

void PetShowWindow::LoadSkin( LPCTSTR szFileName, EntityNode* pEntityNode )
{
    ASSERT(pEntityNode!=NULL);
    if (pEntityNode != NULL)
    {
        SGSkinNode *pNewSkin = new SGSkinNode;
        pNewSkin->LoadRes( szFileName );
        pEntityNode->AttachChild( pNewSkin );
    }
}