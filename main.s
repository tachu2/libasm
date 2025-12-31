section .data
str: db 'hello', 10, 0
result_msg: db 'ft_strlen returned: '
result_buf: times 20 db 0
newline: db 10

section .text
global _start
extern ft_strlen

_start:
    ; ft_strlenを呼び出す
    mov rdi, str        ; 第1引数: 文字列のポインタ
    call ft_strlen      ; ft_strlenを呼び出し
    ; 戻り値はraxに格納される
    mov r8, rax         ; 戻り値をr8に保存

    ; 返り値を10進数文字列に変換
    mov rdi, result_buf
    mov rsi, r8
    call itoa
    mov r9, rax         ; 文字列の長さをr9に保存

    ; "ft_strlen returned: " を出力
    mov rax, 1          ; syscall: write
    mov rdi, 1          ; stdout
    mov rsi, result_msg
    mov rdx, 19         ; メッセージの長さ
    syscall

    ; 数値を出力
    mov rax, 1
    mov rdi, 1
    mov rsi, result_buf
    mov rdx, r9         ; itoaが返した文字列の長さを使用
    syscall

    ; 改行を出力
    mov rax, 1
    mov rdi, 1
    mov rsi, newline
    mov rdx, 1
    syscall

    ; 元の文字列を出力
    mov rax, 1          ; syscall: write
    mov rdi, 1          ; stdout
    mov rsi, str        ; メッセージ
    mov rdx, r8         ; ft_strlenの戻り値を使用
    syscall

    mov rax, 60         ; syscall: exit
    xor rdi, rdi
    syscall

; itoa: 数値を文字列に変換
; rdi: バッファのポインタ
; rsi: 数値
; 戻り値: raxに文字列の長さ
itoa:
    push rbx
    push rcx
    push rdx
    mov rax, rsi
    mov rbx, 10
    mov rcx, 0
    cmp rax, 0
    jne .loop
    mov byte [rdi], '0'
    mov byte [rdi+1], 0
    mov rax, 1          ; 長さ1を返す
    jmp .done
.loop:
    xor rdx, rdx
    div rbx
    add dl, '0'
    push rdx
    inc rcx
    cmp rax, 0
    jne .loop
    mov rbx, rdi
    mov rax, rcx        ; 長さをraxに保存
.reverse:
    pop rdx
    mov [rbx], dl
    inc rbx
    loop .reverse
    mov byte [rbx], 0
.done:
    pop rdx
    pop rcx
    pop rbx
    ret

