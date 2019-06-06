#pragma once
#include "..\Cool3D.h"
#include "..\vfs\IFS.h"
#include "..\Public\MathBaseDefine.h"

#pragma warning( disable : 4251 )

namespace Cool3D
{
	class Cool3D_API AudioSystem
	{
	private:
		AudioSystem(void);
		~AudioSystem(void);

	public:
		static AudioSystem* Inst();

		void Init();
		void SetFS(IFS* pVFS);
		bool IsHWSurpport();
		void Destroy();
		void Update(DWORD delta);
		void GetSystemState(float& cpuUsage,int& mem);

		//--Listener
		void SetListener(const Vector3& pos,const Vector3& vel,const Vector3& forward,const Vector3& up);

		//--����ϵͳ���� ��Χ0~1.0f
		void SetSystemMusicVolume(float vol);
		void SetSystemSoundVolume(float vol);

		//--��������(ѭ������)
		void PlayMusic(const TCHAR* szFileName, bool bLoop, float volume, DWORD fadeinTime=0);
		void StopMusic(DWORD fadeoutTime=0);
		bool MusicIsPlaying(void);

		//--����3D��Ч(ѭ������)
		void ClearEnvironmentSound();
		void AddEnvironmentSound(const TCHAR* szFileName,Vector3 pos,float minDist,float maxDist,float volume,DWORD interval);
		int GetNum3DEnvSoundPlaying();

		//--�¼�3D��Ч
		int  Create3DEventSound(const TCHAR* szFileName,bool bLoop,float minDist,float maxDist,float volume);
		void Destroy3DEventSound(int id);
		void Play3DEventSound(int id);
		void Stop3DEventSound(int id);
		void Set3DEventSoundAtt(int id,const Vector3& pos,const Vector3& vel);
		void StopAll3DEventSound();
		bool SoundIsStoped(int id);
		void Active3DEventSound(int id,float volume);

		//--�¼�2D��Ч
		void Play2DEventSound(const TCHAR* szFileName,float volume);
		void StopAll2DSound();
		int GetNum2DSoundPlaying();


		class Member;

		Member* GetMember() {return m_p;}
		BOOL m_bTerminateThreadUpdate;

		struct tagCacheMusic
		{
			bool    valid;
			bool	loop;
			tstring	strFileName;
			DWORD	fadeinTime;
			float	volume;
			tagCacheMusic():valid(false){}
		};

		void ExecuteMusic(const tagCacheMusic& cache);

	private:

		HANDLE	m_hThreadUpdate;
		Member	*m_p;
	};
}