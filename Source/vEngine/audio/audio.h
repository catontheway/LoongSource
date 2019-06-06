//-----------------------------------------------------------------------------
// File: audio
// Desc: interface for audio
// Auth: Lyp
// Date: 2004/02/09
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine{

//-----------------------------------------------------------------------------
/**	Audio����ӿ� ��ƹ���
	��ջģʽ�Ĳ���: play��push����(ֻ��Channel��������,�����������)
 1.	���²��ŵ�Channel�͵�ǰ���ŵ�Channel��ͬʱ,ֹͣ��ǰ���ŵ�Channel,ѹջ,
	�������µ�Channel
 2.	���²��ŵ�Channel�͵�ǰ���ŵ�Channel��ͬʱ,ֱ��������ȡ������,��ѹջ����
 3.	PopStreamֹͣ���ŵ�ǰChannel,�ָ���ջ����һChannel�Ĳ���

	�ṹ���˼��

	Audio -> audio_port -> fmod_port(or other audi engine)

	�������߼��������ϲ�,Ҳ����CAudio,�����湦�ܽӿ�audio_port���,
	�Ա��������Audio Engine���ӿ�

	engine: "fmod"
			"miles"
			"xaudio"
*/
//-----------------------------------------------------------------------------

CONST INT GT_MAX_STREAM_CHANNEL	= 8;
class  AudioPort;
class VENGINE_API Audio
{
public:
	BOOL Init(LPCTSTR szEngineName);
	VOID Destroy();

	// ����music(mod/s3m/xm/it/mid/rmi/sgt...)�Ĳ���
	BOOL PlayMusic(INT nChannel, LPCTSTR szName, BOOL bLoop=TRUE);
	BOOL StopMusic(INT nChannel);
	BOOL PauseMusic(INT nChannel, BOOL bPause);
	
	VOID Update(FLOAT fDeltaTime);
	
	Audio();
	virtual ~Audio();

private:
	TObjRef<Util>	m_pUtil;
	AudioPort*		m_pAudio;	// mutex factory

	BOOL m_bFadein;
	BOOL m_bFadeout;
	INT	 m_nCurrentVirtualChannel;

	std::vector<DWORD> m_vecMusicHandle;
	
	FLOAT m_fDeltaTime;
	FLOAT m_fCurTime;

	// ���������ļ�ϵͳ��֧��
	TObjRef<VirtualFileSys> m_pVFS;
	static UINT myopen(const char *name);
	static VOID myclose(unsigned int handle);
	static INT  myread(void *buffer, int size, unsigned int handle);
	static INT  myseek(unsigned int handle, int pos, signed char mode);
	static INT  mytell(unsigned int handle);
};


}	// namespace vEngine{
