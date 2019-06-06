#pragma once

/**	\class SGDynamicProjector
	\brief ��̬projector,��texture��vert buffer�����Զ�̬����
	\remarks ʹ�ø��ڵ�����Ⱦ��ͼ�͸���vert buffer
*/
class SGDynamicProjector :	public SceneNode, public IRenderCallBack
{
public:
	SGDynamicProjector(void);
	virtual ~SGDynamicProjector(void);

	// scenenode
	virtual bool IsInFrustum(const Frustum *pFrumstum) const;
	virtual void LoadRes(const TCHAR* szName);
	virtual UINT GetNumRenderNode() const;
	virtual RenderNode *GetRenderNode(UINT i) const;
	virtual const Matrix4& GetWorldMat() const;
	virtual void SetWorldMat(const Matrix4 *pMat);
	virtual void GetBox(AABBox& out) const;
	virtual IMaterial*	GetMtl(int mtlID);
	virtual void Update(float deltaTime,SceneGraph *pSG);
	virtual void ForceUpdate(float deltaTime,SceneGraph *pSG);

	// IRenderCallBack
	virtual void OnRenderBefore(SceneNode* pSGNode,RenderNode* pRenderNode); 

	bool CreateData(int texW,int texH);
	static bool IsHWSupport();

	void WriteToFile( const TCHAR* szPath );
private:
	void MergeChildBox(const SceneNode* pNode,AABBox& boxParent);
	void BuildProjectorMesh( AABBox& boxRole, DynamicMesh *pMesh, CameraBase *pCamera );
private:
	ResPixelShader*     m_pResPS;
	CameraBase	        m_camLight;
	DynamicMesh*        m_pMesh;
	MRenderTex*         m_pRenderTex;
	StateBlock			m_stateBlock;

	DECL_RTTI(SGDynamicProjector);
};
