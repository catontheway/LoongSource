#pragma once

#include "..\WorldDefine\RoleDefine.h"
#include "CombatSysUtil.h"
#include "Render2D.h"
class Player;

//һ���򵥵������������
class SimpleMapCamera : public CameraEuler
{
public:
	SimpleMapCamera(void);

	void Update(const Vector3& nextLookAt);

private:
	mutable Vector3 m_targetLookAt;
};

#define MAX_ROLE 5
/*
	class: SeleRoleMgr
	brief: ����ѡ�����Ľ�ɫ��Ϣ��node��ѡ������봴������ĳ������������Ϊͬһ����
*/
class SeleRoleMgr
{
public:
	struct tagSeleRole
	{
		tagSimRole	simRole;
		Player*		pPlayer;
	};

	struct tagSeleRoleHead
	{
		tstring		strName;			//��ɫ����
		Vector3		scrPos;				//��ɫͷ����Ļ����λ��
		Vector3		worldPos;			//��ɫͷ������������
		float		scale;

		tagSeleRoleHead()
		{
		}

		tagSeleRoleHead(const tagSeleRoleHead& other)
		{
			Copy(other);
		}

		const tagSeleRoleHead& operator = (const tagSeleRoleHead& other)
		{
			if(this!=&other)
				Copy(other);
			return *this;
		}

	private:
		void Copy(const tagSeleRoleHead& other)
		{
			strName		= other.strName;
		}

	};

	enum 
	{
		ESelect_Role = 0,
		ECreate_Role = 1,
	};
public:
	SeleRoleMgr(void);
	~SeleRoleMgr(void);
	static SeleRoleMgr* Inst();

	// ��ʼ���ճ���
	VOID Init();

	// ���ݲ�ͬ�ĳ��������������λ��
	VOID SetCamera(const Vector3& from, const Vector3& lookat);
	VOID SetCamera(const Vector3& lookat, float fYaw, float fPitch, float fRoll, float fDist);

	void OpenMap(const TCHAR* szMapName, CameraBase* pBindCamera, DWORD dwType=0);
	void CloseMap();

	// �õ������
	SimpleMapCamera* GetCamera() { return &m_Camera; }

	// �õ�����
	NullSceneGraph* GetNullSG() { return m_pNullSG; }
	HugeQuadTreeSG* GetHugeSG() { return m_pSG; }

	//	���³���
	VOID UpdateSG(float deltaTime);

	// ɾ��ȫ����ɫ��Ϣ��ģ��
	VOID Destroy(SceneGraph* pSG);
	VOID Destroy();

	// ����һ����ɫʱ�����������뵽������
	VOID CreateRole(tagSimRole tagRole, Player* pNode/*, SceneGraph* pSG*/);

	// ��ӵ�������
	VOID AttachSceneNodeToSG(SceneGraph* pSG);

	// �ӳ������Ƴ�
	VOID DetachSceneNodeFromSG(SceneGraph* pSG);

	// ɾ��һ��ָ���Ľ�ɫ
	VOID DeleteRole(DWORD dwRoleID, SceneGraph* pSG);

	// ��ý�ɫ�ĸ���
	int GetRoleNum()const { return (int)m_vecSeleRole.size(); }

	// ���ָ����ɫ�Ļ�������
	const tagSimRole& GetRoleInfo(int i)const;

	// ���ָ����ɫ��ID
	DWORD GetRoleId(int i)const;

	// 
	bool IsReadyTrackEnd(int i)const;

	void PlayReadyTrack(int i)const;

	void PlayStandTrack(int i)const;

	void PlayStandUpTrack(int nCur, int nLast)const;

	void IsStandUpTrack(int nCur)const;
	void IsSitDownTrack(int nLast)const;

	void PlayIdleTrack()const;

	void RefreshTrack();

	const TCHAR* GetHandUpName(EItemTypeEx eWeapon)const;

	// �ж�ͨ�����ѡ��Ľ�ɫ
	int GetMouseSelectRole(int mouseX,int mouseY);

	// 
	VOID Update();

	void SetSafeCodeCrc(DWORD dwCrc) { m_dwSafeCodeCrc = dwCrc; }
	void SetDWORDTime(tagDWORDTime& time) { m_tagTime = time; }
	void SetInitSafeCode(BOOL bVal) { m_bInitSafeCode = bVal; }
	DWORD GetSafeCodeCrc() { return m_dwSafeCodeCrc; }
	tagDWORDTime& GetDWORDTime() { return m_tagTime; }
	BOOL GetInitSafeCode() { return m_bInitSafeCode; }

	void SetLastLoginInfo(tagDWORDTime time, DWORD dwIPLast, DWORD dwIP) { m_nTimeLastLogin=time; m_nIPLast=dwIPLast; m_nIP = dwIP; }
	tagDWORDTime& GetTimeLastLogin() { return m_nTimeLastLogin; }
	DWORD GetIPLastLogin() { return m_nIPLast; }
	DWORD GetIPCurrent() { return m_nIP; }

	void DrawSelectedRoleFlag(int nRole);

private:
	TSFPTrunk<SeleRoleMgr>	m_Trunk;
	vector<tagSeleRole>		m_vecSeleRole;		//������ҵĽ�ɫ��Ϣ
	NullSceneGraph*			m_pNullSG;				//����
	HugeQuadTreeSG*			m_pSG;
	SimpleMapCamera			m_Camera;			//�����
	EntityNode*				m_EntityNode;		//����
	GameMap					m_gameMap;
	TObjRef<GUIRender>		m_pRender;

	DWORD					m_dwSafeCodeCrc;
	tagDWORDTime			m_tagTime;
	BOOL					m_bInitSafeCode;
	INT						m_nWaterReflect;
	INT						m_nWaterRefract;
	tagDWORDTime			m_nTimeLastLogin;
	DWORD					m_nIPLast;
	DWORD					m_nIP;
	tagSeleRoleHead			m_tagHead;
	IDepthFont*				m_pFont;
	DWORD					m_dwType;


private:
	//���¼��������λ��
	VOID ResetPostion();
};
