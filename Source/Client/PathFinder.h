#pragma once

class LocalPlayer;
/** \class PathFinder
	\brief Ѱ·��
*/
class PathFinder
{
	enum EStep//Ѱ·�׶�
	{
		EStep_EnterRoad,	//�ǵ�·����Ѱ·�����Խ����·����
		EStep_OnRoad,		//��·����Ѱ·
		EStep_LeaveRoad,	//��·����Ѱ·��ɣ��ڷǵ�·����Ѱ·
	};
public:
	PathFinder(void);
	virtual ~PathFinder(void);

	bool Init(const Vector3& startPos,const Vector3& endPos,NavMap* pNav,LocalPlayer* pRole,bool bOnLand);
	bool GetPathPoint(NavMap* pNav,LocalPlayer* pRole,Vector3& out);

private:
	bool PreparePath_EnterRoad(NavMap* pNav,LocalPlayer* pRole);
	bool PreparePath_OnRoad(NavMap* pNav,LocalPlayer* pRole);
	bool PreparePath_LeaveRoad(NavMap* pNav,LocalPlayer* pRole);

protected:
	bool					m_bOnLand;		//�Ƿ���½��
	list<Point>				m_patchPath;	//���ڵؿ�Ѱ·���ص�·��
	list<Vector3>			m_tilePath;		//���ڸ���Ѱ·���ص�·��
	Vector3					m_endPos;		//����Ѱ·�յ�
	EStep					m_step;			//Ѱ·�׶�
	PlayerNavMap::EResult	m_resultByPatch;//����ؿ�Ѱ·����ֵ
};
