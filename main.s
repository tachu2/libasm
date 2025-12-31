section .data
msg: db 'hello 2026', 10, 0
newline: db 10

section .text
global _start
extern ft_strlen
extern ft_write
extern ft_read
extern ft_strdup

_start:
    mov rdi, msg
    call print
    
    ; exit システムコールで終了
    mov rax, 60         ; syscall: exit
    xor rdi, rdi        ; exit code: 0
    syscall

print:
    push rdi            ; 文字列ポインタを保存（ft_strlenの引数として使用）
    
    call ft_strlen      ; 文字列の長さを取得（raxに返る）
    mov rdx, rax        ; 長さをrdxに設定
    
    pop rsi             ; 文字列ポインタをrsiに復元（ft_writeのbuf引数）
    mov rdi, 1          ; fd: stdout
    ; rdxは既に文字列の長さが設定されている
    call ft_write       ; ft_writeを呼び出し
    
    ret