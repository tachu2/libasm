global ft_list_size

;typedef struct s_list {
;    void *data;
;    struct s_list *next;
;} t_list

; int ft_list_size(t_list *begin_list)
; @param rdi: pointer to list
; @returns rax: list size.
ft_list_size:
  mov rax, 0
  jmp .loop

.loop:
  test rdi, rdi ; null判定
  jz .done
  mov rdi, [rdi + 8] ; data文の8bytesを飛ばす

  inc rax
  jmp .loop

.done:
  ret