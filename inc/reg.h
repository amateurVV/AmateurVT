#pragma once

#pragma pack(1)

typedef union
{
	struct
	{
		ULONG PE : 1;		   ///< Protection Enable.
		ULONG MP : 1;		   ///< Monitor Coprocessor.
		ULONG EM : 1;		   ///< Emulation.
		ULONG TS : 1;		   ///< Task Switched.
		ULONG ET : 1;		   ///< Extension Type.
		ULONG NE : 1;		   ///< Numeric Error.
		ULONG Reserved_0 : 10; ///< Reserved.
		ULONG WP : 1;		   ///< Write Protect.
		ULONG Reserved_1 : 1;  ///< Reserved.
		ULONG AM : 1;		   ///< Alignment Mask.
		ULONG Reserved_2 : 10; ///< Reserved.
		ULONG NW : 1;		   ///< Mot Write-through.
		ULONG CD : 1;		   ///< Cache Disable.
		ULONG PG : 1;		   ///< Paging.
	} Bits;
	LARGE_INTEGER value;
} IA32_CR0, *PCR0;

typedef union
{
	struct
	{
		ULONG Reserved1 : 3; // bit-0:2
		ULONG PWT : 1;		 // bit-3
		ULONG PCD : 1;		 // bit-4
		ULONG Reserved2 : 7; // bit-5:11
		ULONG BaseLo : 20;	 // bit-12:31
		ULONG64 BaseHi : 32; // bit-32:63
	} Bits;
	LARGE_INTEGER value;
} IA32_CR3, *PCR3;

typedef union
{
	struct
	{
		ULONG64 VME : 1;		// Bit-0
		ULONG64 PVI : 1;		// Bit-1
		ULONG64 TSD : 1;		// Bit-2
		ULONG64 DE : 1;			// Bit-3
		ULONG64 PSE : 1;		// Bit-4
		ULONG64 PAE : 1;		// Bit-5
		ULONG64 MCE : 1;		// Bit-6
		ULONG64 PGE : 1;		// Bit-7
		ULONG64 PCE : 1;		// Bit-8
		ULONG64 OSFXSR : 1;		// Bit-9
		ULONG64 OSXMMEXCPT : 1; // Bit-10
		ULONG64 UMIP : 1;		// Bit-11
		ULONG64 LA57 : 1;		// Bit-12
		ULONG64 VMXE : 1;		// Bit-13
		ULONG64 SMXE : 1;		// Bit-14
		ULONG64 NOP_15 : 1;		// Bit-15
		ULONG64 FSGSBASE : 1;	// Bit-16
		ULONG64 PCIDE : 1;		// Bit-17
		ULONG64 OSXSAVE : 1;	// Bit-18
		ULONG64 KL : 1;			// Bit-19
		ULONG64 SMEP : 1;		// Bit-20
		ULONG64 SMAP : 1;		// Bit-21
		ULONG64 PKE : 1;		// Bit-22
		ULONG64 CET : 1;		// Bit-23
		ULONG64 PKS : 1;		// Bit-24
		ULONG64 NOP_25_31 : 7;	// These are zero
		ULONG64 Reserved_64 : 32;
	} Bits;
	LARGE_INTEGER value;
} IA32_CR4, *PCR4;

typedef union
{
	struct
	{
		ULONG CF : 1;		   /// 0< Carry Flag.
		ULONG Reserved_0 : 1;  /// 1< Reserved.
		ULONG PF : 1;		   /// 2< Parity Flag.
		ULONG Reserved_1 : 1;  /// 3< Reserved.
		ULONG AF : 1;		   /// 4< Auxiliary Carry Flag.
		ULONG Reserved_2 : 1;  /// 5< Reserved.
		ULONG ZF : 1;		   /// 6< Zero Flag.
		ULONG SF : 1;		   /// 7< Sign Flag.
		ULONG TF : 1;		   /// 8< Trap Flag.
		ULONG IF : 1;		   /// 9< Interrupt Enable Flag.
		ULONG DF : 1;		   /// 10< Direction Flag.
		ULONG OF : 1;		   /// 11< Overflow Flag.
		ULONG IOPL : 2;		   /// 12-13< I/O Privilege Level.
		ULONG NT : 1;		   /// 14< Nested Task.
		ULONG Reserved_3 : 1;  /// 15< Reserved.
		ULONG RF : 1;		   /// 16< Resume Flag.
		ULONG VM : 1;		   /// 17< Virtual 8086 Mode.
		ULONG AC : 1;		   /// 18< Alignment Check.
		ULONG VIF : 1;		   /// 19< Virtual Interrupt Flag.
		ULONG VIP : 1;		   /// 20< Virtual Interrupt Pending.
		ULONG ID : 1;		   /// 21< ID Flag.
		ULONG Reserved_4 : 10; /// 22-31< Reserved.
	} Bits;
	ULONG64 Value;
} IA32_RFLAGS, *PRFLAGS;

typedef union
{
	struct
	{
		USHORT RPL : 2;
		USHORT TI : 1;
		USHORT Index : 13;
	} Bits;
	USHORT value;
} SELECTOR, *PSELECTOR;

typedef struct _SEGMENT_DESCRIPTOR
{
	USHORT limit0;
	USHORT base0;
	UCHAR base1;
	union
	{
		struct
		{
			UCHAR TYPE : 4;
			UCHAR S : 1;
			UCHAR DPL : 2;
			UCHAR P : 1;
			UCHAR limit1 : 4;
			UCHAR AVL : 1;
			UCHAR L : 1;
			UCHAR DB : 1;
			UCHAR G : 1;
		} Bits;
		USHORT value;
	} attr;
	UCHAR base2;
	ULONG base3;
	ULONG reserved;
} SEG_DESCRIPTOR, *PSEG_DESCRIPTOR;

typedef struct _SEG
{
	SELECTOR sel;
	PSEG_DESCRIPTOR descriptor;
	ULONG64 Base;
	ULONG64 Limit;
} SEG, *PSEG;

enum SEGREGS
{
	ES = 0,
	CS,
	SS,
	DS,
	FS,
	GS,
	LDTR,
	TR
};

#pragma pack()