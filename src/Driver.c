#include "Driver.h"

ULONG64 CreateVM(ULONG CpuIndex)
{
	vt->vm[CpuIndex].VmxON.vir = ExAllocatePool2(POOL_FLAG_NON_PAGED, PAGE_SIZE, 'VMON');
	vt->vm[CpuIndex].VmxON.phy = (PVOID)MmGetPhysicalAddress(vt->vm[CpuIndex].VmxON.vir).QuadPart;

	vt->vm[CpuIndex].VmxCS.vir = ExAllocatePool2(POOL_FLAG_NON_PAGED, PAGE_SIZE, 'VMCS');
	vt->vm[CpuIndex].VmxCS.phy = (PVOID)MmGetPhysicalAddress(vt->vm[CpuIndex].VmxCS.vir).QuadPart;

	vt->vm[CpuIndex].MsrBitMap.vir = ExAllocatePool2(POOL_FLAG_NON_PAGED, PAGE_SIZE * 2, 'MBMP');
	vt->vm[CpuIndex].MsrBitMap.phy = (PVOID)MmGetPhysicalAddress(vt->vm[CpuIndex].MsrBitMap.vir).QuadPart;

	vt->vm[CpuIndex].VmxStack = ExAllocatePool2(POOL_FLAG_NON_PAGED, STACK_SIZE, 'STCK');

	return TRUE;
}

ULONG64 DeleteVM(ULONG CpuIndex)
{
	if (vt->vm[CpuIndex].VmxON.vir) { ExFreePool(vt->vm[CpuIndex].VmxON.vir); }
	if (vt->vm[CpuIndex].VmxCS.vir) { ExFreePool(vt->vm[CpuIndex].VmxCS.vir); }
	if (vt->vm[CpuIndex].MsrBitMap.vir) { ExFreePool(vt->vm[CpuIndex].MsrBitMap.vir); }
	if (vt->vm[CpuIndex].VmxStack) { ExFreePool(vt->vm[CpuIndex].VmxStack); }

	return TRUE;
}

ULONG_PTR KipiSetupVT(ULONG_PTR Arg)
{
	ULONG CpuIndex = KeGetCurrentProcessorNumber();
	OpenVMX();
	CreateVM(CpuIndex);

	if (CallBackStartVM(StartVM, CpuIndex))
	{
		KdPrint(("CPU:[%d],开启VMX\n", CpuIndex));
		return TRUE;
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
	else
	{
		if (vt->vm)
			ExFreePool(vt->vm);

		KdPrint(("DriverUnload SUCCESSFUL\n"));
	}
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING RegisterPath)
{
	pDriverObject->DriverUnload = DriverUnload;
	KdPrint(("DriverEntry\n"));
	/*检查与开启多核 VT*/
	if (!CheckVMX())
	{
		KdPrint(("Error:CheckVMX\n"));
		return STATUS_UNSUCCESSFUL;
	}

	vt = ExAllocatePool2(POOL_FLAG_NON_PAGED_EXECUTE, sizeof(VT), 'PVT');
	if (!vt)
		return STATUS_UNSUCCESSFUL;

	/*申请VM所有核的内存*/
	vt->vm = ExAllocatePool2(POOL_FLAG_NON_PAGED_EXECUTE, sizeof(VM) * KeNumberProcessors, 'PVM');
	if (!vt->vm)
		return STATUS_UNSUCCESSFUL;

	//初始化VMEXIT处理函数
	if (!InitHandlerVmExit())
		return STATUS_UNSUCCESSFUL;

	//发送IPI,启动VT
	if (!KeIpiGenericCall(KipiSetupVT, 0))
	{
		KdPrint(("Error:KipiSetupVT\n"));
		return STATUS_UNSUCCESSFUL;
	}

	KdPrint(("DriverEntry SUCCESSFUL\n"));

	return STATUS_SUCCESS;
}



