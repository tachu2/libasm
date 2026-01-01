section .data
    msg_hello: db 'hello 2026', 10, 0
    prompt:    db 'Enter two numbers: ', 0
    fmt_in:    db '%d %d', 0
    fmt_out:   db 'The sum is: %d', 10, 0
    msg_echo:  db 10, '--- Echo Mode (Ctrl+D to exit) ---', 10, 0
    msg_aa:    db 10, '   /\_/\  ', 10, '  ( o.o ) ', 10, '   > ^ <  ', 10, '  Bye Bye!', 10, 0

section .text
    global _start
    ; C標準ライブラリ
    extern printf
    extern scanf
    ; libasmの関数
    extern ft_read
    extern ft_write
    extern ft_strlen

_start:
    ; --- 初期メッセージ ---
    ; 16バイトアライメントのためのダミープッシュは不要だが、printf呼び出し前なので
    ; 現在のrspが16の倍数になっていることを確認する必要がある。
    ; _start開始時、rspは16の倍数になっているはず（ABIによる）
    
    mov rdi, msg_hello
    xor eax, eax
    call printf

    ; --- 計算処理の呼び出し ---
    call calc_loop

    ; --- エコー処理の呼び出し ---
    mov rdi, msg_echo
    xor eax, eax
    call printf
    
    call echo_loop

    ; --- AA出力 (猫) ---
    mov rdi, msg_aa
    xor eax, eax
    call printf

    ; --- 終了 ---
    mov rax, 60         ; syscall: exit
    xor rdi, rdi
    syscall

; --- Functions ---

; 数値計算を行う関数
calc_loop:
    push rbp
    mov rbp, rsp
    sub rsp, 16         ; 2つのint変数のための領域確保 & 16バイトアライメント維持
                        ; push rbp(8) + sub 16 = 24... あれ、これだとアライメントずれる？
                        ; call calc_loopでpush rip(8)される。
                        ; calc_loop突入時: rsp = 8 mod 16
                        ; push rbp: rsp = 0 mod 16
                        ; sub rsp, 16: rsp = 0 mod 16 -> OK

    ; printf(prompt)
    mov rdi, prompt
    xor eax, eax
    call printf

    ; scanf(fmt_in, &num1, &num2)
    mov rdi, fmt_in
    lea rsi, [rbp - 4]    ; rsi = &num1 (第2引数: 1つ目の数値のアドレス)
    lea rdx, [rbp - 8]    ; rdx = &num2 (第3引数: 2つ目の数値のアドレス)
    xor eax, eax          ; 可変引数関数用: 浮動小数点引数の数 = 0
    call scanf

    ; 計算
    mov eax, [rbp - 4]
    add eax, [rbp - 8]

    ; printf(fmt_out, sum)
    mov rdi, fmt_out
    mov esi, eax
    xor eax, eax
    call printf

    leave
    ret

; 標準入力を標準出力にエコーする関数
echo_loop:
    push rbp
    mov rbp, rsp
    sub rsp, 1024       ; スタックバッファ確保
                        ; push rbp(8) -> rsp=0 mod 16. sub 1024 -> rsp=0 mod 16. OK.

.loop:
    mov rdi, 0          ; stdin
    mov rsi, rsp
    mov rdx, 1024
    call ft_read
    
    test rax, rax
    js .done            ; error
    jz .done            ; EOF
    
    mov rdi, 1          ; stdout
    mov rsi, rsp
    mov rdx, rax
    call ft_write
    
    jmp .loop

.done:
    leave
    ret