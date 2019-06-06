#pragma once

const float g_collideStep		= 50.0f;		//��ײ����
const float g_moveFall			= 100.0f;		//�ƶ�ʱ����������
const float g_floatMax			= 1000000.0f;	//��󸡵���
const float g_tileSize			= 50.0f;		//���Ӵ�С
const float g_slideFall			= 50.0f;		//���θ߶�ͼ�����㻬�����
const float g_gravitation		= 1200.0f;		//�������ٶ�		
const float g_halfGravitation	= 600.0f;		//�������ٶ�*0.5f			


inline POINT _World2Tile(Vector3& pos)
{
	POINT tile;
	tile.x=long(pos.x/g_tileSize);
	tile.y=long(pos.z/g_tileSize);
	return tile;
}

inline POINT _World2Tile(float x,float z)
{
	POINT tile;
	tile.x=long(x/g_tileSize);
	tile.y=long(z/g_tileSize);
	return tile;
}