#ifndef DRIVER_H
#define DRIVER_H
#include <ntddk.h>
#include "SSDT.h"

/*
* ������Ҫ��Ϣ
*/
#define	DEVICE_NAME			L"\\Device\\hooktemplate_x64"
#define LINK_NAME			L"\\DosDevices\\hooktemplate_x64"
#define LINK_GLOBAL_NAME	L"\\DosDevices\\Global\\hooktemplate_x64"


/*
* ��ʼ������
*/
VOID
InitDriver(PDRIVER_OBJECT pDriverObj);

/*
* �����豸
*/
NTSTATUS
CreateIODevice(PDRIVER_OBJECT pDriverObj,
	PUNICODE_STRING strDeviceName,
	PUNICODE_STRING strLinkName,
	PUNICODE_STRING strGlobaLinkName);

#endif
