global ft_list_remove_if
extern free

; void ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)(), void (*free_fct)(void *));
; @param rdi: t_list **begin_list
; @param rsi: void *data_ref
; @param rdx: int (*cmp)(void *, void *)
; @param rcx: void (*free_fct)(void *)

ft_list_remove_if:
    push rbp              ; 呼び出し元のベースポインタを保存
    mov rbp, rsp           ; スタックフレームポインタを設定
    push r12
    push r13
    push r14
    push r15
    push rbx
    sub rsp, 8         ; スタックアライメント調整（合計プッシュ: 8*6 + 8 = 56バイト。16バイト境界に揃えるため8バイト追加が必要）

    test rdi, rdi
    jz .done
    test rdx, rdx
    jz .done
    test rcx, rcx
    jz .done

    mov r12, rdi       ; r12 = current_ptr_ptr (address of the pointer to current node)
    mov r13, rsi       ; r13 = data_ref
    mov r14, rdx       ; r14 = cmp
    mov r15, rcx       ; r15 = free_fct
    jmp .loop

.loop:
    mov rbx, [r12]     ; rbx = *current_ptr_ptr (current node)
    test rbx, rbx      ; if (current == NULL) break
    jz .done

    ; cmp(current->data, data_ref)
    mov rdi, [rbx]     ; arg1: current->data
    mov rsi, r13       ; arg2: data_ref
    call r14           ; result in eax
    test eax, eax
    jz .remove         ; if (result == 0) remove

    ; No remove: advance pointer
    lea r12, [rbx + 8] ; current_ptr_ptr = &current->next
    jmp .loop

.remove:
    ; free_fct(current->data)
    mov rdi, [rbx]     ; arg1: current->data
    call r15

    ; Unlink node: *current_ptr_ptr = current->next
    mov r8, [rbx + 8]  ; r8 = current->next
    mov [r12], r8      ; *prev_next = next

    ; free(current)
    mov rdi, rbx       ; arg1: current node
    call free wrt ..plt

    ; Do not advance r12, check the new node at [r12] next time
    jmp .loop

.done:
    add rsp, 8
    pop rbx
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbp
    ret
