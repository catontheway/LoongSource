//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: path_finder.h
// author: wjiang
// actor:
// data: 2009-5-31
// last:
// brief: ·��Ѱ����
//-------------------------------------------------------------------------------------------------------
#pragma once
#include <list>

class Unit;
class Creature;

//-----------------------------------------------------------------------------
// ·��Ѱ����
//-----------------------------------------------------------------------------
class PathFinder
{
public:
	PathFinder() {}
	virtual ~PathFinder() {}

public:
	virtual BOOL	Init(Unit* pUnit) = 0;
	virtual BOOL	FindingPath(const Vector3& startPos,const Vector3& targetPos) = 0;
	virtual BOOL	GetCurPathPoint(const Vector3& startPos, Vector3& movePos) = 0;

protected:
	BOOL									m_bMoveingPathPoint;	// �Ƿ�����ʹ��Ѱ·�б��е�·����
	INT										m_nSynMoveTick;			// ͬ���ͻ����ƶ��ļ��
};

//-----------------------------------------------------------------------------
// ��ײ��·��Ѱ����
//-----------------------------------------------------------------------------
class NPCPathFinderCol : public PathFinder
{
public:
	NPCPathFinderCol() {}
	~NPCPathFinderCol();

public:
	BOOL	Init(Unit* pUnit);
	BOOL	FindingPath(const Vector3& startPos,const Vector3& targetPos);
	BOOL    GetCurPathPoint(const Vector3& startPos, Vector3& movePos);

private:
	Creature*								m_pOwner;

	std::list<Vector3>						m_listPathPoint;		// ����׷��Ѱ·ʱ��·����
	std::list<Vector3>::iterator			m_PathPointIt;			// ��ǰѰ��·�������һ����
};

//-----------------------------------------------------------------------------
// ����ײ��·��Ѱ����
//-----------------------------------------------------------------------------
class NPCPathFinder : public PathFinder
{
public:
	NPCPathFinder() {}
	~NPCPathFinder();

public:
	BOOL	Init(Unit* pUnit);
	BOOL	FindingPath(const Vector3& startPos,const Vector3& targetPos);
	BOOL	GetCurPathPoint(const Vector3& startPos, Vector3& movePos);

private:
	Creature*								m_pOwner;

	std::list<POINT>						m_listPathPoint;		// ����׷��Ѱ·ʱ��·����
};
