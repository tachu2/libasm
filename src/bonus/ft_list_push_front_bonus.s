global ft_list_push_front

extern malloc

;typedef struct s_list {
;    void *data;
;    struct s_list *next;
;} t_list

; void ft_list_push_front(t_list **begin_list, void *data)
; @param rdi: t_list **begin_list (ダブルポインタ)
; @param rsi: void *data (新しいノードのデータ)
; 
; ダブルポインタの扱い:
; - rdi = begin_list (t_list **)
; - [rdi] = *begin_list (t_list *) - 実際のリストの先頭へのポインタ
ft_list_push_front:
  test rdi, rdi        ; begin_list (ダブルポインタ) が NULL かチェック
  jz .done
  
  ; 新しいノードを malloc で作成 (16バイト: data 8バイト + next 8バイト)
  push rdi             ; begin_list を保存
  push rsi             ; data を保存
  mov rdi, 16          ; malloc の引数: 16バイト
  call malloc          ; rax に新しいノードのポインタが返る
  pop rsi              ; data を復元
  pop rdi              ; begin_list を復元
  
  test rax, rax        ; malloc が NULL を返したかチェック
  jz .done             ; NULL なら終了
  
  ; 新しいノードの data フィールド (offset 0) に rsi を設定
  mov [rax], rsi       ; new_node->data = data
  
  ; 新しいノードの next フィールド (offset 8) に現在の先頭を設定
  mov rcx, [rdi]         ; rcx = *begin_list (現在の先頭)
  mov [rax + 8], rcx     ; new_node->next = *begin_list
  
  ; *begin_list を新しいノードに更新
  mov [rdi], rax       ; *begin_list = new_node
  ret

.done:
  ret