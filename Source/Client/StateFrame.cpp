#include "stdafx.h"
#include "StateFrame.h"
#include "CharacterFrame.h"
#include "ButtonEx.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include "RoleEvent.h"
#include "PlayerNameTab.h"
#include "LevelUpData.h"
#include "NetSession.h"
#include "..\WorldDefine\role_att.h"
#include "ChatFrame.h"
#include "CombatSysUtil.h"

StateFrame::StateFrame(void):m_Trunk(this)
{
	m_pWnd = NULL;
	m_pWndFather = NULL;
	INT i;
	for (i = 0; i < EDP_End; i++)
	{
		m_pStcAtt[i] = NULL;
		m_pBtnAtt[i] = NULL;
	}
	for (i = 0; i < EDPE_End; i++)
	{
		m_pStcAttEx[i] = NULL;
	}
	for (i = 0; i < EDR_End; i++)
	{
		m_pStcResPic[i] = NULL;
		m_pStcResValue[i] = NULL;
	}
	m_pStcPlayerName = NULL;
	m_pStcPlayerLevel = NULL;
	m_pStcPlayerClass = NULL;
	m_pStcPlayerGuild = NULL;
	m_pStcOverplus = NULL;
	m_pPbnDisplayAtt = NULL;
	m_pPbnDisplayEquip = NULL;
	m_pPbnDisplayFashion = NULL;
    m_pWndState = NULL;

	m_isSwappingSuitPattern = false;

	m_pWndAtt = NULL;
	m_pWndAttEx = NULL;
	m_pWndAttIn = NULL;
	m_pWndRes = NULL;
	m_pBtnSure = NULL;
	m_pBtnCancel = NULL;

	for (i=0; i<EDT_Origin_End; ++i)
	{
		m_Temporary[i] = 0;
	}
	for (i=0; i<X_ERA_ATTA_NUM; ++i)
	{
		m_nOgnAtt[i] = 0;
		m_nPreBid[i] = 0;
		m_nBidden[i] = 0;
	}
	m_nInternalInjury = GT_INVALID;
}

StateFrame::~StateFrame(void)
{
}

BOOL StateFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);

	// ע����Ϸ�¼�������
	m_pMgr->RegisterEventHandle(_T("tagUpdateLPAttEvent"),	  (FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnGameEvent));
	m_pMgr->RegisterEventHandle(_T("tagUpdateLPBidAttEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnGameEvent));
	m_pMgr->RegisterEventHandle(_T("tagUpdateLPLevel"),		  (FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnGameEvent));
	m_pMgr->RegisterEventHandle(_T("tagRoleGetNameEvent"),	  (FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnGameEvent));
    m_pMgr->RegisterEventHandle(_T("tagInitLPAttEvent"),	  (FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnGameEvent));

	return bRet;
}

BOOL StateFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();
	
	// ע����Ϸ�¼�������
	m_pMgr->UnRegisterEventHandler(_T("tagUpdateLPAttEvent"),	 (FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnGameEvent));
	m_pMgr->UnRegisterEventHandler(_T("tagUpdateLPBidAttEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnGameEvent));
	m_pMgr->UnRegisterEventHandler(_T("tagUpdateLPLevel"),		 (FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnGameEvent));
	m_pMgr->UnRegisterEventHandler(_T("tagRoleGetNameEvent"),	 (FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnGameEvent));
    m_pMgr->UnRegisterEventHandler(_T("tagInitLPAttEvent"),	     (FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnGameEvent));
	
	return bRet;
}

BOOL StateFrame::ReloadUI()
{
	// ����
    m_pWndState = m_pGUI->GetWnd(((CharacterFrame*)m_pFather)->GetStateWndFullName().c_str());
	m_pWndFather = m_pGUI->GetWnd(((CharacterFrame*)m_pFather)->GetGUIFullName().c_str());
	m_pWnd = m_pWndFather->GetChild(_T("rstatepic"));
	
	// ע���¼�������
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&StateFrame::EventHandler));

	TCHAR szTmp[X_SHORT_NAME] = {0};
	//��������
	{
		m_pStcPlayerName = (GUIStatic*)m_pWnd->GetChild(_T("rname"));
		m_pStcPlayerLevel = (GUIStatic*)m_pWnd->GetChild(_T("rgrad1"));
		m_pStcPlayerClass = (GUIStatic*)m_pWnd->GetChild(_T("rcareer"));
		m_pStcPlayerGuild = (GUIStatic*)m_pWnd->GetChild(_T("rpost"));
		m_pStcPlayerLevel->SetText(_T(""));
	}
	//һ�����Ժ�Ͷ��
	{
		m_pWndAtt = m_pWnd->GetChild(_T("equipback\\role\\rs_add"));
		
		for (INT i = 0; i < EDP_End; i++)
		{
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("rnum%d"), i+1);
			m_pStcAtt[i] = (GUIStatic*)m_pWndAtt->GetChild(szTmp);
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("ra_butt%d"), i+1);
			m_pBtnAtt[i] = (GUIButton*)m_pWndAtt->GetChild(szTmp);
			m_pStcAtt[i]->SetText(_T(""));
		}

		m_pStcOverplus = (GUIStatic*)m_pWndAtt->GetChild(_T("num7"));
		m_pBtnCancel = (GUIButton*)m_pWndAtt->GetChild(_T("cancle"));
		m_pBtnSure = (GUIButton*)m_pWndAtt->GetChild(_T("sure"));
		m_pBtnSure->SetEnable(FALSE);
		m_pBtnCancel->SetEnable(FALSE);
	}
	//�������Ժ͹�������
	{
		m_pWndAttIn = m_pWnd->GetChild(_T("rs_pic1"));
		m_pWndAttEx = m_pWnd->GetChild(_T("rs_pic2"));
		for (INT i = 0; i < EDPE_End; i++)
		{
			if(i <= EDPE_Cultivate0)
			{
				_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("num%d"), i+1);
				m_pStcAttEx[i] = (GUIStatic*)m_pWndAttIn->GetChild(szTmp);
			}
			else
			{
				_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("num%d"), i+1);
				m_pStcAttEx[i] = (GUIStatic*)m_pWndAttEx->GetChild(szTmp);
			}
			m_pStcAttEx[i]->SetText(_T(""));
		}
	}
	//����
	{
		m_pWndRes = m_pWnd->GetChild(_T("rs_pic3"));
		for (INT i = 0; i < EDR_End; i++)
		{
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("spic%d"), i+1);
			m_pStcResPic[i] = (GUIStatic*)m_pWndRes->GetChild(szTmp);
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("dnum%d"), i+1);
			m_pStcResValue[i] = (GUIStatic*)m_pWndRes->GetChild(szTmp);
			m_pStcResValue[i]->SetText(_T(""));
			m_pStcResPic[i]->SetInvisible(TRUE);
		}
	}
	//������ʾ
	{
		m_pPbnDisplayAtt = (GUIPushButton*)m_pWnd->GetChild(_T("word1\\optnutt1"));
		m_pPbnDisplayAtt->SetState(EGUIBS_PushDown, FALSE);

		// ʱװ��װ��ת��
		m_pFmSuitOpt = (GUIFrameMutex*)m_pWnd->GetChild(_T("word1\\ra_opt"));
		m_pPbnDisplayEquip = (GUIPushButton*)m_pWnd->GetChild(_T("word1\\ra_opt\\optbutt2"));
		m_pPbnDisplayFashion = (GUIPushButton*)m_pWnd->GetChild(_T("word1\\ra_opt\\optbutt3"));
	}
	
	return TRUE;
}

VOID StateFrame::Update()
{
	/*! �����ۻ�ʱ�䣬����ҵ����װ��/ʱװģʽʱ�����ӳ� */
	static DWORD dwAccumTime = 0;
	if (m_isSwappingSuitPattern)
		{
		dwAccumTime += Kernel::Inst()->GetDeltaTimeDW();
		if (dwAccumTime >= BUTTON_DELAY_MS)
		{
			dwAccumTime = 0;
			m_isSwappingSuitPattern = false;
			m_pPbnDisplayEquip->SetEnable(true);
			m_pPbnDisplayEquip->SetRefresh();
			m_pPbnDisplayFashion->SetEnable(true);
			m_pPbnDisplayFashion->SetRefresh();
		}
	}

    //ˢ������ٶ�
    static DWORD dwTimeFirst = 0;
    LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
    if (P_VALID(m_pWndState)
        && !m_pWndState->IsInvisible()
        && P_VALID(pLp)
        && pLp->IsAttributeInited()
        && Kernel::Inst()->GetAccumTimeDW() - dwTimeFirst > 3000)
    {
        if (Kernel::Inst()->GetAccumTimeDW() - dwTimeFirst > 3000)
        {
            UpdateAttribute(ERA_Speed_XZ);
            UpdateAttribute(ERA_Speed_Mount);

            dwTimeFirst = Kernel::Inst()->GetAccumTimeDW();
        }
    }
}
DWORD StateFrame::OnGameEvent(tagGameEvent* pGameEvent)
{
	if (pGameEvent->strEventName == _T("tagUpdateLPAttEvent"))
	{
		tagUpdateLPAttEvent* pEvent = (tagUpdateLPAttEvent*)pGameEvent;
		UpdateAttribute(pEvent->eType);
	}
	else if (pGameEvent->strEventName == _T("tagUpdateLPLevel"))
	{
		tagUpdateLPLevel* pEvent = (tagUpdateLPLevel*)pGameEvent;
		SetPlayerLevel(pEvent->nCurLevel);
		//�Ƴ�ԤͶ��
		RemoveBidPoint();
		//ˢ���������
		UpdateBidAttribute();
	}
	else if (pGameEvent->strEventName == _T("tagRoleGetNameEvent"))
	{
		tagRoleGetNameEvent* pEvent = (tagRoleGetNameEvent*)pGameEvent;
		if(pEvent->dwRoleID == RoleMgr::Inst()->GetLocalPlayerID())
		{
			SetPlayerName(PlayerNameTab::Inst()->FindNameByID(pEvent->dwRoleID));
		}
	}
	else if (pGameEvent->strEventName == _T("tagUpdateLPBidAttEvent"))
	{
		tagUpdateLPAttEvent* pEvent = (tagUpdateLPAttEvent*)pGameEvent;
		
	}
    else if (pGameEvent->strEventName == _T("tagInitLPAttEvent"))
    {
        // ��ɫ��ʼװ��ģʽ
        LocalPlayer* pLP = RoleMgr::Inst()->GetLocalPlayer();
        if( P_VALID( pLP ) && pLP->IsAttributeInited() )
        {
            bool bFashion = RoleMgr::Inst()->GetLocalPlayer()->GetDisplaySet().bFashionDisplay;
            if (bFashion)
                m_pPbnDisplayFashion->SetState(EGUIBS_PushDown, FALSE);
            else
                m_pPbnDisplayEquip->SetState(EGUIBS_PushDown, FALSE);
        }
    }
	return 0;
}

DWORD StateFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if (pWnd == m_pBtnSure)//ȷ��
			{
				
				//����Ͷ������ 
				SendNetBidAtt();
				//�Ƴ�ԤͶ��
				RemoveBidPoint();
			}
			else if (pWnd == m_pBtnCancel)//����
			{
				//�Ƴ�ԤͶ��
				RemoveBidPoint();
				//ˢ���������
				UpdateBidAttribute();
			} 
			else
			{
				for (INT i = 0; i < EDP_End; i++)
				{
					if (pWnd == m_pBtnAtt[i])
					{
						//ԤͶ��
						if(m_nOverplus > 0)
						{
							AddPreBidPoint(TransBtn2Att((EDisplayProperty)i), 1);
							SetPlayerOverplus(m_nOverplus - 1);
						}
						break;
					}
				}
			}
		}
		break;
	case EGUIE_PushButtonDown:
		{
			if (pWnd == m_pPbnDisplayAtt)
			{
				//Ͷ�����
				m_pWndAtt->SetInvisible(pEvent->dwParam1 == 2);

			}
			else if(pWnd == m_pPbnDisplayEquip)
			{
				m_isSwappingSuitPattern = true;
				m_pPbnDisplayEquip->SetEnable(false);
				m_pPbnDisplayEquip->SetRefresh();
				m_pPbnDisplayFashion->SetEnable(false);
				m_pPbnDisplayFashion->SetRefresh();
				/*! ��������װ��ģʽ��Ϸ�¼� */
				tagFashionEvent e(_T("FashionEvent"), this);
				e.bDisFashion = false;
				m_pFrameMgr->SendEvent(&e);
			}
			else if(pWnd == m_pPbnDisplayFashion)
			{
				m_isSwappingSuitPattern = true;
				m_pPbnDisplayEquip->SetEnable(false);
				m_pPbnDisplayEquip->SetRefresh();
				m_pPbnDisplayFashion->SetEnable(false);
				m_pPbnDisplayFashion->SetRefresh();
				/*! ��������ʱװģʽ��Ϸ�¼� */
				tagFashionEvent e(_T("FashionEvent"), this);
				e.bDisFashion = true;
				m_pFrameMgr->SendEvent(&e);
			}
		}
	}
	return 0;
}

VOID StateFrame::UpdatePalyerAtt()
{
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(P_VALID(pLp))
	{
		SetPlayerName(PlayerNameTab::Inst()->FindNameByID(pLp->GetID()));
		SetPlayerLevel(pLp->GetRoleLevel());
		//�Ƴ�ԤͶ��
		RemoveBidPoint();
		//ˢ���������
		UpdateBidAttribute();
	}
}

VOID StateFrame::SetPlayerName(LPCTSTR szName)
{
	m_pStcPlayerName->SetText(szName);
}

VOID StateFrame::SetPlayerLevel(INT nLevel)
{
	TCHAR szTmp[X_SHORT_NAME] = {0};
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("Role_Level")], nLevel);
	m_pStcPlayerLevel->SetText(szTmp);
	m_pStcPlayerLevel->SetRefresh(TRUE);

	// �ȼ��ı䣬��Ӧ����ɳ�ֵҲҪ�����仯
	const tagExperience* pExp = LevelUpData::Inst()->FindExpData(nLevel);
	if(P_VALID(pExp))
	{
		for(INT i=0; i<X_ERA_ATTA_NUM; ++i)
		{
			m_nOgnAtt[i] = pExp->nAtt[i];
		}
	}
}

VOID StateFrame::SetPlayerClass(LPCTSTR szClass)
{
	m_pStcPlayerLevel->SetText(szClass);
}

VOID StateFrame::SetPlayerGuild(LPCTSTR szGuild)
{
	m_pStcPlayerGuild->SetText(szGuild);
}

VOID StateFrame::SetPlayerOverplus(INT nValue)
{
	TCHAR szTmp[X_SHORT_NAME] = {0};
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue);
	m_pStcOverplus->SetText(szTmp);
	m_pStcOverplus->SetRefresh(TRUE);
	
	if(0 != m_nOverplus && 0 == nValue)
	{
		for(INT i=0; i<X_ERA_ATTA_NUM; ++i)
		{
			m_pBtnAtt[i]->SetEnable(FALSE);
		}
	}
	else if(0 == m_nOverplus && 0 != nValue)
	{
		for(INT i=0; i<X_ERA_ATTA_NUM; ++i)
		{
			m_pBtnAtt[i]->SetEnable(TRUE);
		}
	}
	m_nOverplus = nValue;
}

VOID StateFrame::SetPlayerAtt(EDisplayProperty eType, INT nValue, DWORD dwColor /* = ORDINARY_COLOR */)
{
	TCHAR szTmp[X_SHORT_NAME] = {0};
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue);
	m_pStcAtt[eType]->SetText(szTmp);
	m_pStcAtt[eType]->SetTextColor(dwColor);
	m_pStcAtt[eType]->SetRefresh(TRUE);
}

VOID StateFrame::SetPlayerAttEx(EDisplayPropertyEx eType, INT nValue, DWORD dwColor /* = ORDINARY_COLOR */)
{
	TCHAR szTmp[X_SHORT_NAME] = {0};
	switch(eType)
	{
	case EDPE_ArmDefine0:		// ���׷�������ֵ
	case EDPE_MoveSpeed0:		// �ƶ��ٶ�����ֵ
	case EDPE_Luck0:			// ��Ե����ֵ
	case EDPE_Feature0:			// ��������ֵ
	case EDPE_Command0:			// ͳ������ֵ
	case EDPE_Morality0:		// ��������ֵ
	case EDPE_Cultivate0:		// ��Ϊ����ֵ
	case EDPE_MeleeDamage0:		// �⹦��������ֵ
	case EDPE_MeleeDefine0:		// �⹦��������ֵ
	case EDPE_MagicDamage0:		// �ڹ���������ֵ
	case EDPE_MagicDefine0:		// �ڹ���������ֵ
	case EDPE_DamageSkill0:		// ������������ֵ
	case EDPE_DefineSkill0:		// ������������ֵ
	case EDPE_Hit0:				// ��������ֵ
	case EDPE_Dodge0:			// ��������ֵ
		{
			if(MINUS_RED_COLOR == dwColor)
				_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue);
			else if(PLUS_GREEN_COLOR == dwColor)
				_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("+%d"), nValue);
				
		}
		break;
	case EDPE_Energy0:			// �������ֵ
	case EDPE_Health0:			// �������ֵ
	case EDPE_Power0:			// �������ֵ
	case EDPE_Toughness0:		// �־������ֵ
		{
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("/%d"), nValue);
		}
		break;
	case EDPE_WeaponDamage0:
		{
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("-%d"), nValue);
		}
		break;
	case EDPE_MoveSpeed:
		{
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d%%"), nValue/100);
		}
		break;
	default:
		{
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue);
		}
		break;
	}
	m_pStcAttEx[eType]->SetText(szTmp);
	m_pStcAttEx[eType]->SetTextColor(dwColor);
	m_pStcAttEx[eType]->SetRefresh(TRUE);
}

VOID StateFrame::SetPlayerResist(EDisplayResist eType, INT nValue)
{
	TCHAR szTmp[X_SHORT_NAME] = {0};
	if(nValue >= 0)
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("+%d"), abs(nValue));
	else
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("-%d"), abs(nValue));

	m_pStcResValue[eType]->SetText(szTmp);
	m_pStcResPic[eType]->SetInvisible(FALSE);
	m_pStcResPic[eType]->SetRefresh(TRUE);
}

VOID StateFrame::UpdateAttribute(ERoleAttribute eType)
{
	INT nValue1,nValue2,nValue3;
	LocalPlayer* pLP = RoleMgr::Inst()->GetLocalPlayer();
	nValue1 = pLP->GetAttribute(eType);
	DWORD dwColor = ORDINARY_COLOR;

	switch(eType)
	{
	case ERA_Physique:			// ��ǰ���
		{
			// ��ǰ��ǵ���ʾ��Ҫ���ϵ�ǰ��ԤͶ��ֵ�����ڽ�����ʾЧ��
			nValue1 = nValue1 + m_nPreBid[eType];
			SetPlayerAtt(EDP_Physique, nValue1);

			// ԭʼ����=��ǰ��ǡ�10
			m_Temporary[EDT_Origin_Health] = (INT)(nValue1 * 10);
			if(m_nPreBid[eType] > 0)
			{
				nValue3 = pLP->GetAttribute(ERA_MaxHP) + (INT)(m_nPreBid[eType] * 10);
				SetPlayerAttEx(EDPE_Health0, nValue3);
			}
			
			// ��ǰ��������ʾ��Ҫ���ϵ�ǰ��ԤͶ��ֵ�����ڽ�����ʾЧ�� 
			nValue2 = pLP->GetAttribute(ERA_Strength) + m_nPreBid[ERA_Strength];
			// ԭʼ�⹥����=��ǰ��ǡ�5+��ǰ������1
			m_Temporary[EDT_Origin_ExDefine] = (INT)(nValue1 * 5 + nValue2 * 1); 
			SetPlayerAttEx(EDPE_MeleeDefine, m_Temporary[EDT_Origin_ExDefine]);
		}
		break;
	case ERA_Strength:			// ��ǰ����
		{
			// ��ǰ��������ʾ��Ҫ���ϵ�ǰ��ԤͶ��ֵ�����ڽ�����ʾЧ��
			nValue1 = nValue1 + m_nPreBid[eType];
			SetPlayerAtt(EDP_Strength, nValue1);

			// ԭʼ�⹥����=��ǰ������5
			m_Temporary[EDT_Origin_ExDamge] = (INT)(nValue1 * 5); 
			SetPlayerAttEx(EDPE_MeleeDamage, m_Temporary[EDT_Origin_ExDamge]);
			
			
			nValue2 = pLP->GetAttribute(ERA_Physique) + m_nPreBid[ERA_Physique];
			// ԭʼ�⹥����=��ǰ��ǡ�5+��ǰ������1
			m_Temporary[EDT_Origin_ExDefine] = (INT)(nValue2 * 5 + nValue1 * 1); 
			SetPlayerAttEx(EDPE_MeleeDefine, m_Temporary[EDT_Origin_ExDefine]);
		}
		break;
	case ERA_Pneuma:			// ��ǰԪ��
		{
			nValue1 = nValue1 + m_nPreBid[eType];
			SetPlayerAtt(EDP_Pneuma, nValue1);

			// ԭʼ����=��ǰԪ����10
			m_Temporary[EDT_Origin_Power] = (INT)(nValue1 * 10);
			if(m_nPreBid[eType] > 0)
			{
				nValue3 = pLP->GetAttribute(ERA_MaxMP) + (INT)(m_nPreBid[eType] * 10);
				SetPlayerAttEx(EDPE_Power0, nValue3);
			}
			
			// ԭʼ�ڹ�����=��ǰԪ����5+��ǰ������1
			nValue2 = pLP->GetAttribute(ERA_InnerForce) + m_nPreBid[ERA_InnerForce];
			m_Temporary[EDT_Origin_InDefine] = (INT)(nValue1 * 5 + nValue2 * 1);
			SetPlayerAttEx(EDPE_MagicDefine, m_Temporary[EDT_Origin_InDefine]);
		}
		break;
	case ERA_InnerForce:		// ��ǰ����
		{
			nValue1 = nValue1 + m_nPreBid[eType];
			SetPlayerAtt(EDP_InnerForce, nValue1);

			// ԭʼ�ڹ�����=��ǰ������5
			m_Temporary[EDT_Origin_InDamage] = (INT)(nValue1 * 5);
			SetPlayerAttEx(EDPE_MagicDamage, m_Temporary[EDT_Origin_InDamage]);
			
			// ԭʼ�ڹ�����=��ǰԪ����5+��ǰ������1
			nValue2 = pLP->GetAttribute(ERA_Pneuma) + m_nPreBid[ERA_Pneuma];
			m_Temporary[EDT_Origin_InDefine] = (INT)(nValue2 * 5 + nValue1 * 1);
			SetPlayerAttEx(EDPE_MagicDefine, m_Temporary[EDT_Origin_InDefine]);
		}
		break;
	case ERA_Technique:			// ��ǰ����
		{
			nValue1 = nValue1 + m_nPreBid[eType];
			SetPlayerAtt(EDP_Technique, nValue1);

			// ԭʼ��������=��ǰ������6
			m_Temporary[EDT_Origin_DmgSkill] = (INT)(nValue1 * 6); 
			SetPlayerAttEx(EDPE_DamageSkill, m_Temporary[EDT_Origin_DmgSkill]);

			// ԭʼ����=��ǰ������10
			m_Temporary[EDT_Origin_Hit] = (INT)(nValue1 * 10);
			SetPlayerAttEx(EDPE_Hit, m_Temporary[EDT_Origin_Hit]);

			// ԭʼ�־���=[98]+������ǰֵ��0.1+����ǰֵ��0.1
		}
		break;
	case ERA_Agility:			// ��ǰ��
		{
			nValue1 = nValue1 + m_nPreBid[eType];
			SetPlayerAtt(EDP_Agility, nValue1);

			// ԭʼ��������=��ǰ����4
			m_Temporary[EDT_Origin_DefSkill] = (INT)(nValue1 * 4);
			SetPlayerAttEx(EDPE_DefineSkill, m_Temporary[EDT_Origin_DefSkill]);
			
			// ԭʼ����=��ǰ����10
			m_Temporary[EDT_Origin_Dodge] = (INT)(nValue1 * 10);
			SetPlayerAttEx(EDPE_Dodge, m_Temporary[EDT_Origin_Dodge]);

			// ԭʼ�־���=[98]+������ǰֵ��0.1+����ǰֵ��0.1
		}
		break;
	case ERA_AttPoint:			// ��ǰ���Ե���
		{
			SetPlayerOverplus(nValue1);
		}
		break;
	case ERA_MaxHP:				// �������
		{
			nValue2 = nValue1 - m_Temporary[EDT_Origin_Health];
			SetPlayerAttEx(EDPE_Health0, nValue1, GETTRUECOLOR(nValue2));
		}
		break;
	case ERA_HP:				// ��ǰ����
		{
			SetPlayerAttEx(EDPE_Health, nValue1);
		}
		break;
	case ERA_MaxMP:				// �������
		{
			nValue2 = nValue1 - m_Temporary[EDT_Origin_Power];
			SetPlayerAttEx(EDPE_Power0, nValue1, GETTRUECOLOR(nValue2));
		}
		break;
	case ERA_MP:				// ��ǰ����
		{
			SetPlayerAttEx(EDPE_Power, nValue1);
		}
		break;
	case ERA_ExAttack:			// �⹦����
		{
			nValue2 = nValue1 - m_Temporary[EDT_Origin_ExDamge];
			SetPlayerAttEx(EDPE_MeleeDamage0, nValue2, GETTRUECOLOR(nValue2));
		}
		break;
	case ERA_ExDefense:			// �⹦����
		{
			nValue2 = nValue1 - m_Temporary[EDT_Origin_ExDefine];
			SetPlayerAttEx(EDPE_MeleeDefine0, nValue2, GETTRUECOLOR(nValue2));
		}
		break;
	case ERA_InAttack:			// �ڹ�����
		{
			nValue2 = nValue1 - m_Temporary[EDT_Origin_InDamage];
			SetPlayerAttEx(EDPE_MagicDamage0, nValue2, GETTRUECOLOR(nValue2));
		}
		break;
	case ERA_InDefense:			// �ڹ�����
		{
			nValue2 = nValue1 - m_Temporary[EDT_Origin_InDefine];
			SetPlayerAttEx(EDPE_MagicDefine0, nValue2, GETTRUECOLOR(nValue2));
		}
		break;
	case ERA_AttackTec:			// ��������
		{
			nValue2 = nValue1 - m_Temporary[EDT_Origin_DmgSkill];
			SetPlayerAttEx(EDPE_DamageSkill0, nValue2, GETTRUECOLOR(nValue2));
		}
		break;
	case ERA_DefenseTec:		// ��������
		{
			nValue2 = nValue1 - m_Temporary[EDT_Origin_DefSkill];
			SetPlayerAttEx(EDPE_DefineSkill0, nValue2, GETTRUECOLOR(nValue2));
		}
		break;
	case ERA_HitRate:			// ������
		{
			nValue2 = nValue1 - m_Temporary[EDT_Origin_Hit];
			SetPlayerAttEx(EDPE_Hit0, nValue2, GETTRUECOLOR(nValue2));
		}
		break;
	case ERA_Dodge:				// ������
		{
			nValue2 = nValue1 - m_Temporary[EDT_Origin_Dodge];
			SetPlayerAttEx(EDPE_Dodge0, nValue2, GETTRUECOLOR(nValue2));
		}
		break;
	case ERA_Endurance:			// �־���
		{
			SetPlayerAttEx(EDPE_Toughness, nValue1);
			//nValue2 = nValue1 - m_Temporary[EDT_Origin_Toughness];
			//SetPlayerAttEx(EDPE_Toughness0, nValue2, GETTRUECOLOR(nValue2));
		}
		break;
	case ERA_MaxEndurance:
		{
			SetPlayerAttEx(EDPE_Toughness0, nValue1);
		}
		break;
	case ERA_Vitality:			// ����
		{
			SetPlayerAttEx(EDPE_Energy, nValue1);
		}
		break;
	case ERA_MaxVitality:		// ������
		{
			SetPlayerAttEx(EDPE_Energy0, nValue1);
		}
		break;
	case ERA_Speed_XZ:			// XZƽ���ٶ��ٶ�
		{
            // �Ƿ������״̬
            LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
            if (P_VALID(pLp) && !P_VALID(pLp->GetMount()))
                SetPlayerAttEx(EDPE_MoveSpeed, nValue1);
		}
		break;
    case ERA_Speed_Mount:
        {
            // �Ƿ������״̬
            LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
            if (P_VALID(pLp) && P_VALID(pLp->GetMount()))
                SetPlayerAttEx(EDPE_MoveSpeed, nValue1);
        }
        break;
	case ERA_WeaponDmgMin:		// ��С��������
		{
			SetPlayerAttEx(EDPE_WeaponDamage, nValue1);
		}
		break;
	case ERA_WeaponDmgMax:		// �����������
		{
			SetPlayerAttEx(EDPE_WeaponDamage0, nValue1);
		}
		break;
	case ERA_WeaponSoul:		// ���
		{
			SetPlayerAttEx(EDPE_WuHun, nValue1);
		}
		break;
	case ERA_Armor:				// ���߻���
		{
			SetPlayerAttEx(EDPE_ArmDefine, nValue1);
		}
		break;
	case ERA_Derate_Ordinary:	// ��ͨ�˺�����
		{
            SetPlayerResist(EDR_OrdinaryResist, CalExAttackResist(nValue1));
		}
		break;
	case ERA_Derate_Bleeding:	// ��Ѫ�˺�����
		{
			SetPlayerResist(EDR_BleedResist, CalExAttackResist(nValue1));
		}
		break;
	case ERA_Derate_Brunt:		// ����˺�����
		{
			SetPlayerResist(EDR_BruntResist, CalExAttackResist(nValue1));
		}
		break;
	case ERA_Derate_Bang:		// �ػ��˺�����
		{
			SetPlayerResist(EDR_BangResist, CalExAttackResist(nValue1));
		}
		break;
	case ERA_Derate_Poison:		// �����˺�����
		{
			SetPlayerResist(EDR_PoisonResist, CalInAttackResist(nValue1));
		}
		break;
	case ERA_Derate_Thinker:	// �����˺�����
		{
			SetPlayerResist(EDR_ThinkerResist, CalInAttackResist(nValue1));
		}
		break;
	case ERA_Derate_Injury:		// �����˺�����
		{
			SetPlayerResist(EDR_InjuryResist, CalInAttackResist(nValue1));
		}
		break;
	case ERA_Derate_Stunt:		// �����˺�����
		{
		}
		break;
	case ERA_Derate_ExAttack:	// �⹦�˺�����
		{
            UpdateAttribute(ERA_Derate_Ordinary);
            UpdateAttribute(ERA_Derate_Bleeding);
            UpdateAttribute(ERA_Derate_Brunt);
            UpdateAttribute(ERA_Derate_Bang);
		}
		break;
	case ERA_Derate_InAttack:	// �ڹ��˺�����
		{
            UpdateAttribute(ERA_Derate_Poison);
            UpdateAttribute(ERA_Derate_Thinker);
            UpdateAttribute(ERA_Derate_Injury);
		}
		break;
	case ERA_Derate_ALL:		// �����˺�����
		{
            UpdateAttribute(ERA_Derate_Ordinary);
            UpdateAttribute(ERA_Derate_Bleeding);
            UpdateAttribute(ERA_Derate_Brunt);
            UpdateAttribute(ERA_Derate_Bang);

            UpdateAttribute(ERA_Derate_Poison);
            UpdateAttribute(ERA_Derate_Thinker);
            UpdateAttribute(ERA_Derate_Injury);
		}
		break;
	case ERA_Morale:			// ʿ��
		{
			SetPlayerAttEx(EDPE_Morale, nValue1);
		}
		break;
	case ERA_Injury:			// ����
		{
			SetPlayerAttEx(EDPE_InternalInjury, nValue1, MINUS_RED_COLOR);

			//��ʼ��ʱ�������Ϊ0����ʾ
			if( nValue1!=m_nInternalInjury && 0==nValue1 )
			{
				m_nInternalInjury = nValue1;
				return;
			}

			if( nValue1 == 0 )
			{
				ChatFrame* pFrame = (ChatFrame*)m_pMgr->GetFrame(_T("Chat"));
				if( P_VALID(pFrame) )
				{
					pFrame->PushInfo( g_StrTable[_T("RoleChat_Injury1")], ESCC_System );
				}
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("RoleChat_Injury3")]);
			}
			else if( nValue1 > 0  )
			{
				ChatFrame* pFrame = (ChatFrame*)m_pMgr->GetFrame(_T("Chat"));
				if( P_VALID(pFrame) )
				{
					pFrame->PushInfo( g_StrTable[_T("RoleChat_Injury0")], ESCC_System );
				}
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("RoleChat_Injury2")]);

				LocalPlayer *pLp = RoleMgr::Inst()->GetLocalPlayer();
				if( P_VALID(pLp) )
				{
					int nvalue = pLp->GetAttribute( ERA_HP );
					if( nvalue == 0 || pLp->IsDead() )
					{
						TCHAR szInjury[X_LONG_NAME] = {0};
						_sntprintf( szInjury, X_LONG_NAME, g_StrTable[_T("Dead_Injury")], nValue1 );
						CombatSysUtil::Inst()->ShowScreenCenterMsgEx(szInjury);
					}
				}
			}
		}
		break;
	case ERA_Fortune:			// ��Ե
		{
			SetPlayerAttEx(EDPE_Luck, nValue1);
		}
		break;
	case ERA_Appearance:		// ����
		{
			SetPlayerAttEx(EDPE_Feature, nValue1);
		}
		break;
	case ERA_Rein:				// ͳ��
		{
			SetPlayerAttEx(EDPE_Command, nValue1);
		}
		break;
	case ERA_Knowledge:			// ����
		{

		}
		break;
	case ERA_Morality:			// ����
		{
			SetPlayerAttEx(EDPE_Morality, nValue1);
		}
		break;
	case ERA_Culture:			// ��Ϊ
		{
			SetPlayerAttEx(EDPE_Cultivate, nValue1);
		}
		break;
	default:
		break;
	}
}

VOID StateFrame::AddPreBidPoint( ERoleAttribute eType, INT nInc )
{
	ASSERT(eType >= ERA_AttA_Start && eType <= ERA_AttA_End);

	m_nPreBid[eType] += nInc;
	UpdateAttribute(eType);
	
	m_pBtnSure->SetEnable(TRUE);
	m_pBtnCancel->SetEnable(TRUE);
}

VOID StateFrame::RemoveBidPoint()
{
	for(INT i=0; i<X_ERA_ATTA_NUM; ++i)
	{
		m_nPreBid[i] = 0;
	}

	m_pBtnSure->SetEnable(FALSE);
	m_pBtnCancel->SetEnable(FALSE);
}

VOID StateFrame::SendNetBidAtt()
{
	tagNC_RoleBidAttPoint cmd;
	for(INT i=0; i<X_ERA_ATTA_NUM; ++i)
	{
		cmd.nAttPointAdd[i] = m_nPreBid[i];
	}
	m_pSession->Send(&cmd);
}

ERoleAttribute StateFrame::TransBtn2Att( EDisplayProperty eType )
{
	switch (eType)
	{
	case EDP_Physique:
		return ERA_Physique;
	case EDP_Strength:
		return ERA_Strength;
	case EDP_InnerForce:
		return ERA_InnerForce;
	case EDP_Pneuma:
		return ERA_Pneuma;
	case EDP_Agility:
		return ERA_Agility;
	case EDP_Technique:
		return ERA_Technique;
	}
	return ERA_Null;
}

VOID StateFrame::SetBiddenPoint( ERoleAttribute eType, INT nValue )
{
	ASSERT(eType >= ERA_AttA_Start && eType <= ERA_AttA_End);

	m_nPreBid[eType] = nValue;

	// ��������
}

VOID StateFrame::UpdateBidAttribute()
{
	for(INT i=0; i<X_ERA_ATTA_NUM; ++i)
	{
		UpdateAttribute((ERoleAttribute)i);
	}
    for (int i = 0; i < X_ERA_DERATE_NUM; ++i)
    {
        UpdateAttribute((ERoleAttribute)(ERA_Derate_Start + i));
    }
	UpdateAttribute(ERA_MaxHP);
	UpdateAttribute(ERA_MaxMP);
	UpdateAttribute(ERA_AttPoint);
}

int StateFrame::CalExAttackResist( int src )
{
    LocalPlayer* pLP = RoleMgr::Inst()->GetLocalPlayer();
    if (!P_VALID(pLP))
        return 0;

    int nRetValue = src
        + pLP->GetAttribute(ERA_Derate_ALL)
        + pLP->GetAttribute(ERA_Derate_ExAttack);

    if (nRetValue > MAX_PLAYER_RESIST)
        nRetValue = MAX_PLAYER_RESIST;
    else if (nRetValue < MIN_PLAYER_RESIST)
        nRetValue = MIN_PLAYER_RESIST;

    return nRetValue;
}

int StateFrame::CalInAttackResist( int src )
{
    LocalPlayer* pLP = RoleMgr::Inst()->GetLocalPlayer();
    if (!P_VALID(pLP))
        return 0;

    int nRetValue = src
        + pLP->GetAttribute(ERA_Derate_ALL)
        + pLP->GetAttribute(ERA_Derate_InAttack);

    if (nRetValue > MAX_PLAYER_RESIST)
        nRetValue = MAX_PLAYER_RESIST;
    else if (nRetValue < MIN_PLAYER_RESIST)
        nRetValue = MIN_PLAYER_RESIST;

    return nRetValue;
}