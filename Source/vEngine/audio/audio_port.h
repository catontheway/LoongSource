//-----------------------------------------------------------------------------
// File: audio_port
// Desc: interface for all audio system
// Auth: Lyp
// Date: 2004/02/20
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine{
//-----------------------------------------------------------------------------
// �ṩ������Чϵͳ(��FMOD/MILES/��)��ͳһ�ӿ�
// �뾡����֤�ӿڵ���С��,�Ա���ԽӸ���Audio����
// ���ӵĲ����߼����Էŵ��ϲ�CAudio(audio.cpp)
//-----------------------------------------------------------------------------
class AudioPort
{
public:
	// ��ʼ������
	virtual BOOL Init(){return FALSE;}
	virtual VOID Destroy(){}
	virtual VOID SetVFSCallBack(FP_OPENCALLBACK, FP_CLOSECALLBACK,
		FP_READCALLBACK, FP_SEEKCALLBACK, FP_TELLCALLBACK){}
	
	// ȡ�ú�������������
	virtual INT  GetMasterVolume(){return 0;}
	virtual VOID SetMasterVolume(INT nVolume){}

	// ȡ�ú�����ͨ������
	virtual INT GetChannelVolume(INT nChannel){return 0;}
	virtual INT SetChannelVolume(INT nChannel, INT nVolume){return 0;}
	
	// ����music(mod/s3m/xm/it/mid/rmi/sgt...)�Ĳ���
	virtual DWORD	OpenMusic(LPCTSTR szName){return GT_INVALID;}
	virtual BOOL	CloseMusic(DWORD dwMusicHandle){return FALSE;}
	virtual INT		PlayMusic(DWORD dwMusicHandle, BOOL bLoop){return 0;}
	virtual BOOL	PauseMusic(DWORD dwMusicHandle, BOOL bPause){return FALSE;}
	virtual BOOL	StopMusic(DWORD dwMusicHandle){return FALSE;}


	// ����3d/2d sound effect�Ĳ���
};




}