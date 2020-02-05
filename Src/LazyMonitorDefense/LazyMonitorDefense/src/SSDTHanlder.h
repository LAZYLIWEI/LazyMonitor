#ifndef SSDTHANLDER_H
#define SSDTHANLDER_H
#include <ntddk.h>

typedef struct _SYSTEM_SERVICE_TABLE {
	PVOID  		ServiceTableBase; // ���ָ��ϵͳ��������ַ��
	PVOID  		ServiceCounterTableBase; // SSDT��ÿ�����񱻵��ô����ļ����������������һ����sysenter����
	ULONGLONG  	NumberOfServices; // ��ServiceTableBase �����ķ������Ŀ��
	PVOID  		ParamTableBase; // ÿ��ϵͳ��������ֽ�����Ļ���ַ-ϵͳ���������SSPT 
} SYSTEM_SERVICE_TABLE, * PSYSTEM_SERVICE_TABLE;

typedef struct _SERVICE_DESCRIPTOR_TABLE {
	SYSTEM_SERVICE_TABLE ntoskrnl;  // ntoskrnl.exe (native api)
	SYSTEM_SERVICE_TABLE win32k;    // win32k.sys   (gdi/user)
	SYSTEM_SERVICE_TABLE Table3;    // not used
	SYSTEM_SERVICE_TABLE Table4;    // not used
}SERVICE_DESCRIPTOR_TABLE, * PSERVICE_DESCRIPTOR_TABLE;

/*
* ��ȡssdt��Ļ�ַ
*/
ULONGLONG
GetKeServiceDescriptorTable64();

/*
* ����ssdt����Ŀ
*/
VOID
ForeachSSDTAddress();


VOID
Hook();



#endif // !SSDTHANLDER_H
