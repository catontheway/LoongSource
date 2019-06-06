#pragma once
#include "..\WorldDefine\container_define.h"
#include "..\WorldDefine\specfunc_define.h"
#include "GameClientDefine.h"

/** \���������Ʒ����
*/
struct tagUpdateLPItemEvent : public tagGameEvent
{
	EItemConType	eType;
	DWORD			dwTypeID;
	INT16			n16Index;

	tagUpdateLPItemEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{eType = EICT_Null; n16Index = GT_INVALID;}
};

/** \�Ҽ���������е���Ʒ
*/
struct tagClickItemEvent : public tagGameEvent
{
	EItemConType	eType;
	INT16			n16Index;
	DWORD			dwTypeID;

	tagClickItemEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{eType = EICT_Null; n16Index = GT_INVALID; dwTypeID = GT_INVALID;}
};

/** \��Ʒ��ͬ�������ƶ�
*/
struct tagItemPosChangeExEvent : public tagGameEvent
{
	EItemConType	eTypeSrc;
	EItemConType	eTypeDst;
	INT16			n16IndexSrc;
	DWORD			dwTypeIDSrc;

	tagItemPosChangeExEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{eTypeSrc = eTypeDst = EICT_Null; n16IndexSrc = GT_INVALID; dwTypeIDSrc = GT_INVALID;}
};

/** \���⹦����Ʒʹ�õ�ѡ��ƷΪ�����������
*/
struct tagOnTheRoleOfItemEvent : public tagGameEvent
{
	EItemConType		eTypeSrc;
	EItemConType		eTypeDst;
	INT16				n16IndexSrc;
	INT16				n16IndexDst;
	DWORD				dwTypeIDSrc;
	DWORD				dwTypeIDDst;
	EItemSpecFunc		eSpecFuncSrc;

	tagOnTheRoleOfItemEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{
		eTypeSrc = eTypeDst = EICT_Null;
		n16IndexSrc = n16IndexDst = GT_INVALID;
		dwTypeIDSrc = dwTypeIDDst = GT_INVALID;
		eSpecFuncSrc = EISF_Null;
	}
};

/** \������Ʒ
*/
struct tagPutDownItemEvent : public tagGameEvent
{
	EDragType	eType;
	DWORD		dwParam;
	DWORD		dwTypeID;
	
	tagPutDownItemEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame){}
};

/** \������Ʒ
*/
struct tagBuyItemEvent : public tagGameEvent
{
	INT16			n16Pos;
	INT16			n16Num;
	DWORD			dwTypeID;

	tagBuyItemEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName, pSenderFrame){}
};

/** \���ӻ�ɾ����Ʒlog
*/
struct tagCombatMsgForItemChange : public tagGameEvent
{
	DWORD			dwTypeID;	// ��Ʒԭ��ID
	INT16			n16Num;		// ��Ʒ����
	

	tagCombatMsgForItemChange(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName, pSenderFrame){}
};

/** \�������һ�ֿ�
*/
struct tagItemContainerExtend : public tagGameEvent
{
	bool bUseSilver;	// �Ƿ����Ľ�Ǯ���ǣ����Ľ�Ǯ��������Ԫ��

	tagItemContainerExtend(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent( szEventName, pSenderFrame ){}
};