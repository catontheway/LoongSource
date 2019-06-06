#define _CRT_SECURE_NO_DEPRECATE
#include "stdafx.h"
#include <windows.h>
#include "time.h"
#include <stdio.h>

//-------------------------------------------------------------------------------------
// �õ���ǰʱ��
//-------------------------------------------------------------------------------------
tagDWORDTime GetCurrentDWORDTime()
{
	SYSTEMTIME CurTime;
	GetLocalTime(&CurTime);
	tagDWORDTime time;
	time.day = CurTime.wDay;
	time.hour = CurTime.wHour;
	time.min = CurTime.wMinute;
	time.month = CurTime.wMonth;
	time.sec = CurTime.wSecond;
	time.year = (CurTime.wYear) % 100;

	return time;
}

//-------------------------------------------------------------------------------------
// �ж�ĳ���Ƿ�Ϊ����
//-------------------------------------------------------------------------------------
BOOL IsLeapYear(WORD iYear)
{
	return !(iYear % 4) && (iYear % 100) || !(iYear % 400);
}   

//-------------------------------------------------------------------------------------
// �����������ڵ�ʱ��������
//-------------------------------------------------------------------------------------
DWORD CalcTimeDiff(tagDWORDTime destTime, tagDWORDTime srcTime)
{
	if( *(DWORD*)(&destTime) == *(DWORD*)(&srcTime) )
		return 0;

	tagDWORDTime* pDestTime = (tagDWORDTime*)(&destTime);
	tagDWORDTime* pSrcTime = (tagDWORDTime*)(&srcTime);
	if( srcTime.year > destTime.year )
		return 0;

	// ��dwSrcTime�����һ��һ��0ʱ0��0��Ϊ��׼,�������˶�����ʱ��
	DWORD dwSrcRun =  0;	// src���ŵ�ʱ��
	DWORD dwDestRun =  0;   // dest���ŵ�ʱ��

	{
		// ����src������ʱ��(��)
		for(INT i = 1; i < (INT)pSrcTime->month; i++)
		{
			// ���ŵ��·�
			if( i == 2 )
			{
				// ����,���ǿ����ǲ�������
				if( TRUE == IsLeapYear((WORD)(pSrcTime->year) + 2000) )
				{
					dwSrcRun += 29 * 24 * 60 * 60;
				}
				else
				{
					dwSrcRun += 28 * 24 * 60 * 60;
				}

				continue;
			}

			if( i <= 7 )
			{
				if( i%2 == 0 )
					dwSrcRun += 30 * 24 * 60 * 60;
				else
					dwSrcRun += 31 * 24 * 60 * 60;
			}
			else
			{
				if( i%2 == 1 )
					dwSrcRun += 30 * 24 * 60 * 60;
				else
					dwSrcRun += 31 * 24 * 60 * 60;
			}
		}

		// ������,Сʱ���֣��������ʱ��
		dwSrcRun += (pSrcTime->day-1) * 24 * 60 * 60 + \
			pSrcTime->hour * 60 * 60 + \
			pSrcTime->min * 60 + \
			pSrcTime->sec;

	}

	{
		// ����dest���ŵ�ʱ��
		for(INT i = pSrcTime->year; i < (INT)pDestTime->year; i++)
		{
			// ���ŵ�������
			if( TRUE == IsLeapYear(i + 2000) )
			{
				dwDestRun += 366 * 24 * 60 * 60;
			}
			else
			{
				dwDestRun += 365 * 24 * 60 * 60;
			}
		}

		//dest�Ը���һ��һ��0ʱ0��0��Ϊ��׼�����ŵ�ʱ��
		for(INT i=1; i<(INT)pDestTime->month; i++)
		{
			//���ŵ��·�
			if( i == 2 )
			{
				//����,���ǿ����ǲ�������
				if( TRUE == IsLeapYear((WORD)pDestTime->year + 2000) )
				{
					dwDestRun += 29 * 24 * 60 * 60;
				}
				else
				{
					dwDestRun += 28 * 24 * 60 * 60;
				}

				continue;
			}
			if( i <= 7 )
			{
				if( i%2 == 0 )
					dwDestRun += 30 * 24 * 60 * 60;
				else
					dwDestRun += 31 * 24 * 60 * 60;
			}
			else
			{
				if( i%2 == 1 )
					dwDestRun += 30 * 24 * 60 * 60;
				else
					dwDestRun += 31 * 24 * 60 * 60;
			}
		}

		//������,Сʱ���֣��������ʱ��
		dwDestRun += (pDestTime->day-1) * 24 * 60 * 60 + \
			pDestTime->hour * 60 * 60 + \
			pDestTime->min * 60 + \
			pDestTime->sec;
	}


	// ASSERT( dwDestRun >= dwSrcRun );
	if( dwDestRun < dwSrcRun )
		return 0;

	return (dwDestRun - dwSrcRun);
}


//-----------------------------------------------------------------------------
// ʱ���ϵ���ָ������
//-----------------------------------------------------------------------------
tagDWORDTime IncreaseTime(tagDWORDTime srcTime, INT nIncrementSecond)
{
	tagDWORDTime* pSrcTime = (tagDWORDTime*)(&srcTime);

	// ��dwSrcTime�����һ��һ��0ʱ0��0��Ϊ��׼,�������˶�����ʱ��
	DWORD dwSrcRun =  0;		// src���ŵ�ʱ��

	{
		// ����src������ʱ��(��)
		for(INT i = 1; i < (INT)pSrcTime->month; i++)
		{
			// ���ŵ��·�
			if( i == 2 )
			{
				// ����,���ǿ����ǲ�������
				if( TRUE == IsLeapYear((WORD)pSrcTime->year + 2000) )
				{
					dwSrcRun += 29 * 24 * 60 * 60;
				}
				else
				{ 
					dwSrcRun += 28 * 24 * 60 * 60;
				}

				continue;
			}

			if( i <= 7 )
			{
				if( i%2 == 0 )
					dwSrcRun += 30 * 24 * 60 * 60;
				else
					dwSrcRun += 31 * 24 * 60 * 60;
			}
			else
			{
				if( i%2 == 1 )
					dwSrcRun += 30 * 24 * 60 * 60;
				else
					dwSrcRun += 31 * 24 * 60 * 60;
			}
		}

		// ������,Сʱ���֣��������ʱ��
		dwSrcRun += (pSrcTime->day-1) * 24 * 60 * 60 + \
			pSrcTime->hour * 60 * 60 + \
			pSrcTime->min * 60 + \
			pSrcTime->sec;

	}


	// ���ϵ�����ʱ��
	DWORD dwRunTime = dwSrcRun + nIncrementSecond;

	// ��ʱ������
	INT nDays = dwRunTime / (24*60*60);           // ������
	INT nSecs  = dwRunTime % (24*60*60);          // ����ʣ�������

	// ��ʼ�����ʱ��
	tagDWORDTime destTime;
	tagDWORDTime* pDestTime = (tagDWORDTime*)(&destTime);

	// �ȼ�����
	pDestTime->year = pSrcTime->year;
	for( ;; )
	{
		INT nDaysPerYear = 0;
		if( TRUE == IsLeapYear((WORD)pDestTime->year + 2000) )
			nDaysPerYear = 366;
		else
			nDaysPerYear = 365;

		if( nDays < nDaysPerYear )
			break;

		pDestTime->year++;
		nDays -= nDaysPerYear;
	}

	// �ټ�������
	pDestTime->month = 1;
	pDestTime->day = 1;
	for( ;; )
	{
		INT nDaysPerMonth = 0;

		// ���������Ҫ������
		if( pDestTime->month == 2 )
		{
			// ����,���ǿ����ǲ�������
			if( TRUE == IsLeapYear((WORD)pDestTime->year + 2000) )
			{
				nDaysPerMonth = 29;
			}
			else
			{
				nDaysPerMonth = 28;
			}
		}
		else if( pDestTime->month <= 7 )
		{
			if( pDestTime->month % 2 == 0 )
				nDaysPerMonth = 30 ;
			else
				nDaysPerMonth = 31;
		}
		else
		{
			if( pDestTime->month % 2 == 1 )
				nDaysPerMonth = 30;
			else
				nDaysPerMonth = 31;
		}

		if( nDays < nDaysPerMonth )
		{
			// ������������
			pDestTime->day += nDays;
			break;
		}

		pDestTime->month++;
		nDays -= nDaysPerMonth;
	}

	// ����Сʱ����
	pDestTime->hour = nSecs / (60*60);
	pDestTime->min = (nSecs / 60) % 60;
	pDestTime->sec = nSecs % 60;

	return destTime;
}

//-----------------------------------------------------------------------------
// ʱ���ϵݼ�ָ������
//-----------------------------------------------------------------------------
tagDWORDTime DecreaseTime(tagDWORDTime srcTime, INT nDecrementSecond)
{
	tagDWORDTime* pSrcTime = (tagDWORDTime*)(&srcTime);

	// ��dwSrcTime�����һ��һ��0ʱ0��0��Ϊ��׼,�������˶�����ʱ��
	DWORD dwSrcRun =  0;		// src���ŵ�ʱ��
	{
		// ����src������ʱ��(��)
		for(INT i = 1; i < (INT)pSrcTime->month; i++)
		{
			// ���ŵ��·�
			if( i == 2 )
			{
				// ����,���ǿ����ǲ�������
				if( TRUE == IsLeapYear((WORD)pSrcTime->year + 2000) )
				{
					dwSrcRun += 29 * 24 * 60 * 60;
				}
				else
				{ 
					dwSrcRun += 28 * 24 * 60 * 60;
				}

				continue;
			}

			if( i <= 7 )
			{
				if( i%2 == 0 )
					dwSrcRun += 30 * 24 * 60 * 60;
				else
					dwSrcRun += 31 * 24 * 60 * 60;
			}
			else
			{
				if( i%2 == 1 )
					dwSrcRun += 30 * 24 * 60 * 60;
				else
					dwSrcRun += 31 * 24 * 60 * 60;
			}
		}

		// ������,Сʱ���֣��������ʱ��
		dwSrcRun += (pSrcTime->day-1) * 24 * 60 * 60 + \
			pSrcTime->hour * 60 * 60 + \
			pSrcTime->min * 60 + \
			pSrcTime->sec;

	}

	DWORD dwSrcLeft =  0;		// src��ǰ��ʣ��ʱ�� 
	INT nDaysPerYear = 0;
	if( TRUE == IsLeapYear((WORD)pSrcTime->year + 2000) )
		nDaysPerYear = 366;
	else
		nDaysPerYear = 365;

	dwSrcLeft = nDaysPerYear * 24 * 60 * 60 - dwSrcRun;

	// ���ϵݼ���ʱ��
	DWORD dwDesTime = dwSrcLeft + nDecrementSecond;

	// ��ʱ������
	INT nDays = dwDesTime / (24*60*60);           // ������
	INT nSecs  = dwDesTime % (24*60*60);          // ����ʣ�������

	// ��ʼ�����ʱ��
	tagDWORDTime destTime;
	tagDWORDTime* pDestTime = (tagDWORDTime*)(&destTime);

	// �ȼ�����
	pDestTime->year = pSrcTime->year;
	for( ;; )
	{
		INT nDaysPerYear = 0;
		if( TRUE == IsLeapYear((WORD)pDestTime->year + 2000) )
			nDaysPerYear = 366;
		else
			nDaysPerYear = 365;

		if( nDays < nDaysPerYear )
			break;

		pDestTime->year--;
		nDays -= nDaysPerYear;
	}

	// ת����Des�����ŵ�ʱ��
	nDays = nDaysPerYear - nDays - 1;
	nSecs = 24 * 60 * 60 - nSecs;

	// �ټ�������
	pDestTime->month = 1;
	pDestTime->day = 1;
	for( ;; )
	{
		INT nDaysPerMonth = 0;

		// ���������Ҫ������
		if( pDestTime->month == 2 )
		{
			// ����,���ǿ����ǲ�������
			if( TRUE == IsLeapYear((WORD)pDestTime->year + 2000) )
			{
				nDaysPerMonth = 29;
			}
			else
			{
				nDaysPerMonth = 28;
			}
		}
		else if( pDestTime->month <= 7 )
		{
			if( pDestTime->month % 2 == 0 )
				nDaysPerMonth = 30 ;
			else
				nDaysPerMonth = 31;
		}
		else
		{
			if( pDestTime->month % 2 == 1 )
				nDaysPerMonth = 30;
			else
				nDaysPerMonth = 31;
		}

		if( nDays < nDaysPerMonth )
		{
			// ������������
			pDestTime->day += nDays;
			break;
		}

		pDestTime->month++;
		nDays -= nDaysPerMonth;
	}

	// ����Сʱ����
	pDestTime->hour = nSecs / (60*60);
	pDestTime->min = (nSecs / 60) % 60;
	pDestTime->sec = nSecs % 60;

	return destTime;
}

//---------------------------------------------------------------------------
// �ж�ĳ��ĳ��ĳ�������ڼ���0Ϊ������
//---------------------------------------------------------------------------
DWORD WhichWeekday(tagDWORDTime dwTime)
{
	// �����������һ��
	INT nYear = (INT)dwTime.year + 2000;

	// Ȼ�󿴵�ǰ����һ��ĵڼ���
	INT nDay = 0;
	for( INT i = 1; i < (INT)dwTime.month; i++ )
	{
		if( i == 2 )
		{
			if( TRUE == IsLeapYear(nYear) )
			{
				nDay += 29;
			}
			else
			{
				nDay += 28;
			}
		}

		else if( i <= 7 )
		{
			if( i%2 == 0 )	nDay += 30;
			else			nDay += 31;
		}
		else
		{
			if( i%2 == 0 )	nDay += 31;
			else			nDay += 30;
		}
	}
	nDay += dwTime.day;

	// ����ն��Ѿ�֪�����Ϳ��Լ�����
	// ̩�չ�ʽ��x-1+[(x-1)/4]-[(x-1)/100]+[(x-1)/400]+c
	return ( (nYear-1) + (nYear-1)/4 - (nYear-1)/100 + (nYear-1)/400 + nDay ) % 7;
}

//-------------------------------------------------------------------------------------
// ת����: ��-��-�� �磺2008-09-12
//-------------------------------------------------------------------------------------
BOOL DwordTime2DataTime(OUT CHAR szTime[], INT nSzArray, const tagDWORDTime &dwTime)
{
	if(nSzArray <= X_DATATIME_LEN)
	{
		return FALSE;
	}

	//sprintf(szTime, "%d%d%d-%d%d-%d%d %d%d:%d%d:%d%d", 20, dwTime.year/10, dwTime.year%10,
	//	dwTime.month/10, dwTime.month%10, dwTime.day/10, dwTime.day%10, dwTime.hour/10, 
	//	dwTime.hour%10, dwTime.min/10, dwTime.min%10, dwTime.sec/10, dwTime.sec%10);

	sprintf(szTime, "20%02d-%02d-%02d %02d:%02d:%02d", 
		dwTime.year, dwTime.month, dwTime.day, dwTime.hour, dwTime.min, dwTime.sec);

	return TRUE;
}

//-------------------------------------------------------------------------------------
// �� ��-��-�� ��ʽ������ת��ΪtagDWORDTime
//-------------------------------------------------------------------------------------
BOOL DataTime2DwordTime(OUT tagDWORDTime &dwTime, const CHAR szTime[], INT nSzArray)
{
	dwTime.Clear();

	if(nSzArray != X_DATATIME_LEN)
	{
		return FALSE;
	}

	/************************************
	*  2008-09-12 13:21:00
	*  0123456789ABCDEFGHI19
	*************************************/
	
	CHAR p[X_DATATIME_LEN + 1];
	memcpy(p, szTime, X_DATATIME_LEN);
	p[4] = p[7] = p[10] = p[13] = p[16] = p[19] = '\0';

	dwTime.year		= atoi(p + 2);
	dwTime.month	= atoi(p + 5);
	dwTime.day		= atoi(p + 8);
	dwTime.hour		= atoi(p + 11);
	dwTime.min		= atoi(p + 14);
	dwTime.sec		= atoi(p + 17);

	return TRUE;

	//CHAR p[5];
	//CHAR *pCur = szTime;
	//DWORD dwValue = 0;
	//
	//for( INT i = 0; i < 6; i++ )
	//{
	//	dwValue++;
	//	if( dwValue == 1 )
	//	{//��
	//		memcpy(p, szTime, 4);
	//		p[4] = '\0';
	//		dwTime.year = atoi(p) % 100;
	//		pCur += 2;		//�ȱ��������2���ֽ�
	//	}
	//	else if( dwValue == 2)
	//	{//��
	//		memcpy(p, pCur, 2);
	//		p[2] = '\0';
	//		dwTime.month = atoi(p);
	//	}
	//	else if( dwValue == 3)
	//	{//��
	//		memcpy(p, pCur, 2);
	//		p[2] = '\0';
	//		dwTime.day = atoi(p);
	//	}
	//	else if( dwValue == 4)
	//	{//Сʱ
	//		memcpy(p, pCur, 2);
	//		p[2] = '\0';
	//		dwTime.hour = atoi(p);
	//	}
	//	else if( dwValue == 5)
	//	{//��
	//		memcpy(p, pCur, 2);
	//		p[2] = '\0';
	//		dwTime.min = atoi(p);
	//	}
	//	else if( dwValue == 6)
	//	{//��
	//		memcpy(p, pCur, 2);
	//		p[2] = '\0';
	//		dwTime.sec = atoi(p);
	//	}
	//	else
	//	{
	//		ASSERT(FALSE);
	//	}
	//	pCur += 3;
	//}
}
