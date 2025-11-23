; Function implemented in assembly is responsible for calculating and converting the 
; data type from the input int pixels into the output double float pixels.
section .data
    const_one dq 1.0      ; Constant 1.0 as double
    const_255 dq 255.0    ; Constant 255.0 as double

section .text
    global imgCvtGrayIntToDouble

imgCvtGrayIntToDouble:
    ; rcx = in (int *), rdx = out (double *), r8d = n (int)
    
    push rbp
    mov rbp, rsp
    push rbx
    
    mov r9, rcx        ; r9 = in
    mov r10, rdx       ; r10 = out
    mov ecx, r8d       ; ecx = n (loop counter)
    test ecx, ecx
    jz .done
    
    ; Compute 1/255 = 1.0 / 255.0 (scalar SIMD division)
    movsd xmm2, QWORD [rel const_one]
    movsd xmm3, QWORD [rel const_255]
    divsd xmm2, xmm3   ; xmm2 = 1.0 / 255.0

.loop:
    ; Load int from [r9]
    mov eax, DWORD [r9]
    
    ; Convert int in eax to double in xmm0 (scalar SIMD)
    cvtsi2sd xmm0, eax
    
    ; Multiply by 1/255 (scalar SIMD floating-point)
    mulsd xmm0, xmm2   ; Use precomputed 1/255 in xmm2
    
    ; Store result to [r10]
    movsd QWORD [r10], xmm0
    
    add r9, 4          ; Advance input pointer by 4 bytes (int)
    add r10, 8         ; Advance output pointer by 8 bytes (double)
    dec ecx
    jnz .loop

.done:
    pop rbx
    pop rbp
    ret
