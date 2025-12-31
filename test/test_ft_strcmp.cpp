#include <iostream>
#include <cstring>
#include <cstdlib>

extern "C" {
    int ft_strcmp(const char *s1, const char *s2);
}

static bool test_strcmp(const char *test_name, const char *s1, const char *s2) {
    int expected = strcmp(s1, s2);
    int result = ft_strcmp(s1, s2);
    
    // 符号が一致することを確認（値が完全に一致する必要はないが、符号は一致すべき）
    bool sign_match = (expected == 0 && result == 0) || 
                      (expected < 0 && result < 0) || 
                      (expected > 0 && result > 0);
    
    if (sign_match) {
        std::cout << "✓ " << test_name << ": PASSED (expected sign: " 
                  << (expected < 0 ? "negative" : expected > 0 ? "positive" : "zero")
                  << ", got sign: " 
                  << (result < 0 ? "negative" : result > 0 ? "positive" : "zero")
                  << ", expected: " << expected << ", got: " << result << ")" << std::endl;
        return true;
    } else {
        std::cerr << "✗ " << test_name << ": FAILED (expected sign: " 
                  << (expected < 0 ? "negative" : expected > 0 ? "positive" : "zero")
                  << ", got sign: " 
                  << (result < 0 ? "negative" : result > 0 ? "positive" : "zero")
                  << ", expected: " << expected << ", got: " << result << ")" << std::endl;
        std::cerr << "  s1: \"" << s1 << "\"" << std::endl;
        std::cerr << "  s2: \"" << s2 << "\"" << std::endl;
        return false;
    }
}

int test_ft_strcmp() {
    std::cout << "Testing ft_strcmp..." << std::endl;
    int failed = 0;
    
    // 等しい文字列
    if (!test_strcmp("Equal strings", "hello", "hello")) failed++;
    if (!test_strcmp("Equal empty strings", "", "")) failed++;
    if (!test_strcmp("Equal null character", "\0", "\0\0")) failed++;
    if (!test_strcmp("Equal single char", "a", "a")) failed++;
    
    // s1 < s2 (負の値を返す)
    if (!test_strcmp("s1 < s2 (first char)", "apple", "banana")) failed++;
    if (!test_strcmp("s1 < s2 (length)", "app", "apple")) failed++;
    if (!test_strcmp("s1 < s2 (case)", "A", "a")) failed++;
    if (!test_strcmp("s1 < s2 (numbers)", "123", "456")) failed++;
    if (!test_strcmp("s1 < s2 (empty vs non-empty)", "", "a")) failed++;
    
    // s1 > s2 (正の値を返す)
    if (!test_strcmp("s1 > s2 (first char)", "banana", "apple")) failed++;
    if (!test_strcmp("s1 > s2 (length)", "apple", "app")) failed++;
    if (!test_strcmp("s1 > s2 (case)", "a", "A")) failed++;
    if (!test_strcmp("s1 > s2 (numbers)", "456", "123")) failed++;
    if (!test_strcmp("s1 > s2 (non-empty vs empty)", "a", "")) failed++;
    
    // 特殊文字を含む
    if (!test_strcmp("Strings with newline", "hello\n", "hello\n")) failed++;
    if (!test_strcmp("Strings with tab", "hello\t", "hello\t")) failed++;
    if (!test_strcmp("Strings with special chars", "hello!", "hello?")) failed++;
    
    // 長い文字列
    if (!test_strcmp("Long equal strings", 
                     "This is a longer string for testing",
                     "This is a longer string for testing")) failed++;
    if (!test_strcmp("Long strings s1 < s2", 
                     "This is a longer string for testing",
                     "This is a longer string for testingZ")) failed++;
    if (!test_strcmp("Long strings s1 > s2", 
                     "This is a longer string for testingZ",
                     "This is a longer string for testing")) failed++;
    
    // 数字を含む
    if (!test_strcmp("Strings with numbers equal", "test123", "test123")) failed++;
    if (!test_strcmp("Strings with numbers s1 < s2", "test123", "test456")) failed++;
    if (!test_strcmp("Strings with numbers s1 > s2", "test456", "test123")) failed++;
    
    return failed;
}
