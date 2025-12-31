#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <type_traits>

// リンクリストの構造体定義
struct s_list {
    void *data;
    struct s_list *next;
};

typedef struct s_list t_list;

extern "C" {
    void ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)(void*, void*), void (*free_fct)(void*));
    int ft_strcmp(const char *s1, const char *s2);
}

// ヘルパー: ノード作成
static t_list* create_node(void *data) {
    t_list *node = (t_list*)malloc(sizeof(t_list));
    if (node) {
        node->data = data;
        node->next = nullptr;
    }
    return node;
}

// ヘルパー: 文字列複製
static char* strdup_custom(const char *s) {
    size_t len = strlen(s) + 1;
    char *dup = (char*)malloc(len);
    if (dup) {
        memcpy(dup, s, len);
    }
    return dup;
}

// ヘルパー: リスト解放
static void free_list(t_list *list, bool free_data) {
    while (list) {
        t_list *next = list->next;
        if (free_data && list->data) {
            free(list->data);
        }
        free(list);
        list = next;
    }
}

// 比較関数
static int int_cmp(void *a, void *b) {
    long int_a = (long)a;
    long int_b = (long)b;
    return (int_a == int_b) ? 0 : 1;
}

static int str_cmp(void *a, void *b) {
    return strcmp((const char*)a, (const char*)b);
}

// 解放関数
static void free_int(void *data) {
    (void)data;
}

static void free_str(void *data) {
    free(data);
}

// 汎用テストランナー
template <typename T>
static bool run_remove_if_test(const char* test_name, 
                               const std::vector<T>& input_data, 
                               T data_ref_val,
                               const std::vector<T>& expected_data, 
                               int (*cmp)(void*, void*),
                               void (*free_fct)(void*),
                               bool is_string = false) {
    
    // 1. リスト作成
    t_list *head = nullptr;
    t_list *current = nullptr;

    for (const auto& item : input_data) {
        void* ptr;
        if constexpr (std::is_same<T, const char*>::value || std::is_same<T, char*>::value) {
            ptr = (void*)strdup_custom((const char*)item);
        } else {
            ptr = (void*)(long)item;
        }
        t_list* new_node = create_node(ptr);
        if (!head) head = new_node;
        else current->next = new_node;
        current = new_node;
    }

    // 2. data_ref の準備
    void* data_ref;
    if constexpr (std::is_same<T, const char*>::value || std::is_same<T, char*>::value) {
        data_ref = (void*)data_ref_val; // data_ref は比較にのみ使われ、free_fctには渡されない
    } else {
        data_ref = (void*)(long)data_ref_val;
    }

    // 3. 実行
    ft_list_remove_if(&head, data_ref, cmp, free_fct);

    // 4. 検証
    bool passed = true;
    t_list *check_node = head;
    for (const auto& expected : expected_data) {
        if (!check_node) {
            passed = false;
            break;
        }
        if (is_string) {
            if (strcmp((const char*)check_node->data, (const char*)expected) != 0) {
                passed = false;
                break;
            }
        } else {
            if ((long)check_node->data != (long)expected) {
                passed = false;
                break;
            }
        }
        check_node = check_node->next;
    }
    if (check_node != nullptr) passed = false;

    // 結果出力
    if (passed) {
        std::cout << "✓ " << test_name << ": PASSED" << std::endl;
    } else {
        std::cerr << "✗ " << test_name << ": FAILED" << std::endl;
    }

    // 5. 後片付け (残ったノードを解放)
    free_list(head, is_string);

    return passed;
}

int test_ft_list_remove_if() {
    std::cout << "Testing ft_list_remove_if..." << std::endl;
    int failed = 0;

    // 整数テスト
    if (!run_remove_if_test<long>("Remove middle", {1, 2, 3}, 2, {1, 3}, int_cmp, free_int)) failed++;
    if (!run_remove_if_test<long>("Remove head", {1, 2}, 1, {2}, int_cmp, free_int)) failed++;
    if (!run_remove_if_test<long>("Remove tail", {1, 2}, 2, {1}, int_cmp, free_int)) failed++;
    if (!run_remove_if_test<long>("Remove all", {1, 1}, 1, {}, int_cmp, free_int)) failed++;
    if (!run_remove_if_test<long>("Remove none", {1, 2, 3}, 4, {1, 2, 3}, int_cmp, free_int)) failed++;
    if (!run_remove_if_test<long>("Multiple removal", {1, 2, 1, 3, 1}, 1, {2, 3}, int_cmp, free_int)) failed++;

    // 文字列テスト
    if (!run_remove_if_test<const char*>("String remove", {"apple", "banana", "apple"}, "apple", {"banana"}, str_cmp, free_str, true)) failed++;
    if (!run_remove_if_test<const char*>("String remove none", {"a", "b"}, "c", {"a", "b"}, str_cmp, free_str, true)) failed++;

    // NULL パラメータテスト
    {
         t_list *head = nullptr;
         ft_list_remove_if(nullptr, nullptr, int_cmp, free_int);
         std::cout << "✓ NULL begin_list param: PASSED (no crash)" << std::endl;
         
         ft_list_remove_if(&head, nullptr, nullptr, free_int);
         std::cout << "✓ NULL cmp param: PASSED (no crash)" << std::endl;
         
         ft_list_remove_if(&head, nullptr, int_cmp, nullptr);
         std::cout << "✓ NULL free_fct param: PASSED (no crash)" << std::endl;
    }

    return failed;
}
