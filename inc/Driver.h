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

typedef struct _VM {
	PVOID pOnVa;
	PVOID pCsVa;
	PVOID pStack;

	PHYSICAL_ADDRESS pOnPa;
	PHYSICAL_ADDRESS pCsPa;

	PVOID pMsrMapVa;
	PVOID pMsrMap_R_L;
	PVOID pMsrMap_R_H;
	PVOID pMsrMap_W_L;
	PVOID pMsrMap_W_H;
	MSRSTORE* pMsrMemVa;

	PHYSICAL_ADDRESS pMsrMemPa;
	PHYSICAL_ADDRESS pMsrMapPa;
	ULONG ExecptBitMap;

	ULONG64 SaveMsrLstar;
	ULONG64 EntryLoadMsrCount;
}VM;

#pragma pack()

VM* psVM;


