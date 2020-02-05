#ifndef DRIVER_H
#define DRIVER_H
#include <ntddk.h>
#include "SSDT.h"

/*
* 驱动必要信息
*/
#define	DEVICE_NAME			L"\\Device\\hooktemplate_x64"
#define LINK_NAME			L"\\DosDevices\\hooktemplate_x64"
#define LINK_GLOBAL_NAME	L"\\DosDevices\\Global\\hooktemplate_x64"


/*
* 初始化驱动
*/
VOID
InitDriver(PDRIVER_OBJECT pDriverObj);

/*
* 创建设备
*/
NTSTATUS
CreateIODevice(PDRIVER_OBJECT pDriverObj,
	PUNICODE_STRING strDeviceName,
	PUNICODE_STRING strLinkName,
	PUNICODE_STRING strGlobaLinkName);

#endif
