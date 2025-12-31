#include <iostream>
#include <cstring>
#include <cstdlib>

extern "C" {
    size_t ft_strlen(const char *s);
}

static void test_strlen(const char *test_name, const char *str) {
    size_t expected = strlen(str);
    size_t result = ft_strlen(str);
    
    if (result == expected) {
        std::cout << "✓ " << test_name << ": PASSED (expected: " << expected 
                  << ", got: " << result << ")" << std::endl;
    } else {
        std::cerr << "✗ " << test_name << ": FAILED (expected: " << expected 
                  << ", got: " << result << ")" << std::endl;
        std::exit(1);
    }
}

void test_ft_strlen() {
    std::cout << "Testing ft_strlen..." << std::endl;
    
    // 空文字列
    test_strlen("Empty string", "");
    
    // 通常の文字列
    test_strlen("Simple string", "hello");
    
    // 長い文字列
    test_strlen("Long string", "This is a longer string for testing");
    
    // 1文字
    test_strlen("Single character", "a");
    
    // 改行文字を含む
    test_strlen("String with newline", "hello\nworld");
    
    // null文字のみ
    test_strlen("Null character only", "\0");
    
    // 特殊文字を含む
    test_strlen("String with special chars", "hello\tworld\0test");
    
    // 数字を含む
    test_strlen("String with numbers", "12345");
    
    // 混合
    test_strlen("Mixed characters", "Hello, World! 123");
}
