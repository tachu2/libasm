global ft_list_sort

;typedef struct s_list {
;    void *data;
;    struct s_list *next;
;} t_list

; void ft_list_sort(t_list **begin_list, int (*cmp)())
; @param rdi: t_list **begin_list (ダブルポインタ)
; @param rsi: int (*cmp)() (比較関数ポインタ)
; 
; バブルソートアルゴリズムを使用してリンクリストをソート
ft_list_sort:
  push r12
  push r13
  test rdi, rdi        ; begin_list が NULL かチェック
  jz .done
  test rsi, rsi        ; cmp が NULL かチェック
  jz .done
  
  mov r8, [rdi]        ; リストが空かチェック
  test r8, r8
  jz .done
  jmp .outer_loop
  
  ; 外側ループ: リスト全体を走査
.outer_loop:
  mov r8, rdi          ; r8 = pptr (current pointer address), init with begin_list
  mov r9, 0            ; swapped = false
  jmp .inner_loop
  
  ; 内側ループ: 隣接する要素を比較
.inner_loop:
  mov r10, [r8]        ; r10 = node1 (*pptr)
  test r10, r10        ; node1 == NULL?
  jz .check_swapped
  mov r11, [r10 + 8]   ; r11 = node2 (node1->next)
  test r11, r11        ; node2 == NULL?
  jz .check_swapped
  
  ; 比較関数を呼び出す準備
  ; cmp(node1->data, node2->data)
  push rdi             ; begin_list
  push rsi             ; cmp
  push r8              ; pptr
  push r9              ; swapped
  push r10             ; node1
  push r11             ; node2
  
  mov r13, rsi         ; cmp func
  mov rdi, [r10]       ; arg1: node1->data
  mov rsi, [r11]       ; arg2: node2->data
  
  call r13             ; cmp(node1->data, node2->data)
  
  pop r11
  pop r10
  pop r9
  pop r8
  pop rsi
  pop rdi
  
  test eax, eax        ; 比較結果をチェック
  jle .no_swap         ; <= 0 なら交換不要
  
  ; 交換: ノード自体をスワップ
  ; スワップ前: [pptr] -> node1 -> node2 -> next_next
  ; スワップ後: [pptr] -> node2 -> node1 -> next_next
  
  mov [r8], r11        ; *pptr = node2
  mov r12, [r11 + 8]   ; r12 = node2->next (next_next)
  mov [r10 + 8], r12   ; node1->next = next_next
  mov [r11 + 8], r10   ; node2->next = node1
  
  mov r9, 1            ; swapped = true
  
  ; 次の比較のために pptr を更新
  ; 次は node1 (現在は node2->next にある) とその次を比較したい
  lea r8, [r11 + 8]    ; pptr = &node2->next
  jmp .inner_loop
  
.no_swap:
  ; 交換しなかった場合
  ; 次は node2 (現在は node1->next にある) とその次を比較したい
  lea r8, [r10 + 8]    ; pptr = &node1->next
  jmp .inner_loop
  
.check_swapped:
  test r9, r9          ; swapped が true かチェック
  jnz .outer_loop      ; 交換があった場合は続ける
  jmp .done
  
.done:
  pop r13
  pop r12
  ret
