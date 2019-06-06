#pragma once
#include "..\WorldDefine\role_att.h"

/** ����������Գ�ʼ��
*/
struct tagInitLPAttEvent : public tagGameEvent
{
	tagInitLPAttEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame){}
};

/** ����������Ը���
*/
struct tagUpdateLPAttEvent : public tagGameEvent
{
	ERoleAttribute	eType;
	
	tagUpdateLPAttEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{eType = ERA_Null;}
};


/** Զ��������Ը���
*/
struct tagUpdateRRAttEvent : public tagGameEvent
{
	DWORD			dwRoleID;
	ERoleAttribute	eType;

	tagUpdateRRAttEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{dwRoleID = GT_INVALID; eType = ERA_Null;}
};

/** ���װ����۱仯
*/
struct tagUpdataAvatarEvent : public tagGameEvent
{
	DWORD           dwRoleID;
	
	tagUpdataAvatarEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : 
	tagGameEvent(szEventName,pSenderFrame), dwRoleID( GT_INVALID ) {}
};

/** �����ʾ���ñ仯
*/
struct tagUpdataAvatarDisplaySetEvent : public tagGameEvent
{
	DWORD           dwRoleID;

	tagUpdataAvatarDisplaySetEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : 
	tagGameEvent(szEventName,pSenderFrame), dwRoleID( GT_INVALID ) {}
};

/** �����������
*/
struct tagWeaponChangeEvent : public tagGameEvent
{
	DWORD           dwRoleID;
	BOOL            bRightWeapon;  // �Ƿ���������
	tagWeaponChangeEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : 
	tagGameEvent(szEventName,pSenderFrame), dwRoleID( GT_INVALID ), bRightWeapon( FALSE) {}
};

/** ������ҵȼ��ı�
*/
struct tagUpdateLPLevel : public tagGameEvent
{
	INT		nCurLevel;	// ��ǰ�ȼ�
	INT		nInc;		// �ȼ��仯��
	BOOL	bLevelUp;	// �Ƿ�������	

	tagUpdateLPLevel(LPCTSTR szEventName, GameFrame* pSenderFrame) :
	tagGameEvent(szEventName,pSenderFrame){}
};

/** Զ����ҵȼ��ı�
*/
struct tagUpdateRRLevel : public tagGameEvent
{
	DWORD	dwRoleID;
	INT		nCurLevel;
	BOOL	bLevelUp;

	tagUpdateRRLevel(LPCTSTR szEventName, GameFrame* pSenderFrame) :
	tagGameEvent(szEventName,pSenderFrame){}
};

/** ������Ҿ���仯
*/
struct tagUpdateLPLevelExp : public tagGameEvent
{
	INT nCurLevelExp;
	INT nExpInc;

	tagUpdateLPLevelExp(LPCTSTR szEventName, GameFrame* pSenderFrame) :
	tagGameEvent(szEventName,pSenderFrame){}
};

/** ��ɫbuff�б�仯
*/
struct tagRoleBuffChangeEvent : public tagGameEvent
{
	DWORD dwRoleID;

	tagRoleBuffChangeEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) :
	tagGameEvent(szEventName,pSenderFrame){}
};

/** �������״̬�仯
*/
struct tagLPRoleStateChangeEvent : public tagGameEvent
{
	DWORD dwOldState;
	tagLPRoleStateChangeEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) :
	tagGameEvent(szEventName,pSenderFrame),dwOldState(0){}
};

/** Զ�����״̬�仯
*/
struct tagRPRoleStateChangeEvent : public tagGameEvent
{
	DWORD dwRoleID;
	DWORD dwOldState;
	tagRPRoleStateChangeEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) :
	tagGameEvent(szEventName,pSenderFrame),dwOldState(0){}
};

/*! \struct tagFashionEvent
\brief ��ɫ�л�ʱװģʽ�¼�
*/
struct tagFashionEvent : public tagGameEvent
{
	bool		bDisFashion;	/*!< �Ƿ�����ʱװģʽ */
	tagFashionEvent(LPCTSTR szEventName, GameFrame *pSenderFrame)
		: tagGameEvent(szEventName, pSenderFrame){}
};

/** N%������
*/
struct tagPctNExpAchieve : public tagGameEvent
{
	INT		nPctMultiplied;		// N%
	tagPctNExpAchieve(LPCTSTR szEventName, GameFrame *pSenderFrame)
		: tagGameEvent(szEventName, pSenderFrame){}
};

/** ����\�뿪ս��
*/
struct tagLRoleCombatState : public tagGameEvent
{
	BOOL	bEnterCombat;		//true:����ս�� false:�뿪ս��
	tagLRoleCombatState(LPCTSTR szEventName, GameFrame *pSenderFrame)
		: tagGameEvent(szEventName, pSenderFrame),bEnterCombat(TRUE){}
};

//! ��ɫ����³ƺ�
struct tagNewTitleDisp : public tagGameEvent
{
    tstring     strShow;        // �ƺ���
    tagNewTitleDisp(LPCTSTR szEventName, GameFrame* pSenderFrame)
        : tagGameEvent(szEventName, pSenderFrame) {}
};

//! ��ɫɾ���ƺ�
struct tagDelTitleDisp : public tagGameEvent
{
    tstring     strShow;        // �ƺ���
    tagDelTitleDisp(LPCTSTR szEventName, GameFrame* pSenderFrame)
        : tagGameEvent(szEventName, pSenderFrame) {}
};

//! ���ؼ�Զ����Ҹı�ƺ�
struct tagRoleTitleChange : public tagGameEvent
{
    DWORD			dwRoleID;
    UINT16			dwTitleID;		// �����ѡ�����سƺţ����ֶ�ΪGT_INVALID

    tagRoleTitleChange(LPCTSTR szEventName, GameFrame* pSenderFrame)
        : tagGameEvent(szEventName, pSenderFrame) {}
};
