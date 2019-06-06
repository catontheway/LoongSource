#pragma once
#include "..\Cool3D\Cool3DEngine.h"
using namespace Cool3D;

/** \class ModelSnapshoter
	\brief ģ�������,���Ը�ģ���Ŀ���,������ָ��Ŀ¼��(.tga�ļ�)
*/
class ModelSnapshoter
{
public:
	ModelSnapshoter(void);
	~ModelSnapshoter(void);
	void Render( SceneGraph* pSG, SceneNode* pNode );
	void Init();
	void Destroy();
	bool SaveToFile( const TCHAR* szFilename );
	CString GetFileNameExt() { return _T("tga"); }
private:
	void BltToRenderTarget( IRenderTexture* pDestTex, IRenderTexture* pSrcTex );
	void BltToBackBuffer( int x, int y, IRenderTexture* pTex, ResTexture* pAlphaMap = NULL );
private:
	IRenderTexture* m_pRenderTex;
	IRenderTexture* m_pSaveTex;
	ResTexture*     m_pAlphaMap;
	NullSceneGraph* m_pSG;
	bool            m_bInited;
};
