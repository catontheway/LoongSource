
#ifdef Cool3D_EXPORTS
#define Cool3D_API __declspec(dllexport)
#else
#define Cool3D_API __declspec(dllimport)
#endif

#pragma once
#include <D3D9.h>
#include <d3dx9math.h>
#pragma comment(lib,"d3dx9.lib")

#include ".\Util\UnicodeUtil.h"

/** \mainpage Cool3D Engine
	\section Design
	\par
	���Ŀ��: 
	<ol>
	<li>�����������ݹ���+��ײ�����Ϊ�����㷨, ����Χ�㷨--��Ⱦ�ֿ�, ��Ⱦ�㷨�ɸ����Կ�����
	�Լ�API����Ƴ���ͬ��strategy; 
		</li>
	</ol>

	\section Features
	\par

	\todo �Ƿ�֧�����л�??
	
	\version 1.0.0
	\author	�ı�
	\date	������2008��4��

	\par
	ע������:
	<ol>
		<li>�ⲿ��Ŀ����ʹ�ö��߳����п�</li>
		<li>�ⲿ��Ŀ��Ҫ��Pch�м��볣�õ�stdģ��,������ܻ����link error</li>
	</ol>
*/
