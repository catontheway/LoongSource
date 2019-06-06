/********************************************************************
	created:	2008/08/16
	created:	16:8:2008   17:44
	filename: 	d:\loong_client\Client\QuestActiveFrame.cpp
	file path:	d:\loong_client\Client
	file base:	QuestActiveFrame
	file ext:	cpp
	author:		leili
	
	purpose:	�̶���������ʵ�֣���������߼�
*********************************************************************/
#include "StdAfx.h"
#include "QuestActiveFrame.h"
#include "QuestMgr.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "CombatEvent.h"
#include "ServerTime.h"
#include "CreatureData.h"

//-----------------------------------------------------------------------------
// ���캯��
//-----------------------------------------------------------------------------
QuestActiveFrame::QuestActiveFrame( void )
: m_Trunk(this)
, m_pWnd(0)
, m_pStcDesc(0)
, m_pListQuests(0)
, m_pBtnTime(0)
, m_pBtnName(0)
, m_pBtnLimit(0)
, m_pBtnNPC(0)
, m_pBtnCloseWnd(0)
, m_pPshSwitchCan(0)
, m_curSortType(ActivityMgr::EST_SortByTime)
, m_curRow(0)
, m_showCan(0)
{

}

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
QuestActiveFrame::~QuestActiveFrame( void )
{
	
}

//-----------------------------------------------------------------------------
// ��ʼ��
//-----------------------------------------------------------------------------
BOOL QuestActiveFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	return TRUE;
}

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
BOOL QuestActiveFrame::Destroy()
{
	GameFrame::Destroy();

	if( P_VALID(m_pWnd) )
		m_pGUI->AddToDestroyList(m_pWnd);

	return TRUE;
}

//-----------------------------------------------------------------------------
// ���¶��봰�ڽ���
//-----------------------------------------------------------------------------
BOOL QuestActiveFrame::ReloadUI()
{
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\dtask.xml");
	m_pWnd = m_pGUI->CreateFromFile("VFS_System", strPath.c_str());

	m_pStcDesc		= (GUIStaticEx*)	m_pWnd->GetChild(_T("dtpic\\dtconwin\\dcontent"));
	m_pListQuests	= (GUIListBox*)		m_pWnd->GetChild(_T("dtpic\\dtlistpic\\dtlist"));
	m_pBtnTime		= (GUIButton*)		m_pWnd->GetChild(_T("dtpic\\dtlistpic\\dttime"));
	m_pBtnName		= (GUIButton*)		m_pWnd->GetChild(_T("dtpic\\dtlistpic\\dtmame"));
	m_pBtnLimit		= (GUIButton*)		m_pWnd->GetChild(_T("dtpic\\dtlistpic\\dtgrade"));
	m_pBtnNPC		= (GUIButton*)		m_pWnd->GetChild(_T("dtpic\\dtlistpic\\dtnpc"));
	m_pBtnCloseWnd	= (GUIButton*)		m_pWnd->GetChild(_T("dtpic\\closebutt"));
	m_pPshSwitchCan	= (GUIPushButton*)	m_pWnd->GetChild( _T("dtpic\\new") );

	m_pListQuests->SetColNum( 5, 20 );
	m_pListQuests->SetColWidth( 0, 110 );	// ��ȡʱ��
	m_pListQuests->SetColWidth( 1, 110 );	// ��������
	m_pListQuests->SetColWidth( 2, 70 );	// �ȼ�����
	m_pListQuests->SetColWidth( 3, 110 );	// ��ȡ��

	if( m_showCan )
		m_pPshSwitchCan->SetState( EGUIBS_PushDown, FALSE );
	else
		m_pPshSwitchCan->SetState( EGUIBS_PopUp, FALSE );

	// ע���¼�������
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&QuestActiveFrame::EventHandler));

	m_pWnd->SetInvisible( TRUE );

	ShowQuest();

	return TRUE;
}

//-----------------------------------------------------------------------------
// ������Ϣ�¼�����
//-----------------------------------------------------------------------------
BOOL QuestActiveFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pBtnTime && m_curSortType != ActivityMgr::EST_SortByTime )//����ȡʱ������
			{
				m_curSortType = ActivityMgr::EST_SortByTime;
				ShowQuest();
			}
			else if( pWnd == m_pBtnName && m_curSortType != ActivityMgr::EST_SortByName )//��������������
			{
				m_curSortType = ActivityMgr::EST_SortByName;
				ShowQuest();
			}
			else if( pWnd == m_pBtnLimit && m_curSortType != ActivityMgr::EST_SortByLevel )//���ȼ���������
			{
				m_curSortType = ActivityMgr::EST_SortByLevel;
				ShowQuest();
			}
			else if( pWnd == m_pBtnNPC && m_curSortType != ActivityMgr::EST_SortByNpc )//����ȡ����������
			{
				m_curSortType = ActivityMgr::EST_SortByNpc;
				ShowQuest();
			}
			else if( pWnd == m_pBtnCloseWnd )//�رմ���
			{
				QuestMgr::Inst()->CloseActiveFrame();
			}
		}
		break;

	case EGUIE_ItemClick:
		{
			if( pWnd == m_pListQuests )
			{
				if( m_curRow > pEvent->dwParam1
					&& pEvent->msg.dwType == WM_LBUTTONUP )
				{
					// ��������
					UINT16 u16ID = m_curShowQuestIDs[pEvent->dwParam1];
					ShowDesc(u16ID);
				}
			}
		}
		break;

	case EGUIE_ItemDblClick:
		{
			if( pWnd == m_pListQuests )
			{
				if( m_curRow > pEvent->dwParam1
					&& pEvent->msg.dwType == WM_LBUTTONUP )
				{
					tagPoint ptLT = m_pWnd->GetClient();
					ptLT = ptLT + m_pListQuests->GetFather()->GetFather()->GetClient();
					ptLT = ptLT + m_pListQuests->GetFather()->GetClient();
					ptLT = ptLT + m_pListQuests->GetClient();
					if( pEvent->msg.pt.x > ptLT.x+310
						&& pEvent->msg.pt.x < ptLT.x+420 )
					{
						QuestQuery *pQuery = QuestMgr::Inst()->GetQuery();
						DWORD npcID = pQuery->GetActiveQuestNPCID( pEvent->dwParam1 );
						Vector3 pos;
						if( pQuery->GetNPCPosition( npcID, pos ) )//�Զ�Ѱ·
						{
							LocalPlayer *pLP = RoleMgr::Inst()->GetLocalPlayer();

							tagMouseMoveEvent event( _T("tagMouseMoveEvent"), NULL );
							event.start = pLP->GetPos();
							event.end = pos;
							event.validDist = 100.0f;

							m_pMgr->SendEvent( &event );
						}
					}
				}
			}
		}
		break;

	case EGUIE_PushButtonDown:
		{
			if( pWnd == m_pPshSwitchCan )//��ʾ�ʺ�������Ļ
			{
				if( pEvent->dwParam1 == EGUIBS_PushDown )
				{
					m_showCan = true;
				}
				else
				{
					m_showCan = false;
				}

				ShowQuest();
				break;
			}
		}
		break;

	default:
		break;
	}

	return TRUE;
}

VOID QuestActiveFrame::Show( void )
{
	if( P_VALID(m_pWnd) ) 
	{
		m_pWnd->SetInvisible(FALSE); 
		m_pWnd->FlipToTop();
	}
}

//-----------------------------------------------------------------------------
// ��ʾ�����б���ָ������ʽ
//-----------------------------------------------------------------------------
VOID QuestActiveFrame::ShowQuest( void )
{
	ActivityMgr::Inst()->SortActivities(m_curSortType);		
	m_pListQuests->Clear();
	
	const ActivityMgr::ActivityVector &activities = ActivityMgr::Inst()->GetActivityVector();
	m_curRow = 0;
	m_curShowQuestIDs.clear();
	for ( UINT i= 0; i < activities.size(); ++i)
	{
		const tagActivityProtoData &activitydata = activities[i];
		DWORD color = 0xFFADDF14;
		tstring strWeek;
		EWeek curWeek = ServerTime::Inst()->GetCurrentWeekDay();

		// ����ͬһ��	
		if (activitydata.week != curWeek)
		{
			if( activitydata.week != ((curWeek+1)%7) )//����������Ļ����ʾ
			{
				continue;
			}

			color = 0xFFFFF7E0;
		}
		// �����ʼʱ����ڵ�ǰʱ��
		if (CompareTime(activitydata.startHour, activitydata.startMinute, 
			ServerTime::Inst()->GetCurrentHour(), ServerTime::Inst()->GetCurrentMinute()) == true)
		{
			color = 0xFFFFF7E0;
		}
		// �������ʱ��С�ڵ�ǰʱ��
		if (CompareTime(activitydata.endHour, activitydata.endMinute, ServerTime::Inst()->GetCurrentHour(), 
			ServerTime::Inst()->GetCurrentMinute()) == false)
		{
			color = 0xFFFFF7E0;
		}
		// ����ȼ�������
		int RoleLv = RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel();
		if (RoleLv < activitydata.minLevel || RoleLv > activitydata.maxLevel)
		{
			if( m_showCan )
				continue;

			color = 0xFFD04937;
		}
		TCHAR szLevel[10];
		_stprintf( szLevel, _T("%d-%d"), activitydata.minLevel, activitydata.maxLevel );
		switch(activitydata.week)
		{
		case EWeek_MON:		strWeek = g_StrTable[_T("QuestDate_MON")];		break;
		case EWeek_TUES:	strWeek = g_StrTable[_T("QuestDate_TUES")];		break;
		case EWeek_WEDNES:	strWeek = g_StrTable[_T("QuestDate_WEDNES")];	break;
		case EWeek_THURS:	strWeek = g_StrTable[_T("QuestDate_THURS")];	break;
		case EWeek_FRI:		strWeek = g_StrTable[_T("QuestDate_FRI")];		break;
		case EWeek_SAT:		strWeek = g_StrTable[_T("QuestDate_SAT")];		break;
		case EWeek_SUN:		strWeek = g_StrTable[_T("QuestDate_SUN")];		break;
		}
		TCHAR szBuffer1[4];
		TCHAR szBuffer2[4];
		TCHAR szBuffer3[4];
		TCHAR szBuffer4[4];
		_sntprintf(szBuffer1, 4, _T("%02d"), activitydata.startHour);
		_sntprintf(szBuffer2, 4, _T("%02d"), activitydata.startMinute);
		_sntprintf(szBuffer3, 4, _T("%02d"), activitydata.endHour);
		_sntprintf(szBuffer4, 4, _T("%02d"), activitydata.endMinute);

		strWeek = strWeek + _T(" ") + szBuffer1 + _T(":") + szBuffer2 + _T("-") +
			szBuffer3 + _T(":") + szBuffer4;
		
		m_curShowQuestIDs.push_back(activities[i].id);
		m_pListQuests->SetText(m_curRow, 0, strWeek.c_str(), color);
		m_pListQuests->SetText(m_curRow, 1, activitydata.name.c_str(), color);
		m_pListQuests->SetText(m_curRow, 2, szLevel, color);
		const tagCreatureProto *cp = CreatureData::Inst()->FindNpcAtt(activitydata.acceptNpcID);
		if (cp)
			m_pListQuests->SetText(m_curRow, 3, cp->szName, color);
		else
			m_pListQuests->SetText(m_curRow, 3, _T(" "), color);

		++m_curRow;
	}	
}

//-----------------------------------------------------------------------------
// ��ʾ��������
//-----------------------------------------------------------------------------
VOID QuestActiveFrame::ShowDesc( UINT16 questID )
{
	const tagActivityProtoData *data = ActivityMgr::Inst()->GetData().GetQuestProto(questID);
	if( P_VALID(data) )
	{
		std::vector<tagIDLink> idLinks;
		std::vector<tagPosLink> posLinks;
		tstring strDesc = QuestMgr::Inst()->ParseQuestView(const_cast<tstring&>(data->desc), idLinks, posLinks);
		m_pStcDesc->SetText(strDesc.c_str());
	}
}

BOOL QuestActiveFrame::EscCancel()
{
	if( P_VALID(m_pWnd) && !m_pWnd->IsInvisible() )
	{
		QuestMgr::Inst()->CloseActiveFrame();
		return TRUE;
	}

	return GameFrame::EscCancel();
}