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
  
  mov r8, [rdi]        ; r8 = *begin_list (現在のノード)
  test r8, r8          ; リストが空かチェック
  jz .done
  jmp .outer_loop
  
  ; 外側ループ: リスト全体を走査
.outer_loop:
  mov r8, [rdi]        ; r8 = *begin_list (リセット)
  mov r9, 0            ; r9 = swapped flag (0 = false)
  jmp .inner_loop
  
  ; 内側ループ: 隣接する要素を比較
.inner_loop:
  test r8, r8          ; current が NULL かチェック
  jz .check_swapped
  mov r10, [r8 + 8]    ; r10 = current->next
  test r10, r10        ; next が NULL かチェック
  jz .check_swapped
  
  ; 比較関数を呼び出す準備
  ; cmp(current->data, current->next->data)
  push rdi             ; begin_list を保存
  push rsi             ; cmp 関数ポインタを保存
  push r8              ; current を保存
  push r10             ; next を保存
  push r9              ; swapped flag を保存
  
  mov r13, rsi         ; r13 = cmp 関数ポインタ（一時保存）
  mov rdi, [r8]        ; rdi = current->data (第1引数)
  mov rsi, [r10]       ; rsi = next->data (第2引数)
  
  ; 関数ポインタを呼び出す
  call r13             ; cmp(current->data, next->data) を呼び出す
  ; rax に比較結果が返る（> 0 なら交換が必要）
  
  pop r9               ; swapped flag を復元
  pop r10              ; next を復元
  pop r8               ; current を復元
  pop rsi              ; cmp を復元
  pop rdi              ; begin_list を復元
  
  test eax, eax        ; 比較結果をチェック
  jle .next_pair       ; <= 0 なら交換不要
  
  ; 交換: current->data と next->data を交換
  mov r11, [r8]        ; r11 = current->data
  mov r12, [r10]       ; r12 = next->data
  mov [r8], r12        ; current->data = next->data
  mov [r10], r11       ; next->data = current->data
  mov r9, 1            ; swapped = true
  jmp .next_pair
  
.next_pair:
  mov r8, [r8 + 8]     ; current = current->next
  jmp .inner_loop
  
.check_swapped:
  test r9, r9          ; swapped が true かチェック
  jnz .outer_loop      ; 交換があった場合は続ける
  
.done:
  pop r13
  pop r12
  ret
