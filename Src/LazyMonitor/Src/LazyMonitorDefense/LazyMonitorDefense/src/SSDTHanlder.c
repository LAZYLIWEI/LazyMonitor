#include "SSDTHanlder.h"

ULONGLONG 
GetKeServiceDescriptorTable64() {
	PUCHAR StartSearchAddress = (PUCHAR)__readmsr(0xC0000082);
	PUCHAR EndSearchAddress = StartSearchAddress + 0x500;
	PUCHAR i = NULL;
	UCHAR byte1 = 0, byte2 = 0, byte3 = 0;
	ULONG temp = 0;
	ULONGLONG addr = 0;
	//开始搜索
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
* 遍历ssdt表项目
*/
VOID
ForeachSSDTAddress() {
	PSYSTEM_SERVICE_TABLE KeServiceDescriptorTable = (PSYSTEM_SERVICE_TABLE)GetKeServiceDescriptorTable64();
	DbgPrint("KeServiceDescriptorTable %lld", (ULONGLONG)KeServiceDescriptorTable);
	PULONG  ServiceTableBase = KeServiceDescriptorTable->ServiceTableBase;


}
