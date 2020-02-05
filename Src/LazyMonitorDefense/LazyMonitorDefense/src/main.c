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

NTKERNELAPI
UCHAR*
PsGetProcessImageFileName(PEPROCESS Process);

typedef NTSTATUS(__fastcall* NTTERMINATEPROCESS)(IN HANDLE ProcessHandle, IN NTSTATUS ExitStatus);

NTSTATUS __fastcall Fake_NtTerminateProcess(IN HANDLE ProcessHandle, IN NTSTATUS ExitStatus)
{
	PEPROCESS Process;
	NTSTATUS st = ObReferenceObjectByHandle(ProcessHandle, 0, *PsProcessType, KernelMode, &Process, NULL);
	if (NT_SUCCESS(st))
	{
		if (!_stricmp(PsGetProcessImageFileName(Process), "lazy_service.exe")) {
			KdPrint(("NtTerminateProcess STATUS_ACCESS_DENIED!"));
			return STATUS_ACCESS_DENIED;
		}
		else {
			KdPrint(("NtTerminateProcess called!"));
			NTTERMINATEPROCESS NtTerminateProcess = (NTTERMINATEPROCESS)GetInstance()->oldFunc;
			return NtTerminateProcess(ProcessHandle, ExitStatus);
		}
	}
	else {
		KdPrint(("ObReferenceObjectByHandle failed!"));
		return STATUS_ACCESS_DENIED;
	}
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
		Hook(41, (ULONGLONG)Fake_NtTerminateProcess);
	}

	return status;
}
