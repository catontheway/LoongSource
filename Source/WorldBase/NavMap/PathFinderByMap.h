#pragma once

namespace WorldBase
{
	/** \class PathFinderByMap
		\brief ���ڵ�ͼ��Ѱ·��
	*/
	class PathFinderByMap
	{
		struct tagMapLinkInfo					//��ͼ������Ϣ
		{
			tstring			szMapName;			//��ͼ��
			vector<tstring>	nextMapNames;		//���л�����ĵ�ͼ�����б�
			vector<int>		nextMapIndices;		//���л�����ĵ�ͼ�����б�
			vector<Vector3>	nextMapSwitchPos;	//���л�����ĵ�ͼ��Ӧ���л���
			vector<int>		prevMapIndices;		//���л����˵�ͼ�ĵ�ͼ�����б�
			int				dist;				//��Ŀ�ĵ�ͼ���룬Ѱ·ʱʹ��
		};
	public:
		PathFinderByMap(void);
		~PathFinderByMap(void);

		void LoadFromFile(IFS* pSysFS);
		void Destroy();

		bool GetSwitchPos(const TCHAR* szSrcMap,const TCHAR* szDestMap,Vector3& switchPos);
	
	private:
		tagMapLinkInfo* FindMapLinkInfo(const TCHAR* szMapName);
		int FindMapIndex(const TCHAR* szMapName);

	private:
		vector<tagMapLinkInfo> m_mapLinkInfo;
	};
}//namespace WorldBase
