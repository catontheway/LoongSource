#pragma once

class Role;
/** \class RoleScaler
	\brief ��ɫ���ſ�����
*/
class RoleScaler
{
public:
	RoleScaler(void);
	~RoleScaler(void);

	/** �󶨽�ɫ
	*/
	void BindRole(Role* pRole)	{ m_pRole=pRole;}
	/** ��ʼ����
	*/
	void Start(const Vector3& destScale);
	/** ���Ŵ���
	*/
	void Update();
	/** ���ص�ǰ�Ƿ�����������
	*/
	bool IsScaling() const;
	/** ������������
	*/
	const Vector3& GetDestScale() const;

private:
	Role* m_pRole;
	Vector3 m_srcScale;
	Vector3 m_dstScale;
	float m_startTime;
	float m_totalTime;
	bool  m_bStarted;
};
