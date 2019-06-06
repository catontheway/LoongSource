#include "stdafx.h"
#include "EquipmentFrame.h"
#include "CharacterFrame.h"
#include "ItemButton.h"
#include "PlayerViewWindow.h"
#include "RoleMgr.h"
#include "ItemMgr.h"
#include "Item.h"
#include "LocalPlayer.h"
#include "ItemEvent.h"
#include "RoleEvent.h"
#include "DragBtn.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "CombatSysUtil.h"
#include "..\WorldDefine\msg_item.h"
#include "CombatSysUtil.h"

EquipmentFrame::EquipmentFrame():m_Trunk(this)
{
	m_pWnd = NULL;
	m_pWndFather = NULL;
	m_pWndEquipView = NULL;
	for (INT i=0; i<EEP_MaxEquip; ++i)
	{
		m_pStcEquipBack[EEP_MaxEquip] = NULL;
		m_pBtnEquip[EEP_MaxEquip] = NULL;
	}

	m_bIsFashionDisplay = FALSE;

}

EquipmentFrame::~EquipmentFrame(void)
{
}

BOOL EquipmentFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);

	m_pFrameMgr->RegisterEventHandle(_T("ItemEvent_UpdateItem"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipmentFrame::OnUpdateItemEvent));
	m_pFrameMgr->RegisterEventHandle(_T("ItemEvent_Move2Equip"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipmentFrame::OnChangeItemEvent));
	m_pFrameMgr->RegisterEventHandle(_T("tagUpdataAvatarEvent"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipmentFrame::OnUpdateAvatarEvent));
	m_pFrameMgr->RegisterEventHandle(_T("FashionEvent"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipmentFrame::OnChangDispPattern));
	m_pFrameMgr->RegisterEventHandle(_T("tagUpdataAvatarDisplaySetEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipmentFrame::OnUpdataAvatarDisplaySetEvent));

	m_pKeyMap->Register(0, g_StrTable[_T("Hotkey_SwapWeapon")], (INPUTMAPEVENTHANDLE)m_Trunk.sfp2(&EquipmentFrame::OnKeySwapWeapon), DIK_E);


	return bRet;
}

BOOL EquipmentFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	m_pFrameMgr->UnRegisterEventHandler(_T("ItemEvent_UpdateItem"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipmentFrame::OnUpdateItemEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("ItemEvent_Move2Equip"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipmentFrame::OnChangeItemEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagUpdataAvatarEvent"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipmentFrame::OnUpdateAvatarEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("FashionEvent"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipmentFrame::OnChangDispPattern));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagUpdataAvatarDisplaySetEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipmentFrame::OnUpdataAvatarDisplaySetEvent));

	m_pKeyMap->SetEnable(g_StrTable[_T("Hotkey_SwapWeapon")], false);
	return bRet;
}

BOOL EquipmentFrame::ReloadUI()
{
	// ����
	m_pWndFather = m_pGUI->GetWnd(((CharacterFrame*)m_pFather)->GetGUIFullName().c_str());
	m_pWnd = m_pWndFather->GetChild(_T("rstatepic\\equipback"));

	// ע���¼�������
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&EquipmentFrame::EventHandler));
	
	// ��ȡ�ؼ�
	// װ��Ԥ��
	m_pWndEquipView = (PlayerViewWindow*)m_pWnd->GetChild(_T("role"));
	// ͷ��
	m_pStcEquipBack[EEP_Head] = (GUIStatic*)m_pWnd->GetChild(_T("armet"));
	m_pBtnEquip[EEP_Head] = (ItemButton*)m_pStcEquipBack[EEP_Head]->GetChild(_T("armetpic"));
	// �沿
	m_pStcEquipBack[EEP_Face] = (GUIStatic*)m_pWnd->GetChild(_T("mask"));
	m_pBtnEquip[EEP_Face] = (ItemButton*)m_pStcEquipBack[EEP_Face]->GetChild(_T("maskpic"));
	// ����
	m_pStcEquipBack[EEP_Body] = (GUIStatic*)m_pWnd->GetChild(_T("armor"));
	m_pBtnEquip[EEP_Body] = (ItemButton*)m_pStcEquipBack[EEP_Body]->GetChild(_T("armorpic"));
	// ����
	m_pStcEquipBack[EEP_Legs] = (GUIStatic*)m_pWnd->GetChild(_T("pants"));
	m_pBtnEquip[EEP_Legs] = (ItemButton*)m_pStcEquipBack[EEP_Legs]->GetChild(_T("pantspic"));
	// ����
	m_pStcEquipBack[EEP_Back] = (GUIStatic*)m_pWnd->GetChild(_T("mantle"));
	m_pBtnEquip[EEP_Back] = (ItemButton*)m_pStcEquipBack[EEP_Back]->GetChild(_T("mantlepic"));
	// ��
	m_pStcEquipBack[EEP_Wrist] = (GUIStatic*)m_pWnd->GetChild(_T("cuff"));
	m_pBtnEquip[EEP_Wrist] = (ItemButton*)m_pStcEquipBack[EEP_Wrist]->GetChild(_T("cuffpic"));
	// �Ų�
	m_pStcEquipBack[EEP_Feet] = (GUIStatic*)m_pWnd->GetChild(_T("boot"));
	m_pBtnEquip[EEP_Feet] = (ItemButton*)m_pStcEquipBack[EEP_Feet]->GetChild(_T("bootpic"));
	// ��ָ
	m_pStcEquipBack[EEP_Finger1] = (GUIStatic*)m_pWnd->GetChild(_T("ring1"));
	m_pBtnEquip[EEP_Finger1] = (ItemButton*)m_pStcEquipBack[EEP_Finger1]->GetChild(_T("ring1pic"));
	// ��ָ
	m_pStcEquipBack[EEP_Finger2] = (GUIStatic*)m_pWnd->GetChild(_T("ring2"));
	m_pBtnEquip[EEP_Finger2] = (ItemButton*)m_pStcEquipBack[EEP_Finger2]->GetChild(_T("ring2pic"));
	// ����
	m_pStcEquipBack[EEP_Waist] = (GUIStatic*)m_pWnd->GetChild(_T("sash"));
	m_pBtnEquip[EEP_Waist] = (ItemButton*)m_pStcEquipBack[EEP_Waist]->GetChild(_T("sashpic"));
	// ����
	m_pStcEquipBack[EEP_Neck] = (GUIStatic*)m_pWnd->GetChild(_T("necklace"));
	m_pBtnEquip[EEP_Neck] = (ItemButton*)m_pStcEquipBack[EEP_Neck]->GetChild(_T("nlpic"));
	// ��������
	m_pStcEquipBack[EEP_RightHand] = (GUIStatic*)m_pWnd->GetChild(_T("mainwp"));
	m_pBtnEquip[EEP_RightHand] = (ItemButton*)m_pStcEquipBack[EEP_RightHand]->GetChild(_T("mainwppic"));
	// ��������
	m_pStcEquipBack[EEP_LeftHand] = (GUIStatic*)m_pWnd->GetChild(_T("minorwp"));
	m_pBtnEquip[EEP_LeftHand] = (ItemButton*)m_pStcEquipBack[EEP_LeftHand]->GetChild(_T("minorwppic"));
	
	// ����װ����ʾͼ��
	UpdateAll();

	return TRUE;
}

VOID EquipmentFrame::Update()
{	
}

DWORD EquipmentFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			for(INT i=0; i<EEP_MaxEquip; ++i)
			{
				if (m_bIsFashionDisplay && i > EEP_Feet && i != EEP_RightHand && i != EEP_LeftHand)
				{
					continue;
				}

				if(pWnd == m_pBtnEquip[i])
				{
					Equipment* pEquip = ItemMgr::Inst()->GetCurEquip(GetEquipPos(i));
					
					if(P_VALID(pEquip))
					{
						tagItemPosChangeExEvent event(_T("ItemEvent_Move2Bag"),this);
						event.dwTypeIDSrc = pEquip->GetItemTypeID();
						event.eTypeSrc = EICT_Equip;
						event.eTypeDst = EICT_Bag;
						event.n16IndexSrc = pEquip->GetPos();
						m_pFrameMgr->SendEvent(&event);
						
						// û��װ������ʾװ����λ
						TCHAR szTmp[X_SHORT_NAME];
						_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("EquipPos%d"), GetEquipPos(i)+1);
						tstring strTmp = g_StrTable[_T("TipsDefaultFont")];
						strTmp += g_StrTable[szTmp];
						ShowTip(pWnd, strTmp.c_str());
					}
					break;
				}
			}
		}
		break;
	case EGUIE_Drag:
		{
			for(INT i=0; i<EEP_MaxEquip; ++i)
			{
				if (m_bIsFashionDisplay && i > EEP_Feet && i != EEP_RightHand && i != EEP_LeftHand)
				{
					continue;
				}
				if(pWnd == m_pBtnEquip[i])
				{
					Equipment* pEquip = ItemMgr::Inst()->GetCurEquip(GetEquipPos(i));
					
					if(P_VALID(pEquip))
					{
						DragBtn* pDrag = (DragBtn*)m_pGUI->GetWnd(_T("\\desktop\\DragButton"));
						if (P_VALID(pDrag))
						{
							pEvent->dwParam1 = EDT_ItemEquip;
							pEvent->dwParam2 = pEquip->GetPos();
							pEvent->dwParam3 = pEquip->GetItemTypeID();
							pDrag->OnEvent(pEvent);
	
						}
						break;
					}
					break;
				}
			}
		}
		break;
	case EGUISCB_Drop:
		{
			for(INT i=0; i<EEP_MaxEquip; ++i)
			{
				if (m_bIsFashionDisplay && i > EEP_Feet && i != EEP_RightHand && i != EEP_LeftHand)
				{
						continue;
				}
				if(pWnd == m_pBtnEquip[i])
				{
					if(EDT_Item == pEvent->dwParam1)//�����е���Ʒ������Ʒװ��Ч��
					{
						OnEquipment((INT16)LOWORD(pEvent->dwParam2), GetEquipPos(i));
					}
					else if(EDT_ItemEquip == pEvent->dwParam1)//װ�����е���Ʒ֮�任λ��
					{
						OnEquipSwap((EEquipPos)LOWORD(pEvent->dwParam2), GetEquipPos(i));
					}
					break;
				}
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			for(INT i = 0; i < EEP_MaxEquip; ++i)
			{
				if(pWnd == m_pBtnEquip[i])
				{
					Equipment* pEquip = NULL;
					if (!m_bIsFashionDisplay || i <= EEP_Feet || i == EEP_RightHand || i == EEP_LeftHand)
						pEquip = ItemMgr::Inst()->GetCurEquip(GetEquipPos(i));

					if(P_VALID(pEquip))
					{
						ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pEquip).c_str());
					}
					else
					{
						// û��װ������ʾװ����λ
						TCHAR szTmp[X_SHORT_NAME];
						_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("EquipPos%d"), GetEquipPos(i)+1);
						tstring strTmp = g_StrTable[_T("TipsDefaultFont")];
						strTmp += g_StrTable[szTmp];
						ShowTip(pWnd, strTmp.c_str());
					}
					break;
				}
			}
		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{
			ShowTip(pWnd, NULL);
		}
		break;
		
	}
	return 0;
}


EEquipPos EquipmentFrame::GetEquipPos( INT nIndex )
{
	EEquipPos eRet = EEP_End;
	if(m_bIsFashionDisplay)
	{
		if(nIndex < EEP_End - EEP_MaxEquip)
		{
			eRet = (EEquipPos)(nIndex + EEP_MaxEquip);
		}
		else
		{
			eRet = (EEquipPos)nIndex;
		}
	}
	else
	{
		eRet = (EEquipPos)nIndex;
	}
	return eRet;
}

VOID EquipmentFrame::OnEquipment(INT16 nItemPos, EEquipPos ePos /* = EEP_End */)
{
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	//�����������ڱ�ս״̬��ʹ�ü��ܲ�����װ
	if( P_VALID(pLp) )
	{
		if( pLp->IsSkillState() )
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemMsgError_CombatEquip")]);
			return;
		}
		else if( pLp->IsReadyState() )
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemMsgError_ReadyEquip")]);
			return;
		}
	}

	Item* pItem = ItemMgr::Inst()->GetPocketItem(nItemPos);
	if( P_VALID(pItem) && MIsEquipment(pItem->GetItemTypeID()) )
	{
		Equipment* pEquip = (Equipment*)pItem;
		if(IsEquipPosFix(pEquip->GetEquipPos(), ePos) )
		{
			tagNC_Equip m;
			m.ePosDst = ePos;
			m.n64Serial = pItem->GetItemId();
			m_pSession->Send(&m);
		}
	}
}

BOOL EquipmentFrame::IsEquipPosFix( EEquipPos eEquipPos, EEquipPos &eDstPos )
{
	// Ŀ����λΪĬ��ֵ����Ϊ��ָ����λ����λ����װ����λȷ��
	if(eDstPos == EEP_End)
	{
		eDstPos = eEquipPos;
		
		// ����������ǽ�ָ������������Ϊ�ɷ���������λ��������Ҫ�ж���λ�Ƿ�Ϊ��
		if(EEP_Finger1 == eEquipPos)
		{
			if( ItemMgr::Inst()->IsNotEquip(EEP_Finger1) || 
				( !ItemMgr::Inst()->IsNotEquip(EEP_Finger1) 
				&& !ItemMgr::Inst()->IsNotEquip(EEP_Finger2) ) )
			{
				eDstPos = EEP_Finger1;
			}
			else
			{
				eDstPos = EEP_Finger2;
			}
		}
		else if(EEP_RightHand == eEquipPos)
		{
			if( ItemMgr::Inst()->IsNotEquip(EEP_RightHand) || 
				( !ItemMgr::Inst()->IsNotEquip(EEP_RightHand) 
				&& !ItemMgr::Inst()->IsNotEquip(EEP_LeftHand) ) )
			{
				eDstPos = EEP_RightHand;
			}
			else
			{
				eDstPos = EEP_LeftHand;
			}
		}
		return TRUE;
	}
	
	// ����Ĭ��ֵ�������Ŀ����λ�жϣ�
	// ����������ǽ�ָ������������Ϊ�ɷ���������λ��������Ҫ�ж�������λ
	if(EEP_Finger1 == eEquipPos)
	{
		if(EEP_Finger1 == eDstPos || EEP_Finger2 == eDstPos)
		{
			return TRUE;
		}
	}
	else if(EEP_RightHand == eEquipPos)
	{
		if(EEP_RightHand == eDstPos || EEP_LeftHand == eDstPos)
		{
			return TRUE;
		}
	}
	else if(eEquipPos == eDstPos)
	{
		return TRUE;
	}
	
	return FALSE;
}

VOID EquipmentFrame::UpdateEquipAvatar()
{
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(P_VALID(pLp))
		m_pWndEquipView->UpdataAvatar(pLp);
}


VOID EquipmentFrame::UpdateEquipment( EEquipPos ePos )
{
	EEquipPos ePosTmp = ePos;
	DWORD dwTypeID = GT_INVALID;
	BYTE byQuality = 0;
	Equipment* pEquip = (Equipment*)GT_INVALID;

	if (m_bIsFashionDisplay)
	{
		if (ePos >= EEP_MaxEquip && ePos < EEP_End)
		{
			ePosTmp = (EEquipPos)(ePos - EEP_MaxEquip);
			pEquip = ItemMgr::Inst()->GetCurEquip(ePos);
		}
		/*! ʱװģʽ����Ȼ�������� */
		else if (EEP_RightHand == ePos || EEP_LeftHand == ePos)
		{
			ePosTmp = ePos;
			pEquip = ItemMgr::Inst()->GetCurEquip(ePos);
		}
		else
		{
			ePosTmp = ePos;
			pEquip = ItemMgr::Inst()->GetCurEquip((EEquipPos)(EEP_MaxEquip+ePos));			
		}
	}
	else
	{
		if(ePos < EEP_MaxEquip)
		{
			ePosTmp = ePos;
			pEquip = ItemMgr::Inst()->GetCurEquip(ePos);
		}
	}

	if(P_VALID(pEquip))
	{
		dwTypeID = pEquip->GetItemTypeID();
		byQuality = pEquip->GetItemQuality();
	}

    // ˢ�°�ť
    ASSERT(ePosTmp < EEP_MaxEquip);
    /*if (m_bIsFashionDisplay)
    {
        if (ePos >= EEP_MaxEquip && ePos < EEP_End || ePos == EEP_RightHand || ePos == EEP_LeftHand)
            m_pBtnEquip[ePosTmp]->RefreshItem(dwTypeID, 0, byQuality);
        else
            m_pBtnEquip[ePosTmp]->RefreshItem();
    }
    else
    {
        if (ePos < EEP_MaxEquip)
            m_pBtnEquip[ePosTmp]->RefreshItem(dwTypeID, 0, byQuality);
        else
            m_pBtnEquip[ePosTmp]->RefreshItem();
    }*/
	m_pBtnEquip[ePosTmp]->RefreshItem(dwTypeID, 0, byQuality);	

}

VOID EquipmentFrame::OnEquipSwap( EEquipPos ePosSrc, EEquipPos ePosDst )
{
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	//������������ս��״̬������������װ��
	if( !P_VALID(pLp) || pLp->IsCombatState() )
	{
		// ����ǻ�������ʾս��״̬����������
		if ( (EEP_RightHand == ePosSrc && EEP_LeftHand == ePosDst)
			|| (EEP_LeftHand == ePosSrc && EEP_RightHand == ePosDst) )
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemMsgError_CombatSwap")]);
		}
		else
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemMsgError_CombatEquip")]);
		}
		return;
	}
	
	// ���Դװ����Ϊ�գ�����Ҫ����Ŀ��װ����
	Equipment* pEquip = ItemMgr::Inst()->GetCurEquip(ePosSrc);
	if(!P_VALID(pEquip))
	{
		pEquip = ItemMgr::Inst()->GetCurEquip(ePosDst);
		if(!P_VALID(pEquip))
		{
			// ���Ŀ��װ����Ϊ�գ��򲻴���
			return;
		}
		else
		{
			// ���򣬵���Ŀ��װ����װ���Ƶ�Դװ��������
			EEquipPos eTmp = ePosDst;
			ePosDst = ePosSrc;
			ePosSrc = eTmp;
		}
	}

	//ֻ������������л��ͽ�ָ����л�
	if( (EEP_RightHand == ePosSrc && EEP_LeftHand == ePosDst)
		|| (EEP_LeftHand == ePosSrc && EEP_RightHand == ePosDst) )
	{
		//�������л����л�����
		tagNC_SwapWeapon m;
		m_pSession->Send(&m);
	}
	else if( (EEP_Finger1 == ePosSrc && EEP_Finger2 == ePosDst)
		|| (EEP_Finger2 == ePosSrc && EEP_Finger1 == ePosDst) )
	{
		//�����������ڱ�ս״̬��������ָ
		if( pLp->IsReadyState() )
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemMsgError_ReadyEquip")]);
			return;
		}

		//��ָ���л�����Ʒλ�øı�
		tagNC_ItemPosChange m;
		m.eConType = EICT_Equip;
		m.n16PosDst = ePosDst;
		m.n64Serial = pEquip->GetItemId();
		m_pSession->Send(&m);
	}
}

DWORD EquipmentFrame::OnUpdateItemEvent( tagUpdateLPItemEvent* pGameEvent )
{
	//ֻ��װ�����е���Ʒ��ˢ��
	if(EICT_Equip == pGameEvent->eType)
	{
        if (m_bIsFashionDisplay)
        {
		    UpdateEquipment((EEquipPos)pGameEvent->n16Index);
        }
        else
        {
            if (pGameEvent->n16Index < EEP_MaxEquip)
                UpdateEquipment((EEquipPos)pGameEvent->n16Index);
        }
	}
	return 0;
}

DWORD EquipmentFrame::OnChangeItemEvent( tagItemPosChangeExEvent* pGameEvent )
{
	//ֻ�б����е���Ʒ�ϵ�װ����������װ����
	if(EICT_Equip == pGameEvent->eTypeDst && EICT_Bag == pGameEvent->eTypeSrc)
	{
		OnEquipment(pGameEvent->n16IndexSrc);
	}
	return 0;
}

DWORD EquipmentFrame::OnUpdateAvatarEvent( tagUpdataAvatarEvent* pGameEvent )
{
	//�������װ���仯����Avatar
	if(pGameEvent->dwRoleID == RoleMgr::Inst()->GetLocalPlayerID())
		UpdateEquipAvatar();

	return 0;
}

VOID EquipmentFrame::Render3D()
{
	if( P_VALID( m_pWndEquipView ) )
		m_pWndEquipView->Render3D();
	GameFrame::Render3D();
}

DWORD EquipmentFrame::OnKeySwapWeapon( DWORD dwID, BOOL bDown )
{
	if(bDown)
	{
		//�������л�
		OnEquipSwap(EEP_RightHand, EEP_LeftHand);
	}

	return 0;
}

DWORD EquipmentFrame::OnChangDispPattern( tagFashionEvent *pGameEvent )
{
    /*! ����������Ϣ */
    tagNC_Fashion e;
    e.bFashion = pGameEvent->bDisFashion;
    if (m_pSession->IsConnect())
        m_pSession->Send(&e);

	/*! ����װ��/ʱװ��ʾģʽ */
	m_bIsFashionDisplay = pGameEvent->bDisFashion;
	UpdateAll();

	/*! ������ʾ */
	tstring str = pGameEvent->bDisFashion ? g_StrTable[_T("PatternFashion")] : g_StrTable[_T("PatternEquip")];
	CombatSysUtil::Inst()->ShowScreenCenterMsgEx(str.c_str());

	return 0;
}

VOID EquipmentFrame::UpdateAll()
{
    TurnFashionPos(!m_bIsFashionDisplay);

	if (m_bIsFashionDisplay)
	{
		for (INT i = 0; i < EEP_End; ++i)
		{
			UpdateEquipment((EEquipPos)i);
		}
	}
	else
	{
		for (INT i=0; i<EEP_MaxEquip; ++i)
		{
			UpdateEquipment((EEquipPos)i);
		}
	}
}

DWORD EquipmentFrame::OnUpdataAvatarDisplaySetEvent(tagUpdataAvatarDisplaySetEvent* pEvent)
{
	if( pEvent->dwRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
	{
		m_pWndEquipView->UpdataDisplaySet( RoleMgr::Inst()->GetLocalPlayer()->GetDisplaySet() );
	}
	return 0;
}

void EquipmentFrame::TurnFashionPos( bool on )
{
    m_pStcEquipBack[EEP_Face]->SetInvisible(!on);
    m_pStcEquipBack[EEP_Finger1]->SetInvisible(!on);
    m_pStcEquipBack[EEP_Finger2]->SetInvisible(!on);
    m_pStcEquipBack[EEP_Waist]->SetInvisible(!on);
    m_pStcEquipBack[EEP_Neck]->SetInvisible(!on);
}