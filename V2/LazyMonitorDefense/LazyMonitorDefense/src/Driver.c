#include "Driver.h"


/*
* 卸载驱动
*/
static VOID 
DriverUnload(PDRIVER_OBJECT pDriverObj) {

	UNICODE_STRING strLinkname;
	RtlInitUnicodeString(&strLinkname, LINK_NAME);
	IoDeleteSymbolicLink(&strLinkname);
	IoDeleteDevice(pDriverObj->DeviceObject);

	// 恢复hook
	Unhook();
	KdPrint(("卸载驱动"));
}


/*
* 创建irp的callback
*/
NTSTATUS 
DispatchCreate(PDEVICE_OBJECT pDevObj, PIRP pIrp) {
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}


/*
* 关闭irpcallback
*/
static NTSTATUS 
DispatchClose(PDEVICE_OBJECT pDevObj, PIRP pIrp) {
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}


static 
NTSTATUS CreateSymbolicLink(PUNICODE_STRING strDeviceName, 
	PUNICODE_STRING strLinkName,
	PUNICODE_STRING strGlobaLinkName) {
	NTSTATUS status = STATUS_SUCCESS;
	if (IoIsWdmVersionAvailable(1, 0x10)) {
		status = IoCreateSymbolicLink(strGlobaLinkName, strDeviceName);
	}
	else {
		status = IoCreateSymbolicLink(strLinkName, strDeviceName);
	}
	return status;
}


/*
* 创建driver设备
*/
NTSTATUS
CreateIODevice(PDRIVER_OBJECT pDriverObj,
	PUNICODE_STRING strDeviceName,
	PUNICODE_STRING strLinkName,
	PUNICODE_STRING strGlobaLinkName) {
	KdPrint(("创建driver设备"));
	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_OBJECT pDeviceObj = NULL;
	status = IoCreateDevice(pDriverObj, 0, strDeviceName, FILE_DEVICE_UNKNOWN, 0, FALSE, &pDeviceObj);
	if (!NT_SUCCESS(status)) {
		return status;
	}
	
	status = CreateSymbolicLink(strDeviceName, strLinkName, strGlobaLinkName);
	if (!NT_SUCCESS(status)) {
		IoDeleteDevice(pDeviceObj);
	}
	else {
		KdPrint(("创建driver设备成功"));
	}
	return status;
}


/*
* 初始化驱动
*/
VOID
InitDriver (PDRIVER_OBJECT pDriverObj) {
	KdPrint(("初始化驱动"));
	pDriverObj->DriverUnload = DriverUnload;
	pDriverObj->MajorFunction[IRP_MJ_CREATE] = DispatchCreate;
	pDriverObj->MajorFunction[IRP_MJ_CLOSE] = DispatchClose;
}