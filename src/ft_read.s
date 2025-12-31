global ft_read
extern ft_set_errno

; @param rdi: fd (file descriptor)
; @param rsi: buf (pointer to the buffer)
; @param rdx: count (number of bytes to read)
; @returns rax: number of bytes read, or -1 on error
ft_read:
  mov rax, 0          ; syscall: read (0)
  syscall             ; read(fd, buf, count)
  cmp rax, 0
  jl .err             ; rax < 0 ならエラー処理へ
  ret

.err:
  call ft_set_errno   ; errnoを設定して-1を返す
  ret