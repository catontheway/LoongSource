#pragma once
#include "..\WorldBase.h"

namespace WorldBase
{
	/** \class NPCNavMapDoors
		\brief NPC���ӵ���ͼ�е���
	*/
	class NPCNavMapDoors
	{
	public:
		struct tagDoor						//������
		{	
			DWORD	dwObjID;				//�༭����ָ�������ID
			BitMap*	pOpenStateCangoMap;		//����״̬����ͼ
			BitMap* pCloseStateCangoMap;	//�ر�״̬����ͼ
			POINT	leftTopTile;			//����ͼ���Ͻ�λ��

			tagDoor()
			{
				pOpenStateCangoMap=NULL;
				pCloseStateCangoMap=NULL;
			}

			~tagDoor()
			{
				SAFE_DELETE(pOpenStateCangoMap);
				SAFE_DELETE(pCloseStateCangoMap);
			}

			void Clone(tagDoor* pTarget)
			{
				ASSERT(pTarget->pOpenStateCangoMap==NULL);
				ASSERT(pTarget->pCloseStateCangoMap==NULL);
				ASSERT(pOpenStateCangoMap!=NULL);
				ASSERT(pCloseStateCangoMap!=NULL);

				pTarget->dwObjID=dwObjID;

				pTarget->pOpenStateCangoMap=new BitMap;
				pOpenStateCangoMap->Clone(pTarget->pOpenStateCangoMap);

				pTarget->pCloseStateCangoMap=new BitMap;
				pCloseStateCangoMap->Clone(pTarget->pCloseStateCangoMap);

				pTarget->leftTopTile=leftTopTile;
			}
		};
	public:
		NPCNavMapDoors(void);
		~NPCNavMapDoors(void);

		/** ��������
		*/
		void LoadFromFile(const TCHAR* szMapName,IFS* pFS);
		void Destroy();
		void Clone(NPCNavMapDoors* pTarget);

		/** �޸ĵ���ͼ������\�ر�ָ����
		*/
		void OpenCloseDoor(DWORD dwObjID,bool bOpen,BitMap* pCangoMap);
		/** �޸ĵ���ͼ������\�ر�������
		*/
		void OpenCloseAllDoors(bool bOpen,BitMap* pCangoMap);

		//--editor suppport
		static bool EditorSave(const vector<tagDoor*>& doors,const TCHAR* szSavePath);

	private:
		map<DWORD,tagDoor*>	m_doorsMap;
	};
}//namespace WorldBase
