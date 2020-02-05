#ifndef SSDT_H
#define SSDT_H
#include <ntddk.h>

/*
* ssdt表的格式
*/
typedef struct _SYSTEM_SERVICE_TABLE {
	PVOID  		ServiceTableBase;
	PVOID  		ServiceCounterTableBase;
	ULONGLONG  	NumberOfServices;
	PVOID  		ParamTableBase;
} SYSTEM_SERVICE_TABLE, * PSYSTEM_SERVICE_TABLE;


/*
* hook的上下文
*/
typedef struct _HOOK_CONTEXT {
	ULONG IndexOfSSDT;
	ULONGLONG newFunc;
	ULONGLONG oldFunc;
	PSYSTEM_SERVICE_TABLE KeServiceDescriptorTable;
	ULONG OldTpVal;
} HOOK_CONTEXT, * PHOOK_CONTEXT;


/*
* 获取实例
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