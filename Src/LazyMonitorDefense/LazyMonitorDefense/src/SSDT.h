#ifndef SSDT_H
#define SSDT_H
#include <ntddk.h>

/*
* ssdt��ĸ�ʽ
*/
typedef struct _SYSTEM_SERVICE_TABLE {
	PVOID  		ServiceTableBase;
	PVOID  		ServiceCounterTableBase;
	ULONGLONG  	NumberOfServices;
	PVOID  		ParamTableBase;
} SYSTEM_SERVICE_TABLE, * PSYSTEM_SERVICE_TABLE;


/*
* hook��������
*/
typedef struct _HOOK_CONTEXT {
	ULONG IndexOfSSDT;
	ULONGLONG newFunc;
	ULONGLONG oldFunc;
	PSYSTEM_SERVICE_TABLE KeServiceDescriptorTable;
	ULONG OldTpVal;
} HOOK_CONTEXT, * PHOOK_CONTEXT;


/*
* ��ȡʵ��
*/
PHOOK_CONTEXT GetInstance();

/*
* hook
*/
NTSTATUS
Hook(ULONG IndexOfSSDT, ULONGLONG newFunc);

/*
* Unhook
*/
VOID Unhook();

#endif