#pragma once
#include "servermerge.h"
#include "Loong_CommonDef.h"
#include "Loong_MergeLogicCtrl.h"

//ǰ������
class CLoongMergeLogicCtrl;
struct tagThreadPara;

class CLoongServerMerge :public CServerMerge
{
public:
	CLoongServerMerge(void);
	virtual ~CLoongServerMerge(void);

//�ӿ�ʵ��
public:
	/*
	/	��ʼ��
	*/
	BOOL Init(tagConfigDetail stConfigOne[EM_DB_TYPE_END],
				tagConfigDetail stConfigTwo[EM_DB_TYPE_END],
				EDBMergeType eMergeType,
				tstring strSrc2WorldName,
				const std::multimap<ERoleCompensationType, tagCompensation*> mmCompensation);
	/*
	/	��ʼ�ϲ�
	*/
	BOOL ServerMerge();

public:
	//�ϲ��߼�
	CLoongMergeLogicCtrl m_clsMergeLogicCtrl;

	//�����̳߳غϲ����¼�
	HANDLE				m_hPoolMergeEvent;

private:
	//��ʼ���ϲ��߼�
	BOOL LoongMergeLogicCtrlInit(CLoongMergeLogicCtrl &clsMergeLogicCtrl);

	//�õ��̳߳�״̬
	EThreadPoolState GetThreadPoolState();

	//��Ҳ���
	BOOL LoongRoleCompensation();

	//��������
	BOOL LoongRoleCompensationChangeRoleName(const tagRoleCompensation &st);
private:  

	EDBMergeType		m_eMergeType;				// db�ϲ�����

	tstring				m_strSrc2WorldName;			// Դ2������

	DataBase			m_LoongDB[EM_DB_TYPE_END];	// Beton���ݿ�

	std::list<HANDLE>	m_listSimpleThreadPool;		// ���̳߳�

	std::vector<tagThreadPara*> vecParaList;		

	std::multimap<ERoleCompensationType, tagCompensation*> m_mmCompensation; // ��Ҳ���
};

//����
struct tagThreadPara
{
	INT32 _n;
	CLoongServerMerge* const _p;

	tagThreadPara(INT32 n,CLoongServerMerge* const p):_n(n),_p(p)
	{

	}

	~tagThreadPara()
	{

	}		
};


UINT MyThreadPool(LPVOID pParam);
