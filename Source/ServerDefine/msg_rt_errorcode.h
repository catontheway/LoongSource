//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_rt_errorcode.h
// author: 
// actor:
// data: 2009-03-27
// last:
// brief: ���ظ�Զ�̼�ع��ߵĴ�����
//-----------------------------------------------------------------------------
#pragma once


//-----------------------------------------------------------------------------
enum
{
	E_RT_Success				= 0,

	// �̳����
	E_Mall_ProtoLoad_Failed		= 1,	// �̳���Ʒ��̬���Զ�ȡʧ��
	E_Mall_Init_Failed			= 2,	// �̳ǳ�ʼ��ʧ��

	// ��ͣ����˺�ʱloginserver���ظ�GMserver����Ϣ
	E_Login_GMServer_Forbidded	= 10,	// �˺��Ѿ�����
	E_Login_GMServer_Freed		= 11,	// �˺��Ѿ����
	E_Login_GMServer_NoAccount	= 12,	// �˺Ų�����
	
	E_RT_Section_Error			= 20,	// ����������
	E_RT_Error,	
};