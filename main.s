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
    
    ; エコー処理を関数として呼び出し
    call echo_loop

    ; 終了処理
    mov rax, 60         ; syscall: exit
    xor rdi, rdi        ; exit code: 0
    syscall

; 標準入力を標準出力にエコーする関数
echo_loop:
    push rbp
    mov rbp, rsp
    sub rsp, 1024       ; スタック上に1024バイトのバッファを確保

.loop:
    mov rdi, 0          ; fd: stdin
    mov rsi, rsp        ; buf: 現在のスタックポインタ（バッファ先頭）
    mov rdx, 1024       ; count
    call ft_read
    
    test rax, rax       ; check read result
    js .done            ; error (ignore and exit loop)
    jz .done            ; EOF
    
    mov rdi, 1          ; fd: stdout
    mov rsi, rsp        ; buf
    mov rdx, rax        ; count: bytes read
    call ft_write
    
    jmp .loop

.done:
    leave               ; mov rsp, rbp; pop rbp と同義
    ret

print:
    push rdi            ; 文字列ポインタを保存
    
    call ft_strlen      ; 文字列の長さを取得
    mov rdx, rax        ; 長さ
    
    pop rsi             ; 文字列ポインタを復元
    mov rdi, 1          ; fd: stdout
    call ft_write
    
    ret