#ifndef _PROOF_Dll_INTERFACE_H_
#define _PROOF_Dll_INTERFACE_H_

/**
/	Ѹ����֤���� add by songg 2009.6.22
*/

//Ѹ��   ���ڼ������ݣ�deDataΪ�����ܴ���nBufLenΪ�����ܴ��ĳ��ȣ�enDataΪ���ܺ󴮣�
typedef int ( *PROOF_XUNLEI_AESENCRYPT)(const char *deData, char *enData, int nBufLen);
//Ѹ��   ���ڽ������ݣ�enDataΪ����ǰ�Ĵ���deDataΪ���ܺ�Ĵ���
typedef void ( *PROOF_XUNLEI_AESDECRYPT)(const char *enData, char *deData);

extern void InitProofXunleiInterface(HINSTANCE);

extern PROOF_XUNLEI_AESENCRYPT        PROOF_XUNLEI_AesEncrypt;
extern PROOF_XUNLEI_AESDECRYPT        PROOF_XUNLEI_AesDecrypt;





#endif