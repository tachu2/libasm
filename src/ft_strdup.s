global ft_strdup

extern malloc
extern ft_strlen
extern ft_strcpy

; @param rdi: pointer to the string to copy
; @returns rax: pointer to copied string
ft_strdup:
  push rdi
  call ft_strlen      ; rax に文字列の長さが返る

  inc rax             ; null文字用に1を足す
  mov rdi, rax        ; malloc の引数に設定
  call malloc         ; rax に新しいメモリ領域のポインタが返る

  pop rsi
  test rax, rax       ; malloc が NULL を返したかチェック
  jz .err             ; NULL ならそのまま返す
  
  mov rdi, rax        ; ft_strcpy の第1引数（dest）に設定
  call ft_strcpy      ; 文字列をコピー（rax に dest のポインタが返る）
  ret

.err:
  ret