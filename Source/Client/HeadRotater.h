#pragma once

class Role;
/** \class HeadRotater
	\brief ������ת������
*/
class HeadRotater
{
public:
	HeadRotater(Role* pRole);
	~HeadRotater(void);

	/** ��ʼ��ת
	*/
	void Start(float destYaw,float destPitch);
	/** ��ת����
	*/
	void Update(void);
	/** ���ص�ǰ�Ƿ�������ת��
	*/
	bool IsRotating(void)		const;
	/** ������ת���ջ���
	*/
	float GetDestYaw(void)		const	{ return m_dstYaw; }
	float GetDestPitch(void)	const	{ return m_dstPitch; }

private:
	void GetAniCtrl(void);

private:
	Role	*m_pRole;
	NodeAnimationCtrl *m_pAniCtrl;
	float	m_srcYaw;
	float	m_dstYaw;
	float	m_srcPitch;
	float	m_dstPitch;
	float	m_yawStartTime;
	float	m_yawTotalTime;
	bool	m_bYawStarted;
	bool	m_bPitchStarted;
	float	m_pitchStartTime;
	float	m_pitchTotalTime;
	bool	m_bInit;
};
