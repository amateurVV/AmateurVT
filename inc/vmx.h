#pragma once

#pragma pack(1)
typedef struct _VMCS_REGION {
	ULONG revision_identifier : 31;		//bit-0:30
	ULONG shadow : 1;					//bit-31
	ULONG vmx_abort_indicator : 32;		//bit-32:63
	ULONG dataStart;					//all_VMCS_Fields
}VMCS_REGION, * PVMCS_REGION;

typedef struct _GUESTREG {
	ULONG64 rax;
	ULONG64 rcx;
	ULONG64 rdx;
	ULONG64 rbx;
	ULONG64 rbp;
	ULONG64 rsi;
	ULONG64 rdi;
	ULONG64 r8;
	ULONG64 r9;
	ULONG64 r10;
	ULONG64 r11;
	ULONG64 r12;
	ULONG64 r13;
	ULONG64 r14;
	ULONG64 r15;
	ULONG64 rflags;

	PVOID EProcess;
	PVOID EThread;
	ULONG64 ExitReason;
	ULONG64 GuestRip;
	ULONG64 GuestRsp;
	ULONG64 GuestFlag;
	ULONG64 CpuIndex;
	ULONG64 InstrLen;
}GUESTREG, * PGUESTREG;


#pragma pack()

enum VMCS_Fields {
	/*16-Bit Control Fields*/
	VIRTUAL_PROCESSOR_IDENTIFIER = 0x0,												//16-Bit Control Fields
	POSTED_INTERRUPT_NOTIFICATION_VECTOR = 0x2,										//16-Bit Control Fields
	EPTP_INDEX = 0x4,																//16-Bit Control Fields
	HLAT_PREFIX_SIZE = 0x6,															//16-Bit Control Fields
	/*32-Bit Control Fields*/
	PIN_BASED_VM_EXECUTION_CONTROLS = 0x4000,										//32-Bit Control Fields
	PRIMARY_PROCESSOR_BASED_VM_EXECUTION_CONTROLS = 0x4002,							//32-Bit Control Fields
	EXCEPTION_BITMAP = 0x4004,														//32-Bit Control Fields
	PAGE_FAULT_ERROR_CODE_MASK = 0x4006,											//32-Bit Control Fields
	PAGE_FAULT_ERROR_CODE_MATCH = 0x4008,											//32-Bit Control Fields
	CR3_TARGET_COUNT = 0x400A,														//32-Bit Control Fields
	PRIMARY_VM_EXIT_CONTROLS = 0x400C,												//32-Bit Control Fields
	VM_EXIT_MSR_STORE_COUNT = 0x400E,												//32-Bit Control Fields
	VM_EXIT_MSR_LOAD_COUNT = 0x4010,												//32-Bit Control Fields
	VM_ENTRY_CONTROLS = 0x4012,														//32-Bit Control Fields
	VM_ENTRY_MSR_LOAD_COUNT = 0x4014,												//32-Bit Control Fields
	VM_ENTRY_INTERRUPTION_INFORMATION_FIELD = 0x4016,								//32-Bit Control Fields
	VM_ENTRY_EXCEPTION_ERROR_CODE = 0x4018,											//32-Bit Control Fields
	VM_ENTRY_INSTRUCTION_LENGTH = 0x401A,											//32-Bit Control Fields
	TPR_THRESHOLD = 0x401C,															//32-Bit Control Fields
	SECONDARY_PROCESSOR_BASED_VM_EXECUTION_CONTROLS = 0x401E,						//32-Bit Control Fields
	PLE_GAP = 0x4020,																//32-Bit Control Fields
	PLE_WINDOW = 0x4022,															//32-Bit Control Fields
	/*64-Bit Control Fields*/
	ADDRESS_OF_IO_BITMAP_A_FULL = 0x2000,											//64-Bit Control Fields
	ADDRESS_OF_IO_BITMAP_A_HIGH = 0x2001,											//64-Bit Control Fields
	ADDRESS_OF_IO_BITMAP_B_FULL = 0x2002,											//64-Bit Control Fields
	ADDRESS_OF_IO_BITMAP_B_HIGH = 0x2003,											//64-Bit Control Fields
	ADDRESS_OF_MSR_BITMAPS_FULL = 0x2004,											//64-Bit Control Fields
	ADDRESS_OF_MSR_BITMAPS_HIGH = 0x2005,											//64-Bit Control Fields
	VM_EXIT_MSR_STORE_ADDRESS_FULL = 0x2006,										//64-Bit Control Fields
	VM_EXIT_MSR_STORE_ADDRESS_HIGH = 0x2007,										//64-Bit Control Fields
	VM_EXIT_MSR_LOAD_ADDRESS_FULL = 0x2008,											//64-Bit Control Fields
	VM_EXIT_MSR_LOAD_ADDRESS_HIGH = 0x2009,											//64-Bit Control Fields
	VM_ENTRY_MSR_LOAD_ADDRESS_FULL = 0x200A,										//64-Bit Control Fields
	VM_ENTRY_MSR_LOAD_ADDRESS_HIGH = 0x200B,										//64-Bit Control Fields
	EXECUTIVE_VMCS_POINTER_FULL = 0x200C,											//64-Bit Control Fields
	EXECUTIVE_VMCS_POINTER_HIGH = 0x200D,											//64-Bit Control Fields
	PML_ADDRESS_FULL = 0x200E,														//64-Bit Control Fields
	PML_ADDRESS_HIGH = 0x200F,														//64-Bit Control Fields
	TSC_OFFSET_FULL = 0x2010,														//64-Bit Control Fields
	TSC_OFFSET_HIGH = 0x2011,														//64-Bit Control Fields
	VIRTUAL_APIC_ADDRESS_FULL = 0x2012,												//64-Bit Control Fields
	VIRTUAL_APIC_ADDRESS_HIGH = 0x2013,												//64-Bit Control Fields
	APIC_ACCESS_ADDRESS_FULL = 0x2014,												//64-Bit Control Fields
	APIC_ACCESS_ADDRESS_HIGH = 0x2015,												//64-Bit Control Fields
	POSTED_INTERRUPT_DESCRIPTOR_ADDRESS_FULL = 0x2016,								//64-Bit Control Fields
	POSTED_INTERRUPT_DESCRIPTOR_ADDRESS_HIGH = 0x2017,								//64-Bit Control Fields
	VM_FUNCTION_CONTROLS_FULL = 0x2018,												//64-Bit Control Fields
	VM_FUNCTION_CONTROLS_HIGH = 0x2019,												//64-Bit Control Fields
	EPT_POINTER_EPTP_FULL = 0x201A,													//64-Bit Control Fields
	EPT_POINTER_EPTP_HIGH = 0x201B,													//64-Bit Control Fields
	EOI_EXIT_BITMAP_0_EOI_EXIT0_FULL = 0x201C,										//64-Bit Control Fields
	EOI_EXIT_BITMAP_0_EOI_EXIT0_HIGH = 0x201D,										//64-Bit Control Fields
	EOI_EXIT_BITMAP_1_EOI_EXIT1_FULL = 0x201E,										//64-Bit Control Fields
	EOI_EXIT_BITMAP_1_EOI_EXIT1_HIGH = 0x201F,										//64-Bit Control Fields
	EOI_EXIT_BITMAP_2_EOI_EXIT2_FULL = 0x2020,										//64-Bit Control Fields
	EOI_EXIT_BITMAP_2_EOI_EXIT2_HIGH = 0x2021,										//64-Bit Control Fields
	EOI_EXIT_BITMAP_3_EOI_EXIT2_FULL = 0x2022,										//64-Bit Control Fields
	EOI_EXIT_BITMAP_3_EOI_EXIT2_HIGH = 0x2023,										//64-Bit Control Fields
	EPTP_LIST_ADDRESS_FULL = 0x2024,												//64-Bit Control Fields
	EPTP_LIST_ADDRESS_HIGH = 0x2025,												//64-Bit Control Fields
	VMREAD_BITMAP_ADDRESS_FULL = 0x2026,											//64-Bit Control Fields
	VMREAD_BITMAP_ADDRESS_HIGH = 0x2027,											//64-Bit Control Fields
	VMWRITE_BITMAP_ADDRESS_FULL = 0x2028,											//64-Bit Control Fields
	VMWRITE_BITMAP_ADDRESS_HIGH = 0x2029,											//64-Bit Control Fields
	VIRTUALIZATION_EXCEPTION_INFORMATION_ADDRESS_FULL = 0x202A,						//64-Bit Control Fields
	VIRTUALIZATION_EXCEPTION_INFORMATION_ADDRESS_HIGH = 0x202B,						//64-Bit Control Fields
	XSS_EXITING_BITMAP_FULL = 0x202C,												//64-Bit Control Fields
	XSS_EXITING_BITMAP_HIGH = 0x202D,												//64-Bit Control Fields
	ENCLS_EXITING_BITMAP_FULL = 0x202E,												//64-Bit Control Fields
	ENCLS_EXITING_BITMAP_HIGH = 0x202F,												//64-Bit Control Fields
	SUB_PAGE_PERMISSION_TABLE_POINTER_FULL = 0x2030,								//64-Bit Control Fields
	SUB_PAGE_PERMISSION_TABLE_POINTER_HIGH = 0x2031,								//64-Bit Control Fields
	TSC_MULTIPLIER_FULL = 0x2032,													//64-Bit Control Fields
	TSC_MULTIPLIER_HIGH = 0x2033,													//64-Bit Control Fields
	TERTIARY_PROCESSOR_BASED_VM_EXECUTION_CONTROLS_FULL = 0x2034,					//64-Bit Control Fields
	TERTIARY_PROCESSOR_BASED_VM_EXECUTION_CONTROLS_HIGH = 0x2035,					//64-Bit Control Fields
	ENCLV_EXITING_BITMAP_FULL = 0x2036,												//64-Bit Control Fields
	ENCLV_EXITING_BITMAP_HIGH = 0x2037,												//64-Bit Control Fields
	HYPERVISOR_MANAGED_LINEAR_ADDRESS_TRANSLATION_POINTER_HLATP_FULL = 0x2040,		//64-Bit Control Fields
	HYPERVISOR_MANAGED_LINEAR_ADDRESS_TRANSLATION_POINTER_HLATP_HIGH = 0x2041,		//64-Bit Control Fields
	SECONDARY_VM_EXIT_CONTROLS_FULL = 0x2044,										//64-Bit Control Fields
	SECONDARY_VM_EXIT_CONTROLS_HIGH = 0x2045,										//64-Bit Control Fields

	/*Natural-Width Control Fields*/
	CR0_GUEST_HOST_MASK = 0x6000,													//Natural-Width Control Fields
	CR4_GUEST_HOST_MASK = 0x6002,													//Natural-Width Control Fields
	CR0_READ_SHADOW = 0x6004,														//Natural-Width Control Fields
	CR4_READ_SHADOW = 0x6006,														//Natural-Width Control Fields
	CR3_TARGET_VALUE_0 = 0x6008,													//Natural-Width Control Fields
	CR3_TARGET_VALUE_1 = 0x600A,													//Natural-Width Control Fields
	CR3_TARGET_VALUE_2 = 0x600C,													//Natural-Width Control Fields
	CR3_TARGET_VALUE_3 = 0x600E,													//Natural-Width Control Fields

	/*16-Bit Guest-State Fields*/
	GUEST_ES_SELECTOR = 0x800,														//16-Bit Guest-State Fields
	GUEST_CS_SELECTOR = 0x802,														//16-Bit Guest-State Fields
	GUEST_SS_SELECTOR = 0x804,														//16-Bit Guest-State Fields
	GUEST_DS_SELECTOR = 0x806,														//16-Bit Guest-State Fields
	GUEST_FS_SELECTOR = 0x808,														//16-Bit Guest-State Fields
	GUEST_GS_SELECTOR = 0x80A,														//16-Bit Guest-State Fields
	GUEST_LDTR_SELECTOR = 0x80C,													//16-Bit Guest-State Fields
	GUEST_TR_SELECTOR = 0x80E,														//16-Bit Guest-State Fields
	GUEST_INTERRUPT_STATUS = 0x810,													//16-Bit Guest-State Fields
	PML_INDEX = 0x812,																//16-Bit Guest-State Fields
	/*32-Bit Guest-State Fields*/
	GUEST_ES_LIMIT = 0x4800,														//32-Bit Guest-State Fields
	GUEST_CS_LIMIT = 0x4802,														//32-Bit Guest-State Fields
	GUEST_SS_LIMIT = 0x4804,														//32-Bit Guest-State Fields
	GUEST_DS_LIMIT = 0x4806,														//32-Bit Guest-State Fields
	GUEST_FS_LIMIT = 0x4808,														//32-Bit Guest-State Fields
	GUEST_GS_LIMIT = 0x480A,														//32-Bit Guest-State Fields
	GUEST_LDTR_LIMIT = 0x480C,														//32-Bit Guest-State Fields
	GUEST_TR_LIMIT = 0x480E,														//32-Bit Guest-State Fields
	GUEST_GDTR_LIMIT = 0x4810,														//32-Bit Guest-State Fields
	GUEST_IDTR_LIMIT = 0x4812,														//32-Bit Guest-State Fields
	GUEST_ES_ACCESS_RIGHTS = 0x4814,												//32-Bit Guest-State Fields
	GUEST_CS_ACCESS_RIGHTS = 0x4816,												//32-Bit Guest-State Fields
	GUEST_SS_ACCESS_RIGHTS = 0x4818,												//32-Bit Guest-State Fields
	GUEST_DS_ACCESS_RIGHTS = 0x481A,												//32-Bit Guest-State Fields
	GUEST_FS_ACCESS_RIGHTS = 0x481C,												//32-Bit Guest-State Fields
	GUEST_GS_ACCESS_RIGHTS = 0x481E,												//32-Bit Guest-State Fields
	GUEST_LDTR_ACCESS_RIGHTS = 0x4820,												//32-Bit Guest-State Fields
	GUEST_TR_ACCESS_RIGHTS = 0x4822,												//32-Bit Guest-State Fields
	GUEST_INTERRUPTIBILITY_STATE = 0x4824,											//32-Bit Guest-State Fields
	GUEST_ACTIVITY_STATE = 0x4826,													//32-Bit Guest-State Fields
	GUEST_SMBASE = 0x4828,															//32-Bit Guest-State Fields
	GUEST_IA32_SYSENTER_CS = 0x482A,												//32-Bit Guest-State Fields
	VMX_PREEMPTION_TIMER_VALUE = 0x482E,											//32-Bit Guest-State Fields
	/*64-Bit Guest-State Fields*/
	VMCS_LINK_POINTER_FULL = 0x2800,												//64-Bit Guest-State Fields
	VMCS_LINK_POINTER_HIGH = 0x2801,												//64-Bit Guest-State Fields
	GUEST_IA32_DEBUGCTL_FULL = 0x2802,												//64-Bit Guest-State Fields
	GUEST_IA32_DEBUGCTL_HIGH = 0x2803,												//64-Bit Guest-State Fields
	GUEST_IA32_PAT_FULL = 0x2804,													//64-Bit Guest-State Fields
	GUEST_IA32_PAT_HIGH = 0x2805,													//64-Bit Guest-State Fields
	GUEST_IA32_EFER_FULL = 0x2806,													//64-Bit Guest-State Fields
	GUEST_IA32_EFER_HIGH = 0x2807,													//64-Bit Guest-State Fields
	GUEST_IA32_PERF_GLOBAL_CTRL_FULL = 0x2808,										//64-Bit Guest-State Fields
	GUEST_IA32_PERF_GLOBAL_CTRL_HIGH = 0x2809,										//64-Bit Guest-State Fields
	GUEST_PDPTE0_FULL = 0x280A,														//64-Bit Guest-State Fields
	GUEST_PDPTE0_HIGH = 0x280B,														//64-Bit Guest-State Fields
	GUEST_PDPTE1_FULL = 0x280C,														//64-Bit Guest-State Fields
	GUEST_PDPTE1_HIGH = 0x280D,														//64-Bit Guest-State Fields
	GUEST_PDPTE2_FULL = 0x280E,														//64-Bit Guest-State Fields
	GUEST_PDPTE2_HIGH = 0x280F,														//64-Bit Guest-State Fields
	GUEST_PDPTE3_FULL = 0x2810,														//64-Bit Guest-State Fields
	GUEST_PDPTE3_HIGH = 0x2811,														//64-Bit Guest-State Fields
	GUEST_IA32_BNDCFGS_FULL = 0x2812,												//64-Bit Guest-State Fields
	GUEST_IA32_BNDCFGS_HIGH = 0x2813,												//64-Bit Guest-State Fields
	GUEST_IA32_RTIT_CTL_FULL = 0x2814,												//64-Bit Guest-State Fields
	GUEST_IA32_RTIT_CTL_HIGH = 0x2815,												//64-Bit Guest-State Fields
	GUEST_IA32_LBR_CTL_FULL = 0x2816,												//64-Bit Guest-State Fields
	GUEST_IA32_LBR_CTL_HIGH = 0x2817,												//64-Bit Guest-State Fields
	GUEST_IA32_PKRS_FULL = 0x2818,													//64-Bit Guest-State Fields
	GUEST_IA32_PKRS_HIGH = 0x2819,													//64-Bit Guest-State Fields
	/*Natural-Width Guest-State Fields*/
	GUEST_CR0 = 0x6800,																//Natural-Width Guest-State Fields
	GUEST_CR3 = 0x6802,																//Natural-Width Guest-State Fields
	GUEST_CR4 = 0x6804,																//Natural-Width Guest-State Fields
	GUEST_ES_BASE = 0x6806,															//Natural-Width Guest-State Fields
	GUEST_CS_BASE = 0x6808,															//Natural-Width Guest-State Fields
	GUEST_SS_BASE = 0x680A,															//Natural-Width Guest-State Fields
	GUEST_DS_BASE = 0x680C,															//Natural-Width Guest-State Fields
	GUEST_FS_BASE = 0x680E,															//Natural-Width Guest-State Fields
	GUEST_GS_BASE = 0x6810,															//Natural-Width Guest-State Fields
	GUEST_LDTR_BASE = 0x6812,														//Natural-Width Guest-State Fields
	GUEST_TR_BASE = 0x6814,															//Natural-Width Guest-State Fields
	GUEST_GDTR_BASE = 0x6816,														//Natural-Width Guest-State Fields
	GUEST_IDTR_BASE = 0x6818,														//Natural-Width Guest-State Fields
	GUEST_DR7 = 0x681A,																//Natural-Width Guest-State Fields
	GUEST_RSP = 0x681C,																//Natural-Width Guest-State Fields
	GUEST_RIP = 0x681E,																//Natural-Width Guest-State Fields
	GUEST_RFLAGS = 0x6820,															//Natural-Width Guest-State Fields
	GUEST_PENDING_DEBUG_EXCEPTIONS = 0x6822,										//Natural-Width Guest-State Fields
	GUEST_IA32_SYSENTER_ESP = 0x6824,												//Natural-Width Guest-State Fields
	GUEST_IA32_SYSENTER_EIP = 0x6826,												//Natural-Width Guest-State Fields
	GUEST_IA32_S_CET = 0x6828,														//Natural-Width Guest-State Fields
	GUEST_SSP = 0x682A,																//Natural-Width Guest-State Fields
	GUEST_IA32_INTERRUPT_SSP_TABLE_ADDR = 0x682C,									//Natural-Width Guest-State Fields

	/*16-Bit Host-State Fields*/
	HOST_ES_SELECTOR = 0xC00,														//16-Bit Host-State Fields
	HOST_CS_SELECTOR = 0xC02,														//16-Bit Host-State Fields
	HOST_SS_SELECTOR = 0xC04,														//16-Bit Host-State Fields
	HOST_DS_SELECTOR = 0xC06,														//16-Bit Host-State Fields
	HOST_FS_SELECTOR = 0xC08,														//16-Bit Host-State Fields
	HOST_GS_SELECTOR = 0xC0A,														//16-Bit Host-State Fields
	HOST_TR_SELECTOR = 0xC0C,														//16-Bit Host-State Fields
	/*32-Bit Host-State Fields*/
	HOST_IA32_SYSENTER_CS = 0x4C00,													//32-Bit Host-State Fields
	/*64-Bit Host-State Fields*/
	HOST_IA32_PAT_FULL = 0x2C00,													//64-Bit Host-State Fields
	HOST_IA32_PAT_HIGH = 0x2C01,													//64-Bit Host-State Fields
	HOST_IA32_EFER_FULL = 0x2C02,													//64-Bit Host-State Fields
	HOST_IA32_EFER_HIGH = 0x2C03,													//64-Bit Host-State Fields
	HOST_IA32_PERF_GLOBAL_CTRL_FULL = 0x2C04,										//64-Bit Host-State Fields
	HOST_IA32_PERF_GLOBAL_CTRL_HIGH = 0x2C05,										//64-Bit Host-State Fields
	HOST_IA32_PKRS_FULL = 0x2C06,													//64-Bit Host-State Fields
	HOST_IA32_PKRS_HIGH = 0x2C07,													//64-Bit Host-State Fields
	/*Natural-Width Host-State Fields*/
	HOST_CR0 = 0x6C00,																//Natural-Width Host-State Fields
	HOST_CR3 = 0x6C02,																//Natural-Width Host-State Fields
	HOST_CR4 = 0x6C04,																//Natural-Width Host-State Fields
	HOST_FS_BASE = 0x6C06,															//Natural-Width Host-State Fields
	HOST_GS_BASE = 0x6C08,															//Natural-Width Host-State Fields
	HOST_TR_BASE = 0x6C0A,															//Natural-Width Host-State Fields
	HOST_GDTR_BASE = 0x6C0C,														//Natural-Width Host-State Fields
	HOST_IDTR_BASE = 0x6C0E,														//Natural-Width Host-State Fields
	HOST_IA32_SYSENTER_ESP = 0x6C10,												//Natural-Width Host-State Fields
	HOST_IA32_SYSENTER_EIP = 0x6C12,												//Natural-Width Host-State Fields
	HOST_RSP = 0x6C14,																//Natural-Width Host-State Fields
	HOST_RIP = 0x6C16,																//Natural-Width Host-State Fields
	HOST_IA32_S_CET = 0x6C18,														//Natural-Width Host-State Fields
	HOST_SSP = 0x6C1A,																//Natural-Width Host-State Fields
	HOST_IA32_INTERRUPT_SSP_TABLE_ADDR = 0x6C1C,									//Natural-Width Host-State Fields

	/*32-Bit Read-Only Data Fields*/
	VM_INSTRUCTION_ERROR = 0x4400,													//32-Bit Read-Only Data Fields
	EXIT_REASON = 0x4402,															//32-Bit Read-Only Data Fields
	VM_EXIT_INTERRUPTION_INFORMATION = 0x4404,										//32-Bit Read-Only Data Fields
	VM_EXIT_INTERRUPTION_ERROR_CODE = 0x4406,										//32-Bit Read-Only Data Fields
	IDT_VECTORING_INFORMATION_FIELD = 0x4408,										//32-Bit Read-Only Data Fields
	IDT_VECTORING_ERROR_CODE = 0x440A,												//32-Bit Read-Only Data Fields
	VM_EXIT_INSTRUCTION_LENGTH = 0x440C,											//32-Bit Read-Only Data Fields
	VM_EXIT_INSTRUCTION_INFORMATION = 0x440E,										//32-Bit Read-Only Data Fields
	/*64-Bit Read-Only Data Fields*/
	GUEST_PHYSICAL_ADDRESS_FULL = 0x2400,											//64-Bit Read-Only Data Fields
	GUEST_PHYSICAL_ADDRESS_HIGH = 0x2401,											//64-Bit Read-Only Data Fields
	/*Natural-Width Read-Only Data Fields*/
	EXIT_QUALIFICATION = 0x6400,													//Natural-Width Read-Only Data Fields
	IO_RCX = 0x6402,																//Natural-Width Read-Only Data Fields
	IO_RSI = 0x6404,																//Natural-Width Read-Only Data Fields
	IO_RDI = 0x6406,																//Natural-Width Read-Only Data Fields
	IO_RIP = 0x6408,																//Natural-Width Read-Only Data Fields
	GUEST_LINEAR_ADDRESS = 0x640A													//Natural-Width Read-Only Data Fields
};

ULONG64 CheckVMX();
ULONG64 OpenVMX();
ULONG64 CloseVMX();

ULONG64 StopVM();
ULONG64 StartVM(UCHAR CpuIndex, ULONG64 GeustRip, ULONG64 GuestRsp);
ULONG64 HostEntry(PGUESTREG pGuestRegs);

