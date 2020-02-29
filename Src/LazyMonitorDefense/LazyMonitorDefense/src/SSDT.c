#include "SSDT.h"

/*
* �ر��ڴ�д����
*/
static
KIRQL WPOFFx64() {
	KIRQL irql = KeRaiseIrqlToDpcLevel();
	UINT64 cr0 = __readcr0();
	cr0 &= 0xfffffffffffeffff;
	__writecr0(cr0);
	_disable();
	return irql;
}


/*
* �����ڴ�д����
*/
static
void WPONx64(KIRQL irql) {
	UINT64 cr0 = __readcr0();
	cr0 |= 0x10000;
	_enable();
	__writecr0(cr0);
	KeLowerIrql(irql);
}


static VOID
ShowStuff0(LONG int32num) {
	CHAR b[4] = { 0 };
	memcpy(&b[0], (PUCHAR)(&int32num) + 0, 1);
	memcpy(&b[1], (PUCHAR)(&int32num) + 1, 1);
	memcpy(&b[2], (PUCHAR)(&int32num) + 2, 1);
	memcpy(&b[3], (PUCHAR)(&int32num) + 3, 1);
}



/*
* �޸�KeBugCheckEx��������
* newfun: Ҫִ�еĺ�����ַ
*/
static
VOID FuckKeBugCheckEx(ULONGLONG newfun) {
	KIRQL irql;
	UCHAR jmp_code[] = "\xFF\x25\x00\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
	memcpy(jmp_code + 6, &newfun, 8);
	irql = WPOFFx64();
	memset(KeBugCheckEx, 0x90, 15);
	memcpy(KeBugCheckEx, jmp_code, 14);
	WPONx64(irql);
}


/*
* ��ȡssdt��ĵ�ַ
*/
static
ULONGLONG GetKeServiceDescriptorTable64(){
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



/*
* ��ȡ������ַ
*/
static ULONGLONG 
GetSSDTFuncCurAddr(PSYSTEM_SERVICE_TABLE KeServiceDescriptorTable, ULONG id){
	LONG dwtmp = 0;
	ULONGLONG add = 0;
	PULONG ServiceTableBase = (PULONG)KeServiceDescriptorTable->ServiceTableBase;
	dwtmp = ServiceTableBase[id];
	ShowStuff0(dwtmp);
	dwtmp = dwtmp >> 4;
	add = ((LONGLONG)dwtmp + (ULONGLONG)ServiceTableBase);//&0xFFFFFFF0;
	return add;
}


#define SETBIT(x,y) x|=(1<<y) //��X�ĵ�Yλ��1
#define CLRBIT(x,y) x&=~(1<<y) //��X�ĵ�Yλ��0
#define GETBIT(x,y) (x & (1 << y)) //ȡX�ĵ�Yλ������0���0

static ULONG 
GetOffsetAddress(
	PSYSTEM_SERVICE_TABLE KeServiceDescriptorTable,
	ULONGLONG FuncAddr, CHAR ParamCount) {
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




PHOOK_CONTEXT
GetInstance() {
	static PHOOK_CONTEXT HookContext = NULL;
	if (HookContext == NULL) {
		HookContext = ExAllocatePool(PagedPool, sizeof(HOOK_CONTEXT));
	}
	return HookContext;
}


/*
* hook
* ������ssdt���ĸ�����
* �µĺ�����ַ
*/
NTSTATUS
Hook(ULONG IndexOfSSDT,
	ULONGLONG newFunc) {
	PHOOK_CONTEXT HookContext = GetInstance();
	if (HookContext == NULL) {
		KdPrint(("hook failed"));
		return STATUS_UNSUCCESSFUL;
	}
	HookContext->IndexOfSSDT = IndexOfSSDT;
	HookContext->newFunc = newFunc;

	PSYSTEM_SERVICE_TABLE KeServiceDescriptorTable = (PSYSTEM_SERVICE_TABLE)GetKeServiceDescriptorTable64();
	HookContext->KeServiceDescriptorTable = KeServiceDescriptorTable;
	PULONG ServiceTableBase = (PULONG)KeServiceDescriptorTable->ServiceTableBase;
	HookContext->oldFunc = GetSSDTFuncCurAddr(KeServiceDescriptorTable, HookContext->IndexOfSSDT);
	KdPrint(("function address is: %llx", HookContext->oldFunc));
	FuckKeBugCheckEx(HookContext->newFunc);

	HookContext->OldTpVal = ServiceTableBase[HookContext->IndexOfSSDT];
	KIRQL irql = WPOFFx64();
	ServiceTableBase[HookContext->IndexOfSSDT] = GetOffsetAddress(KeServiceDescriptorTable, (ULONGLONG)KeBugCheckEx, 11);
	WPONx64(irql);
	KdPrint(("KeBugCheckEx: %llx", (ULONGLONG)KeBugCheckEx));
	return STATUS_SUCCESS;
}


/*
* unhook
* ������ssdt���ĸ�����
* ��Ļ�ַ
* �����ɵ�ֵ
*/
VOID 
Unhook() {
	PHOOK_CONTEXT HookContext = GetInstance();
	if (HookContext == NULL) {
		return;
	}

	KIRQL irql;
	PULONG ServiceTableBase = NULL;
	ServiceTableBase = (PULONG)HookContext->KeServiceDescriptorTable->ServiceTableBase;
	irql = WPOFFx64();
	ServiceTableBase[HookContext->IndexOfSSDT] = HookContext->OldTpVal;
	WPONx64(irql);

	ExFreePool(HookContext);
	HookContext = NULL;
}
