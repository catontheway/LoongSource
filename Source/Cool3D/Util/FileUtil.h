#pragma once
#include <string>
#include <vector>
using namespace std;
#include "..\Cool3D.h"

namespace Cool3D
{
	/** ����ͨ��������ļ�
	*/
	void Cool3D_API SearchFiles(const TCHAR* szFind,vector<tstring>& out);

	/** ����ͨ������Ҳ�ɾ���ļ�
	*/
	bool Cool3D_API DeleteFiles(const TCHAR* szFind);

	/** ɾ��ָ��Ŀ¼�µ������ļ�
	*/
	bool Cool3D_API DeleteAllFiles(const TCHAR* szDir);


}//namespace Cool3D