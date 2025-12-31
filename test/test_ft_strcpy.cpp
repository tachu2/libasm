#include <iostream>
#include <cstring>
#include <cstdlib>

extern "C" {
    char *ft_strcpy(char *dst, const char *src);
}

static bool test_strcpy(const char *test_name, const char *src) {
    // バッファを確保（srcより大きめに）
    size_t buf_size = strlen(src) + 100;
    char *dst = new char[buf_size];
    char *expected_dst = new char[buf_size];
    
    // バッファを初期化
    memset(dst, 0xAA, buf_size);  // 0xAAで埋める（テスト用）
    memset(expected_dst, 0xAA, buf_size);
    
    // 標準ライブラリのstrcpyを呼び出し
    strcpy(expected_dst, src);
    
    // ft_strcpyを呼び出し
    char *result = ft_strcpy(dst, src);
    
    // 結果をチェック
    bool passed = true;
    std::string error_msg;
    
    // 戻り値がdstのポインタであることを確認
    if (result != dst) {
        passed = false;
        error_msg += "Return value mismatch; ";
    }
    
    // コピーされた文字列が一致することを確認
    if (strcmp(dst, expected_dst) != 0) {
        passed = false;
        error_msg += "String mismatch; ";
    }
    
    // null終端文字が正しくコピーされていることを確認
    if (dst[strlen(src)] != '\0') {
        passed = false;
        error_msg += "Null terminator missing; ";
    }
    
    if (passed) {
        std::cout << "✓ " << test_name << ": PASSED (length: " << strlen(src) << ")" << std::endl;
    } else {
        std::cerr << "✗ " << test_name << ": FAILED (" << error_msg << ")" << std::endl;
        std::cerr << "  Expected: \"" << expected_dst << "\"" << std::endl;
        std::cerr << "  Got:      \"" << dst << "\"" << std::endl;
    }
    
    delete[] dst;
    delete[] expected_dst;
    
    return passed;
}

int test_ft_strcpy() {
    std::cout << "Testing ft_strcpy..." << std::endl;
    int failed = 0;
    
    // 空文字列
    if (!test_strcpy("Empty string", "")) failed++;
    
    // 通常の文字列
    if (!test_strcpy("Simple string", "hello")) failed++;
    
    // 長い文字列
    if (!test_strcpy("Long string", "This is a longer string for testing")) failed++;
    
    // 1文字
    if (!test_strcpy("Single character", "a")) failed++;
    
    // 改行文字を含む
    if (!test_strcpy("String with newline", "hello\nworld")) failed++;
    
    // 特殊文字を含む
    if (!test_strcpy("String with tab", "hello\tworld")) failed++;
    
    // 数字を含む
    if (!test_strcpy("String with numbers", "12345")) failed++;
    
    // 混合
    if (!test_strcpy("Mixed characters", "Hello, World! 123")) failed++;
    
    // 非常に長い文字列
    const char *very_long = "This is a very long string that contains many characters and should test the strcpy function thoroughly with a lot of data to copy from source to destination buffer.";
    if (!test_strcpy("Very long string", very_long)) failed++;
    
    return failed;
}
