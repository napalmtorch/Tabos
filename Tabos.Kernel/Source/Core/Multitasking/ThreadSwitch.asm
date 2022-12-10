[global _context_switch]
[extern THIS_THREAD]
[extern NEXT_THREAD]

_context_switch:
    mov eax, [THIS_THREAD]

    mov [eax + 0],  edi
    mov [eax + 4],  esi
    mov [eax + 8],  ebp
    mov [eax + 12], esp
    mov [eax + 16], ebx
    mov [eax + 20], edx
    mov [eax + 24], ecx
    mov [eax + 28], eax

    pushf
    pop ecx
    mov [eax + 32], ecx

    mov eax, [NEXT_THREAD]
    mov [THIS_THREAD], eax

    mov ecx, [eax + 32]
    push ecx
    popf

    mov edi, [eax + 0]
    mov esi, [eax + 4]
    mov ebp, [eax + 8]
    mov esp, [eax + 12]
    mov ebx, [eax + 16]
    mov edx, [eax + 20]
    mov ecx, [eax + 24]
    mov eax, [eax + 28]

    sti
    ret