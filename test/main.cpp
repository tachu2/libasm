#include <iostream>

// 各テスト関数の宣言
void test_ft_strlen();

int main() {
    std::cout << "Running all tests..." << std::endl;
    std::cout << "====================" << std::endl;
    
    test_ft_strlen();
    
    std::cout << "====================" << std::endl;
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
