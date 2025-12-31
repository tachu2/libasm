#include <iostream>
#include <cstdlib>
#include <cassert>

// リンクリストの構造体定義
struct s_list {
    void *data;
    struct s_list *next;
};

typedef struct s_list t_list;

extern "C" {
    void ft_list_push_front(t_list **begin_list, void *data);
}

// リンクリストを解放するヘルパー関数
static void free_list(t_list *list) {
    while (list) {
        t_list *next = list->next;
        free(list);
        list = next;
    }
}

// リンクリストのサイズを取得するヘルパー関数
static int list_size(t_list *list) {
    int count = 0;
    while (list) {
        count++;
        list = list->next;
    }
    return count;
}

// リンクリストの内容を検証するヘルパー関数
static bool verify_list(t_list *list, void **expected_data, int size) {
    for (int i = 0; i < size; i++) {
        if (!list || list->data != expected_data[i]) {
            return false;
        }
        list = list->next;
    }
    return list == nullptr;
}

static bool test_push_front(const char *test_name, t_list **list, void *data, int expected_size) {
    ft_list_push_front(list, data);
    int actual_size = list_size(*list);
    
    if (actual_size == expected_size) {
        std::cout << "✓ " << test_name << ": PASSED (expected size: " << expected_size 
                  << ", got: " << actual_size << ")" << std::endl;
        return true;
    } else {
        std::cerr << "✗ " << test_name << ": FAILED (expected size: " << expected_size 
                  << ", got: " << actual_size << ")" << std::endl;
        return false;
    }
}

int test_ft_list_push_front() {
    std::cout << "Testing ft_list_push_front..." << std::endl;
    int failed = 0;
    
    // NULL ポインタへの push（エラーケース、何もしない）
    t_list **null_ptr = nullptr;
    ft_list_push_front(null_ptr, (void*)1);
    std::cout << "✓ NULL pointer: PASSED (no crash)" << std::endl;
    
    // 空のリストに push
    t_list *list1 = nullptr;
    t_list **begin1 = &list1;
    if (!test_push_front("Push to empty list", begin1, (void*)1, 1)) failed++;
    if (list1 && list1->data != (void*)1) {
        std::cerr << "✗ Push to empty list: FAILED (wrong data)" << std::endl;
        failed++;
    }
    free_list(list1);
    
    // 1ノードのリストに push
    t_list *list2 = nullptr;
    t_list **begin2 = &list2;
    ft_list_push_front(begin2, (void*)1);
    if (!test_push_front("Push to single node list", begin2, (void*)2, 2)) failed++;
    // 順序を確認: [2, 1]
    if (list2 && list2->data == (void*)2 && list2->next && list2->next->data == (void*)1) {
        std::cout << "✓ Push to single node list: PASSED (correct order)" << std::endl;
    } else {
        std::cerr << "✗ Push to single node list: FAILED (wrong order)" << std::endl;
        failed++;
    }
    free_list(list2);
    
    // 複数ノードのリストに push
    t_list *list3 = nullptr;
    t_list **begin3 = &list3;
    ft_list_push_front(begin3, (void*)3);
    ft_list_push_front(begin3, (void*)2);
    if (!test_push_front("Push to multi-node list", begin3, (void*)1, 3)) failed++;
    // 順序を確認: [1, 2, 3]
    void *expected[] = {(void*)1, (void*)2, (void*)3};
    if (verify_list(list3, expected, 3)) {
        std::cout << "✓ Push to multi-node list: PASSED (correct order)" << std::endl;
    } else {
        std::cerr << "✗ Push to multi-node list: FAILED (wrong order)" << std::endl;
        failed++;
    }
    free_list(list3);
    
    // 複数回 push して順序を確認
    t_list *list4 = nullptr;
    t_list **begin4 = &list4;
    for (int i = 5; i >= 1; i--) {
        ft_list_push_front(begin4, (void*)(long)i);
    }
    // 順序を確認: [1, 2, 3, 4, 5]
    void *expected2[] = {(void*)1, (void*)2, (void*)3, (void*)4, (void*)5};
    if (list_size(list4) == 5 && verify_list(list4, expected2, 5)) {
        std::cout << "✓ Multiple pushes: PASSED (correct order and size)" << std::endl;
    } else {
        std::cerr << "✗ Multiple pushes: FAILED" << std::endl;
        failed++;
    }
    free_list(list4);
    
    return failed;
}
