#pragma once

//ime��ʾ
struct tagOwnerImeShow : public tagGameEvent
{
	BOOL bShow;
	tagOwnerImeShow():tagGameEvent(_T("tagOwnerImeShow"),NULL)
	{}
};

//ime��������
struct tagOwnerInputWordEvent : public tagGameEvent
{
	tstring strText;
	tagOwnerInputWordEvent():tagGameEvent(_T("tagOwnerInputWordEvent"),NULL)
	{}
};

//ime���
struct tagOwnerInputWordClear : public tagGameEvent
{
	tagOwnerInputWordClear():tagGameEvent(_T("tagOwnerInputWordClear"),NULL)
	{}
};

//ime��ѡ��
struct tagOwnerImeWordEvent : public tagGameEvent
{
	tstring		strText;
	tagOwnerImeWordEvent():tagGameEvent(_T("tagOwnerImeWordEvent"),NULL)
	{}
};