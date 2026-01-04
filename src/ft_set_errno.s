global ft_set_errno:function hidden
extern __errno_location

; システムコールのエラー処理
; @param rax: システムコールの戻り値（負の値、エラーコード）
; @returns rax: -1
; 副作用: errnoにエラーコードを設定
ft_set_errno:
  mov r10, rax        ; エラーコードをr10に保存（負の値）
  neg r10             ; 絶対値に変換（正の値にする）
  
  call __errno_location WRT ..plt ; errnoのアドレスを取得（raxに返る）
  mov [rax], r10      ; errnoにエラーコードを設定
  
  mov rax, -1         ; -1を返す
  ret
