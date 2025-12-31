#include <iostream>
#include <cstdlib>

// 各テスト関数の宣言
int test_ft_strlen();
int test_ft_write();
int test_ft_read();

int main() {
    std::cout << "Running all tests..." << std::endl;
    std::cout << "====================" << std::endl;
    
    int total_failed = 0;
    
    total_failed += test_ft_strlen();
    total_failed += test_ft_write();
    total_failed += test_ft_read();
    
    std::cout << "====================" << std::endl;
    if (total_failed == 0) {
        std::cout << "All tests passed!" << std::endl;
        return 0;
    } else {
        std::cerr << total_failed << " test(s) failed!" << std::endl;
        return 1;
    }
}
