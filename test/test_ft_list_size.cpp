#include <iostream>
#include <cstdlib>

// リンクリストの構造体定義
struct s_list {
    void *data;
    struct s_list *next;
};

typedef struct s_list t_list;

extern "C" {
    int ft_list_size(t_list *begin_list);
}

static bool test_list_size(const char *test_name, t_list *list, int expected) {
    int result = ft_list_size(list);
    
    if (result == expected) {
        std::cout << "✓ " << test_name << ": PASSED (expected: " << expected 
                  << ", got: " << result << ")" << std::endl;
        return true;
    } else {
        std::cerr << "✗ " << test_name << ": FAILED (expected: " << expected 
                  << ", got: " << result << ")" << std::endl;
        return false;
    }
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

// 指定されたサイズのリンクリストを作成するヘルパー関数（データは1から始まる連番）
static t_list* create_list_with_size(int size) {
    if (size == 0) {
        return nullptr;
    }
    
    t_list *head = create_node((void*)(long)1);
    t_list *current = head;
    
    for (int i = 2; i <= size; i++) {
        current->next = create_node((void*)(long)i);
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

int test_ft_list_size() {
    std::cout << "Testing ft_list_size..." << std::endl;
    int failed = 0;
    
    // NULL リスト
    if (!test_list_size("NULL list", nullptr, 0)) failed++;
    
    // 1ノードのリスト
    t_list *list1 = create_node((void*)1);
    if (!test_list_size("Single node", list1, 1)) failed++;
    free_list(list1);
    
    // 2ノードのリスト（配列から作成）
    void *data2[] = {(void*)1, (void*)2};
    t_list *list2 = create_list(data2, 2);
    if (!test_list_size("Two nodes", list2, 2)) failed++;
    free_list(list2);
    
    // 3ノードのリスト（配列から作成）
    void *data3[] = {(void*)1, (void*)2, (void*)3};
    t_list *list3 = create_list(data3, 3);
    if (!test_list_size("Three nodes", list3, 3)) failed++;
    free_list(list3);
    
    // 5ノードのリスト（サイズ指定で作成）
    t_list *list5 = create_list_with_size(5);
    if (!test_list_size("Five nodes", list5, 5)) failed++;
    free_list(list5);
    
    // 10ノードのリスト（サイズ指定で作成）
    t_list *list10 = create_list_with_size(10);
    if (!test_list_size("Ten nodes", list10, 10)) failed++;
    free_list(list10);
    
    return failed;
}
