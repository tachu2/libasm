section .data
str: db 'hello', 10, 0
result_msg: db 'ft_strlen returned: '
result_buf: times 20 db 0
msg: db 'hello 2026', 10, 0
newline: db 10
read_buf: times 100 db 0
test_file: db '/tmp/ft_read_test.txt', 0
test_content: db 'This is a test file for ft_read!', 10, 0

section .text
global _start
extern ft_strlen
extern ft_write
extern ft_read

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
    mov rdi, 1          ; fd: stdout
    mov rsi, result_msg ; buf: メッセージ
    mov rdx, 19         ; count: メッセージの長さ
    call ft_write       ; ft_writeを呼び出し

    ; msgを出力
    mov rdi, msg        ; 文字列のポインタを設定
    call print          ; print関数を呼び出し

    ; 数値を出力
    mov rdi, 1          ; fd: stdout
    mov rsi, result_buf ; buf: 数値文字列
    mov rdx, r9         ; count: itoaが返した文字列の長さを使用
    call ft_write       ; ft_writeを呼び出し

    ; 改行を出力
    mov rdi, 1          ; fd: stdout
    mov rsi, newline    ; buf: 改行文字
    mov rdx, 1          ; count: 1バイト
    call ft_write       ; ft_writeを呼び出し

    ; 元の文字列を出力
    mov rdi, 1          ; fd: stdout
    mov rsi, str        ; buf: メッセージ
    mov rdx, r8         ; count: ft_strlenの戻り値を使用
    call ft_write       ; ft_writeを呼び出し

    ; テストファイルを作成して書き込む
    mov rax, 2          ; syscall: open
    mov rdi, test_file  ; ファイル名
    mov rsi, 577        ; O_CREAT | O_WRONLY | O_TRUNC (64+1+512)
    mov rdx, 0644       ; パーミッション
    syscall
    mov r10, rax        ; ファイルディスクリプタをr10に保存

    ; テストファイルにデータを書き込む
    mov rdi, r10        ; fd
    mov rsi, test_content ; buf
    mov rdx, 35         ; count: テストコンテンツの長さ
    call ft_write       ; ft_writeを呼び出し

    ; ファイルを閉じる
    mov rax, 3          ; syscall: close
    mov rdi, r10
    syscall

    ; ファイルを開いて読み込む
    mov rax, 2          ; syscall: open
    mov rdi, test_file  ; ファイル名
    mov rsi, 0          ; O_RDONLY
    syscall
    mov r10, rax        ; ファイルディスクリプタをr10に保存

    ; ft_readでファイルから読み込む
    mov rdi, r10        ; fd
    mov rsi, read_buf   ; buf
    mov rdx, 100        ; count: 最大100バイト
    call ft_read        ; ft_readを呼び出し
    mov r11, rax        ; 読み込んだバイト数をr11に保存

    ; ファイルを閉じる
    mov rax, 3          ; syscall: close
    mov rdi, r10
    syscall

    ; 読み込んだデータを出力
    mov rdi, 1          ; fd: stdout
    mov rsi, read_buf   ; buf: 読み込んだデータ
    mov rdx, r11        ; count: 読み込んだバイト数
    call ft_write       ; ft_writeを呼び出し

    ; テストファイルを削除
    mov rax, 87         ; syscall: unlink
    mov rdi, test_file
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


; print: 文字列を標準出力に出力
; @param rdi: 文字列のポインタ
print:
    push rdi            ; 文字列ポインタを保存（ft_strlenの引数として使用）
    
    call ft_strlen      ; 文字列の長さを取得（raxに返る）
    mov rdx, rax        ; 長さをrdxに設定
    
    pop rsi             ; 文字列ポインタをrsiに復元（ft_writeのbuf引数）
    mov rdi, 1          ; fd: stdout
    ; rdxは既に文字列の長さが設定されている
    call ft_write       ; ft_writeを呼び出し
    
    ret