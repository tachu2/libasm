#include <iostream>
#include <cstring>
#include <cstdlib>

extern "C" {
    char *ft_strdup(const char *s);
}

static bool test_strdup(const char *test_name, const char *str) {
    // 標準ライブラリのstrdupを呼び出し
    char *expected = strdup(str);
    if (!expected) {
        std::cerr << "✗ " << test_name << ": FAILED (strdup returned NULL)" << std::endl;
        return false;
    }
    
    // ft_strdupを呼び出し
    char *result = ft_strdup(str);
    
    // 結果をチェック
    bool passed = true;
    std::string error_msg;
    
    // NULLチェック
    if (result == nullptr) {
        passed = false;
        error_msg += "Returned NULL; ";
        free(expected);
        std::cerr << "✗ " << test_name << ": FAILED (" << error_msg << ")" << std::endl;
        return false;
    }
    
    // 文字列が一致することを確認
    if (strcmp(result, expected) != 0) {
        passed = false;
        error_msg += "String mismatch; ";
    }
    
    // 長さが一致することを確認
    if (strlen(result) != strlen(expected)) {
        passed = false;
        error_msg += "Length mismatch; ";
    }
    
    // 異なるメモリ領域を指していることを確認（ポインタが異なる）
    if (result == str) {
        passed = false;
        error_msg += "Returned same pointer as input; ";
    }
    
    if (passed) {
        std::cout << "✓ " << test_name << ": PASSED (length: " << strlen(result) << ")" << std::endl;
    } else {
        std::cerr << "✗ " << test_name << ": FAILED (" << error_msg << ")" << std::endl;
        std::cerr << "  Expected: \"" << expected << "\"" << std::endl;
        std::cerr << "  Got:      \"" << result << "\"" << std::endl;
    }
    
    // メモリを解放
    free(result);
    free(expected);
    
    return passed;
}

int test_ft_strdup() {
    std::cout << "Testing ft_strdup..." << std::endl;
    int failed = 0;
    
    // 空文字列
    if (!test_strdup("Empty string", "")) failed++;
    
    // 通常の文字列
    if (!test_strdup("Simple string", "hello")) failed++;
    
    // 長い文字列
    if (!test_strdup("Long string", "This is a longer string for testing")) failed++;
    
    // 1文字
    if (!test_strdup("Single character", "a")) failed++;
    
    // 改行文字を含む
    if (!test_strdup("String with newline", "hello\nworld")) failed++;
    
    // 特殊文字を含む
    if (!test_strdup("String with tab", "hello\tworld")) failed++;
    
    // 数字を含む
    if (!test_strdup("String with numbers", "12345")) failed++;
    
    // 混合
    if (!test_strdup("Mixed characters", "Hello, World! 123")) failed++;
    
    // 非常に長い文字列
    const char *very_long = "This is a very long string that contains many characters and should test the strdup function thoroughly with a lot of data to copy from source to destination buffer.";
    if (!test_strdup("Very long string", very_long)) failed++;
    
    return failed;
}
