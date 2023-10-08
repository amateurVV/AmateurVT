#include "Driver.h"


ULONG64 HandlerException(PGUESTREG pGuestRegs)
{
	EXIT_INTR_INFO Exit = { 0 };
	vmx_read(VM_EXIT_INTERRUPTION_INFORMATION, &Exit.value);
	ULONG64 ExitQualification = 0;
	vmx_read(EXIT_QUALIFICATION, &ExitQualification);


	switch (Exit.Bits.Vector)
	{
	default:

		break;
	}

	return TRUE;
}

ULONG64 HandlerVMCALL(PGUESTREG pGuestRegs)
{
	return TRUE;
}


VOID HandlerCPUID(PGUESTREG pGuestRegs)
{

	switch (pGuestRegs->rax)
	{
	case 0x5555:
	{		
		pGuestRegs->rax = 0x11111111;
		pGuestRegs->rcx = 0x33333333;
		pGuestRegs->rdx = 0x44444444;
		pGuestRegs->rbx = 0x22222222;
		break;
	}
	default:
		_cpuid(pGuestRegs);
		return;
	}

	return;
}

VOID HandlerRDMSR(PGUESTREG pGuestRegs)
{
	_readmsr(pGuestRegs);
}

VOID HandlerWRMSR(PGUESTREG pGuestRegs)
{
	_writemsr(pGuestRegs);
}

VOID HandlerCRX(PGUESTREG pGuestRegs)
{
	EXIT_QUA_CR_ACCESS Exit = { 0 };
	vmx_read(EXIT_QUALIFICATION, &Exit.value);

	switch (Exit.Bits.RegNum)
	{
	case 0:
	{
		if (Exit.Bits.AccessType)
			vmx_read(GUEST_CR0, ((PULONG64)pGuestRegs + Exit.Bits.MovToCr));
		else
			vmx_write(GUEST_CR0, *((PULONG64)pGuestRegs + Exit.Bits.MovToCr));
		break;
	}
	case 3:
	{
		if (Exit.Bits.AccessType)
			vmx_read(GUEST_CR3, ((PULONG64)pGuestRegs + Exit.Bits.MovToCr));
		else
			vmx_write(GUEST_CR3, *((PULONG64)pGuestRegs + Exit.Bits.MovToCr));
		break;
	}
	case 4:
	{
		if (Exit.Bits.AccessType)
			vmx_read(GUEST_CR4, ((PULONG64)pGuestRegs + Exit.Bits.MovToCr));
		else
			vmx_write(GUEST_CR4, *((PULONG64)pGuestRegs + Exit.Bits.MovToCr));
		break;
	}
	default:
		break;
	}
}

ULONG64 DispatchHandler(PGUESTREG pGuestRegs)
{
	EXIT_REASON_FIELDS* Exit = (EXIT_REASON_FIELDS*)&pGuestRegs->ExitReason;
	ULONG64 CpuIndex = pGuestRegs->CpuIndex;
	ULONG64 GuestRip = pGuestRegs->GuestRip;
	ULONG64 GuestRsp = pGuestRegs->GuestRsp;
	ULONG64 InstrLen = pGuestRegs->InstrLen;

	switch (Exit->Bits.Basic)
	{
	case EXIT_REASON_EXCEPTION_NMI: {
		HandlerException(pGuestRegs);
		return TRUE;
	}
	case EXIT_REASON_CPUID: {
		HandlerCPUID(pGuestRegs);
		pGuestRegs->GuestRip += InstrLen;
		vmx_write(GUEST_RIP, pGuestRegs->GuestRip);
		break;
	}
	case EXIT_REASON_RDMSR: {

		HandlerRDMSR(pGuestRegs);
		pGuestRegs->GuestRip += InstrLen;
		vmx_write(GUEST_RIP, pGuestRegs->GuestRip);
		break;
	}
	case EXIT_REASON_WRMSR: {
		HandlerWRMSR(pGuestRegs);
		pGuestRegs->GuestRip += InstrLen;
		vmx_write(GUEST_RIP, pGuestRegs->GuestRip);
		break;
	}
	case EXIT_REASON_RDTSC: {
		_rdtsc(pGuestRegs);
		pGuestRegs->GuestRip += InstrLen;
		vmx_write(GUEST_RIP, pGuestRegs->GuestRip);
		break;
	}
	case EXIT_REASON_RDTSCP: {
		_rdtscp(pGuestRegs);
		pGuestRegs->GuestRip += InstrLen;
		vmx_write(GUEST_RIP, pGuestRegs->GuestRip);
		break;
	}
	/*case EXIT_REASON_VMPTRLD:
	{
	   pGuestRegs->rax = vmx_ptrld(pGuestRegs->rcx);
	   pGuestRegs->GuestRip += InstrLen;
	   vmx_write(GUEST_RIP, pGuestRegs->GuestRip);
	   break;
	}
	case EXIT_REASON_VMCLEAR:
	{
	   pGuestRegs->rax = vmx_clear(pGuestRegs->rcx);
	   pGuestRegs->GuestRip += InstrLen;
	   vmx_write(GUEST_RIP, pGuestRegs->GuestRip);
	   break;
	}
	case EXIT_REASON_VMXON:
	{
	   pGuestRegs->rax = vmx_on(pGuestRegs->rcx);
	   pGuestRegs->GuestRip += InstrLen;
	   vmx_write(GUEST_RIP, pGuestRegs->GuestRip);
	   break;
	}*/
	case EXIT_REASON_VMXOFF: {
		IA32_RFLAGS GuestFlags = { 0 };
		ULONG64 cr3 = 0;
		vmx_read(GUEST_CR3, &cr3);
		vmx_read(GUEST_RFLAGS, &GuestFlags.Value);
		vmx_clear(&psVM[CpuIndex].pCsPa.QuadPart);

		vmx_off();
		__writecr3(cr3);

		pGuestRegs->rcx = GuestRsp;
		pGuestRegs->rax = GuestRip + InstrLen;
		pGuestRegs->rflags = GuestFlags.Value;

		return ExitVMX;
	}
	case EXIT_REASON_CR_ACCESS: {
		HandlerCRX(pGuestRegs);
		pGuestRegs->GuestRip += InstrLen;
		vmx_write(GUEST_RIP, pGuestRegs->GuestRip);
		break;
	}
	//case EXIT_REASON_MTF_TRAP_FLAG:
	//{
	//	
	//	DbgBreakPoint();
	//	//InjectMTF();
	//	//DebugPrintDisAsm(pGuestRegs);
	//	vmx_write(GUEST_RFLAGS, pGuestRegs->GuestFlag & ~0x10000);
	//	//InjectExceptionInt1();
	//	break;
	//}
	//case EXIT_REASON_INVD:
	//{
	//	_invd();
	//	vmx_write(GUEST_RIP, GuestRip + InstrLen);
	//	break;
	//}
	//case EXIT_REASON_WBINVD:
	//{
	//	__wbinvd();
	//	vmx_write(GUEST_RIP, GuestRip + InstrLen);
	//	break;
	//}
	//case EXIT_REASON_XSETBV:
	//{
	//	asm_xsetbv(pGuestRegs);
	//	pGuestRegs->GuestRip += InstrLen;
	//	vmx_write(GUEST_RIP, pGuestRegs->GuestRip);
	//	break;
	//}
	default:

		DbgBreakPoint();
		break;
	}
	return TRUE;
}
