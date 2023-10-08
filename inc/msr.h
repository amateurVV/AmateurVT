#pragma once

#define MSR_IA32_APIC_BASE					0x1B
#define	MSR_IA32_FEATURE_CONTROL			0x3A
#define MSR_IA32_SPEC_CTRL					0x48
#define MSR_IA32_PRED_CMD					0x49
#define MSR_IA32_SMM_MONITOR_CTL			0x9B
#define	MSR_IA32_SMBASE						0x9E
#define MSR_IA32_MISC_PACKAGE_CTLS			0xBC
#define MSR_IA32_CORE_CAPABILITIES			0xCF
#define MSR_IA32_TSX_CTRL					0x122
#define MSR_IA32_SYSENTER_CS				0x174
#define MSR_IA32_SYSENTER_ESP				0x175
#define MSR_IA32_SYSENTER_EIP				0x176
#define MSR_IA32_DEBUGCTL					0x1D9
#define MSR_IA32_MISC_ENABLE				0x1A0
#define MSR_IA32_SMRR_PHYSBASE				0x1F2
#define MSR_IA32_SMRR_PHYSMASK				0x1F3 
#define MSR_IA32_PAT						0x277
#define MSR_IA32_PERF_CAPABILITIES			0x345
#define MSR_IA32_FIXED_CTR_CTRL				0x38D
#define MSR_IA32_PERF_GLOBAL_STATUS			0x38E
#define MSR_IA32_PERF_GLOBAL_CTRL			0x38F
#define MSR_IA32_PERF_GLOBAL_OVF_CTRL		0x390
#define MSR_IA32_PERF_GLOBAL_STATUS_RESET	0x390
#define MSR_IA32_PERF_GLOBAL_STATUS_SET		0x391
#define MSR_IA32_PERF_GLOBAL_INUSE			0x392
#define MSR_IA32_PEBS_ENABLE				0x3F1
#define MSR_IA32_RTIT_OUTPUT_BASE			0x560
#define MSR_IA32_RTIT_CR3_MATCH				0x572
#define MSR_IA32_DS_AREA					0x600
#define MSR_IA32_U_CET						0x6A0
#define MSR_IA32_S_CET						0x6A2
#define MSR_IA32_PL0_SSP					0x6A4
#define MSR_IA32_INTERRUPT_SSP_TABLE_ADDR	0x6A8
#define MSR_IA32_TSC_DEADLINE				0x6E0
#define MSR_IA32_PKRS						0x6E1 
#define MSR_IA32_PM_ENABLE					0x770
#define MSR_IA32_HWP_REQUEST				0x774
#define MSR_IA32_X2APIC_APICID				0x802
#define MSR_IA32_TME_CAPABILITY				0x981
#define MSR_IA32_DEBUG_INTERFACE			0xC80
#define MSR_IA32_BNDCFGS					0xD90

//#define x2APIC 0x802 - 0x83F

#define	MSR_IA32_VMX_BASIC					0x480
#define	MSR_IA32_VMX_PINBASED_CTLS			0x481
#define	MSR_IA32_VMX_PROCBASED_CTLS			0x482
#define	MSR_IA32_VMX_EXIT_CTLS				0x483
#define	MSR_IA32_VMX_ENTRY_CTLS				0x484
#define	MSR_IA32_VMX_MISC					0x485
#define	MSR_IA32_VMX_CR0_FIXED0				0x486
#define	MSR_IA32_VMX_CR0_FIXED1				0x487
#define	MSR_IA32_VMX_CR4_FIXED0				0x488
#define	MSR_IA32_VMX_CR4_FIXED1				0x489
#define	MSR_IA32_VMX_VMCS_ENUM				0x48A
#define	MSR_IA32_VMX_PROCBASED_CTLS2		0x48B
#define	MSR_IA32_VMX_EPT_VPID_CAP			0x48C
#define	MSR_IA32_VMX_TRUE_PINBASED_CTLS		0x48D
#define	MSR_IA32_VMX_TRUE_PROCBASED_CTLS	0x48E
#define	MSR_IA32_VMX_TRUE_EXIT_CTLS			0x48F
#define	MSR_IA32_VMX_TRUE_ENTRY_CTLS		0x490
#define	MSR_IA32_VMX_VMFUNC					0x491
#define	MSR_IA32_VMX_PROCBASED_CTLS3		0x492
#define	MSR_IA32_VMX_EXIT_CTLS2				0x493

#define	MSR_IA32_EFER						0xC0000080
#define MSR_IA32_STAR						0xC0000081  
#define MSR_IA32_LSTAR						0xC0000082  
#define MSR_IA32_CSTAR						0xC0000083  
#define MSR_IA32_FMASK						0xC0000084  
#define MSR_IA32_FS_BASE					0xC0000100  
#define MSR_IA32_GS_BASE					0xC0000101  
#define MSR_IA32_KERNEL_GS_BASE				0xC0000102  
#define MSR_IA32_TSC_AUX					0xC0000103

#pragma pack(1)

typedef union {

	struct {
		ULONG  Lock : 1;							//bit-0			Lock bit (0 = unlocked, 1 = locked). When set to '1' further writes to this MSR are blocked.
		ULONG  EnableVmxInsideSmx : 1;				//bit-1			Enable VMX in SMX operation.
		ULONG  EnableVmxOutsideSmx : 1;				//bit-2			Enable VMX outside SMX operation.
		ULONG  Reserved1 : 5;						//bit-3:7		Reserved
		ULONG  SenterLocalFunctionEnables : 7;		//bit-8:14		SENTER Local Function Enables: When set, each bit in the field represents an enable control for a corresponding SENTER function.
		ULONG  SenterGlobalEnable : 1;				//bit-15		SENTER Global Enable: Must be set to ‘1’ to enable operation of GETSEC[SENTER].
		ULONG  Reserved2 : 1;						//bit-16		Reserved
		ULONG  SgxLaunchControlEnable : 1;			//bit-17		SGX Launch Control Enable: Must be set to ‘1’ to enable runtime re-configuration of SGX Launch Control via the IA32_SGXLEPUBKEYHASHn MSR.
		ULONG  SgxEnable : 1;						//bit-18		SGX Global Enable: Must be set to ‘1’ to enable Intel SGX leaf functions.
		ULONG  Reserved3 : 1;						//bit-19		Reserved
		ULONG  LmceOn : 1;							//bit-20		LMCE On: When set, system software can program the MSRs associated with LMCE to configure delivery of some machine check exceptions to a single logical processor.
		ULONG  Reserved4 : 11;						//bit-21:31		Reserved
		ULONG  Reserved5 : 32;						//bit-32:64		Reserved
	} Bits;
	LARGE_INTEGER  value;
} MSR_IA32_FEATURE_CONTROL_REGISTER, * PMSR_IA32_FEATURE_CONTROL_REGISTER;

typedef union {

	struct {

		ULONG  VmcsRevisonId : 31;		//bit-0:30	VMCS版本标识符
		ULONG  MustBeZero : 1;			//bit-31	始终为0

		ULONG  VmcsSize : 13;			//bit-32:44	VMON 或 VMCS 分配的字节数
		ULONG  Reserved1 : 3;			//bit-45:47	保留

		ULONG  VmcsAddressWidth : 1;	//bit-48	VMON 或 VMCS （I/O 位图、虚拟 APIC 页面、用于 VMX 转换的 MSR 区域）的物理地址的宽度。
		//			如果位为 0，则这些地址限制为处理器的物理地址宽度。
		//			如果位为 1，则这些地址限制为 32 位。
		//			对于支持英特尔 64 位架构的处理器，此位始终为 0。

		ULONG  DualMonitor : 1;		//bit-49	如果位为 1，则逻辑处理器支持系统管理中断和系统管理模式的双监视器处理。

		ULONG  MemoryType : 4;			//bit-50:53	VMCS、VMCS 中指针引用的数据结构（I/O 位图、虚拟 APIC 页面、用于 VMX 转换的 MSR 区域）和 MSEG 标头的内存类型。
		//			如果 软件需要访问这些数据结构（例如，修改 MSR 位图的内容），它可以配置分页结构以将它们映射到线性地址空间中。
		//			如果这样做，则应建立使用此 MSR 中报告的内存类型位 53：50 的映射。
		//			值   0		Uncacheable(UC)
		//			值	 1-5	Not Used
		//			值	 6		Write Back(WB)
		//			值	 7-15	Not Used

		ULONG  InsOutsReporting : 1;	//bit-54	如果位为 1，则处理器将报告由于执行 INS 和 OUTS 指令而导致的虚拟机退出指令字段中的虚拟机出口信息

		ULONG  VmxControls : 1;		//bit-55	如果位为 0，控件是默认0组，不使用以下MSR:(带TRUE控件)
		//			IA32_VMX_TRUE_PINBASED_CTLS
		//			IA32_VMX_TRUE_PROCBASED_CTLS
		//			IA32_VMX_TRUE_EXIT_CTLS
		//			IA32_VMX_TRUE_ENTRY_CTLS
		ULONG  HardwareException : 1;	//bit-56	如果位为 1，则软件可以使用虚拟机条目在有或没有错误代码的情况下提供硬件异常，而不管向量如何
		ULONG  Reserved2 : 7;			//bit-57:63	保留
	} Bits;

	LARGE_INTEGER  value;
} MSR_IA32_VMX_BASIC_REGISTER, * PMSR_IA32_VMX_BASIC_REGISTER;

typedef union {
	struct {
		ULONG ExIDT : 1;		//Bit-0		外部中断		External-interrupt exiting
		ULONG Reserved1 : 2;	//Bit-1:2				Reserved
		ULONG ExNMI : 1;		//Bit-3					NMI exiting
		ULONG Reserved2 : 1;	//Bit-4		保留
		ULONG V_NMI : 1;		//Bit-5					Virtual NMIs
		ULONG A_VMXpt : 1;	    //Bit-6		抢占定时器	Activate VMXpreemption timer
		ULONG Ppi : 1;		    //Bit-7		发布中断		Process posted interrupts
		ULONG Reserved3 : 24;  //Bit-8-31	
	}Bits[2];
	LARGE_INTEGER value;
} MSR_IA32_VMX_PINBASED_CTLS_REGISTER, * PMSR_IA32_VMX_PINBASED_CTLS_REGISTER;

typedef union {
	struct
	{
		ULONG Reserved1 : 2;			//bit-0:1
		ULONG ExIDT : 1;				//bit-2
		ULONG TSC : 1;					//bit-3
		ULONG Reserved2 : 3;			//bit-4:6
		ULONG HLT : 1;					//bit-7
		ULONG Reserved3 : 1;			//bit-8
		ULONG INVLPG : 1;				//bit-9
		ULONG MWAIT : 1;				//bit-10
		ULONG RDPMC : 1;				//bit-11
		ULONG RDTSC : 1;				//bit-12
		ULONG Reserved4 : 2;			//bit-13:14
		ULONG LCR3 : 1;					//bit-15
		ULONG SCR3 : 1;					//bit-16
		ULONG Tertiary : 1;				//bit-17
		ULONG Reserved5 : 1;			//bit-18
		ULONG LCR8 : 1;					//bit-19
		ULONG SCR8 : 1;					//bit-20
		ULONG TPR : 1;					//bit-21
		ULONG NMI : 1;					//bit-22
		ULONG MOV_DR : 1;				//bit-23
		ULONG UnconditionIO : 1;		//bit-24
		ULONG IOmap : 1;				//bit-25
		ULONG Reserved6 : 1;			//bit-26
		ULONG MonitorTrap : 1;			//bit-27
		ULONG MSRmap : 1;				//bit-28
		ULONG MONITOR : 1;				//bit-29
		ULONG PAUSE : 1;				//bit-30
		ULONG Secondary : 1;			//bit-31

	}Bits[2];
	LARGE_INTEGER value;
}MSR_IA32_VMX_PROCBASED_CTLS_REGISTER, * PMSR_IA32_VMX_PROCBASED_CTLS_REGISTER;

typedef union {
	struct
	{
		ULONG VirtualAPIC : 1;			//bit-0
		ULONG EPT : 1;					//bit-1
		ULONG DescriptTable : 1;		//bit-2
		ULONG RDTSCP : 1;				//bit-3
		ULONG Virtualx2APIC : 1;		//bit-4
		ULONG VPID : 1;					//bit-5
		ULONG WBINVD : 1;				//bit-6
		ULONG UnGuest : 1;				//bit-7
		ULONG APIC : 1;					//bit-8
		ULONG VirInterrupt : 1;			//bit-9
		ULONG PAUSE : 1;				//bit-10
		ULONG RDRAND : 1;				//bit-11
		ULONG INVPCID : 1;				//bit-12
		ULONG VMFUNC : 1;				//bit-13
		ULONG ShadowVMCS : 1;			//bit-14
		ULONG ENCLS : 1;				//bit-15
		ULONG RDSEED : 1;				//bit-16
		ULONG PML : 1;					//bit-17
		ULONG EPT_VE : 1;				//bit-18
		ULONG ConcealVMX : 1;			//bit-19	
		ULONG XSAVES_XRSTORS : 1;		//bit-20
		ULONG Reserved1 : 1;			//bit-21
		ULONG ModeBaseEPT : 1;			//bit-22
		ULONG SubPageEPT : 1;			//bit-23
		ULONG GuestUsePhysicalPT : 1;	//bit-24
		ULONG TSC : 1;					//bit-25
		ULONG UserWaitPause : 1;		//bit-26
		ULONG PCONFIG : 1;				//bit-27
		ULONG ENCLV : 1;				//bit-28
		ULONG Reserved3 : 3;			//bit-29:31
	}Bits[2];
	LARGE_INTEGER value;
}MSR_IA32_VMX_PROCBASED_CTLS2_REGISTER, * PMSR_IA32_VMX_PROCBASED_CTLS2_REGISTER;

typedef union {
	struct
	{
		ULONG LOADIWKEY : 1;			//bit-0
		ULONG HLAT : 1;					//bit-1
		ULONG EPT_write_control : 1;	//bit-2		EPT paging-write control
		ULONG Guest_page : 1;			//bit-3		Guest-paging verification

	}Bits;
	LARGE_INTEGER value;
}MSR_IA32_VMX_PROCBASED_CTLS3_REGISTER, * PMSR_IA32_VMX_PROCBASED_CTLS3_REGISTER;

typedef union {
	struct
	{
		ULONG Reserved1 : 2;					//bit-0:1
		ULONG SaveDebug : 1;					//bit-2
		ULONG Reserved2 : 6;					//bit-3:8
		ULONG HOST_addr_size : 1;				//bit-9
		ULONG Reserved3 : 2;					//bit-10:11
		ULONG Load_IA32_PERF_GLOBAL_CTRL : 1;	//bit-12
		ULONG Reserved4 : 2;					//bit-13:14
		ULONG InterruptExit : 1;				//bit-15
		ULONG Reserved5 : 2;					//bit-16:17
		ULONG Save_IA32_PAT : 1;				//bit-18
		ULONG Load_IA32_PAT : 1;				//bit-19
		ULONG Save_IA32_EFER : 1;				//bit-20
		ULONG Load_IA32_EFER : 1;				//bit-21
		ULONG SaveVMXpt : 1;					//bit-22	抢占定时器
		ULONG Clear_IA32_BNDCFGS : 1;			//bit-23
		ULONG Conceal_VMX_from_PT : 1;			//bit-24
		ULONG Clear_IA32_RTIT_CTL : 1;			//bit-25
		ULONG Clear_IA32_LBR_CTL : 1;			//bit-26
		ULONG Reserved6 : 1;					//bit-27
		ULONG Load_CET_state : 1;				//bit-28
		ULONG Load_PKRS : 1;					//bit-29
		ULONG Reserved7 : 1;					//bit-30
		ULONG Secondary : 1;					//bit-31		
	}Bits[2];
	LARGE_INTEGER value;
} MSR_IA32_VMX_EXIT_CTLS_REGISTER, * PMSR_IA32_VMX_EXIT_CTLS_REGISTER;

typedef union {
	struct
	{
		ULONG Reserved1 : 2;					//bit-0:1
		ULONG LoadDebug : 1;					//bit-2
		ULONG Reserved2 : 6;					//bit-3:8
		ULONG Guest_IA32e : 1;					//bit-9
		ULONG EntrySMM : 1;						//bit-10
		ULONG Dual_Monitor : 1;					//bit-11
		ULONG Reserved3 : 1;					//bit-12
		ULONG Load_IA32_PERF_GLOBA_L_CTRL : 1;	//bit-13
		ULONG Load_IA32_PAT : 1;				//bit-14
		ULONG Load_IA32_EFER : 1;				//bit-15
		ULONG Load_IA32_BNDCFGS : 1;			//bit-16
		ULONG Conceal_VMX_from_PT : 1;			//bit-17
		ULONG Load_IA32_RTIT_CTL : 1;			//bit-18
		ULONG Reserved4 : 1;					//bit-19
		ULONG Load_CET_state : 1;				//bit-20
		ULONG Load_guest_IA32_LBR_CTL : 1;		//bit-21
		ULONG Load_PKRS : 1;					//bit-22
		ULONG Reserved5 : 9;					//bit-23:31
	}Bits[2];
	LARGE_INTEGER value;
}MSR_IA32_VMX_ENTRY_CTLS_REGISTER, * PMSR_IA32_VMX_ENTRY_CTLS_REGISTER;


typedef union {
	struct {
		ULONG SYSCALL : 1;				//bit-0
		ULONG Reserved1 : 7;			//bit-1:7
		ULONG EnableIA_32e : 1;			//bit-8
		ULONG Reserved2 : 1;			//bit-9
		ULONG ActiveIA_32e : 1;			//bit-10
		ULONG ExecuteDisableBit : 1;	//bit-11
		ULONG Reserved3 : 21;			//bit-12:32
	}Bits;
	LARGE_INTEGER value;
}MSR_IA32_EFER_REGISTER, * PMSR_IA32_EFER_REGISTER;


#pragma pack()


