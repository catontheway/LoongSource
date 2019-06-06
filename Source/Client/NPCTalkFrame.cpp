/********************************************************************
	created:	2008/08/16
	created:	16:8:2008   17:21
	filename: 	d:\loong_client\Client\NPCTalkFrame.cpp
	file path:	d:\loong_client\Client
	file base:	NPCTalkFrame
	file ext:	cpp
	author:		leili
	
	purpose:	��ͨNPC�Ի�����ʵ�֣������ڵĽ����߼�
*********************************************************************/
#include "StdAfx.h"
#include "NPCTalkFrame.h"
#include "QuestMgr.h"
#include "ComposeMgr.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "NPC.h"
#include "ItemFrame.h"
#include "CombatEvent.h"
#include "..\WorldDefine\msg_map.h"

//-----------------------------------------------------------------------------
// ���캯��
//-----------------------------------------------------------------------------
NPCTalkFrame::NPCTalkFrame( void )
: m_Trunk(this)
, m_pWnd(0)
, m_pStcCondition(0)
, m_pListOptions(0)
, m_pBtnCloseWnd(0)
, m_curRow(0)
, m_pCaption(0)
, m_escapeTime(0)
, m_targetType(0)
, m_targetID(GT_INVALID)
, m_isUsingTransportItem(false)
{

}

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
NPCTalkFrame::~NPCTalkFrame( void )
{
	
}

//-----------------------------------------------------------------------------
// ��ʼ��
//-----------------------------------------------------------------------------
BOOL NPCTalkFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
    m_isUsingTransportItem = false;

	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	return TRUE;
}

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
BOOL NPCTalkFrame::Destroy()
{
	GameFrame::Destroy();

	if( P_VALID(m_pWnd) )
		m_pGUI->AddToDestroyList(m_pWnd);
	m_pWnd = NULL;

	return TRUE;
}

//-----------------------------------------------------------------------------
// ���¶��봰�ڽ���
//-----------------------------------------------------------------------------
BOOL NPCTalkFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\npctalk.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );

	// ע���¼�������
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&NPCTalkFrame::EventHandler));

	m_pCaption		= (GUIPatch*)		m_pWnd->GetChild( _T("nt_pic") );
	m_pStcCondition	= (GUIStaticEx*)	m_pWnd->GetChild( _T("nt_pic\\nt_pic2\\npctalkpic\\contentWnd\\npctalk") );
	m_pListOptions	= (GUIListBox*)		m_pWnd->GetChild( _T("nt_pic\\nt_pic2\\optpic\\optlist") );
	m_pBtnCloseWnd	= (GUIButton*)		m_pWnd->GetChild( _T("nt_pic\\new0") );

	m_pListOptions->SetColNum(1, 39);

	m_pWnd->SetInvisible( TRUE );

	return TRUE;
}

//-----------------------------------------------------------------------------
// ������Ϣ�¼�����
//-----------------------------------------------------------------------------
BOOL NPCTalkFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pBtnCloseWnd )//�رմ���
			{
				Hide();

				m_pMgr->AddToDestroyList(this);
			}
		}
		break;

	case EGUIE_ItemClick:
		{
			if( pWnd == m_pListOptions )
			{
				if( m_curRow > pEvent->dwParam1 )
				{
					QuestScript *pQS = QuestMgr::Inst()->GetScript();
					if( !P_VALID(pQS) )
						return TRUE;

					const tagOptAttr& optAttr = m_optAttrs[pEvent->dwParam1];
					switch( optAttr.type )
					{
					case EOT_AcceptQuest:
						{
							pQS->OnAcceptQuestTalk(optAttr.id, optAttr.step);
						}
						break;
					case EOT_CompleteQuest:
						{
							pQS->OnCompleteQuestTalk(optAttr.id, optAttr.step);
						}
						break;
					case EOT_ScenarioTalk:
						{
							pQS->OnScenarioTalk(optAttr.id, optAttr.step);
						}
						break;
					case EOT_ProduceSkillTalk:
						{
							pQS->OnProduceSkillTalk(optAttr.id, optAttr.step);
						}
						break;
					case EOT_LearnProduceSkill:
						{
							ComposeMgr::Inst()->LearnProduceSkill(optAttr.step, QuestMgr::Inst()->GetTalkNPCGlobalID());
						}
						break;
					case EOT_LeaveProduceSkill:
						{
							ComposeMgr::Inst()->LeaveProduceSkill(optAttr.step, QuestMgr::Inst()->GetTalkNPCGlobalID());
						}
						break;
					case EOT_CancelProduceSkill:
						{
							ClearOptions();
							QuestMgr::Inst()->OnNPCTalk( QuestMgr::Inst()->GetTalkNPCGlobalID(),
								QuestMgr::Inst()->GetTalkNPCTypeID() );
						}
						break;
					case EOT_GuildCommerce:
						{
							pQS->OnGuildCommerceTalk(optAttr.id, optAttr.step);
							
						}
						break;
					case EOT_ServerCtrl:	//����������ѡ����ʱ��Ҫ�������������Ϣ
						{
							tagNC_DlgDefaultMsg msg;
							msg.eDlgTarget = (EMsgUnitType)m_targetType;
							msg.dwTargetID = m_targetID;
							msg.eDlgOption = (EDlgOption)optAttr.step;
							TObjRef<NetSession>()->Send(&msg);

							Hide();
						}
						break;
                    case EOT_SetTransportPoint:
                        {
                            if (0 == optAttr.id)
                            {
                                // ��λ
                                tagNC_SetTransportMap msg;
                                if (P_VALID(m_pTransportItem))
                                    msg.n64ItemID   = m_pTransportItem->GetItemId();
                                TObjRef<NetSession>()->Send(&msg);
                            }
                            else if (1 == optAttr.id)
                            {
                                // ����
                                tagNC_UseTransportMap msg;
                                if (P_VALID(m_pTransportItem))
                                {
                                    msg.n64ItemID   = m_pTransportItem->GetItemId();
                                }
                                TObjRef<NetSession>()->Send(&msg);
                            }

                            Hide();
                        }
                        break;
					}
				}
			}
		}
		break;

	default:
		break;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// ����NPC��������
//-----------------------------------------------------------------------------
VOID NPCTalkFrame::SetCaption( LPCTSTR szCaption )
{
	m_pCaption->SetText( szCaption );
}

//-----------------------------------------------------------------------------
// ���öԻ�����
//-----------------------------------------------------------------------------
VOID NPCTalkFrame::SetContent( LPCTSTR szContent )
{
	tstring strContent = szContent;
	vector<tagIDLink> idLinks;
	vector<tagPosLink> posLinks;
	QuestMgr::Inst()->ParseQuestView(strContent,idLinks,posLinks);
	m_pStcCondition->SetText(strContent.c_str());

	tagPoint size = m_pStcCondition->GetSize();
	if( m_pStcCondition->GetTextTotalHeight() > size.y )
	{
		size.y = m_pStcCondition->GetTextTotalHeight();
		m_pStcCondition->SetResizable(TRUE);
		m_pStcCondition->Resize( size );
		m_pStcCondition->SetResizable(FALSE);
	}
}

//-----------------------------------------------------------------------------
// ������жԻ�ѡ��
//-----------------------------------------------------------------------------
VOID NPCTalkFrame::ClearOptions( void )
{
	if( P_VALID(m_pWnd) )
	{
		m_pListOptions->Clear();
		m_pListOptions->SetCurSelectedRow(0);
		m_curRow = 0;
		m_optAttrs.clear();
	}
}

//-----------------------------------------------------------------------------
// ���ӶԻ�ѡ��
//-----------------------------------------------------------------------------
VOID NPCTalkFrame::AddTalkOption( LPCTSTR szOption, DWORD param1, DWORD param2, EOptionType optionType )
{
	m_pListOptions->SetText(m_curRow++, 0, szOption);

	tagOptAttr optAttr;
	optAttr.id = param1;
	optAttr.step = param2;
	optAttr.type = optionType;
	m_optAttrs.push_back(optAttr);
}

//-----------------------------------------------------------------------------
// ����ĳNPC�Ŀɽ�����ѡ��
//-----------------------------------------------------------------------------
VOID NPCTalkFrame::AddAcceptOptions( DWORD npcID )
{
	QuestQuery *pQQ = QuestMgr::Inst()->GetQuery();
	if( !P_VALID(pQQ) )
		return;

	QuestData *pQD = QuestMgr::Inst()->GetData();
	if( !P_VALID(pQD) )
		return;

	vector<UINT16> quests;
	pQQ->GetNPCAcceptQuests(npcID, quests);
	for( size_t i = 0; i < quests.size(); i++ )
	{
		UINT16 questID = quests[i];
		tstring name;
		pQD->GetQuestName(questID, name);
		name += g_StrTable[_T("NPCTalk_AcceptOption")];
		m_pListOptions->SetText(m_curRow++, 0, name.c_str());

		tagOptAttr optAttr;
		optAttr.id = questID;
		optAttr.step = 0;
		optAttr.type = EOT_AcceptQuest;
		m_optAttrs.push_back(optAttr);
	}
}

//-----------------------------------------------------------------------------
// ����ĳNPC�Ŀɽ�����ѡ��
//-----------------------------------------------------------------------------
VOID NPCTalkFrame::AddCompleteOptions( DWORD npcID )
{
	QuestQuery *pQQ = QuestMgr::Inst()->GetQuery();
	if( !P_VALID(pQQ) )
		return;

	QuestData *pQD = QuestMgr::Inst()->GetData();
	if( !P_VALID(pQD) )
		return;

	vector<UINT16> quests;

	// �ɸ�������
	pQQ->GetNPCUpdateQuests(npcID, quests);
	for( size_t i = 0; i < quests.size(); i++ )
	{
		UINT16 questID = quests[i];
		tstring name;
		pQD->GetQuestName(questID, name);
		name += g_StrTable[_T("NPCTalk_CompleteOption")];
		m_pListOptions->SetText(m_curRow++, 0, name.c_str());

		tagOptAttr optAttr;
		optAttr.id = questID;
		optAttr.step = 0;
		optAttr.type = EOT_CompleteQuest;
		m_optAttrs.push_back(optAttr);
	}

	// ���������
	pQQ->GetNPCCompleteQuests(npcID, quests);
	for( size_t i = 0; i < quests.size(); i++ )
	{
		UINT16 questID = quests[i];
		tstring name;
		pQD->GetQuestName(questID, name);
		name += g_StrTable[_T("NPCTalk_CompleteOption")];
		m_pListOptions->SetText(m_curRow++, 0, name.c_str());

		tagOptAttr optAttr;
		optAttr.id = questID;
		optAttr.step = 0;
		optAttr.type = EOT_CompleteQuest;
		m_optAttrs.push_back(optAttr);
	}
}

//-----------------------------------------------------------------------------
// ��ʾ�Ի�����
//-----------------------------------------------------------------------------
VOID NPCTalkFrame::Show( void )
{
	if( P_VALID(m_pWnd) ) 
	{
		m_pWnd->SetInvisible(FALSE);
		m_pWnd->FlipToTop();

		// �Ի�NPC�������
		NPC *pNPC = (NPC*)RoleMgr::Inst()->FindRole(QuestMgr::Inst()->GetTalkNPCGlobalID());
		if( P_VALID(pNPC) )
		{
			Vector3 localPlayerHead;
			RoleMgr::Inst()->GetLocalPlayer()->GetHeadPos(localPlayerHead);
			pNPC->FaceTo(localPlayerHead);
		}
	}
}

//-----------------------------------------------------------------------------
// ���ضԻ�����
//-----------------------------------------------------------------------------
VOID NPCTalkFrame::Hide( void )
{
	if( P_VALID(m_pWnd) ) 
	{
		m_pWnd->SetInvisible(TRUE);
        m_isUsingTransportItem  = false;

		// �Ի�NPC�ָ�ԭ�ȳ���
		NPC *pNPC = (NPC*)RoleMgr::Inst()->FindRole(QuestMgr::Inst()->GetTalkNPCGlobalID());
		if( P_VALID(pNPC) )
		{
			pNPC->FaceBack();
		}
	}
}

VOID NPCTalkFrame::Update()
{
    if (m_isUsingTransportItem)
        return;

	m_escapeTime += Kernel::Inst()->GetDeltaTimeDW();
	if( m_escapeTime > 1000 )//ÿ��1�������Ŀ��NPC�ľ���
	{
		m_escapeTime = 0;

		if (RoleMgr::Inst()->IsOutValidDist( QuestMgr::Inst()->GetTalkNPCGlobalID() ) )
		{
			Hide();
			m_pMgr->AddToDestroyList(this);
		}
	}

	GameFrame::Update();
}

BOOL NPCTalkFrame::EscCancel()
{
	if( P_VALID(m_pWnd) && !m_pWnd->IsInvisible() )
	{
		Hide();
		m_pMgr->AddToDestroyList(this);

		return TRUE;
	}

	return GameFrame::EscCancel();
}