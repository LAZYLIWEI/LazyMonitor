#include <ntddk.h>
#include "Driver.h"
#include "SSDT.h"

//typedef NTSTATUS(__fastcall* NTCREATEFILE)(
//	__out     PHANDLE FileHandle,
//	__in      ACCESS_MASK DesiredAccess,
//	__in      POBJECT_ATTRIBUTES ObjectAttributes,
//	__out     PIO_STATUS_BLOCK IoStatusBlock,
//	__in_opt  PLARGE_INTEGER AllocationSize,
//	__in      ULONG FileAttributes,
//	__in      ULONG ShareAccess,
//	__in      ULONG CreateDisposition,
//	__in      ULONG CreateOptions,
//	__in      PVOID EaBuffer,
//	__in      ULONG EaLength
//	);
//

//
//NTSTATUS __fastcall Fake_NtCreateFile(
//	__out     PHANDLE FileHandle,
//	__in      ACCESS_MASK DesiredAccess,
//	__in      POBJECT_ATTRIBUTES ObjectAttributes,
//	__out     PIO_STATUS_BLOCK IoStatusBlock,
//	__in_opt  PLARGE_INTEGER AllocationSize,
//	__in      ULONG FileAttributes,
//	__in      ULONG ShareAccess,
//	__in      ULONG CreateDisposition,
//	__in      ULONG CreateOptions,
//	__in      PVOID EaBuffer,
//	__in      ULONG EaLength
//)
//{
//	NTCREATEFILE NtCreateFile = (NTCREATEFILE)GetInstance()->oldFunc;
//	NTSTATUS st;
//	DbgPrint("目录:%ws", ObjectAttributes->ObjectName->Buffer);
//	if (wcsstr(ObjectAttributes->ObjectName->Buffer, L"liwei.txt"))
//		st = STATUS_ACCESS_DENIED;
//	else
//		st = NtCreateFile(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength);;
//
//	DbgPrint("Fake_NtCreateFile called: %x", st);
//	return st;
//}
NTSTATUS
PsLookupProcessByProcessId(
	__in HANDLE ProcessId,   //进程ID
	__deref_out PEPROCESS* Process //返回的EPROCESS
);

NTKERNELAPI
UCHAR*
PsGetProcessImageFileName(PEPROCESS Process);

//typedef NTSTATUS(__fastcall* NTTERMINATEPROCESS)(IN HANDLE ProcessHandle, IN NTSTATUS ExitStatus);
//
//NTSTATUS __fastcall Fake_NtTerminateProcess(IN HANDLE ProcessHandle, IN NTSTATUS ExitStatus)
//{
//	PEPROCESS Process;
//	NTSTATUS st = ObReferenceObjectByHandle(ProcessHandle, 0, *PsProcessType, KernelMode, &Process, NULL);
//	if (NT_SUCCESS(st))
//	{
//		DbgPrint(PsGetProcessImageFileName(Process));
//		if (!_stricmp(PsGetProcessImageFileName(Process), "lservice.exe")) {
//			DbgPrint(("NtTerminateProcess STATUS_ACCESS_DENIED!"));
//			return STATUS_ACCESS_DENIED;
//		}
//	}
//
//	DbgPrint(("NtTerminateProcess called!"));
//	NTTERMINATEPROCESS NtTerminateProcess = (NTTERMINATEPROCESS)GetInstance()->oldFunc;
//	return NtTerminateProcess(ProcessHandle, ExitStatus);
//}


//定义NTOPENPROCESS  
typedef NTSTATUS(__stdcall* NTOPENPROCESS)(OUT PHANDLE  ProcessHandle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_ATTRIBUTES  ObjectAttributes,
	IN OPTIONAL PCLIENT_ID  ClientId);

NTSTATUS __stdcall Fake_NtOpenProcess(OUT PHANDLE ProcessHandle,
    IN ACCESS_MASK  DesiredAccess,
    IN POBJECT_ATTRIBUTES  ObjectAttributes,
    IN OPTIONAL PCLIENT_ID  ClientId) {
	PEPROCESS Process;
	NTSTATUS st = PsLookupProcessByProcessId(ClientId->UniqueProcess, &Process);
	if (NT_SUCCESS(st)) {
		KdPrint((PsGetProcessImageFileName(Process)));
		if (!_stricmp(PsGetProcessImageFileName(Process), "lmonitor.exe")) {
			KdPrint(("NtOpenProcess STATUS_ACCESS_DENIED!"));
			return STATUS_ACCESS_DENIED;
		}
	}
	else {
		KdPrint(("ObReferenceObjectByHandle failed!"));
	}

	NTOPENPROCESS NtOpenProcess = (NTOPENPROCESS)GetInstance()->oldFunc;
	return NtOpenProcess(ProcessHandle, DesiredAccess, ObjectAttributes, ClientId);
}


NTSTATUS 
DriverEntry(PDRIVER_OBJECT pDriverObj, PUNICODE_STRING pRegistryString) {
	UNICODE_STRING ustrDevName;
	UNICODE_STRING ustrLinkName;
	UNICODE_STRING ustrGlobaLinkName;
	RtlInitUnicodeString(&ustrDevName, DEVICE_NAME);
	RtlInitUnicodeString(&ustrLinkName, LINK_NAME);
	RtlInitUnicodeString(&ustrGlobaLinkName, LINK_GLOBAL_NAME);

	// 初始化driver
	InitDriver(pDriverObj);

	// 创建设备
	NTSTATUS status = STATUS_SUCCESS;
	status = CreateIODevice(pDriverObj, &ustrDevName, &ustrLinkName, &ustrGlobaLinkName);

	// hook
	if (NT_SUCCESS(status)) {
		Hook(35, (ULONGLONG)Fake_NtOpenProcess);
	}

	return status;
}
