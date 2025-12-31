global ft_write
extern ft_set_errno

; @param rdi: fd (file descriptor)
; @param rsi: buf (pointer to the buffer)
; @param rdx: count (number of bytes to write)
; @returns rax: number of bytes written, or -1 on error
ft_write:
  mov rax, 1          ; syscall: write (1)
  syscall             ; write(fd, buf, count)
  
  ; エラーチェック: システムコールが負の値を返した場合
  cmp rax, 0
  jl .fail            ; rax < 0 ならエラー処理へ
  ret                 ; return value is in rax
  
.fail:
  call ft_set_errno   ; errnoを設定して-1を返す