#include "Driver.h"

ULONG64 CreateVM(ULONG CpuIndex)
{
	psVM[CpuIndex].pOnVa = ExAllocatePool2(POOL_FLAG_NON_PAGED, PAGE_SIZE, 'VMON');
	if (!psVM[CpuIndex].pOnVa) { return FALSE; }
	RtlZeroMemory(psVM[CpuIndex].pOnVa, PAGE_SIZE);
	psVM[CpuIndex].pOnPa = MmGetPhysicalAddress(psVM[CpuIndex].pOnVa);

	psVM[CpuIndex].pCsVa = ExAllocatePool2(POOL_FLAG_NON_PAGED, PAGE_SIZE, 'VMCS');
	if (!psVM[CpuIndex].pCsVa) { return FALSE; }
	RtlZeroMemory(psVM[CpuIndex].pCsVa, PAGE_SIZE);
	psVM[CpuIndex].pCsPa = MmGetPhysicalAddress(psVM[CpuIndex].pCsVa);

	psVM[CpuIndex].pStack = ExAllocatePool2(POOL_FLAG_NON_PAGED, STACK_SIZE, 'VMSK');
	if (!psVM[CpuIndex].pStack) { return FALSE; }
	RtlZeroMemory(psVM[CpuIndex].pStack, STACK_SIZE);

	psVM[CpuIndex].pMsrMapVa = ExAllocatePool2(POOL_FLAG_NON_PAGED, PAGE_SIZE, 'msrB');
	if (!psVM[CpuIndex].pMsrMapVa) { return FALSE; }
	RtlZeroMemory(psVM[CpuIndex].pMsrMapVa, PAGE_SIZE);
	psVM[CpuIndex].pMsrMapPa = MmGetPhysicalAddress(psVM[CpuIndex].pMsrMapVa);
	psVM[CpuIndex].pMsrMap_R_L = (PVOID)((ULONG64)psVM[CpuIndex].pMsrMapVa + 0x000);
	psVM[CpuIndex].pMsrMap_R_H = (PVOID)((ULONG64)psVM[CpuIndex].pMsrMapVa + 0x400);
	psVM[CpuIndex].pMsrMap_W_L = (PVOID)((ULONG64)psVM[CpuIndex].pMsrMapVa + 0x800);
	psVM[CpuIndex].pMsrMap_W_H = (PVOID)((ULONG64)psVM[CpuIndex].pMsrMapVa + 0xC00);

	psVM[CpuIndex].pMsrMemVa = ExAllocatePool2(POOL_FLAG_NON_PAGED, PAGE_SIZE * 2, 'msrM');
	if (!psVM[CpuIndex].pMsrMemVa) { return FALSE; }
	RtlZeroMemory(psVM[CpuIndex].pMsrMemVa, PAGE_SIZE * 2);
	psVM[CpuIndex].pMsrMemPa = MmGetPhysicalAddress(psVM[CpuIndex].pMsrMemVa);

	return TRUE;
}

ULONG64 DeleteVM(ULONG CpuIndex)
{
	if (psVM[CpuIndex].pOnVa) { ExFreePool(psVM[CpuIndex].pOnVa); }
	if (psVM[CpuIndex].pCsVa) { ExFreePool(psVM[CpuIndex].pCsVa); }
	if (psVM[CpuIndex].pStack) { ExFreePool(psVM[CpuIndex].pStack); }
	if (psVM[CpuIndex].pMsrMapVa) { ExFreePool(psVM[CpuIndex].pMsrMapVa); }
	if (psVM[CpuIndex].pMsrMemVa) { ExFreePool(psVM[CpuIndex].pMsrMemVa); }
	return TRUE;
}

ULONG_PTR KipiSetupVT(ULONG_PTR Arg)
{

	ULONG CpuIndex = KeGetCurrentProcessorNumber();
	OpenVMX();
	if (CreateVM(CpuIndex))
	{
		if (_StartVM(CpuIndex))
		{
			KdPrint(("CPU:[%d],开启VMX\n", CpuIndex));
			return TRUE;
		}
	}
	return FALSE;
}

ULONG_PTR KipiUnloadVT(ULONG_PTR Arg)
{
	ULONG CpuIndex = KeGetCurrentProcessorNumber();
	StopVM();	
	CloseVMX();
	DeleteVM(CpuIndex);

	KdPrint(("CPU:[%d],关闭VMX\n", CpuIndex));
	return TRUE;
}

VOID DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	// 发送IPI,关闭VT
	if (!KeIpiGenericCall(KipiUnloadVT, 0))
	{
		KdPrint(("Error:KipiUnloadVT\n"));
	}

	if (psVM)
		ExFreePool(psVM);

	KdPrint(("DriverUnload SUCCESSFUL\n"));
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING RegisterPath)
{
	pDriverObject->DriverUnload = DriverUnload;

	/*检查与开启多核 VT*/
	if (!CheckVMX())
	{
		KdPrint(("Error:CheckVMX\n"));
		return STATUS_UNSUCCESSFUL;
	}

	/*申请VM所有核的内存*/
	psVM = ExAllocatePool2(POOL_FLAG_NON_PAGED_EXECUTE, sizeof(VM) * KeNumberProcessors, 'PSVM');
	if (psVM)
	{
		RtlZeroMemory(psVM, sizeof(VM) * KeNumberProcessors);
	}
	else
	{
		KdPrint(("Error:psVM\n"));
		return STATUS_UNSUCCESSFUL;
	}

	//发送IPI,启动VT
	if (!KeIpiGenericCall(KipiSetupVT, 0))
	{
		KdPrint(("Error:KipiSetupVT\n"));
		return STATUS_UNSUCCESSFUL;
	}

	KdPrint(("DriverEntry SUCCESSFUL\n"));

	return STATUS_SUCCESS;
}



