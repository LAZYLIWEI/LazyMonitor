#ifndef SSDTHANLDER_H
#define SSDTHANLDER_H
#include <ntddk.h>

typedef struct _SYSTEM_SERVICE_TABLE {
	PVOID  		ServiceTableBase; // 这个指向系统服务函数地址表
	PVOID  		ServiceCounterTableBase; // SSDT中每个服务被调用次数的计数器。这个计数器一般由sysenter更新
	ULONGLONG  	NumberOfServices; // 由ServiceTableBase 描述的服务的数目。
	PVOID  		ParamTableBase; // 每个系统服务参数字节数表的基地址-系统服务参数表SSPT 
} SYSTEM_SERVICE_TABLE, * PSYSTEM_SERVICE_TABLE;

typedef struct _SERVICE_DESCRIPTOR_TABLE {
	SYSTEM_SERVICE_TABLE ntoskrnl;  // ntoskrnl.exe (native api)
	SYSTEM_SERVICE_TABLE win32k;    // win32k.sys   (gdi/user)
	SYSTEM_SERVICE_TABLE Table3;    // not used
	SYSTEM_SERVICE_TABLE Table4;    // not used
}SERVICE_DESCRIPTOR_TABLE, * PSERVICE_DESCRIPTOR_TABLE;

/*
* 获取ssdt表的基址
*/
ULONGLONG
GetKeServiceDescriptorTable64();

/*
* 遍历ssdt表项目
*/
VOID
ForeachSSDTAddress();


VOID
Hook();



#endif // !SSDTHANLDER_H
