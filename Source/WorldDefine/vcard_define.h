//-----------------------------------------------------------------------------
//!\file vcard_define.h
//!\author client/server hyu/xlguo
//!
//!\date 2009-02-03
//! last 
//!
//!\brief ��ɫ����ͨ���ýṹ LoongWorld<->LoongDB  LoongWorld<->client
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once
#include "msg_common_errorcode.h"

#pragma pack(push,1)

//----------------------------------------------------------------------------
// 12����
//----------------------------------------------------------------------------
enum EnumVCardConstellation
{
	EVC_Aries			=		1,		//������(3.21-4.20)
	EVC_Taurus			=		2,		//��ţ��(4.21-5.21)
	EVC_Gemini			=		3,		//˫����(5.22-6.21)
	EVC_Cancer			=		4,		//��з��(6.22-7.22)
	EVC_Leo				=		5,		//ʨ����(7.23-8.23)
	EVC_Virgo			=		6,		//��Ů��(8.24-9.23)
	EVC_Libra			=		7,		//�����(9.24-10.23)
	EVC_Scorpio			=		8,		//��з��(10.24-11.22)
	EVC_Sagittarius		=		9,		//������(11.23-12.21)
	EVC_Capricorn		=		10,		//Ħ����(12.22-1.20)
	EVC_Aquarius		=		11,		//ˮƿ��(1.21-2.19)
	EVC_Pisces			=		12,		//˫����(2.20-3.20)

	EVC_Default			=		13		//Ĭ�ϱ���
};

//----------------------------------------------------------------------------
// 12��Ф
//----------------------------------------------------------------------------
enum EnumVCardChineseZodiac
{
	EVCZ_Rat				=		1,		//����
	EVCZ_Ox					=		2,		//��ţ
	EVCZ_Tiger				=		3,		//����
	EVCZ_Rabbit				=		4,		//î��
	EVCZ_Loong				=		5,		//����
	EVCZ_Snake				=		6,		//����
	EVCZ_Horse				=		7,		//����
	EVCZ_Goat				=		8,		//δ��
	EVCZ_Monkey				=		9,		//���
	EVCZ_Rooster			=		10,		//�ϼ�
	EVCZ_Dog				=		11,		//�繷
	EVCZ_Boar				=		12,		//����

	EVCZ_Default			=		13		//Ĭ�ϱ���
};

//----------------------------------------------------------------------------
// �ձ�
//----------------------------------------------------------------------------
enum EnumVCardSex
{
	EVS_Male				=		1,		//��
	EVS_Female				=		2,		//Ů
	EVS_Default				=		3,		//Ĭ�ϱ���
};

//----------------------------------------------------------------------------
// ������ö��
//----------------------------------------------------------------------------
enum
{
	E_VCard_Success					= E_Success,	// �ɹ�
	E_VCard_NotVisible				= 1,			// �������ɼ�
	E_VCard_NoPrivilege				= 2,			// �޸ı�Ȩ��
	E_VCard_Failed					= 3,			// ��������
};

enum
{
	LEN_CITY_NAME					= 6,
	LEN_MAX_URL						= 200,
	LEN_MAX_SIGNATURE				= 200,
};

//-----------------------------------------------------------------------------
// �û��Զ�����Ϣ����(LoongWorld<->LoongDB��client->LoongWorld)
//-----------------------------------------------------------------------------
struct tagCustomVCardData
{
	BOOL	bVisibility;			// �ɼ���
	BYTE	bySex;					// �Ա�Ĭ��Ϊ��ɫ�Ա���3���ѡ���С�Ů�����ܡ�
	BYTE	byConstellation;		// �����������˵�ѡ����13����ѡ���ֱ�Ϊ12�����ͱ��ܣ�Ĭ��Ϊ���ܡ�
	BYTE	byChineseZodiac;		// ��Ф�������˵�ѡ����13����ѡ
	BYTE	byArea;					// ����0Ϊ�գ�
	BYTE	byProvince;				// ʡ�ݣ�0Ϊ�գ�

	TCHAR	chCity[LEN_CITY_NAME];	// ����(��'\0'����)
	BYTE	byHeadUrlSize;			// ͷ��ߴ�(byte����)
	BYTE	bySignatureSize;		// ǩ���ߴ� ���200(byte����)
	BYTE	byNameHistory;			// ������¼
	BYTE	byData[1];				// ͷ��Ĭ��Ϊ��ɫ�Ա�ͷ�񡣸���ǩ�������100�����֡�200���ַ���(��'\0'����); ������¼
};

//-----------------------------------------------------------------------------
// ��������(LoongWorld->client)
//-----------------------------------------------------------------------------
struct tagVCardData
{
	DWORD				dwRoleID;			// ��ɫID����ʾ��ҵĽ�ɫ���ơ�
	DWORD				dwLevel;			// ��ɫ�ȼ�����ʾ��ҵĽ�ɫ�ȼ���
	DWORD				dwJob;				// ��ɫְҵ����ʾ��ҵĽ�ɫְҵ��δתְ��ɫ��ʾΪ�����ˡ�
	DWORD				dwMateRoleID;		// ��ɫ��ż����ʾ��ҵĽ�ɫ��ż��δ���ɫ��ʾΪ��
	DWORD				dwFactionID;		// �������ɣ���ʾ��ҵ������������ƣ��ް��������ʾΪ�ա�
	DWORD				dwPosition;			// ����ְλ����ʾ��ҵİ���ְλ���ް��������ʾΪ�ա�
	
	tagCustomVCardData 	customVCardData;	// �û��Զ�������
};
#pragma pack(pop)