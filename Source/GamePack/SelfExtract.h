#pragma once
#include "./CPKCore/CPKCore.h"

//�ϲ���ѹ����͸��°��γ�һ���ļ�----�ֶ�������
//�ļ��ĸ�ʽ����Ϊ3���֣��Խ�ѹ����+���°��ļ�+β��Ϣ����¼���°��ļ���ʼ�ĵ�ַ�ʹ˸��°��İ汾�ţ�
class CSelfExtract :public CPK
{
public:
	//���������ļ������bAddExe= true�������Խ�ѹ����д���ļ��Ŀ�ͷ��
	//������û���ĺ��������Զ������ļ���.cpk��
	BOOL	CreateSaveFile( const TCHAR* szFileName, BOOL bAddExe );

	//д��β����Ϣ����¼���°��ļ���ʼ�ĵ�ַ�ʹ˸��°��İ汾��
	BOOL	WriteTail( CString& strVersionSrc, CString& strVersionDest );


protected:

	//д��ͷ�ļ���Ϣ
	BOOL	WriteHeader();
};
