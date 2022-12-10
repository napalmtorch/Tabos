[GLOBAL _gdt_flush]
[GLOBAL _idt_flush]

[GLOBAL _regrd_cr0]
[GLOBAL _regrd_cr2]
[GLOBAL _regrd_cr3]
[GLOBAL _regrd_cr4]
[GLOBAL _regrd_eflags]

[GLOBAL _regwr_cr0]
[GLOBAL _regwr_cr3]
[GLOBAL _regwr_cr4]

_gdt_flush:
    mov eax, [esp + 4]
    lgdt [eax]
    jmp 0x08:.done
.done:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret

_idt_flush:
    mov eax, [esp + 4]
    lidt [eax]
    jmp 0x08:.done
.done:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret

_regrd_cr0:
    mov eax, cr0
    retn

_regrd_cr2:
    mov eax, cr2
    retn

_regrd_cr3:
    mov eax, cr3
    retn
    
_regrd_cr4:
    mov eax, cr4
    retn

_regrd_eflags:
    pushf
    pop eax
    retn

_regwr_cr0:
    mov eax, [esp+4]
    mov cr0, eax
    ret

_regwr_cr3:
    mov eax, [esp+4]
    mov cr3, eax
    ret

_regwr_cr4:
    mov eax, [esp+4]
    mov cr4, eax
    ret