#include "StdAfx.h"
#include "wx/wx.h"
#include "Resource.h"
#include "Loong_ServerMerge.h"
#include "Loong_LoginDBCtrl.h"
#include "Loong_LoongDBCtrl.h"
#include "Loong_OutputCtrl.h"
#include "Loong_TMethod.h"


CLoongServerMerge::CLoongServerMerge(void)
{

}

CLoongServerMerge::~CLoongServerMerge(void)
{	
	std::vector<tagThreadPara*>::iterator itLoop = vecParaList.begin();
	for(;itLoop != vecParaList.end(); ++itLoop)
	{
		SAFE_DEL(*itLoop);
	}

	//�رվ��
	std::list<HANDLE>::iterator itH = m_listSimpleThreadPool.begin();
	for(;itH != m_listSimpleThreadPool.end(); ++itH)
	{
		if(*itH != NULL)
		{
			TerminateThread(*itH,0);
			CloseHandle(*itH);
			*itH = 0;
		}
	}

	SAFE_CLOSE_HANDLE(m_hPoolMergeEvent);

	//�������ݿ�
	sLoginBeton.Destroy();
	sLoongBeton.Destroy();
}
/*
/	��ʼ��DB
/
*/
BOOL CLoongServerMerge::Init(tagConfigDetail stConfigOne[EM_DB_TYPE_END],
							 tagConfigDetail stConfigTwo[EM_DB_TYPE_END],EDBMergeType eMergeType,
							 tstring strSrc2WorldName,
							 const std::multimap<ERoleCompensationType, tagCompensation*> mmCompensation)
{
	BOOL bRet = FALSE;

	//����ǿ���ϲ�
	if(eMergeType == EM_MERGE_CROSS_SECTION)
	{
		//��ʼ��Login
		bRet = sLoginBeton.Init(stConfigOne);
		if(bRet == FALSE)
		{
			return FALSE;
		}
	}
	//��ʼ��Loong
	bRet = sLoongBeton.Init(stConfigTwo);
	if(bRet == FALSE)
	{
		return FALSE;
	}
	//�ϲ�����
	m_eMergeType = eMergeType;
	m_strSrc2WorldName = strSrc2WorldName;

	//�̳߳غϲ�ͬ��
	m_hPoolMergeEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	//�رվ��
	std::list<HANDLE>::iterator itH = m_listSimpleThreadPool.begin();
	for(;itH != m_listSimpleThreadPool.end(); ++itH)
	{
		if(*itH != NULL)
		{
			TerminateThread(*itH,0);
			CloseHandle(*itH);
			*itH = 0;
		}
	}

	for(INT32 nLoop = 0; nLoop<THREAD_POOL_NUM; nLoop++)
	{
		tagThreadPara *par = new tagThreadPara(nLoop,this);
		vecParaList.push_back(par);

		HANDLE thread_work_handle_ = (HANDLE)_beginthreadex(NULL, 0, (THREADPROC)&MyThreadPool, (LPVOID)par, 0, NULL);
		m_listSimpleThreadPool.push_back(thread_work_handle_);
	}

	//��Ҳ�����Ϣ
	m_mmCompensation.insert(mmCompensation.begin(),mmCompensation.end());

	return TRUE;
}
/*
/	��ʼ���ϲ��߼�
*/
BOOL CLoongServerMerge::LoongMergeLogicCtrlInit(CLoongMergeLogicCtrl &clsMergeLogicCtrl)
{	
	//�ϲ�����
	clsMergeLogicCtrl.SetMergeType(m_eMergeType);
	//Դ2����Ϸ������
	clsMergeLogicCtrl.SetSrc2WorldName(m_strSrc2WorldName);

	//����ǿ�����ϲ�
	if(m_eMergeType == EM_MERGE_CROSS_SECTION)
	{
		//Դ1��account�����num
		DWORD dwSrc1LoginMaxNum = sLoginBeton.GetAccountMaxNum(EM_DB_SRC_ONE);
		if(dwSrc1LoginMaxNum == GT_INVALID)
		{
			return FALSE;
		}
		clsMergeLogicCtrl.SetSrc1LoginMaxNum(dwSrc1LoginMaxNum);
	}

	//Դ1roledate�����num
	DWORD dwSrc1RoleMaxNum = sLoongBeton.GetRoleDataMaxNum(EM_DB_SRC_ONE);
	if(dwSrc1RoleMaxNum == GT_INVALID)
	{
		return FALSE;
	}
	clsMergeLogicCtrl.SetSrc1RoleMaxNum(dwSrc1RoleMaxNum);

	//Դ1petdata�����num
	DWORD dwSrc1PetMaxNum = sLoongBeton.GetPetDataMaxNum(EM_DB_SRC_ONE);
	if(dwSrc1PetMaxNum == GT_INVALID)
	{
		return FALSE;
	}
	clsMergeLogicCtrl.SetSrc1PetMaxNum(dwSrc1PetMaxNum);

	//Դ1������Ʒ��С���
	INT64 n64ItemMinSerialBill = sLoongBeton.GetItemMinSerialBill(EM_DB_SRC_ONE);
	if(n64ItemMinSerialBill == GT_INVALID)
	{
		return FALSE;
	}
	clsMergeLogicCtrl.SetSrc1ItemMinSerialBill(n64ItemMinSerialBill);

	//Դ1�ڲ���Ʒ��������
	INT64 n64ItemMaxSerialInternal = sLoongBeton.GetMaxSerialInternal(EM_DB_SRC_ONE);
	if(n64ItemMaxSerialInternal == GT_INVALID)
	{
		return FALSE;
	}
	clsMergeLogicCtrl.SetSrc1ItemMaxSerialInternal(n64ItemMaxSerialInternal);

	//Դ1�ⲿ��Ʒ����С���
	INT64 n64ItemMinSerialOutter = sLoongBeton.GetMinSerialOutter(EM_DB_SRC_ONE);
	if(n64ItemMinSerialOutter == GT_INVALID)
	{
		return FALSE;
	}
	clsMergeLogicCtrl.SetSrc1ItemMinSerialOutter(n64ItemMinSerialOutter);

	return TRUE;
}

/*
/	�����ֵĲ���
*/
BOOL CLoongServerMerge::LoongRoleCompensationChangeRoleName(const tagRoleCompensation &st)
{
	//���߲���
	std::multimap<ERoleCompensationType, tagCompensation*>::iterator itLoop =  m_mmCompensation.find(st.Type);

	if(itLoop != m_mmCompensation.end())
	{
		//Ѱ�Ҷ�Ӧ�Ĳ�������
		INT32 nCount = m_mmCompensation.count(st.Type);
		for(INT32 nLoop = 0; nLoop != nCount; ++nLoop,++itLoop)
		{
			//��Ʒ����
			INT64 n64SerialNum = m_clsMergeLogicCtrl.GetNewItemSerialOutter();
			if(typeid(*(itLoop->second)) == typeid(tagItemCompensation))
			{
				INT64 n64SerialNum = m_clsMergeLogicCtrl.GetNewItemSerialOutter();
				BOOL bRet = sLoongBeton.CreateItem(n64SerialNum,
													((tagItemCompensation*)(itLoop->second))->dwItemTypeID,
													st.dwAccountID,
													st.dwRoleID);
				if(bRet == FALSE)
				{
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}

/*
/	��Ҳ���
*/
BOOL CLoongServerMerge::LoongRoleCompensation()
{
	const std::vector<tagRoleCompensation> &vecRoleCompensation = m_clsMergeLogicCtrl.GetRoleCompensation();
	std::vector<tagRoleCompensation>::const_iterator itLoop =  vecRoleCompensation.begin();
	for(;itLoop != vecRoleCompensation.end(); ++itLoop)
	{
		switch((*itLoop).Type)
		{
		case ERC_CHANGE_ROLE_NAME:
			{
				BOOL bRet = LoongRoleCompensationChangeRoleName((*itLoop));
				if(bRet != TRUE)
				{
					return FALSE;
				}
			}
			break;
		default:
			break;
		}
	}

	return TRUE;
}

/*
/	���̳߳�
*/
UINT MyThreadPool(LPVOID pParam)
{
	//�쳣����
	EXCEPTION_PROTECT;

	tagThreadPara		*pPara;
	pPara	= (tagThreadPara*)pParam;


	WaitForSingleObject(pPara->_p->m_hPoolMergeEvent, INFINITE);

	if(pPara->_n == 0)
	{
		//////////////////////////////////////////////////////////////////////////
		//itemdel
		g_clsOutput.Output(_T("��ʼ�ϲ�--itemdel!\r\n"));
		tagTWrap_Loong_ItemDel *TypeItemDel = NULL;
		BOOL bRet = TMethod_Loong_PagingServerMerge(TypeItemDel,pPara->_p->m_clsMergeLogicCtrl);
		if(bRet == FALSE)
		{	
			g_clsOutput.Output(_T("�ϲ�ʧ��--itemdel!\r\n"));
			return FALSE;
		}
		g_clsOutput.Output(_T("�ϲ��ɹ�--itemdel!\r\n"));
	}
	else if(pPara->_n == 1)
	{
		//////////////////////////////////////////////////////////////////////////
		//item
		g_clsOutput.Output(_T("��ʼ�ϲ�--item!\r\n"));
		tagTWrap_Loong_Item *TypeItem = NULL;
		BOOL bRet = TMethod_Loong_AccountServerMerge(TypeItem,pPara->_p->m_clsMergeLogicCtrl);
		if(bRet == FALSE)
		{	
			g_clsOutput.Output(_T("�ϲ�ʧ��--item!\r\n"));
			return FALSE;
		}
		g_clsOutput.Output(_T("�ϲ��ɹ�--item!\r\n"));

		//////////////////////////////////////////////////////////////////////////
		//itembaibao
		g_clsOutput.Output(_T("��ʼ�ϲ�--itembaibao!\r\n"));
		tagTWrap_Loong_ItemBaiBao *TypeItemBaiBao = NULL;
		bRet = TMethod_Loong_ServerMerge(TypeItemBaiBao,pPara->_p->m_clsMergeLogicCtrl);
		if(bRet == FALSE)
		{	
			g_clsOutput.Output(_T("�ϲ�itembaibaoʧ��!\r\n"));
			return FALSE;
		}
		g_clsOutput.Output(_T("�ϲ��ɹ�--itembaibao!\r\n"));
	}
	else if(pPara->_n == 2)
	{
		//equip
		g_clsOutput.Output(_T("��ʼ�ϲ�--equip!\r\n"));
		tagTWrap_Loong_Equip *TypeEquip = NULL;
		BOOL bRet = TMethod_Loong_PagingServerMerge(TypeEquip,pPara->_p->m_clsMergeLogicCtrl);
		if(bRet == FALSE)
		{	
			g_clsOutput.Output(_T("�ϲ�ʧ��--equip!\r\n"));
			return FALSE;
		}
		g_clsOutput.Output(_T("�ϲ��ɹ�--equip!\r\n"));
	}
	else if(pPara->_n == 3)
	{
		//equipdel
		g_clsOutput.Output(_T("��ʼ�ϲ�--equipdel!\r\n"));
		tagTWrap_Loong_EquipDel *TypeEquipDel = NULL;
		BOOL bRet = TMethod_Loong_PagingServerMerge(TypeEquipDel,pPara->_p->m_clsMergeLogicCtrl);
		if(bRet == FALSE)
		{
			g_clsOutput.Output(_T("�ϲ�ʧ��--equipdel!\r\n"));
			return FALSE;
		}
		g_clsOutput.Output(_T("�ϲ��ɹ�--equipdel!\r\n"));

		//equipbaibao
		g_clsOutput.Output(_T("��ʼ�ϲ�--Equipbaibao!\r\n"));
		tagTWrap_Loong_EquipBaiBao *TypeEquipBaiBao = NULL;
		bRet = TMethod_Loong_SeparateServerMerge(TypeEquipBaiBao,pPara->_p->m_clsMergeLogicCtrl);
		if(bRet == FALSE)
		{
			g_clsOutput.Output(_T("�ϲ�ʧ��--Equipbaibao!\r\n"));
			return FALSE;
		}
		g_clsOutput.Output(_T("�ϲ��ɹ�--Equipbaibao!\r\n"));
	}
	else if(pPara->_n == 4)
	{
		//task
		g_clsOutput.Output(_T("��ʼ�ϲ�--Task!\r\n"));
		tagTWrap_Loong_Task *TypeTask = NULL;
		BOOL bRet = TMethod_Loong_PagingServerMerge(TypeTask,pPara->_p->m_clsMergeLogicCtrl);
		if(bRet == FALSE)
		{
			g_clsOutput.Output(_T("�ϲ�ʧ��--Task!\r\n"));
			return FALSE;
		}
		g_clsOutput.Output(_T("�ϲ��ɹ�--Task!\r\n"));
	
		//taskdone
		g_clsOutput.Output(_T("��ʼ�ϲ�--TaskDone!\r\n"));
		tagTWrap_Loong_TaskDone *TypeTaskDone = NULL;
		bRet = TMethod_Loong_PagingServerMerge(TypeTaskDone,pPara->_p->m_clsMergeLogicCtrl);
		if(bRet == FALSE)
		{	
			g_clsOutput.Output(_T("�ϲ�ʧ��--TaskDone!\r\n"));
			return FALSE;
		}
		g_clsOutput.Output(_T("�ϲ��ɹ�--TaskDone!\r\n"));
	}


	return TRUE;

}

/*
/	��ʼ�ϲ�
*/
BOOL CLoongServerMerge::ServerMerge()
{	
	//��ʼ���ϲ��߼�����

	BOOL bRet = LoongMergeLogicCtrlInit(m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ��߼����Ƴ�ʼ��ʧ��!\r\n"));
		return FALSE;
	}

//////////////////////////////////////////////////////////////////////////
//���·�����Ҫ��˳���ںϲ�ʱ���ȵ��� 
//////////////////////////////////////////////////////////////////////////
	g_clsOutput.Output(_T("��ʼ�ϲ�--AccountCommon!\r\n"));
	tagTWrap_Loong_AccountCommon *TypeAccountCommon = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeAccountCommon,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--AccountCommon!\r\n"));
		return FALSE;
	}	
	g_clsOutput.Output(_T("�ϲ��ɹ�--AccountCommon!\r\n"));
//////////////////////////////////////////////////////////////////////////
	//����
	g_clsOutput.Output(_T("��ʼ�ϲ�--Guild!\r\n"));
	tagTWrap_Loong_Guild *TypeGuild = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeGuild,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("�ϲ�ʧ��--Guild!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--Guild!\r\n"));
//////////////////////////////////////////////////////////////////////////
	//����role data
	g_clsOutput.Output(_T("��ʼ�ϲ�--RoleData!\r\n"));
	tagTWrap_Loong_RoleData *TypeRoleData = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeRoleData,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--RoleData!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--RoleData!\r\n"));
//////////////////////////////////////////////////////////////////////////
//���Ϸ�����Ҫ��˳���ںϲ�ʱ���ȵ��� 
//////////////////////////////////////////////////////////////////////////

//�����ںϲ���������login�ϲ�
if(m_eMergeType == EM_MERGE_INSIDE_SECTION)
	goto INSIDE_SECTION;
//////////////////////////////////////////////////////////////////////////
//���·���Ϊlogin��ı�
//////////////////////////////////////////////////////////////////////////
	g_clsOutput.Output(_T("��ʼ�ϲ�--LoginAccount!\r\n"));
	tagTWrap_Longin_Account *TypeAccount = NULL;
	bRet = TMethod_Login_ServerMerge(TypeAccount,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--LoginAccount!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--LoginAccount!\r\n"));

//////////////////////////////////////////////////////////////////////////
	g_clsOutput.Output(_T("��ʼ�ϲ�--LoginWorldForbid!\r\n"));
	tagTWrap_Longin_WorldForbid *TypeWorldForbid = NULL;
	bRet = TMethod_Login_ServerMerge(TypeWorldForbid,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--LoginWorldForbid!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--LoginWorldForbid!\r\n"));
//////////////////////////////////////////////////////////////////////////
	g_clsOutput.Output(_T("��ʼ�ϲ�--LoginBlackList!\r\n"));
	tagTWrap_Longin_BlackList *TypeLoginBlackList = NULL;
	bRet = TMethod_Login_ServerMerge(TypeLoginBlackList,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--LoginBlackList!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--LoginBlackList!\r\n"));
//////////////////////////////////////////////////////////////////////////
	g_clsOutput.Output(_T("��ʼ�ϲ�--LoginFatigueTime!\r\n"));
	tagTWrap_Longin_FatigueTime *TypeLoginFatigueTime = NULL;
	bRet = TMethod_Login_ServerMerge(TypeLoginFatigueTime,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--LoginFatigueTime!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--LoginFatigueTime!\r\n"));

//////////////////////////////////////////////////////////////////////////
//���Ϸ���Ϊlogin��ı�
//////////////////////////////////////////////////////////////////////////

//�����ںϲ���������login�ϲ�
INSIDE_SECTION:
//////////////////////////////////////////////////////////////////////////
	//activity
	g_clsOutput.Output(_T("��ʼ�ϲ�--activity!\r\n"));
	tagTWrap_Loong_ActivityData *TypeActivityData = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeActivityData,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--activity!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--activity!\r\n"));	
//////////////////////////////////////////////////////////////////////////
	//ActClanTreasure
	g_clsOutput.Output(_T("��ʼ�ϲ�--ActClanTreasure!\r\n"));
	tagTWrap_Loong_ActClanTreasure *TypeActClanTreasure = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeActClanTreasure,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--ActClanTreasure!\r\n"));
		return FALSE;
	}	
	g_clsOutput.Output(_T("�ϲ��ɹ�--ActClanTreasure!\r\n"));
//////////////////////////////////////////////////////////////////////////
	g_clsOutput.Output(_T("��ʼ�ϲ�--RepRstTime!\r\n"));
	tagTWrap_Loong_RepRstTime *TypeRepRstTime = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeRepRstTime,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--RepRstTime!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--RepRstTime!\r\n"));
//////////////////////////////////////////////////////////////////////////
	g_clsOutput.Output(_T("��ʼ�ϲ�--FameHallEnterSnap!\r\n"));
	tagTWrap_Loong_FameHallEnterSnap *TypeFameHallEnterSnap = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeFameHallEnterSnap,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--FameHallEnterSnap!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--FameHallEnterSnap!\r\n"));
//////////////////////////////////////////////////////////////////////////
	//yuanbaoorder
	g_clsOutput.Output(_T("��ʼ�ϲ�--yuanbaoorder!\r\n"));
	tagTWrap_Loong_YuanBaoOrder *TypeYuanBaoOrder = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeYuanBaoOrder,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--yuanbaoorder!\r\n"));
		return FALSE;
	}	
	g_clsOutput.Output(_T("�ϲ��ɹ�--yuanbaoorder!\r\n"));
//////////////////////////////////////////////////////////////////////////
	//vip_stall
	g_clsOutput.Output(_T("��ʼ�ϲ�--VipStall!\r\n"));
	tagTWrap_Loong_VipStall *TypeVipStall = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeVipStall,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--VipStall!\r\n"));
		return FALSE;
	}	
	g_clsOutput.Output(_T("�ϲ��ɹ�--VipStall!\r\n"));
//////////////////////////////////////////////////////////////////////////
	//pet_skill
	g_clsOutput.Output(_T("��ʼ�ϲ�--PetSkill!\r\n"));
	tagTWrap_Loong_PetSkill *TypePetSkill = NULL;
	bRet = TMethod_Loong_ServerMerge(TypePetSkill,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--PetSkill!\r\n"));
		return FALSE;
	}	
	g_clsOutput.Output(_T("�ϲ��ɹ�--PetSkill!\r\n"));
//////////////////////////////////////////////////////////////////////////
	//pet_data
	g_clsOutput.Output(_T("��ʼ�ϲ�--PetData!\r\n"));
	tagTWrap_Loong_PetData *TypePetData = NULL;
	bRet = TMethod_Loong_ServerMerge(TypePetData,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--PetData!\r\n"));
		return FALSE;
	}	
	g_clsOutput.Output(_T("�ϲ��ɹ�--PetData!\r\n"));
//////////////////////////////////////////////////////////////////////////
	//BlackList
	g_clsOutput.Output(_T("��ʼ�ϲ�--BlackList!\r\n"));
	tagTWrap_Loong_BlackList *TypeBlackList = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeBlackList,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("�ϲ�ʧ��--BlackList!\r\n"));
		return FALSE;
	}	
	g_clsOutput.Output(_T("�ϲ��ɹ�--BlackList!\r\n"));
//////////////////////////////////////////////////////////////////////////
	//left msg
	g_clsOutput.Output(_T("��ʼ�ϲ�--left_msg!\r\n"));
	tagTWrap_Loong_LeftMsg *TypeLeftMsg = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeLeftMsg,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--left_msg!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--left_msg!\r\n"));
//////////////////////////////////////////////////////////////////////////
	//buff
	g_clsOutput.Output(_T("��ʼ�ϲ�--buff!\r\n"));
	tagTWrap_Loong_Buff *TypeBuff = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeBuff,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--buff!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--buff!\r\n"));	

//////////////////////////////////////////////////////////////////////////
	//clan_data
	g_clsOutput.Output(_T("��ʼ�ϲ�--clan_data!\r\n"));
	tagTWrap_Loong_ClanData *TypeClanDate = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeClanDate,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("�ϲ�ʧ��--clan_data!\r\n"));
		return FALSE;
	}	
	g_clsOutput.Output(_T("�ϲ��ɹ�--clan_data!\r\n"));	

//////////////////////////////////////////////////////////////////////////
	//enemy
	g_clsOutput.Output(_T("��ʼ�ϲ�--enemy!\r\n"));
	tagTWrap_Loong_Enemy *TypeEnemy = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeEnemy,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("�ϲ�ʧ��--enemy!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--enemy!\r\n"));
//////////////////////////////////////////////////////////////////////////
	//friend
	g_clsOutput.Output(_T("��ʼ�ϲ�--Friend!\r\n"));
	tagTWrap_Loong_Friend *TypeFriend = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeFriend,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("�ϲ�ʧ��--Friend!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--Friend!\r\n"));

//////////////////////////////////////////////////////////////////////////
	//friendship
	g_clsOutput.Output(_T("��ʼ�ϲ�--FriendShip!\r\n"));
	tagTWrap_Loong_FriendshipSave *TypeFriendShip = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeFriendShip,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("�ϲ�FriendShipʧ��!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--FriendShip!\r\n"));

//////////////////////////////////////////////////////////////////////////
	//GroupPurchase
	g_clsOutput.Output(_T("��ʼ�ϲ�--GroupPuichase!\r\n"));
	tagTWrap_Loong_GroupPurchase *TypeGroupPurchase = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeGroupPurchase,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�GroupPuichaseʧ��!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--GroupPuichase!\r\n"));

//////////////////////////////////////////////////////////////////////////
	//GuildMem
	g_clsOutput.Output(_T("��ʼ�ϲ�--GuildMem!\r\n"));
	tagTWrap_Loong_GuildMem *TypeGuildMem = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeGuildMem,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--GuildMem!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--GuildMem!\r\n"));

//////////////////////////////////////////////////////////////////////////
	//item_cdtime
	g_clsOutput.Output(_T("��ʼ�ϲ�--ItemCDTime!\r\n"));
	tagTWrap_Loong_ItemCDTime *TypeItemCDTime = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeItemCDTime,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("�ϲ�ʧ��--ItemCDTime!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--ItemCDTime!\r\n"));

//////////////////////////////////////////////////////////////////////////
	//item_needlog
	g_clsOutput.Output(_T("��ʼ�ϲ�--ItemNeedLog!\r\n"));
	tagTWrap_Loong_ItemNeedLog *TypeItemNeedLog = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeItemNeedLog,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--ItemNeedLog!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--ItemNeedLog!\r\n"));

//////////////////////////////////////////////////////////////////////////
	//log_baibao
	g_clsOutput.Output(_T("��ʼ�ϲ�--log_baibao!\r\n"));
	tagTWrap_Loong_LogBaiBao *TypeLogBaiBao = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeLogBaiBao,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("�ϲ�ʧ��--log_baibao!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--log_baibao!\r\n"));

//////////////////////////////////////////////////////////////////////////
	//role_del
	g_clsOutput.Output(_T("��ʼ�ϲ�--RoleDel!\r\n"));
	tagTWrap_Loong_RoleDel *TypeRoleDel = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeRoleDel,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("�ϲ�ʧ��--RoleDel!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--RoleDel!\r\n"));

//////////////////////////////////////////////////////////////////////////
	//skill
	g_clsOutput.Output(_T("��ʼ�ϲ�--Skill!\r\n"));
	tagTWrap_Loong_Skill *TypeSkill = NULL;
	bRet = TMethod_Loong_SeparateServerMerge(TypeSkill,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--Skill!\r\n"));
		return FALSE;
	}		
	g_clsOutput.Output(_T("�ϲ��ɹ�--Skill!\r\n"));

//////////////////////////////////////////////////////////////////////////
	//title
	g_clsOutput.Output(_T("��ʼ�ϲ�--Title!\r\n"));
	tagTWrap_Loong_Title *TypeTitle = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeTitle,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--Title!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--Title!\r\n"));

//////////////////////////////////////////////////////////////////////////
	//��Ƭ
	g_clsOutput.Output(_T("��ʼ�ϲ�--VisitingCard!\r\n"));
	tagTWrap_Loong_VisitingCard *TypeVisitingCard = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeVisitingCard,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--VisitingCard!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--VisitingCard!\r\n"));

//////////////////////////////////////////////////////////////////////////
	//Ԫ���˺�
	g_clsOutput.Output(_T("��ʼ�ϲ�--FameYBAccount!\r\n"));
	tagTWrap_Loong_YBAccount *TypeYBAccount = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeYBAccount,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--FameYBAccount!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--FameYBAccount!\r\n"));

//////////////////////////////////////////////////////////////////////////
	//guild_commodity
	g_clsOutput.Output(_T("��ʼ�ϲ�--guild_commodity!\r\n"));
	tagTWrap_Loong_GuildCommodity *TypeGuildCommodity = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeGuildCommodity,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--guild_commodity!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--guild_commodity!\r\n"));

//////////////////////////////////////////////////////////////////////////
	//guild_skill
	g_clsOutput.Output(_T("��ʼ�ϲ�--guild_skill!\r\n"));
	tagTWrap_Loong_GuildSkill *TypeGuildSkill = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeGuildSkill,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--guild_skill!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--guild_skill!\r\n"));

//////////////////////////////////////////////////////////////////////////
	//vip_netbar
	g_clsOutput.Output(_T("��ʼ�ϲ�--vip_netbar!\r\n"));
	tagTWrap_Loong_VipNetBar *TypeVipNetBar = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeVipNetBar,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--vip_netbar!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--vip_netbar!\r\n"));

//////////////////////////////////////////////////////////////////////////
	//guild_upgrade
	g_clsOutput.Output(_T("��ʼ�ϲ�--guild_upgrade!\r\n"));
	tagTWrap_Loong_GuildFacilities *TypeGuildFacilities = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeGuildFacilities,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--guild_upgrade!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--guild_upgrade!\r\n"));	

//////////////////////////////////////////////////////////////////////////
	//commerce_rank
	g_clsOutput.Output(_T("��ʼ�ϲ�--commerce_rank!\r\n"));
	tagTWrap_Loong_CommerceRank *TypeCommerceRank = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeCommerceRank,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--commerce_rank!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--commerce_rank!\r\n"));	


//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//�������������ܴ�ļ����� (item itembaibao ˳���ܱ�) �����̳߳غϲ�
//////////////////////////////////////////////////////////////////////////
	//�����̳߳غϲ�
	for(INT32 nLoop = 0; nLoop<THREAD_POOL_NUM; nLoop++)
		SetEvent(m_hPoolMergeEvent);

	//�ȴ��̳߳��������߳�
	EThreadPoolState e;
	while(TRUE)
	{
		e = GetThreadPoolState();
		if(e == ETPS_STILL_HAVE_ACTIVE)
		{
			Sleep(10000); //10��
			continue;
		}
		else if(e == ETPS_FAILURE)
		{
			break;
		}
		else if(e == ETPS_SUCCESS)
		{
			break;
		}
	}

	//����Ϸ��ɹ� ���������Ӧ�Ĳ���
	if(e == ETPS_SUCCESS)
	{
		g_clsOutput.Output(_T("��ʼ��Ҳ���!\r\n"));
		BOOL bRet = LoongRoleCompensation();
		if(bRet  == FALSE)
		{
			g_clsOutput.Output(_T("��Ҳ���ʧ��!\r\n"));
			e = ETPS_FAILURE;
		}	
		g_clsOutput.Output(_T("��Ҳ����ɹ�!\r\n"));
	}

	//����߼�
	m_clsMergeLogicCtrl.ClearMemory();

	if(e == ETPS_SUCCESS)
		g_clsOutput.Output(_T("�ɹ�!\r\n"));
	else if(e == ETPS_FAILURE)
	{
		//�رվ��
		std::list<HANDLE>::iterator itH = m_listSimpleThreadPool.begin();
		for(;itH != m_listSimpleThreadPool.end(); ++itH)
		{
			if(*itH != NULL)
			{
				TerminateThread(*itH,0);
				CloseHandle(*itH);
				*itH = 0;
			}
		}
		g_clsOutput.Output(_T("ʧ�� ���������� ���ºϲ�!\r\n"));
	}

	return TRUE;
}
/*
/	�̳߳����Ƿ����̻߳���
*/
EThreadPoolState CLoongServerMerge::GetThreadPoolState()
{
	std::list<HANDLE>::iterator itLoop = m_listSimpleThreadPool.begin();

	for(;itLoop != m_listSimpleThreadPool.end(); ++itLoop)
	{
		HANDLE hHandle = *itLoop;
		DWORD dwResult = 0;
		if( hHandle )
		{
			DWORD dwConnectExitCode = STILL_ACTIVE;
			GetExitCodeThread(hHandle, &dwConnectExitCode);
			if( STILL_ACTIVE == dwConnectExitCode )
				return ETPS_STILL_HAVE_ACTIVE;
			else if(FALSE == dwConnectExitCode)
				return ETPS_FAILURE;
		}
	}

	return ETPS_SUCCESS;
}