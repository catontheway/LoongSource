//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: sspawnpoint_define.h
// author: xlguo
// actor:
// data: 2009-03-20
// last:
// brief: �������÷Ǹ���ˢ�ֵ�ԭ��
//-----------------------------------------------------------------------------

// ÿ��ˢ�ֵ�����������
const INT MAX_CREATURE_PER_SSPAWNPOINT	= 20;

//--------------------------------------------------------------------------------
// ��������ˢ�ֵ�ṹ����ͨ��ͼ��
//--------------------------------------------------------------------------------
struct tagSSpawnPointProto
{
	DWORD				dwSpawnPointID;								// ˢ�ֵ�id
	DWORD				dwTypeIDs[MAX_CREATURE_PER_SSPAWNPOINT];	// ����typeid

	tagSSpawnPointProto() { ZeroMemory(this, sizeof(*this)); }
};
