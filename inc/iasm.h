#pragma once

__int64 _readBaseGDT();
__int64 _readLimitGDT();
__int64 _readBaseIDT();
__int64 _readLimitIDT();

__int64 _writeGDTR(void *);
__int64 _writeIDTR(void *);

__int16 _readCS();
__int16 _readDS();
__int16 _readES();
__int16 _readSS();
__int16 _readFS();
__int16 _readGS();
__int16 _readTR();
__int16 _readLDTR();

__int64 _StartVM(int);
__int64 _HostEntry();

__int64 _cpuid(PGUESTREG GuestRegs);
__int64 _readmsr(PGUESTREG GuestRegs);
__int64 _writemsr(PGUESTREG GuestRegs);

__int64 _rdtsc(PGUESTREG GuestRegs);
__int64 _rdtscp(PGUESTREG GuestRegs);
__int64 asm_xsetbv(PGUESTREG GuestRegs);

void _invd();
void _cli();
void _sti();

__int8 vmx_write(__int64 rcx, __int64 rdx);
__int8 vmx_read(__int64 rcx, void* rdx);
__int8 vmx_on(void* rcx);
__int8 vmx_off();
__int8 vmx_ptrst(void* rcx);
__int8 vmx_ptrld(void* rcx);
__int8 vmx_launch();
__int8 vmx_resume();
__int8 vmx_func(void* rcx, void* rdx, void* r8, void* r9);
__int8 vmx_call(void* rcx, void* rdx, void* r8, void* r9);
__int8 vmx_clear(void*rcx);
__int8 vmx_invvpid(void* rcx, void* m128);
__int8 vmx_invept(void* rcx, void* m128);

