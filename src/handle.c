#include "Driver.h"


ULONG64 HandlerEXCPTION(PGUESTREG pGuestRegs)
{
	EXIT_INTR_INFO Exit = { 0 };
	vmx_read(VM_EXIT_INTERRUPTION_INFORMATION, &Exit.value);
	ULONG64 ExitQualification = 0;
	vmx_read(VM_EXIT_QUALIFICATION, &ExitQualification);


	switch (Exit.Bits.Vector)
	{
	default:

		break;
	}

	return TRUE;
}

ULONG64 HandlerVMCALL(PGUESTREG GuestRegs)
{
	GuestRegs->GuestRip += GuestRegs->InstrLen;
	vmx_write(GUEST_RIP, GuestRegs->GuestRip);
	return TRUE;
}

ULONG64 HandlerVMOFF(PGUESTREG GuestRegs)
{
	ULONG64 eflags;
	ULONG64 cr3 = 0;
	vmx_read(GUEST_CR3, &cr3);
	vmx_read(GUEST_RFLAGS, &eflags);
	vmx_clear(&vt->vm[GuestRegs->CpuIndex].VmxCS.phy);

	vmx_off();
	__writecr3(cr3);

	GuestRegs->rcx = GuestRegs->GuestRsp;
	GuestRegs->rax = GuestRegs->GuestRip + GuestRegs->InstrLen;
	GuestRegs->rflags = eflags;

	return ExitVMX;
}

ULONG64 HandlerCPUID(PGUESTREG GuestRegs)
{

	switch (GuestRegs->rax)
	{
	case 0x5555:
	{
		GuestRegs->rax = 0x11111111;
		GuestRegs->rcx = 0x33333333;
		GuestRegs->rdx = 0x44444444;
		GuestRegs->rbx = 0x22222222;
		break;
	}
	default:
		vmx_cpuid(GuestRegs);
		break;
	}
	GuestRegs->GuestRip += GuestRegs->InstrLen;
	vmx_write(GUEST_RIP, GuestRegs->GuestRip);
	return TRUE;
}

ULONG64 HandlerRDMSR(PGUESTREG GuestRegs)
{
	vmx_rdmsr(GuestRegs);
	GuestRegs->GuestRip += GuestRegs->InstrLen;
	vmx_write(GUEST_RIP, GuestRegs->GuestRip);
	return TRUE;
}

ULONG64 HandlerWRMSR(PGUESTREG GuestRegs)
{
	vmx_wrmsr(GuestRegs);
	GuestRegs->GuestRip += GuestRegs->InstrLen;
	vmx_write(GUEST_RIP, GuestRegs->GuestRip);
	return TRUE;
}

ULONG64 HandlerCRX(PGUESTREG GuestRegs)
{
	enum ACCESSTYPE
	{
		MOV_TO_CR,
		MOV_FROM_CR,
		CLTS,
		LMSW
	};

	long CR[] = { GUEST_CR0, 1, 2, GUEST_CR3, GUEST_CR4 };

	EXIT_QUALIFICATION_CR_ACCESS Exit = { 0 };
	vmx_read(VM_EXIT_QUALIFICATION, &Exit.value);	

	if (Exit.Bits.AccessType == MOV_TO_CR)
		vmx_write(CR[Exit.Bits.NumCR], *((ULONG64*)GuestRegs + Exit.Bits.NumReg));
	else if (Exit.Bits.AccessType == MOV_FROM_CR)
		vmx_read(CR[Exit.Bits.NumCR], ((ULONG64*)GuestRegs + Exit.Bits.NumReg));
	else if (Exit.Bits.AccessType == CLTS)
		KdPrint(("HandlerCRX CLTS"));
	else if (Exit.Bits.AccessType == LMSW)
		KdPrint(("HandlerCRX LMSW"));

	GuestRegs->GuestRip += GuestRegs->InstrLen;
	vmx_write(GUEST_RIP, GuestRegs->GuestRip);
	return TRUE;
}

ULONG64 HandlerMTF(PGUESTREG GuestRegs)
{
	vmx_write(GUEST_RFLAGS, GuestRegs->GuestFlag & ~0x10000);
	return TRUE;
}

ULONG64 HandlerRDTSC(PGUESTREG GuestRegs)
{
	vmx_rdtsc(GuestRegs);
	GuestRegs->GuestRip += GuestRegs->InstrLen;
	vmx_write(GUEST_RIP, GuestRegs->GuestRip);
	return TRUE;
}

ULONG64 HandlerRDTSCP(PGUESTREG GuestRegs)
{
	vmx_rdtscp(GuestRegs);
	GuestRegs->GuestRip += GuestRegs->InstrLen;
	vmx_write(GUEST_RIP, GuestRegs->GuestRip);
	return TRUE;
}

ULONG64 ExitNoHandler(PGUESTREG GuestRegs)
{
	DbgBreakPoint();
	
	return TRUE;
}

ULONG64 InitHandlerVmExit()
{
	HandlerExit = ExAllocatePool2(POOL_FLAG_NON_PAGED, sizeof(PVOID) * EXIT_MAX, 'Exit');
	if (!HandlerExit)
		return FALSE;

	for (size_t i = 0; i < EXIT_MAX; i++)
	{
		HandlerExit[i] = ExitNoHandler;//暂时先将所有处理函数设置成不处理
	}
	HandlerExit[EXIT_REASON_CPUID] = HandlerCPUID;

	HandlerExit[EXIT_REASON_EXCEPTION_NMI] = HandlerEXCPTION;
	HandlerExit[EXIT_REASON_CR_ACCESS] = HandlerCRX;

	HandlerExit[EXIT_REASON_RDMSR] = HandlerRDMSR;
	HandlerExit[EXIT_REASON_WRMSR] = HandlerWRMSR;

	HandlerExit[EXIT_REASON_RDTSC] = HandlerRDTSC;
	HandlerExit[EXIT_REASON_RDTSCP] = HandlerRDTSCP;

	HandlerExit[EXIT_REASON_VMCALL] = HandlerVMCALL;

	HandlerExit[EXIT_REASON_VMXOFF] = HandlerVMOFF;

	HandlerExit[EXIT_REASON_MTF_TRAP_FLAG] = HandlerMTF;

	return TRUE;

}