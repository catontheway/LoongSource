// �ı������ļ�����
// .�ı���ʽӦΪ:
//		BEGIN �������� ;ע��
//		���ݹؼ��� {����ֵ�ַ���} ;ֵ���Ժ������ַ�
//		END
// .ÿ��NextToken()ȡ����һ���ؼ���
// .NextValueToke()ȡ�ý���ؼ��ֵ�һ��ֵ,
//////////////////////////////////////////////////////////////////////

#pragma once
#include "..\Cool3D\vfs\ifs.h"

class TxtFile  
{
public:
	bool GetTokenValue_String(LPCTSTR name,TCHAR* buffer,int bufferlen);
	void RewindSection();
	void NextValueToken();
	void Destroy();
	void OpenSection(LPCTSTR name);
	void Load(IFS *pFS,LPCTSTR filename);
	float GetTokenValue(LPCTSTR name);

	TxtFile();
	~TxtFile();

	TCHAR m_token[512];

	BOOL NextToken();
protected:	
	TCHAR *m_buf;
	TCHAR *m_sectionstart;
	TCHAR *m_curpos;
};
