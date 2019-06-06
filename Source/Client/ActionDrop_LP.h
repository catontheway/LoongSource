#pragma once

class LocalPlayer;
/** \class ActionDrop_LP
	\brief ������ҵ�����Ϊ
*/
class ActionDrop_LP
{
	enum EStep
	{
		EStep_Drop,		//������
		EStep_Land,		//���
		EStep_Closed,	//����
	};
public:
	enum EResult
	{
		ER_Unknow,
		ER_Slide,
		ER_VDrop,
		ER_Swim,
		ER_OnWater,
		ER_Land,
	};
public:
	ActionDrop_LP(void);
	virtual ~ActionDrop_LP(void);

	//--
	void Active(const Vector3& start,const Vector3& dir,NavMap* pNav,LocalPlayer* pRole);
	void Update(bool bPlayLandAni,NavMap* pNav,LocalPlayer* pRole);
	bool IsClosed(){ return m_step==EStep_Closed;}
	EResult GetResult(){ return m_result;}
	bool IsLanding(){ return m_step==EStep_Land;}

	void ReplayAni(LocalPlayer* pRole);

private:
	NavCollider_Jump		m_collider;
	EStep					m_step;
	EResult					m_result;
	bool					m_bPlayInEffect;//�Ƿ��Ѳ�����ˮ��Ч
};
