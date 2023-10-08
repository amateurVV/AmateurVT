
StartVM proto		;外部的函数,开启VT,设置VT环境
HostEntry proto		;外部的函数,VMM入口

.data

ExitVMX = 0DEADh

GUEST_RAX = 0
GUEST_RCX = 8
GUEST_RDX = 10h
GUEST_RBX = 18h


pushaq MACRO
push r15
push r14
push r13
push r12
push r11
push r10
push r9
push r8
push rdi
push rsi
push rbp
push rbx
push rdx
push rcx
push rax
ENDM

popaq MACRO
pop rax
pop rcx
pop rdx
pop rbx
pop rbp
pop rsi
pop rdi
pop r8
pop r9
pop r10
pop r11
pop r12
pop r13
pop r14
pop r15
ENDM

ReturnErrorCode MACRO
	jc err1
	jz err2
	xor eax,eax
	ret

err1:
	mov eax,1
	ret
	
err2:
	mov eax,2
	ret
ENDM

.code

_readBaseGDT PROC
	sub rsp,20
	sgdt [rsp]
	xor rax,rax
	mov rax, qword ptr [rsp+2]
	add rsp,20
	ret
_readBaseGDT ENDP

_readLimitGDT PROC
	sub rsp,20
	sgdt [rsp]
	xor rax,rax
	mov ax, word ptr [rsp]
	add rsp,20
	ret
_readLimitGDT ENDP

_readBaseIDT PROC
	sub rsp,20
	sidt [rsp]
	xor rax,rax
	mov rax, qword ptr [rsp+2]
	add rsp,20
	ret
_readBaseIDT ENDP

_readLimitIDT PROC
	sub rsp,20
	sidt [rsp]
	xor rax,rax
	mov ax, word ptr [rsp]
	add rsp,20
	ret
_readLimitIDT ENDP

_writeGDTR PROC
	lgdt fword ptr[rcx]
	ret
_writeGDTR ENDP

_writeIDTR PROC
	lidt fword ptr[rcx]
	ret
_writeIDTR ENDP

_readCS proc
	xor rax,rax
	mov	ax, cs
	ret
_readCS endp

_readDS proc
	xor rax,rax
	mov	ax, ds
	ret
_readDS endp

_readES proc
	xor rax,rax
	mov	ax, es
	ret
_readES endp

_readSS proc
	xor rax,rax
	mov ax, ss
	ret
_readSS endp

_readFS proc
	xor rax,rax
	mov	ax, fs
	ret
_readFS endp

_readGS proc
	xor rax,rax
	mov	ax, gs
	ret
_readGS endp

_readTR proc
	xor rax,rax
	str	ax
	ret
_readTR endp

_readLDTR PROC
	xor rax,rax
	sldt ax
	ret
_readLDTR ENDP


_cpuid proc
	push r8
    mov r8,rcx
	mov rax,[r8+GUEST_RAX]
	mov rcx,[r8+GUEST_RCX]
    cpuid
    mov [r8+GUEST_RAX],rax
    mov [r8+GUEST_RCX],rcx
    mov [r8+GUEST_RDX],rdx
    mov [r8+GUEST_RBX],rbx
	pop r8
    ret
_cpuid endp

_readmsr proc
	push r8
	mov r8,rcx
	mov rcx,[r8+GUEST_RCX]
    rdmsr
	mov [r8+GUEST_RDX],rdx
	mov [r8+GUEST_RAX],rax
	pop r8
    ret
_readmsr endp

_writemsr proc
	push r8
	mov r8,rcx
	mov rax,[r8+GUEST_RAX]
	mov rdx,[r8+GUEST_RDX]
	mov rcx,[r8+GUEST_RCX]
    wrmsr
	pop r8
    ret
_writemsr endp

_rdtsc proc
	push r8
	mov r8,rcx	
	rdtsc
	mov [r8+GUEST_RAX],rax
	mov [r8+GUEST_RDX],rdx
	pop r8
	ret
_rdtsc endp

_rdtscp PROC
	push r8	
	mov r8,rcx
	rdtscp
	mov [r8+GUEST_RAX],rax
	mov [r8+GUEST_RCX],rcx
	mov [r8+GUEST_RDX],rdx
	pop r8
	ret
_rdtscp ENDP

asm_xsetbv proc
	push r8
	mov r8,rcx	
	mov rax,[r8+GUEST_RAX]
	mov rcx,[r8+GUEST_RCX]
	mov rdx,[r8+GUEST_RDX]
	xsetbv
	pop r8
	ret
asm_xsetbv endp

_invd proc
	invd
	ret
_invd endp

_cli proc
	cli
	ret
_cli endp

_sti proc
	sti
	ret
_sti endp

_StartVM proc
	pushfq
	pushaq	
	
	sub rsp,20h
	mov r8,rsp					;GuestRsp	
	mov rdx,offset returnAddr	;GuestRip	
	call StartVM

returnAddr:	
	add rsp,20h
	popaq
	popfq
	ret
_StartVM endp


_HostEntry PROC	
	lfence
	sub rsp,40h;GuestRegs 结构预留
	pushfq
	pushaq	
	mov rcx,rsp	;GuestRegs		

	;sub rsp,70h				
    ;movups xmmword ptr [rsp +  8h], xmm0
    ;movups xmmword ptr [rsp + 18h], xmm1
    ;movups xmmword ptr [rsp + 28h], xmm2
    ;movups xmmword ptr [rsp + 38h], xmm3
    ;movups xmmword ptr [rsp + 48h], xmm4
    ;movups xmmword ptr [rsp + 58h], xmm5

	sub rsp,20h
	call HostEntry
	add rsp,20h

	;movups xmm0, xmmword ptr [rsp +  8h]
    ;movups xmm1, xmmword ptr [rsp + 18h]
    ;movups xmm2, xmmword ptr [rsp + 28h]
    ;movups xmm3, xmmword ptr [rsp + 38h]
    ;movups xmm4, xmmword ptr [rsp + 48h]
    ;movups xmm5, xmmword ptr [rsp + 58h]
    ;add rsp, 70h
		
	cmp rax,ExitVMX
	jz leave_vmx
	popaq		
	popfq		
	add rsp,40h
	vmresume
	int 3

leave_vmx:
	popaq
	popfq	
	add rsp,40h
	mov rsp,rcx
	jmp rax
	int 3
_HostEntry ENDP


vmx_write PROC
	vmwrite rcx,rdx
	ReturnErrorCode
vmx_write ENDP

vmx_read PROC	
	vmread qword ptr [rdx],rcx
	ReturnErrorCode
vmx_read ENDP

vmx_on proc
	vmxon qword ptr [rcx]
	ReturnErrorCode
vmx_on endp

vmx_off proc
	vmxoff
	ReturnErrorCode
vmx_off endp

vmx_ptrst proc
	vmptrst qword ptr [rcx]
	ReturnErrorCode
vmx_ptrst endp

vmx_ptrld proc
	vmptrld qword ptr [rcx]
	ReturnErrorCode
vmx_ptrld endp

vmx_launch proc
	vmlaunch
	ReturnErrorCode
vmx_launch endp

vmx_resume proc
	vmresume
	ReturnErrorCode
vmx_resume endp

vmx_func proc
	vmfunc
	ReturnErrorCode
vmx_func endp

vmx_call proc
	vmcall
	ReturnErrorCode
vmx_call endp

vmx_clear proc
	vmclear qword ptr [rcx]
	ReturnErrorCode
vmx_clear endp

vmx_invvpid proc
	invvpid rcx,oword ptr [rdx]
	ReturnErrorCode
vmx_invvpid endp

vmx_invept proc
	invept rcx,oword ptr [rdx]
	ReturnErrorCode
vmx_invept endp


end


