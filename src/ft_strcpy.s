global ft_strcpy

; @param rdi: pointer to dst
; @param rsi: pointer to src
; @returns rax: pointer to dst
ft_strcpy:
  mov rcx, rdi
  jmp .loop

.loop:
  mov al, [rsi] ; srcから1バイト読み込む
  mov [rdi], al ; dstに1バイトコピー
  cmp al, 0     ; null文字比較
  je .done      ; null終端文字なら.doneにjump
  inc rdi       ; dstポインタを進める
  inc rsi       ; srcポインタを進める
  jmp .loop

.done:
  mov rax, rcx
  ret
