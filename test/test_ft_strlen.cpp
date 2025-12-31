#include <iostream>
#include <cstring>
#include <cstdlib>

extern "C" {
    size_t ft_strlen(const char *s);
}

static bool test_strlen(const char *test_name, const char *str) {
    size_t expected = strlen(str);
    size_t result = ft_strlen(str);
    
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

int test_ft_strlen() {
    std::cout << "Testing ft_strlen..." << std::endl;
    int failed = 0;
    
    // 空文字列
    if (!test_strlen("Empty string", "")) failed++;
    
    // 通常の文字列
    if (!test_strlen("Simple string", "hello")) failed++;
    
    // 長い文字列
    if (!test_strlen("Long string", "This is a longer string for testing")) failed++;
    
    // 1文字
    if (!test_strlen("Single character", "a")) failed++;
    
    // 改行文字を含む
    if (!test_strlen("String with newline", "hello\nworld")) failed++;
    
    // null文字のみ
    if (!test_strlen("Null character only", "\0")) failed++;
    
    // 特殊文字を含む
    if (!test_strlen("String with special chars", "hello\tworld\0test")) failed++;
    
    // 数字を含む
    if (!test_strlen("String with numbers", "12345")) failed++;
    
    // 混合
    if (!test_strlen("Mixed characters", "Hello, World! 123")) failed++;
    
    return failed;
}
