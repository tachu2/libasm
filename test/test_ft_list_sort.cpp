#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <functional>

// リンクリストの構造体定義
struct s_list {
    void *data;
    struct s_list *next;
};

typedef struct s_list t_list;

extern "C" {
    void ft_list_sort(t_list **begin_list, int (*cmp)(void*, void*));
    int ft_strcmp(const char *s1, const char *s2);
}

// 比較関数
static int int_cmp(void *a, void *b) {
    long int_a = (long)a;
    long int_b = (long)b;
    if (int_a < int_b) return -1;
    if (int_a > int_b) return 1;
    return 0;
}

static int str_cmp_wrapper(void *a, void *b) {
    return ft_strcmp((const char*)a, (const char*)b);
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

// ヘルパー: 文字列複製
static char* strdup_custom(const char *s) {
    size_t len = strlen(s) + 1;
    char *dup = (char*)malloc(len);
    if (dup) {
        memcpy(dup, s, len);
    }
    return dup;
}

// 汎用テストランナー
template <typename T>
static bool run_sort_test(const char* test_name, 
                          const std::vector<T>& input_data, 
                          const std::vector<T>& expected_data, 
                          int (*cmp)(void*, void*),
                          bool is_string = false) {
    
    // 1. リスト作成
    t_list *head = nullptr;
    t_list *current = nullptr;

    for (const auto& item : input_data) {
        void* data_ptr;
        if constexpr (std::is_same<T, const char*>::value || std::is_same<T, char*>::value || std::is_same<T, std::string>::value) {
             // 文字列の場合、複製してポインタを渡す（C文字列として扱う）
             // std::string の場合も考慮して .c_str() を使うか、テンプレート特化が必要だが
             // 今回は const char* を想定
             data_ptr = (void*)strdup_custom((const char*)item);
        } else {
            // 整数の場合、値をポインタとしてキャスト
            data_ptr = (void*)(long)item;
        }

        t_list* new_node = create_node(data_ptr);
        if (!head) {
            head = new_node;
            current = head;
        } else {
            current->next = new_node;
            current = current->next;
        }
    }

    t_list **begin_list = &head;

    // 2. ソート実行
    ft_list_sort(begin_list, cmp);

    // 3. 検証
    bool passed = true;
    t_list *check_node = *begin_list;
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
    if (check_node != nullptr) passed = false; // リストが長すぎる場合

    // 結果出力
    if (passed) {
        std::cout << "✓ " << test_name << ": PASSED" << std::endl;
    } else {
        std::cerr << "✗ " << test_name << ": FAILED" << std::endl;
    }

    free_list(*begin_list, is_string); // 文字列ならデータも解放

    return passed;
}


int test_ft_list_sort() {
    std::cout << "Testing ft_list_sort..." << std::endl;
    int failed = 0;
    
    // NULL ポインタテスト (手動)
    t_list **null_ptr = nullptr;
    ft_list_sort(null_ptr, int_cmp);
    std::cout << "✓ NULL begin_list: PASSED (no crash)" << std::endl;
    
    t_list *list_null = nullptr;
    t_list **begin_null = &list_null;
    ft_list_sort(begin_null, nullptr);
    std::cout << "✓ NULL cmp function: PASSED (no crash)" << std::endl;
    
    t_list *empty_list = nullptr;
    t_list **begin_empty = &empty_list;
    ft_list_sort(begin_empty, int_cmp);
    std::cout << "✓ Empty list: PASSED (no crash)" << std::endl;


    // 整数テスト
    if (!run_sort_test<long>("Single node", {5}, {5}, int_cmp)) failed++;
    if (!run_sort_test<long>("Two nodes (sorted)", {1, 2}, {1, 2}, int_cmp)) failed++;
    if (!run_sort_test<long>("Two nodes (reverse)", {2, 1}, {1, 2}, int_cmp)) failed++;
    if (!run_sort_test<long>("Three nodes (random)", {3, 1, 2}, {1, 2, 3}, int_cmp)) failed++;
    if (!run_sort_test<long>("Five nodes (reverse)", {5, 4, 3, 2, 1}, {1, 2, 3, 4, 5}, int_cmp)) failed++;
    if (!run_sort_test<long>("Five nodes (random)", {3, 1, 5, 2, 4}, {1, 2, 3, 4, 5}, int_cmp)) failed++;
    if (!run_sort_test<long>("List with duplicates", {3, 1, 3, 2, 1}, {1, 1, 2, 3, 3}, int_cmp)) failed++;

    // 文字列テスト
    // 入力は逆順で作成していたので { "cherry", "banana", "apple", "zebra" } の順で追加
    // create_listは配列順に追加するので、期待する入力順序をそのまま書けばよい
    // 元のテスト: strs[] = {"zebra", "apple", "banana", "cherry"}; loopでpush_frontしていた -> cherry, banana, apple, zebra の順
    std::vector<const char*> str_input = {"cherry", "banana", "apple", "zebra"};
    std::vector<const char*> str_expected = {"apple", "banana", "cherry", "zebra"};
    
    if (!run_sort_test<const char*>("String list", str_input, str_expected, str_cmp_wrapper, true)) failed++;

    return failed;
}
