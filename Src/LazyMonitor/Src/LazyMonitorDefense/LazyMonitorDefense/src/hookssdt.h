#include <ntddk.h>

typedef struct _SYSTEM_SERVICE_TABLE {
	PVOID  		ServiceTableBase;
	PVOID  		ServiceCounterTableBase;
	ULONGLONG  	NumberOfServices;
	PVOID  		ParamTableBase;
} SYSTEM_SERVICE_TABLE, * PSYSTEM_SERVICE_TABLE;


typedef NTSTATUS(__fastcall* NTCREATEFILE)(
	__out     PHANDLE FileHandle,
	__in      ACCESS_MASK DesiredAccess,
	__in      POBJECT_ATTRIBUTES ObjectAttributes,
	__out     PIO_STATUS_BLOCK IoStatusBlock,
	__in_opt  PLARGE_INTEGER AllocationSize,
	__in      ULONG FileAttributes,
	__in      ULONG ShareAccess,
	__in      ULONG CreateDisposition,
	__in      ULONG CreateOptions,
	__in      PVOID EaBuffer,
	__in      ULONG EaLength
	);



PSYSTEM_SERVICE_TABLE KeServiceDescriptorTable;
NTCREATEFILE NtCreateFile = NULL;
ULONG OldTpVal;
ULONG index = 82;	//��HOOK������INDEX

NTSTATUS __fastcall Fake_NtCreateFile(
	__out     PHANDLE FileHandle,
	__in      ACCESS_MASK DesiredAccess,
	__in      POBJECT_ATTRIBUTES ObjectAttributes,
	__out     PIO_STATUS_BLOCK IoStatusBlock,
	__in_opt  PLARGE_INTEGER AllocationSize,
	__in      ULONG FileAttributes,
	__in      ULONG ShareAccess,
	__in      ULONG CreateDisposition,
	__in      ULONG CreateOptions,
	__in      PVOID EaBuffer,
	__in      ULONG EaLength
)
{
	NTSTATUS st;
	st = NtCreateFile(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength);
	DbgPrint("Fake_NtCreateFile called: %x", st);
	return st;
}

KIRQL WPOFFx64()
{
	KIRQL irql = KeRaiseIrqlToDpcLevel();
	UINT64 cr0 = __readcr0();
	cr0 &= 0xfffffffffffeffff;
	__writecr0(cr0);
	_disable();
	return irql;
}

void WPONx64(KIRQL irql)
{
	UINT64 cr0 = __readcr0();
	cr0 |= 0x10000;
	_enable();
	__writecr0(cr0);
	KeLowerIrql(irql);
}

ULONGLONG GetKeServiceDescriptorTable64() //���еķ���
{
	PUCHAR StartSearchAddress = (PUCHAR)__readmsr(0xC0000082);
	PUCHAR EndSearchAddress = StartSearchAddress + 0x500;
	PUCHAR i = NULL;
	UCHAR byte1 = 0, byte2 = 0, byte3 = 0;
	ULONG temp = 0;
	ULONGLONG addr = 0;
	//��ʼ����
	for (i = StartSearchAddress; i < EndSearchAddress; i++)
	{
		if (MmIsAddressValid(i) && MmIsAddressValid(i + 1) && MmIsAddressValid(i + 2))
		{
			byte1 = *i;
			byte2 = *(i + 1);
			byte3 = *(i + 2);
			if (byte1 == 0x4c && byte2 == 0x8d && byte3 == 0x15) //4c8d15
			{
				memcpy(&temp, i + 3, 4);
				addr = (ULONGLONG)temp + (ULONGLONG)i + 7;
				return addr;
			}
		}
	}
	return 0;
}

void ShowStuff0(LONG int32num)
{
	CHAR b[4] = { 0 };
	memcpy(&b[0], (PUCHAR)(&int32num) + 0, 1); DbgPrint("b[0] & 0xF=%ld\n", b[0] & 0xF);	//����������ĸ���-4
	memcpy(&b[1], (PUCHAR)(&int32num) + 1, 1); DbgPrint("b[1] & 0xF=%ld\n", b[1] & 0xF);
	memcpy(&b[2], (PUCHAR)(&int32num) + 2, 1); DbgPrint("b[2] & 0xF=%ld\n", b[2] & 0xF);
	memcpy(&b[3], (PUCHAR)(&int32num) + 3, 1); DbgPrint("b[3] & 0xF=%ld\n", b[3] & 0xF);
}

ULONGLONG GetSSDTFuncCurAddr(ULONG id)
{
	LONG dwtmp = 0;
	ULONGLONG add = 0;
	PULONG ServiceTableBase = NULL;
	ServiceTableBase = (PULONG)KeServiceDescriptorTable->ServiceTableBase;
	dwtmp = ServiceTableBase[id];
	ShowStuff0(dwtmp);
	dwtmp = dwtmp >> 4;
	add = ((LONGLONG)dwtmp + (ULONGLONG)ServiceTableBase);//&0xFFFFFFF0;
	return add;
}

#define SETBIT(x,y) x|=(1<<y) //��X�ĵ�Yλ��1
#define CLRBIT(x,y) x&=~(1<<y) //��X�ĵ�Yλ��0
#define GETBIT(x,y) (x & (1 << y)) //ȡX�ĵ�Yλ������0���0

ULONG GetOffsetAddress(ULONGLONG FuncAddr, CHAR ParamCount)
{
	LONG dwtmp = 0, i;
	CHAR b = 0, bits[4] = { 0 };
	PULONG ServiceTableBase = NULL;
	ServiceTableBase = (PULONG)KeServiceDescriptorTable->ServiceTableBase;
	dwtmp = (LONG)(FuncAddr - (ULONGLONG)ServiceTableBase);
	dwtmp = dwtmp << 4;
	//�������
	if (ParamCount > 4)
		ParamCount = ParamCount - 4;
	else
		ParamCount = 0;
	//���dwtmp�ĵ�һ���ֽ�
	memcpy(&b, &dwtmp, 1);
	//�������λ����д��������
	for (i = 0; i < 4; i++)
	{
		bits[i] = GETBIT(ParamCount, i);
		if (bits[i])
			SETBIT(b, i);
		else
			CLRBIT(b, i);
	}
	//�����ݸ��ƻ�ȥ
	memcpy(&dwtmp, &b, 1);
	return dwtmp;
}

VOID FuckKeBugCheckEx()
{
	KIRQL irql;
	ULONGLONG myfun;
	UCHAR jmp_code[] = "\xFF\x25\x00\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
	myfun = (ULONGLONG)Fake_NtCreateFile;
	memcpy(jmp_code + 6, &myfun, 8);
	irql = WPOFFx64();
	memset(KeBugCheckEx, 0x90, 15);
	memcpy(KeBugCheckEx, jmp_code, 14);
	WPONx64(irql);
}

/*
��дKeBugCheckEx�ĵ�ַ
��KeBugCheckEx��дjmp������Fake_NtCreateFile
����ֱ����дFake_NtCreateFile�ĵ�ַ����Ϊ���ǲ���ͬһ��4GB
*/

VOID HookSSDT()
{
	KIRQL irql;
	PULONG ServiceTableBase = NULL;
	//get old address
	dprintf("NtCreateFile index: %ld", index);
	NtCreateFile = (NTCREATEFILE)GetSSDTFuncCurAddr(index);
	dprintf("Old_NtCreateFile: %llx", (ULONGLONG)NtCreateFile);
	//show new address
	ServiceTableBase = (PULONG)KeServiceDescriptorTable->ServiceTableBase;
	//set kebugcheckex
	FuckKeBugCheckEx();
	//record old offset value
	OldTpVal = ServiceTableBase[index];
	irql = WPOFFx64();
	ServiceTableBase[index] = GetOffsetAddress((ULONGLONG)KeBugCheckEx, 11);
	WPONx64(irql);
	dprintf("KeBugCheckEx: %llx", (ULONGLONG)KeBugCheckEx);
	dprintf("New_NtCreateFile: %llx", GetSSDTFuncCurAddr(index));
}

VOID UnhookSSDT()
{
	KIRQL irql;
	PULONG ServiceTableBase = NULL;
	ServiceTableBase = (PULONG)KeServiceDescriptorTable->ServiceTableBase;
	//set value
	irql = WPOFFx64();
	ServiceTableBase[index] = OldTpVal;	//GetOffsetAddress((ULONGLONG)NtCreateFile);
	WPONx64(irql);
}