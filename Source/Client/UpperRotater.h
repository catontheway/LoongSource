#pragma once

class Role;
/** \class UpperRotater
	\brief ������ת������
*/
class UpperRotater
{
public:
	UpperRotater(Role *pRole);
	~UpperRotater(void);

	/** ��ʼ��ת
	*/
	void Start(float destYaw);
	/** ��ת����
	*/
	void Update();
	/** ���ص�ǰ�Ƿ�������ת��
	*/
	bool IsRotating();
	/** ������ת���ջ���
	*/
	float GetDestYaw();

private:
	void GetAniCtrl(void);

private:
	Role	*m_pRole;
	NodeAnimationCtrl *m_pAniCtrl;
	float	m_srcYaw;
	float	m_dstYaw;
	float	m_startTime;
	float	m_totalTime;
	bool	m_bStarted;
	bool	m_bInit;
};
