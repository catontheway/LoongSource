//-----------------------------------------------------------------------------
//!\file title_mgr.h
//!\author xlguo
//!
//!\date 2009-01-18
//! last 2009-02-02
//!
//!\brief �ƺ���Ϣ�������
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------

#pragma once

// project
#include "..\ServerDefine\title_define.h"
#include "..\WorldDefine\msg_role_title.h"
#include "att_res.h"

// stl
#include <bitset>
#include <vector>
#include <list>
#include <algorithm>

using std::bitset;
using std::vector;
using std::list;
using std::find;

class TitleMgr
{
	typedef list<UINT16>					ListUINT16;
	typedef vector<UINT16>					VecUINT16;
	typedef bitset<MAX_TITLE_NUM>			BitSet;

public:
	~TitleMgr(){	Destroy();	}

	// 1��ʼ��ѡ��
	void InitOpts(Role* pRole, UINT16 u16ActTitleID, BOOL bVisibility);

	// 2��ʼ���ƺ�����
	void InitTitles(const BYTE* &pData, const INT32 nNum);

	// ��������
	void Destroy();

public:
	// �洢�����ݿ�
	void SaveTitlesToDB(IN LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum);

	// ���ź�
	bool SigEvent(EnumTitleEvent titleEvent, DWORD dwArg1, DWORD dwArg2);

	// ���óƺſɼ���
	DWORD SetVisibility(bool bVisibility)	{	m_bVisibility = bVisibility;	return E_Title_Success;	}

	// ���ؿɼ���
	bool Visibility() const { return TRUE == m_bVisibility; }

	// ����ƺ�
	DWORD ActiviteTitle(const UINT16 u16TitleID);
	
	// ��ǰʹ�óƺ�
	UINT16 GetActiviteTitle() const {	return m_u16ActiveID;	}

	// ȡ�������ѻ�óƺ�����
	DWORD GetObtainedTitleIDs(UINT16* &pData, UINT16 &u16Num);

	// �ѻ�óƺ���Ŀ
	DWORD GetObtainedTitlesNum() {	return m_bitsetObtainedMark.count();}

	// �����ƺ�����
	DWORD ReturnTitlesData(LPVOID pData) {	SAFE_DEL(pData);	return E_Title_Success;	}

	//ɾ���ƺ�
	BOOL SigRemoveEvent( EnumTitleEvent titleEvent, DWORD dwArg1 = GT_INVALID);
private:
	// �ƺŲ��뵽���ݿ�
	VOID InsertTitle2DB( UINT16 u16TitleID, BOOL bNew );

	// �ƺŴ����ݿ�ɾ��
	VOID RemoveTitleFromDB( UINT16 u16TitleID);

	// �����ϵ
	void CleanRelation(const UINT16 u16TitleID);

	// ֪ͨ�ͻ����»�õĳƺ�
	void NotifyClient(VecUINT16& vecNewObtTitles);
	// ֪ͨ�ͻ��� ��Ҷ�ʧ�˳ƺ�
	void NotifyClientDelete(VecUINT16& vecDelTitleIDs);
private:
	UINT16								m_u16ActiveID;						// ��ǰ�ƺ�
	BOOL								m_bVisibility;						// �ɼ���

	ListUINT16							m_EventMap[ETE_MAX_EVENTNUM];		// �¼�ӳ��
	Condition*							m_ArrPtrCondition[MAX_TITLE_NUM];	// ����ָ������

	BitSet								m_bitsetObtainedMark;				// �ƺ��Ƿ��ѻ��
	BitSet								m_bitsetDBInserted;					// �Ѳ������ݿ�
	BitSet								m_bitsetNeedSaveDB;					// ��Ҫ���浽���ݿ�

	Role*								m_pRole;							// ��ɫָ��
};

