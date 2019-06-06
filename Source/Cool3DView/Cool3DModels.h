#pragma once
#include "..\Cool3D\Cool3DEngine.h"
#include "..\Cool3D\ResSys\AABBTreeCollider.h"
#include "AABBTreeBuilder.h"
using namespace Cool3D;


class Cool3DModels
{
public:
	Cool3DModels(void);
	~Cool3DModels(void);

	static Cool3DModels* Inst();
	void InitEngine(HWND hDrawWnd);
	void Destroy();

	void LoadModel(const TCHAR* szPath);
	void Render();
	void SetBkColor(DWORD color);
	void ShowSkelDlg();
	void AttachMdl(const TCHAR* szMdlPath,const TCHAR* szTagNodeName);
	void ShowNormal();
	void ShowTangent();
	void SetShowSkel(bool bShow);
	void EnableSnapshoter( bool bEnable );
	bool SavePhoto( const TCHAR* szFilename );
	void SplitSkel();
	CString GetPhotoFileExt();
	CString GetCurMdlPath();

	// ����AABBTree��ʽ
	void UpdateAABBTree(LPCTSTR szPath);
	// ����Skin�Ĳ���ϵ��
	void ModifySkin(LPCTSTR szPath);
	// ������
	void CheckSkel(void);

	ArcBall* GetArcBall();
	CameraEuler* GetCamera();
	SceneGraph* GetSceneGraph();
	SceneNode* GetSceneNode();
	SceneNode* GetAvatarNode();
	AnimationCtrl* GetSceneNodeAni();
	CString	GetSceneNodeResName();

	void SetCameraFov(float rad);

	// �����������ģ��
	void LoadHorsemanModel(const TCHAR* szPath);
	// �����������ģ��
	void LoadHorseModel(const TCHAR* szPath);
	// ���������󶨵����������
	void BindToHorse();
	// ������������β��߶�
	float GetBunsHeight();

	// �����ϰ���Ĺ�������
	void UpdateUpperSkeAni(const TCHAR* szTrackName);
	// �����°���Ĺ�������
	void UpdateLowerSkeAni(const TCHAR* szTrackName);
	// ����ȫ��Ĺ�������
	void UpdateAllSkeAni(const TCHAR* szTrackName);
	// ת���������
	void RotateUpperSkeAni(float yaw, float pitch, float roll);
	// ת��ͷ������
	void RotateHeadSkeAni(float yaw, float pitch, float roll);
	// ����ϰ���Ĺ�������
	void BlendUpperSkeAni(const TCHAR *szTrackName,float weight=1.0f);
	// ����°���Ĺ�������
	void BlendLowerSkeAni(const TCHAR *szTrackName,float weight=1.0f);
	// ���ȫ��Ĺ�������
	void BlendAllSkeAni(const TCHAR *szTrackName,float weight=1.0f);

	// �޸�lightmap�Ĵ�С
	void ModifyLightmapSize(int size);
	int GetLightmapSize() const;

	typedef enum _skintype
	{
		ST_INVALID		= -1,
		ST_BOY			= 0,		//��װ
		ST_GIRL						//Ůװ
	} SkinType;

	//����ϵ����Ƥ
	//serialIndex����0-based
	bool LoadSkin(SkinType st, unsigned char serialIndex=0);

	//�������λ���ӳ��
	void ChangeSEMMtl(const TCHAR* szEnvMap);

private:
	BOOL ReadAABBTree(IFS *pFS,LPCTSTR szFile);
	void WriteAABBTree(LPCTSTR szFile);
	
	AABBTreeBuilder		m_builder;
	Cool3D::EVertType	m_vertType;
	AABBTreeCollider	*m_pCDTree;
	BYTE *m_pVB;

private:
	class Member;
	Member *m_p;
};
