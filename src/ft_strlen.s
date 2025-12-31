global ft_strlen

; @param rdi: pointer to the string
; @returns rax: length of the string
ft_strlen:
  mov rcx, 0
  jmp .loop

.loop:
  mov al, [rdi] ; al=raxの下位8bits
  cmp al, 0 ; null文字比較
  je .done ; trueなら.doneにjump
  inc rcx
  inc rdi
  jmp .loop

.done:
  mov rax, rcx
  ret
