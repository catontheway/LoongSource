#pragma once

class Role;
/** \class RoleRotater
	\brief ��ɫ��ת������
*/
class RoleRotater
{
public:
	enum EType
	{
		ET_None = -1,
		ET_Pitch,
		ET_Yaw,
	};
	RoleRotater(void);
	~RoleRotater(void);

	/** �󶨽�ɫ
	*/
	void BindRole(Role* pRole,EType type,bool setTime,float rotateSpeed);
	/** ��ʼ��ת
	*/
	void Start(float destAngle);
	/** ��ת����
	*/
	void Update();
	/** ���ص�ǰ�Ƿ�������ת��
	*/
	bool IsRotating();
	/** ������ת���ջ���
	*/
	float GetDestAngle();

private:
	bool  m_bSetTime;     // true: m_paramΪ��תʱ�� false: m_paramΪ��ת�ٶ�
	float m_param;
	EType m_type;
	Role* m_pRole;
	float m_srcAngle;
	float m_dstAngle;
	float m_startTime;
	float m_totalTime;
	bool  m_bStarted;
};
