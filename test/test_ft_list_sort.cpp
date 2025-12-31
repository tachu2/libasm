#include <iostream>
#include <cstdlib>
#include <cstring>

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

// 整数比較関数（昇順）
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

// リンクリストのノードを作成するヘルパー関数
static t_list* create_node(void *data) {
    t_list *node = (t_list*)malloc(sizeof(t_list));
    if (node) {
        node->data = data;
        node->next = nullptr;
    }
    return node;
}

// 配列からリンクリストを作成するヘルパー関数
static t_list* create_list(void **data_array, int size) {
    if (size == 0) {
        return nullptr;
    }
    
    t_list *head = create_node(data_array[0]);
    t_list *current = head;
    
    for (int i = 1; i < size; i++) {
        current->next = create_node(data_array[i]);
        current = current->next;
    }
    
    return head;
}

// リンクリストを解放するヘルパー関数
static void free_list(t_list *list) {
    while (list) {
        t_list *next = list->next;
        free(list);
        list = next;
    }
}

// 文字列リストを解放するヘルパー関数
static void free_str_list(t_list *list) {
    while (list) {
        t_list *next = list->next;
        if (list->data) {
            free(list->data);
        }
        free(list);
        list = next;
    }
}

// リンクリストの内容を検証するヘルパー関数（整数用）
static bool verify_int_list(t_list *list, long *expected_data, int size) {
    for (int i = 0; i < size; i++) {
        if (!list || (long)list->data != expected_data[i]) {
            return false;
        }
        list = list->next;
    }
    return list == nullptr;
}

// リンクリストの内容を検証するヘルパー関数（文字列用）
static bool verify_str_list(t_list *list, const char **expected_data, int size) {
    for (int i = 0; i < size; i++) {
        if (!list || strcmp((const char*)list->data, expected_data[i]) != 0) {
            return false;
        }
        list = list->next;
    }
    return list == nullptr;
}

// 文字列をコピーするヘルパー関数
static char* strdup_custom(const char *s) {
    size_t len = strlen(s) + 1;
    char *dup = (char*)malloc(len);
    if (dup) {
        memcpy(dup, s, len);
    }
    return dup;
}

static bool test_sort(const char *test_name, t_list **list, int (*cmp)(void*, void*), 
                      void **expected_data, int size, bool is_string = false) {
    ft_list_sort(list, cmp);
    
    bool result = false;
    if (is_string) {
        result = verify_str_list(*list, (const char**)expected_data, size);
    } else {
        result = verify_int_list(*list, (long*)expected_data, size);
    }
    
    if (result) {
        std::cout << "✓ " << test_name << ": PASSED" << std::endl;
        return true;
    } else {
        std::cerr << "✗ " << test_name << ": FAILED" << std::endl;
        return false;
    }
}

int test_ft_list_sort() {
    std::cout << "Testing ft_list_sort..." << std::endl;
    int failed = 0;
    
    // NULL ポインタのテスト
    t_list **null_ptr = nullptr;
    ft_list_sort(null_ptr, int_cmp);
    std::cout << "✓ NULL begin_list: PASSED (no crash)" << std::endl;
    
    t_list *list_null = nullptr;
    t_list **begin_null = &list_null;
    ft_list_sort(begin_null, nullptr);
    std::cout << "✓ NULL cmp function: PASSED (no crash)" << std::endl;
    
    // 空のリスト
    t_list *empty_list = nullptr;
    t_list **begin_empty = &empty_list;
    ft_list_sort(begin_empty, int_cmp);
    std::cout << "✓ Empty list: PASSED (no crash)" << std::endl;
    
    // 1ノードのリスト（既にソート済み）
    void *data1[] = {(void*)5};
    t_list *list1 = create_list(data1, 1);
    t_list **begin1 = &list1;
    long expected1[] = {5};
    if (!test_sort("Single node", begin1, int_cmp, (void**)expected1, 1)) failed++;
    free_list(list1);
    
    // 2ノードのリスト（既にソート済み）
    void *data2_sorted[] = {(void*)1, (void*)2};
    t_list *list2_sorted = create_list(data2_sorted, 2);
    t_list **begin2_sorted = &list2_sorted;
    long expected2_sorted[] = {1, 2};
    if (!test_sort("Two nodes (already sorted)", begin2_sorted, int_cmp, 
                   (void**)expected2_sorted, 2)) failed++;
    free_list(list2_sorted);
    
    // 2ノードのリスト（逆順）
    void *data2_reverse[] = {(void*)2, (void*)1};
    t_list *list2_reverse = create_list(data2_reverse, 2);
    t_list **begin2_reverse = &list2_reverse;
    long expected2_reverse[] = {1, 2};
    if (!test_sort("Two nodes (reverse order)", begin2_reverse, int_cmp, 
                   (void**)expected2_reverse, 2)) failed++;
    free_list(list2_reverse);
    
    // 3ノードのリスト（ランダム順）
    void *data3[] = {(void*)3, (void*)1, (void*)2};
    t_list *list3 = create_list(data3, 3);
    t_list **begin3 = &list3;
    long expected3[] = {1, 2, 3};
    if (!test_sort("Three nodes (random order)", begin3, int_cmp, 
                   (void**)expected3, 3)) failed++;
    free_list(list3);
    
    // 5ノードのリスト（逆順）
    void *data5[] = {(void*)5, (void*)4, (void*)3, (void*)2, (void*)1};
    t_list *list5 = create_list(data5, 5);
    t_list **begin5 = &list5;
    long expected5[] = {1, 2, 3, 4, 5};
    if (!test_sort("Five nodes (reverse order)", begin5, int_cmp, 
                   (void**)expected5, 5)) failed++;
    free_list(list5);
    
    // 5ノードのリスト（ランダム順）
    void *data5_random[] = {(void*)3, (void*)1, (void*)5, (void*)2, (void*)4};
    t_list *list5_random = create_list(data5_random, 5);
    t_list **begin5_random = &list5_random;
    long expected5_random[] = {1, 2, 3, 4, 5};
    if (!test_sort("Five nodes (random order)", begin5_random, int_cmp, 
                   (void**)expected5_random, 5)) failed++;
    free_list(list5_random);
    
    // 文字列リストのテスト
    t_list *str_list = nullptr;
    t_list **begin_str = &str_list;
    
    // 文字列を追加（逆順）
    const char *strs[] = {"zebra", "apple", "banana", "cherry"};
    for (int i = 0; i < 4; i++) {
        t_list *node = create_node(strdup_custom(strs[i]));
        node->next = *begin_str;
        *begin_str = node;
    }
    
    const char *expected_strs[] = {"apple", "banana", "cherry", "zebra"};
    if (!test_sort("String list (alphabetical)", begin_str, str_cmp_wrapper, 
                   (void**)expected_strs, 4, true)) failed++;
    free_str_list(str_list);
    
    // 重複要素を含むリスト
    void *data_dup[] = {(void*)3, (void*)1, (void*)3, (void*)2, (void*)1};
    t_list *list_dup = create_list(data_dup, 5);
    t_list **begin_dup = &list_dup;
    long expected_dup[] = {1, 1, 2, 3, 3};
    if (!test_sort("List with duplicates", begin_dup, int_cmp, 
                   (void**)expected_dup, 5)) failed++;
    free_list(list_dup);
    
    return failed;
}
