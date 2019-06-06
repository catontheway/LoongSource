#pragma once

#include "Role.h"

class ActiveStatic;

struct tagBuffUI
{
	tagRoleBuff    stBuff;                 // BUFF�ṹ
	ActiveStatic*  pSt_BuffPic;            // BUFFͼ��
	GUIProgress*   pPro_BuffTime;          // BUFF����ʱ������
	tagBuffUI() :  pSt_BuffPic( NULL ), pPro_BuffTime( NULL ) { ZeroMemory( &stBuff, sizeof(stBuff) ); }
};

/** \class   BuffUIList
    \brief  
    \remarks ����������ʾBUFF�Ŀؼ����ڲ��������ƿؼ�����������
*/
class BuffUIList
{
public:
	BuffUIList();
	~BuffUIList();

	/** \���BUFF���棬Ӧ���ս��������˳�����
	*/
	BOOL AddUI( ActiveStatic* pBuffStatic, GUIProgress* pTimeProgress );
	/** \���BUFF
	*/
	BOOL AddBuff( const tagRoleBuff* pBuff );
	/** \���������ʾ��BUFF
	*/
	VOID ClearBuffs();
	/** \���½���״̬
	*/
	VOID Updata();
	/** \����GUI�¼�
	*/
	BOOL GUIEventHandler( tagGUIEvent* pEvent, const GUIWnd* pEventWnd, const DWORD dwRoleID );
private:
	vector<tagBuffUI>		m_vecBuffUI;              // BUFF UI�б�
	TObjRef<GUISystem>      m_pGUI;
	INT						m_nCurShowNum;            // ��ǰ��ʾ��BUFF����
	DWORD                   m_dwLastUpdataTipsTime;   // �ϴθ���BUFF TIPS��ʱ��
};
