#include <cstring>
#include <iostream>
#include <cstdlib>

extern "C" {
    ssize_t ft_read(int fd, void *buf, size_t count);
    ssize_t ft_write(int fd, const void *buf, size_t count);
    char* ft_strcpy(char* dest, const char* src);
    int ft_strcmp(const char* s1, const char* s2);
    char* ft_strdup(const char* s);
    int ft_list_size(void *begin_list);
    void ft_list_push_front(void **begin_list, void *data);
    void ft_list_sort(void **begin_list, int (*cmp)(void*, void*));
}

// 各テスト関数の宣言
int test_ft_strlen();
int test_ft_write();
int test_ft_read();
int test_ft_strcpy();
int test_ft_strcmp();
int test_ft_strdup();
int test_ft_list_size();
int test_ft_list_push_front();
int test_ft_list_sort();
int test_ft_list_remove_if();

void playground() {
    // char buf[1024];
    // ssize_t bytes = ft_read(0, buf, 1024);
    // std::cout << "read_result: " << bytes << std::endl;
    // buf[bytes] = '\0';
    // std::cout << "read_result: " << buf << std::endl;
    // char buf[] = "test";
    // strcpy(buf, "const char *__restrict src");
    // std::cout << buf << "\n";
    // ft_strcpy(buf, "const char *__restrict src");
    // std::cout << buf << "\n";
    std::cout << ft_strcmp("test", "test") << "\n";
}

int run_tests() {
    std::cout << "Running all tests..." << std::endl;
    std::cout << "====================" << std::endl;
    
    int total_failed = 0;
    
    total_failed += test_ft_strlen();
    total_failed += test_ft_write();
    total_failed += test_ft_read();
    total_failed += test_ft_strcpy();
    total_failed += test_ft_strcmp();
    total_failed += test_ft_strdup();
    total_failed += test_ft_list_size();
    total_failed += test_ft_list_push_front();
    total_failed += test_ft_list_sort();
    total_failed += test_ft_list_remove_if();
    
    std::cout << "====================" << std::endl;
    if (total_failed == 0) {
        std::cout << "All tests passed!" << std::endl;
        return 0;
    } else {
        std::cerr << total_failed << " test(s) failed!" << std::endl;
        return 1;
    }
}

int main() {
    playground();
    return run_tests();
}
