global ft_strlen

; @param rdi: pointer to the string
; @returns rax: length of the string
ft_strlen:
  mov rcx, 0
  jmp .loop

.loop:
  mov al, [rdi]
  cmp al, 0
  je .done
  inc rcx
  inc rdi
  jmp .loop

.done:
  mov rax, rcx
  ret
