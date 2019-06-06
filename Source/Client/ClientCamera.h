#pragma once

/** ��ת��ͷ��Ϸ�¼�
*/
struct tagRotateCameraEvent : public tagGameEvent
{
	tagRotateCameraEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};

class Role;
/**	\class ClientCamera 
	\brief �ͻ��˾�ͷ����
*/
class ClientCamera : public CameraEuler
{
public:
	VOID SetDefault(FLOAT fZFar);
	VOID SetFov(float deg);

	VOID Update(Role *pLookRole,NavMap *pNavMap);
	
	VOID PlayQuake();	//! ��ͷ��

	VOID EnableFlyMode(bool enable)	{m_bFly=enable;}
	bool IsFly() { return m_bFly; }

    VOID SetHeightRate(FLOAT rate) { 
        m_HeightScaleRate = rate;
    }

	ClientCamera();

private:
	TObjRef<InputDX8>			m_pInput;
	TObjRef<Console>			m_pConsole;
	TObjRef<GameFrameMgr>		m_pFrmMgr;

	POINT m_ptLast;
	
	float m_fExpectDist;		// ��������
	float m_fOccultDist;		// ��ͷ����סʱ��dist
	DWORD m_dwLastOccultTime;	// ��ͷ�ϴα���ס��ʱ��
	

	bool m_bFly;				// �Ƿ��Ƿ���ģʽ
	bool m_bRotating;			// �����������ת��ͷ

	bool m_bQuaking;			// �Ƿ���о�ͷ�𶯴���
	DWORD m_dwQuakeStartTime;	// ��ͷ�𶯿�ʼʱ��

	BOOL m_bDecTransparent;		
	FLOAT m_fTransparent;

    FLOAT m_HeightScaleRate;    //! ��ͷLookAt�ĸ߶����ű��� add by hyu
	
	virtual void Occult(NavMap *pNavMap);
	void UpdateQuake(Vector3& nextLookAt);

	void UpdateRoleTransparent();
};





