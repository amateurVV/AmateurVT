#pragma once

#include <ntifs.h>
#include <intrin.h>

#include "vmx.h"
#include "handle.h"
#include "msr.h"
#include "reg.h"
#include "iasm.h"

#define STACK_SIZE				0x8000
#define CPUID_ECX_VMX_ABILITY	(1<<5)
#define ExitVMX					0x0DEAD

#pragma pack(1)

typedef struct {
	ULONG MsrIndex;
	ULONG Reserved;
	ULONG64 MsrData;
}MSRSTORE;

typedef struct _SMEMORY{
	PVOID vir;
	PVOID phy;
}SMEMORY,*PSMEMORY;

typedef struct _VM {
	SMEMORY VmxON;
	SMEMORY VmxCS;
	SMEMORY MsrBitMap;
	PVOID VmxStack;

	ULONG ExecptBitMap;

	ULONG64 SaveMsrLstar;
	ULONG64 EntryLoadMsrCount;
}VM,*PVM;

typedef struct _VT {
	PVM vm;
}VT,*PVT;

#pragma pack()

PVT vt;



