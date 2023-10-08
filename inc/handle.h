#pragma once

#pragma pack(1)
/*Basic VM-Exit Information*/
typedef union _EXIT_REASON_FIELDS
{
	struct
	{
		ULONG Basic : 16;	  // bit-0:15	退出基本信息
		ULONG AlwaysZero : 1; // bit-16
		ULONG Undefine1 : 10; // bit-17:26
		ULONG Enclave : 1;	  // bit-27
		ULONG MTF : 1;		  // bit-28
		ULONG VMXroot : 1;	  // bit-29
		ULONG Undefine2 : 1;  // bit-30
		ULONG IsValid : 1;	  // bit-31
	} Bits;
	ULONG64 value;
} EXIT_REASON_FIELDS, *PEXIT_REASON_FIELDS;

/*Exit Qualification for Debug Exceptions*/
typedef union _EXIT_QUA_DEBUG_EXCEPTION
{
	struct
	{
		ULONG64 BPCondition : 4; // bit-0:3	B3 – B0. When set, each of these bits indicates that the corresponding breakpoint condition was met. Any of these bits may be set even if its corresponding enabling bit in DR7 is not set.
		ULONG64 UnDefine1 : 9;	 // bit-4:12	Not currently defined.
		ULONG64 BD : 1;			 // bit-13	BD. When set, this bit indicates that the cause of the debug exception is “debug register access detected.”
		ULONG64 BS : 1;			 // bit-14	BS. When set, this bit indicates that the cause of the debug exception is either the execution of a single instruction(if RFLAGS.TF = 1 and IA32_DEBUGCTL.BTF = 0) or a taken branch(if RFLAGS.TF = DEBUGCTL.BTF = 1).
		ULONG64 UnDefine2 : 1;	 // bit-15	Not currently defined.
		ULONG64 RTM : 1;		 // bit-16	RTM. When set, this bit indicates that a debug exception (#DB) or a breakpoint exception (#BP) occurred inside an RTM region while advanced debugging of RTM transactional regions was enabled
		ULONG64 UnDefine3 : 15;	 // bit-17:31	Not currently defined. Bits 63:32 exist only on processors that support Intel 64 architecture.
		ULONG64 UnDefine4 : 32;
	} Bits;
	ULONG64 value;
} EXIT_QUA_DEBUG_EXCEPTION, *PEXIT_QUA_DEBUG_EXCEPTION;

/*Exit Qualification for Task Switches*/
typedef union _EXIT_QUA_TASK_SWITCHES
{
	struct
	{
		ULONG64 SelTSS : 16;	// bit-0:15	Selector of task-state segment (TSS) to which the guest attempted to switch
		ULONG64 UnDefine1 : 14; // bit-16:29	Not currently defined
		ULONG64 Source : 2;		// bit-30:31	[0]: CALL instruction,[1]: IRET instruction [2]: JMP instruction [3]: Task gate in IDT
		ULONG64 UnDefine4 : 32;
	} Bits;
	ULONG64 value;
} EXIT_QUA_TASK_SWITCHES, *PEXIT_QUA_TASK_SWITCHES;

/*Exit Qualification for Control-Register Accesses*/
typedef union _EXIT_QUA_CR_ACCESS
{
	struct
	{
		ULONG64 RegNum : 4;		 // bit-0:3	控制寄存器的编号（CLTS 和 LMSW 为 0）
		ULONG64 AccessType : 2;	 // bit-4:5	//0 = MOV to CR;	//1 = MOV from CR;	//2 = CLTS;3 = LMSW
		ULONG64 LMSW : 1;		 // bit-6		//0 = register;	//1 = memory
		ULONG64 UnDefine1 : 1;	 // bit-7
		ULONG64 MovToCr : 4;	 // bit-8:11	[0-15] = RegNum
		ULONG64 UnDefine2 : 4;	 // bit-12:15
		ULONG64 SourceData : 16; // bit-16:31	//For LMSW, the LMSW source data	//For CLTSand MOV CR, cleared to 0
		ULONG64 UnDefine3 : 32;	 // bit-32:63
	} Bits;
	ULONG64 value;
} EXIT_QUA_CR_ACCESS, *PEXIT_QUA_CR_ACCESS;

/*Exit Qualification for APIC-Access VM Exits from Linear Accesses and Guest-Physical Accesses*/
typedef union _EXIT_QUA_ADDR_ACCESS
{
	struct
	{
		// bit-0:11
		// • If the APIC-access VM exit is due to a linear access, the offset of access within the APIC page.
		// • Undefined if the APIC - access VM exit is due a guest - physical access
		ULONG64 Offset : 12;

		// bit - 12:15		Access type :
		// 0 = linear access for a data read during instruction execution
		// 1 = linear access for a data write during instruction execution
		// 2 = linear access for an instruction fetch
		// 3 = linear access(read or write) during event delivery
		// 4 = linear access for monitoring
		// 10 = guest - physical access during event delivery
		// 11 = guest - physical access for monitoring or trace
		// 15 = guest - physical access for an instruction fetch or during instruction execution
		// Other values not used
		ULONG64 AccessType : 4;

		// bit-16
		// This bit is set for certain accesses that are asynchronous to instruction execution and not part of event delivery.
		// These includes guest - physical accesses related to trace output by Intel PT(see Section 25.5.4) and accesses
		// related to PEBS on processors with the “EPT - friendly” enhancement(see Section 19.9.5).
		ULONG64 async : 1;
		ULONG64 UnDefine1 : 15; // bit-17:31
		ULONG64 UnDefine3 : 32; // bit-32:63
	} Bits;
	ULONG64 value;
} EXIT_QUA_ADDR_ACCESS, *PEXIT_QUA_ADDR_ACCESS;

typedef union _EXIT_INTR_INFO
{
	struct
	{
		ULONG Vector : 8;		  // bit-0:7
		ULONG IntrType : 3;		  // bit-8:10
		ULONG ValidErrorCode : 1; // bit-11	(0 = invalid; 1 = valid)
		ULONG NMIToIRET : 1;	  // bit-12
		ULONG Undefine : 18;	  // bit-13:30
		ULONG IsValid : 1;		  // bit-31
	} Bits;
	ULONG value;
} EXIT_INTR_INFO, *PEXIT_INTR_INFO;

typedef union _INTERRUPT_INJECT_INFO_FIELD
{
	struct
	{
		ULONG Vector : 8;		  // bit-0:7
		ULONG IntrType : 3;		  // bit-8:10
		ULONG ValidErrorCode : 1; // bit-11	(0 = invalid; 1 = valid)
		ULONG Reserved : 19;	  // bit-12:30
		ULONG IsValid : 1;		  // bit-31
	} Bits;
	ULONG64 value;
} INTERRUPT_INJECT_INFO_FIELD, *PINTERRUPT_INJECT_INFO_FIELD;

typedef union _INTERRUPT_IBILITY_INFO
{
	struct
	{
		ULONG STI : 1;		 // bit-0
		ULONG MOV_SS : 1;	 // bit-1
		ULONG SMI : 1;		 // bit-2
		ULONG NMI : 1;		 // bit-3
		ULONG enclave : 1;	 // bit-4
		ULONG Reserved : 27; // bit-5:31
	} Bits;
	ULONG64 value;
} INTERRUPT_IBILITY_INFO, *PINTERRUPT_IBILITY_INFO;

#pragma pack()



enum vm_exit_reason
{
	EXIT_REASON_EXCEPTION_NMI = 0,
	EXIT_REASON_EXTERNAL_INTERRUPT = 1,
	EXIT_REASON_TRIPLE_FAULT = 2,
	EXIT_REASON_INIT = 3,
	EXIT_REASON_SIPI = 4,
	EXIT_REASON_IO_SMI = 5,
	EXIT_REASON_OTHER_SMI = 6,
	EXIT_REASON_INTERRUPT_WINDOW = 7,
	EXIT_REASON_PENDING_VIRT_NMI = 8,
	EXIT_REASON_TASK_SWITCH = 9,
	EXIT_REASON_CPUID = 10,
	EXIT_REASON_GETSEC = 11,
	EXIT_REASON_HLT = 12,
	EXIT_REASON_INVD = 13,
	EXIT_REASON_INVLPG = 14,
	EXIT_REASON_RDPMC = 15,
	EXIT_REASON_RDTSC = 16,
	EXIT_REASON_RSM = 17,
	EXIT_REASON_VMCALL = 18,
	EXIT_REASON_VMCLEAR = 19,
	EXIT_REASON_VMLAUNCH = 20,
	EXIT_REASON_VMPTRLD = 21,
	EXIT_REASON_VMPTRST = 22,
	EXIT_REASON_VMREAD = 23,
	EXIT_REASON_VMRESUME = 24,
	EXIT_REASON_VMWRITE = 25,
	EXIT_REASON_VMXOFF = 26,
	EXIT_REASON_VMXON = 27,
	EXIT_REASON_CR_ACCESS = 28,
	EXIT_REASON_DR_ACCESS = 29,
	EXIT_REASON_IO_INSTRUCTION = 30,
	EXIT_REASON_RDMSR = 31,
	EXIT_REASON_WRMSR = 32,
	EXIT_REASON_ENTRY_INVALID_GUEST_STATE = 33,
	EXIT_REASON_ENTRY_MSR_LOADING = 34,
	EXIT_REASON_MWAIT = 36,
	EXIT_REASON_MTF_TRAP_FLAG = 37,
	EXIT_REASON_MONITOR = 39,
	EXIT_REASON_PAUSE = 40,
	EXIT_REASON_MACHINE_CHECK = 41,
	EXIT_REASON_TPR_BELOW_THRESHOLD = 43,
	EXIT_REASON_APIC_ACCESS = 44,
	EXIT_REASON_VIR_EOI = 45,
	EXIT_REASON_GDTR_IDTR = 46,
	EXIT_REASON_TR_ACCESS = 47,
	EXIT_REASON_EPT_VIOLATION = 48,
	EXIT_REASON_EPT_MISCONFIG = 49,
	EXIT_REASON_INVEPT = 50,
	EXIT_REASON_RDTSCP = 51,
	EXIT_REASON_VMX_TIMER = 52,
	EXIT_REASON_INVVPID = 53,
	EXIT_REASON_WBINVD = 54,
	EXIT_REASON_XSETBV = 55,
	EXIT_REASON_APIC_WRITE = 56,
	EXIT_REASON_RDRAND = 57,
	EXIT_REASON_INVPCID = 58,
	EXIT_REASON_VMFUNC = 59,
	EXIT_REASON_ENCLS = 60,
	EXIT_REASON_RDSEED = 61,
	EXIT_REASON_PAGE_FULL = 62,
	EXIT_REASON_XSAVES = 63,
	EXIT_REASON_XRSTORS = 64,
	EXIT_REASON_SPP_RELATED_EVENT = 66,
	EXIT_REASON_UMWAIT = 67,
	EXIT_REASON_TPAUSE = 68,
	EXIT_REASON_LOADIWKEY = 69,
};




ULONG64 DispatchHandler(PGUESTREG pGuestRegs);