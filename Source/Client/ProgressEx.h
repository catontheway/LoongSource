#pragma once

/**
*class ProgressEx
*brief ���Ը���ǰ��ͼƬ����չ״̬��
*author hyu
*remark
*base GUIProgress
*/
class ProgressEx :
	public GUIProgress
{
public:
	ProgressEx(void);
	~ProgressEx(void);

	//����ǰ��ͼƬ,����·���ļ���
	VOID SetForePic(const tstring strFileName);

	//��ȡ��ǰǰ���ļ���
	const tstring GetForePic() { return m_strForePicFileName; }
};
