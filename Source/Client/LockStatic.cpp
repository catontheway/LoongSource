#include "StdAfx.h"
#include "LockStatic.h"
#include "..\WorldDefine\exchange_define.h"

LockStatic::LockStatic(void)
{
	m_pLastWnd = NULL;
}

LockStatic::~LockStatic(void)
{
}

BOOL LockStatic::OnInputMessage( tagGUIInputMsg* pMsg )
{
	switch(pMsg->dwType)
	{
	case WM_MOUSEMOVE:	// ��������ƶ�
		{
			if( m_pSystem->GetMousePoint() == this )
			{
				//���������ƶ���������Ϣ���ÿؼ��������Ʒ��ť�ؼ�
				for (INT i=0; i<MAX_EXCHANGE_ITEM; ++i)
				{
					TCHAR szTmp[X_SHORT_NAME] = {0};
					_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("itempic%d\\item%d"), i+1, i+1);
					GUIWnd* pWnd = m_pFather->GetChild(szTmp);
					if (P_VALID(pWnd) && pWnd->PointInWnd(pMsg->pt))
					{
						if (pWnd != m_pLastWnd)
						{
							//�����һ���ؼ����ڣ����ȷ����Ƴ��¼�
							if(P_VALID(m_pLastWnd))
							{
								tagGUIEvent e1(m_pLastWnd, EGUIE_MoveOutOfCtrl, pMsg);
								m_pLastWnd->SendEvent(&e1);
							}
							
							//��������ؼ�����¼
							tagGUIEvent e2(pWnd, EGUIE_MoveIntoCtrl, pMsg);
							pWnd->SendEvent(&e2);
							m_pLastWnd = pWnd;
							
							break;
						}
					}
				}
			}
			else
			{
				m_pLastWnd = NULL;
			}
		}
		break;
	}
	return GUIStatic::OnInputMessage(pMsg);
}
