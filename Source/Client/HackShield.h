#pragma once

long NetSendToGameServer(const char * pBuffer,int nLen);
struct tagNS_GameGuarder;
class HackShield
{
public:
	HackShield(void);
	~HackShield(void);

	static HackShield* Inst();

	void Init();

	/**
	����	:	Game Client �ṩ��Apex Client �ķ��ͺ���;
			Apex Client ����������� ��������;
	����		pBuffer	 �������ݵĻ�����
			nLen	 �������ݵĳ���
	*/
	friend long NetSendToGameServer(const char * pBuffer,int nLen);

	/**
	����ApexClient;
	*/
	int	StartApexClient();

	/**
	ֹͣApexClient
	*/
	int	StopApexClient();

	/**
	֪ͨApexClient,�����ݴ� GameServer ������
	����	:	pBuf	�������ݵĻ�����
			nBufLen �������ݵĳ���
	*/
	DWORD	NoticeApec_UserData(tagNS_GameGuarder* pMsg, DWORD pPrama);

private:
	TSFPTrunk<HackShield>		m_Trunk;

};