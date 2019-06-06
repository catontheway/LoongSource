#pragma once

//-------------------------------------------------
//  Game World �� server ����
//-------------------------------------------------
enum EServerType {
	EST_World,		//��Ϸ���������
	EST_DB,			//��Ϸ�������ݿ������
	EST_Login,		//��Ϸ����login������	
};

//--------------------------------------------------
// �ͻ���Ȩ��
//--------------------------------------------------
enum EPrivilege
{
	EP_Null		=	-1,		// δ��ʼ��
	EP_Pri_GM_D	=	0,		// 1��Ȩ��
	EP_Pri_GM_C	=	1,		// 2��Ȩ��
	EP_Pri_GM_B	=	2,		// 3��Ȩ��
	EP_Pri_GM_A	=	3,		// 4��Ȩ��
	EP_Pri_GM_S	=	4,		// 5��Ȩ��

	// (6������ΪԶ�̼��ʹ��)
	EP_Pri_Rt_B	=	5,		// 6��Ȩ��
	EP_Pri_Rt_A	=	6,		// 7��Ȩ��
	EP_Pri_Rt_C	=	7,		// 8��Ȩ��(��������������������)
	EP_Pri_Rt_D	=	8,		// 9��Ȩ��(�����̳���)
	EP_End		=	9
};

#define MIS_GMTOOL_PRI(ePri)	((ePri) >= EP_Pri_GM_D && (ePri) <= EP_Pri_GM_S)
#define MIS_RTCLIENT_PRI(ePri)	((ePri) >= EP_Pri_Rt_B && (ePri) <= EP_Pri_Rt_D)

#define MIS_GMOPGM(root, gm)		(MIS_GMTOOL_PRI(root)		&& MIS_GMTOOL_PRI(gm) )
#define MIS_RTOPRT(root, rt)		(MIS_RTCLIENT_PRI(root)		&& MIS_RTCLIENT_PRI(rt) )