#include "Driver.h"


/*
* ж������
*/
static VOID 
DriverUnload(PDRIVER_OBJECT pDriverObj) {

	UNICODE_STRING strLinkname;
	RtlInitUnicodeString(&strLinkname, LINK_NAME);
	IoDeleteSymbolicLink(&strLinkname);
	IoDeleteDevice(pDriverObj->DeviceObject);

	// �ָ�hook
	Unhook();
	KdPrint(("ж������"));
}


/*
* ����irp��callback
*/
NTSTATUS 
DispatchCreate(PDEVICE_OBJECT pDevObj, PIRP pIrp) {
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}


/*
* �ر�irpcallback
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
* ����driver�豸
*/
NTSTATUS
CreateIODevice(PDRIVER_OBJECT pDriverObj,
	PUNICODE_STRING strDeviceName,
	PUNICODE_STRING strLinkName,
	PUNICODE_STRING strGlobaLinkName) {
	KdPrint(("����driver�豸"));
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
		KdPrint(("����driver�豸�ɹ�"));
	}
	return status;
}


/*
* ��ʼ������
*/
VOID
InitDriver (PDRIVER_OBJECT pDriverObj) {
	KdPrint(("��ʼ������"));
	pDriverObj->DriverUnload = DriverUnload;
	pDriverObj->MajorFunction[IRP_MJ_CREATE] = DispatchCreate;
	pDriverObj->MajorFunction[IRP_MJ_CLOSE] = DispatchClose;
}