#include "Driver.h"

ULONG64 CheckCPUID()
{
	int ctx[4] = { 0 };//顺序：eax,ebx,ecx,edx
	__cpuid(ctx, 1);

	return ctx[2] & CPUID_ECX_VMX_ABILITY;	//Bit-5
}

ULONG64 CheckMsr()
{
	MSR_IA32_FEATURE_CONTROL_REGISTER msr = { 0 };
	msr.value.QuadPart = __readmsr(MSR_IA32_FEATURE_CONTROL);

	return msr.Bits.Lock;
}

ULONG64 FixCr0()
{
	IA32_CR0 uCr0 = { 0 };
	uCr0.value.QuadPart = __readcr0();

	uCr0.value.QuadPart |= __readmsr(MSR_IA32_VMX_CR0_FIXED0);
	uCr0.value.QuadPart &= __readmsr(MSR_IA32_VMX_CR0_FIXED1);

	__writecr0(uCr0.value.QuadPart);
	return TRUE;
}

ULONG64 FixCr4()
{
	IA32_CR4 uCr4 = { 0 };
	uCr4.value.QuadPart = __readcr4();

	uCr4.value.QuadPart |= __readmsr(MSR_IA32_VMX_CR4_FIXED0);
	uCr4.value.QuadPart &= __readmsr(MSR_IA32_VMX_CR4_FIXED1);
	uCr4.Bits.VMXE = 1;

	__writecr4(uCr4.value.QuadPart);
	return TRUE;
}

ULONG64 CheckVMX()
{
	if (!CheckCPUID())
	{
		KdPrint(("CPU不支持\n"));
		return FALSE;
	}
	if (!CheckMsr())
	{
		KdPrint(("BIOS未开启VMX\n"));
		return FALSE;
	}
	return TRUE;
}

ULONG64 OpenVMX()
{
	FixCr0();
	FixCr4();
	return TRUE;
}

ULONG64 CloseVMX()
{
	IA32_CR4 uCr4 = { 0 };
	uCr4.value.QuadPart = __readcr4();
	uCr4.Bits.VMXE = FALSE;
	__writecr4(uCr4.value.QuadPart);
	return TRUE;
}

ULONG64 GetSegInfo(PSEG seg, USHORT selector, ULONG64 gdtBase)
{
	PSELECTOR pSel = (PSELECTOR)&selector;
	ULONG tempBase = 0;
	if (!seg)
		return FALSE;
	if (!gdtBase)
		return FALSE;
	if (!pSel->Bits.Index)
		return FALSE;

	/*这一顿操作就是把GDT表的项，分解，组合，因为是历史遗留的原因，表项数据七零八落*/
	if (pSel->Bits.TI == 0)
	{//GDT
		seg->sel.value = selector;
		seg->descriptor = (PSEG_DESCRIPTOR)(gdtBase + (selector & ~0x7));

		tempBase = seg->descriptor->base0 | (seg->descriptor->base1 << 16) | (seg->descriptor->base2 << 24);
		if (!seg->descriptor->attr.Bits.S)
		{
			seg->Base = seg->descriptor->base3;
			seg->Base = seg->Base << 32;
		}
		seg->Base |= tempBase;

		seg->Limit = seg->descriptor->attr.Bits.limit1;
		seg->Limit = seg->Limit << 16;
		seg->Limit |= seg->descriptor->limit0;
		if (seg->descriptor->attr.Bits.G & seg->descriptor->attr.Bits.DB)
		{
			seg->Limit = (seg->Limit << 12) | 0xFFF;
		}
		else
		{
			seg->Limit &= 0xFFFF;
		}
	}
	else
	{//LDT
		DbgBreakPoint();
		seg->sel.value = selector;
		seg->descriptor = (PSEG_DESCRIPTOR)(gdtBase + (selector & ~0x7));
		seg->Base = seg->descriptor->base3;
		seg->Base = seg->Base << 32;
		tempBase = seg->descriptor->base0 | (seg->descriptor->base1 << 16) | (seg->descriptor->base2 << 24);
		seg->Base |= tempBase;

		seg->Limit = seg->descriptor->attr.Bits.limit1;
		seg->Limit = seg->Limit << 16;
		seg->Limit |= seg->descriptor->limit0;
	}

	return TRUE;
}

ULONG64 SetGuestSeg(ULONG64 seg, USHORT selector, ULONG64 gdtBase)
{
	SEG TmpSeg = { 0 };
	ULONG64 ret = 0;
	GetSegInfo(&TmpSeg, selector, gdtBase);

	ret += vmx_write(GUEST_ES_SELECTOR + (seg * 2), selector & 0xFFF8);
	ret += vmx_write(GUEST_ES_BASE + (seg * 2), TmpSeg.Base);
	ret += vmx_write(GUEST_ES_LIMIT + (seg * 2), TmpSeg.Limit);
	ret += vmx_write(GUEST_ES_ACCESS_RIGHTS + (seg * 2),
		TmpSeg.descriptor ? (TmpSeg.descriptor->attr.value) : 0x10000);//(&0xF0FF)?

	return ret;
}

ULONG64 SetControls(ULONG64* value, LARGE_INTEGER msr)
{
	ULONG64 check = *value;
	if ((check & msr.HighPart) == check)//该位能被设置成1
	{
		check |= msr.LowPart;			//和必须设置成1值的位按位或
		*value = check;
		return TRUE;
	}
	return FALSE;
}

ULONG64 SetVmxExitCTLS(UCHAR CpuIndex, ULONG64 vmxctrl)
{
	ULONG64 ret = 0;

	MSR_IA32_VMX_EXIT_CTLS_REGISTER VmExit = { 0 }, SetExit = { 0 };
	VmExit.value.QuadPart = vmxctrl ? __readmsr(MSR_IA32_VMX_TRUE_EXIT_CTLS) : __readmsr(MSR_IA32_VMX_EXIT_CTLS);
	/*在这里设置《VmExit》*/
	SetExit.Bits->HOST_addr_size = TRUE;
	
	if (!SetControls(&SetExit.value.QuadPart, VmExit.value))
	{
		KdPrint(("MSR_IA32_VMX_EXIT_CTLS_REGISTER 设置错误\n"));
		return 0xC1;
	}
	ret += vmx_write(PRIMARY_VM_EXIT_CONTROLS, SetExit.value.QuadPart);

	return ret;
}

ULONG64 SetVmxEntryCTLS(UCHAR CpuIndex, ULONG64 vmxctrl)
{
	ULONG64 ret = 0;

	MSR_IA32_VMX_ENTRY_CTLS_REGISTER VmEntry = { 0 }, SetEntry = { 0 };
	VmEntry.value.QuadPart = vmxctrl ? __readmsr(MSR_IA32_VMX_TRUE_ENTRY_CTLS) : __readmsr(MSR_IA32_VMX_ENTRY_CTLS);
	/*在这里设置《VmEntry》*/
	SetEntry.Bits->Guest_IA32e = TRUE;

	if(!SetControls(&SetEntry.value.QuadPart, VmEntry.value))
	{
		KdPrint(("MSR_IA32_VMX_ENTRY_CTLS_REGISTER 设置错误\n"));
		return 0xC2;
	}
	ret += vmx_write(VM_ENTRY_CONTROLS, SetEntry.value.QuadPart);

	return ret;
}

ULONG64 SetVmxExecutionCTLS(UCHAR CpuIndex, ULONG64 vmxctrl)
{
	UINT64 ret = 0;
	ULONG64 value = 0;

	MSR_IA32_VMX_PINBASED_CTLS_REGISTER PinBased = { 0 }, SetPinBase = { 0 };
	MSR_IA32_VMX_PROCBASED_CTLS_REGISTER Primary = { 0 }, SetPrimary = { 0 };
	MSR_IA32_VMX_PROCBASED_CTLS2_REGISTER Secondry = { 0 }, SetSecondry = { 0 };
	MSR_IA32_VMX_PROCBASED_CTLS3_REGISTER Tertiary = { 0 };

	PinBased.value.QuadPart = vmxctrl ? __readmsr(MSR_IA32_VMX_TRUE_PINBASED_CTLS) : __readmsr(MSR_IA32_VMX_PINBASED_CTLS);
	/*在这里设置《PinBased》*/
	//SetPinBase.Bits->ExIDT = TRUE;

	if(!SetControls(&SetPinBase.value.QuadPart, PinBased.value)) 
	{
		KdPrint(("MSR_IA32_VMX_PINBASED_CTLS_REGISTER 设置错误\n"));
		return 0xC3;
	}
	ret += vmx_write(PIN_BASED_VM_EXECUTION_CONTROLS, SetPinBase.value.QuadPart);

	Primary.value.QuadPart = vmxctrl ? __readmsr(MSR_IA32_VMX_TRUE_PROCBASED_CTLS) : __readmsr(MSR_IA32_VMX_PROCBASED_CTLS);
	/*在这里设置《Primary》*/
	//SetPrimary.Bits->ExIDT = TRUE;
	
	SetPrimary.Bits->MSRmap = TRUE;//如果为 FALSE，代表拦截所有MSR读写
	SetPrimary.Bits->RDTSC = TRUE;
	SetPrimary.Bits->Secondary = TRUE;
	
	if(!SetControls(&SetPrimary.value.QuadPart, Primary.value))
	{
		KdPrint(("MSR_IA32_VMX_PROCBASED_CTLS_REGISTER 设置错误\n"));
		return 0xC4;
	}
	ret += vmx_write(PRIMARY_PROCESSOR_BASED_VM_EXECUTION_CONTROLS, SetPrimary.value.QuadPart);

	if (SetPrimary.Bits->Secondary)
	{
		Secondry.value.QuadPart = __readmsr(MSR_IA32_VMX_PROCBASED_CTLS2);

		/*在这里设置《Secondry》*/
		SetSecondry.Bits->XSAVES_XRSTORS = TRUE;
		SetSecondry.Bits->INVPCID = TRUE;
		SetSecondry.Bits->RDTSCP = TRUE;

		if(!SetControls(&SetSecondry.value.QuadPart, Secondry.value))
		{
			KdPrint(("MSR_IA32_VMX_PROCBASED_CTLS2_REGISTER 设置错误\n"));
			return 0xC5;
		}

		ret += vmx_write(SECONDARY_PROCESSOR_BASED_VM_EXECUTION_CONTROLS, SetSecondry.value.QuadPart);
	}
	return ret;
}

ULONG64 SetVmxExtend(UCHAR CpuIndex)
{
	ULONG64 ret = 0;

	psVM[CpuIndex].ExecptBitMap = 0;
	ret += vmx_write(EXCEPTION_BITMAP, psVM[CpuIndex].ExecptBitMap);

	ret += vmx_write(VMCS_LINK_POINTER_FULL, ~0Ull);
	ret += vmx_write(VIRTUAL_PROCESSOR_IDENTIFIER, CpuIndex);

	ret += vmx_write(VM_ENTRY_MSR_LOAD_ADDRESS_FULL, psVM[CpuIndex].pMsrMemPa.QuadPart);
	ret += vmx_write(ADDRESS_OF_MSR_BITMAPS_FULL, psVM[CpuIndex].pMsrMapPa.QuadPart);

	return ret;
}

ULONG64 SetHostState(ULONG64 HostRip, ULONG64 HostRsp)
{
	SEG seg = { 0 };
	ULONG64 ret = 0;
	ULONG64 GdtBase = _readBaseGDT();
	ULONG64 IdtBase = _readBaseIDT();
	ULONG64 TR = GetSegInfo(&seg, _readTR(), GdtBase);

	ret += vmx_write(HOST_CR0, __readcr0());
	ret += vmx_write(HOST_CR3, __readcr3());
	ret += vmx_write(HOST_CR4, __readcr4());

	ret += vmx_write(HOST_RIP, HostRip);
	ret += vmx_write(HOST_RSP, HostRsp);

	ret += vmx_write(HOST_CS_SELECTOR, _readCS() & 0xF8);
	ret += vmx_write(HOST_SS_SELECTOR, _readSS() & 0xF8);
	ret += vmx_write(HOST_DS_SELECTOR, _readDS() & 0xF8);
	ret += vmx_write(HOST_ES_SELECTOR, _readES() & 0xF8);
	ret += vmx_write(HOST_FS_SELECTOR, _readFS() & 0xF8);
	ret += vmx_write(HOST_GS_SELECTOR, _readGS() & 0xF8);
	ret += vmx_write(HOST_TR_SELECTOR, _readTR() & 0xF8);

	ret += vmx_write(HOST_FS_BASE, __readmsr(MSR_IA32_FS_BASE));
	ret += vmx_write(HOST_GS_BASE, __readmsr(MSR_IA32_GS_BASE));
	ret += vmx_write(HOST_TR_BASE, TR ? seg.Base : 0);
	ret += vmx_write(HOST_GDTR_BASE, GdtBase);
	ret += vmx_write(HOST_IDTR_BASE, IdtBase);

	ret += vmx_write(HOST_IA32_SYSENTER_CS, __readmsr(MSR_IA32_SYSENTER_CS));
	ret += vmx_write(HOST_IA32_SYSENTER_ESP, __readmsr(MSR_IA32_SYSENTER_ESP));
	ret += vmx_write(HOST_IA32_SYSENTER_EIP, __readmsr(MSR_IA32_SYSENTER_EIP));

	return ret;
}

ULONG64 SetGuestState(ULONG64 GuestRip, ULONG64 GuestRsp)
{
	ULONG64 ret = 0;

	ULONG64 GdtBase = _readBaseGDT();
	ULONG64 IdtBase = _readBaseIDT();

	ret += vmx_write(GUEST_CR0, __readcr0());
	ret += vmx_write(GUEST_CR3, __readcr3());
	ret += vmx_write(GUEST_CR4, __readcr4());
	ret += vmx_write(GUEST_DR7, __readdr(7));

	ret += SetGuestSeg(ES, _readES(), GdtBase);
	ret += SetGuestSeg(FS, _readFS(), GdtBase);
	ret += SetGuestSeg(DS, _readDS(), GdtBase);
	ret += SetGuestSeg(CS, _readCS(), GdtBase);
	ret += SetGuestSeg(SS, _readSS(), GdtBase);
	ret += SetGuestSeg(GS, _readGS(), GdtBase);
	ret += SetGuestSeg(TR, _readTR(), GdtBase);
	ret += SetGuestSeg(LDTR, _readLDTR(), GdtBase);

	ret += vmx_write(GUEST_FS_BASE, __readmsr(MSR_IA32_FS_BASE));
	ret += vmx_write(GUEST_GS_BASE, __readmsr(MSR_IA32_GS_BASE));
	ret += vmx_write(GUEST_GDTR_BASE, GdtBase);
	ret += vmx_write(GUEST_GDTR_LIMIT, _readLimitGDT());
	ret += vmx_write(GUEST_IDTR_BASE, IdtBase);
	ret += vmx_write(GUEST_IDTR_LIMIT, _readLimitIDT());

	ret += vmx_write(GUEST_RFLAGS, __readeflags());
	ret += vmx_write(GUEST_RIP, GuestRip);
	ret += vmx_write(GUEST_RSP, GuestRsp);

	ret += vmx_write(GUEST_IA32_SYSENTER_CS, __readmsr(MSR_IA32_SYSENTER_CS));
	ret += vmx_write(GUEST_IA32_SYSENTER_ESP, __readmsr(MSR_IA32_SYSENTER_ESP));
	ret += vmx_write(GUEST_IA32_SYSENTER_EIP, __readmsr(MSR_IA32_SYSENTER_EIP));

	return ret;
}

ULONG64 InitVmxON(UCHAR CpuIndex, ULONG RevisonId)
{
	*((PULONG)psVM[CpuIndex].pOnVa) = RevisonId;

	ULONG64 ret = vmx_on(&psVM[CpuIndex].pOnPa.QuadPart);
	if (ret)
	{
		KdPrint(("vmx_on 失败！\n"));
		return FALSE;
	}

	return TRUE;
}

ULONG64 InitVmxCS(UCHAR CpuIndex, ULONG RevisonId)
{
	*((PULONG)psVM[CpuIndex].pCsVa) = RevisonId;

	if (vmx_clear(&psVM[CpuIndex].pCsPa.QuadPart))
		return FALSE;
	if (vmx_ptrld(&psVM[CpuIndex].pCsPa.QuadPart))
		return FALSE;

	return TRUE;
}

ULONG64 SetVMXCS(UCHAR CpuIndex, ULONG64 GuestRip, ULONG64 GuestRsp, ULONG64 HostRip, ULONG64 HostRsp, ULONG64 vmxctrl)
{
	if (SetVmxExecutionCTLS(CpuIndex, vmxctrl))
		return FALSE;
	if (SetVmxEntryCTLS(CpuIndex, vmxctrl))
		return FALSE;
	if (SetVmxExitCTLS(CpuIndex, vmxctrl))
		return FALSE;
	if (SetVmxExtend(CpuIndex))
		return FALSE;

	if (SetHostState(HostRip, HostRsp))
		return FALSE;
	if (SetGuestState(GuestRip, GuestRsp))
		return FALSE;

	return TRUE;
}

ULONG64 StopVM()
{
	vmx_off();
	return TRUE;
}

ULONG64 StartVM(UCHAR CpuIndex, ULONG64 GuestRip, ULONG64 GuestRsp)
{
	ULONG64 ret = 0;
	MSR_IA32_VMX_BASIC_REGISTER VmxBasic = { 0 };
	VmxBasic.value.QuadPart = __readmsr(MSR_IA32_VMX_BASIC);

	if (!InitVmxON(CpuIndex, VmxBasic.Bits.VmcsRevisonId)) {
		KdPrint(("Init VMX ON fail...\n"));
		DbgBreakPoint();
		return FALSE;
	}
	if (!InitVmxCS(CpuIndex, VmxBasic.Bits.VmcsRevisonId)) {
		KdPrint(("Init VMX CS fail...\n"));
		DbgBreakPoint();
		return FALSE;
	}
	ULONG64 HostRip = (ULONG64)_HostEntry;
	ULONG64 HostRsp = (ULONG64)psVM[CpuIndex].pStack + STACK_SIZE;
	if (!SetVMXCS(CpuIndex, GuestRip, GuestRsp, HostRip, HostRsp, VmxBasic.Bits.VmxControls)) {
		KdPrint(("Set VMX CS fail...\n"));
		DbgBreakPoint();
		return FALSE;
	}
	
	char error = vmx_launch();
	//下面得到执行代表出错	
	KdPrint(("vmx_launch error code:[%d]\n", error));
	DbgBreakPoint();
	return FALSE;
}

ULONG64 HostEntry(PGUESTREG pGuestRegs)
{
	vmx_read(VIRTUAL_PROCESSOR_IDENTIFIER, &pGuestRegs->CpuIndex);
	vmx_read(EXIT_REASON, &pGuestRegs->ExitReason);

	vmx_read(GUEST_RIP, &pGuestRegs->GuestRip);
	vmx_read(GUEST_RSP, &pGuestRegs->GuestRsp);
	vmx_read(GUEST_RFLAGS, &pGuestRegs->GuestFlag);
	vmx_read(VM_EXIT_INSTRUCTION_LENGTH, &pGuestRegs->InstrLen);

	/*此处可升级成get pcr struct,需要修改 _HostEntry 的汇编代码和 PGUESTREG 的结构
	判断是哪种系统的 pcr 从而采用不同结构的 pcr*/
	pGuestRegs->EProcess = PsGetCurrentProcess();
	pGuestRegs->EThread = PsGetCurrentThread();

	return DispatchHandler(pGuestRegs);
}
