#pragma once

/** \���õ�ͼ��־�¼�
*/
struct tagSetFlagEvent : public tagGameEvent
{
	float x;
	float z;

	tagSetFlagEvent():tagGameEvent(_T("tagSetFlagEvent"),NULL){}
	tagSetFlagEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame){}
};

/** \��/�رյ�ͼ�����¼�
*/
struct tagOpenCloseMapWndEvent : public tagGameEvent
{
	bool bWorldMap;

	tagOpenCloseMapWndEvent():tagGameEvent(_T("tagOpenCloseMapWndEvent"),NULL), bWorldMap( false ){}
	tagOpenCloseMapWndEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame){}
};

/** \�򿪵�ͼ�����¼�
*/
struct tagOpenMapWndEvent : public tagGameEvent
{
	bool bWorldMap;

	tagOpenMapWndEvent():tagGameEvent(_T("tagOpenMapWndEvent"),NULL), bWorldMap( false ){}
	tagOpenMapWndEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame){}
};