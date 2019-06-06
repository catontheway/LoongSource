
#ifndef PLAYER_H_
#define PLAYER_H_
//#include "player_net_cmd_mgr.h"
#include "FrameMgr.h"
#include "Mutex.h"

#include <string>
#include <vector>
using namespace std;

enum EPlayerType
{
	EPT_Login,
	EPT_World,
	EPT_Center,

	EPT_End
};

class VCMutex;

class Player
{
public:
	Player();
	~Player();

	BOOL Init();
	VOID Destroy();

	//���ڵ�½,��ͼ
	VOID ThreadUpdate();

	//Frame�л�
	BOOL ChangeFrame(FrameMgr::EFrame eFrame);

	//��Ϊ����
	VOID SetPlayerChat(BOOL bChat, BYTE byChannel = 5);
	VOID SetPlayerMove(BOOL bMove);

	//ȡ����Ϊ״̬
	BOOL GetChatStatus()	{ return m_bCanChat; }
	BYTE GetChatChannel()	{ return m_byChatChannel; }
	BOOL GetMoveStatus()	{ return m_bCanWalk; }
	VOID GetNextWayPoint();

	//ȡ��Log����
	TObjRef<Log>& GetLog()	{ return m_pLog; }

	//��װ�߳���
	VOID Acquire() { m_pLock->Acquire(); }
	VOID Release() { m_pLock->Release(); }
	BOOL AttemptAcquire() { return m_pLock->AttemptAcquire(); }

public:
	TObjRef<XClient>			m_pWorldSession;	//��world������
	TObjRef<StreamTransport>	m_pLoginSession;	//��login������

	DWORD						m_dwID;				//������ID
	tstring						m_strVersion;		//��Ϸ�汾
	tstring						m_strName;			//�û���
	tstring						m_roleName;			//������ɫ����
	tstring						m_strPassword;		//����
	tstring						m_strThreadName;	//�߳�����
	VOLATILE BOOL				m_bTerminate;		// �߳�ֹͣ��־
	tstring						m_strMapName;		// Map����
	Vector3						m_srcPos;			// ���ߵ���ʼ��
	Vector3						m_desPos;			// ���ߵ��յ�
	INT							m_wayPoint;			// ·������
	INT							m_type;				//���� 1: ��½�� 2. ��½,��ͼ�� 3.���ļ���
	DWORD						m_SendTime;			//�������
	BOOL						m_bLog;				//�Ƿ��Log	
	BOOL						m_bAbandon;			//�Ƿ��Ѿ�����
	INT							m_Start_x;			//��ʼ��,����GM goto
	INT							m_Start_z;			//��ʼ��,����GM goto
	vector<Vector3>				m_vecWayPoint;
	vector<Vector3>::iterator	m_it;
	tagLoginInfo*				m_pLoginInfo;		//��ҵ�ǰ��¼��Ϣ

private:
	TSFPTrunk<Player>	        m_Trunk;
	TObjRef<Thread>				m_pThread;
	TObjRef<VarContainer>		m_pVar;
	TObjRef<Util>				m_pUtil;
	TObjRef<Util>				m_pLoginIPUtil;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<Log>				m_pLog;				//Log
	string						log_name;			//Log����

	BOOL						m_bCanWalk;			//��������
	BOOL						m_bCanCombat;		//����ս��
	BOOL						m_bCanChat;			//�Ƿ�����

	BYTE						m_byChatChannel;	//����Ƶ��

	DWORD						m_dwCreatureID;
	DWORD						m_dwMsgCount;
	BOOL						m_bBlock;			//����ʧ��

private:
	FrameMgr*					m_pFrameMgr;			// Frame����
	VCMutex*					m_pLock;
};

#endif /* PLAYER_H_ */