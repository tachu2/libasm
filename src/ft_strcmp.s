global ft_strcmp

; @param rdi: pointer to s1
; @param rsi: pointer to s2
; @returns rax: 0, if the s1 and s2 are equal;
; a negative value if s1 is less than s2; a positive value if s1 is greater than s2.
ft_strcmp:
  jmp .loop

.loop:
  mov al, [rdi] 
  mov bl, [rsi] 
  cmp al, bl          ; 比較（フラグ更新、値は維持）
  jne .done           ; 差があればジャンプ
  test al, al         ; 終端チェック（値が残っているので可能）
  jz .done            ; 終端ならジャンプ
  inc rdi
  inc rsi
  jmp .loop

.done:
  movzx rax, al       ; al (s1の1文字) を rax に拡張（8ビット→64ビット、符号なしとして扱う）
  movzx rbx, bl       ; bl (s2の1文字) を rbx に拡張（8ビット→64ビット、符号なしとして扱う）
  sub rax, rbx        ; rax - rbx を計算して返り値を設定
  ret
