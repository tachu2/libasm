#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <cerrno>

extern "C" {
    ssize_t ft_read(int fd, void *buf, size_t count);
}

static bool test_read(const char *test_name, int fd, void *buf, size_t count) {
    // バッファをクリア
    memset(buf, 0, count);
    
    // errnoをリセット
    errno = 0;
    
    // 標準ライブラリのreadを呼び出し
    ssize_t expected = read(fd, buf, count);
    int expected_errno = errno;
    
    // 読み込んだデータを保存
    char expected_buf[1024] = {0};
    memcpy(expected_buf, buf, expected < 0 ? 0 : expected);
    
    // ファイルポインタをリセット
    if (fd != STDIN_FILENO && expected > 0) {
        lseek(fd, -expected, SEEK_CUR);
    }
    
    // バッファをクリア
    memset(buf, 0, count);
    
    // errnoをリセット
    errno = 0;
    
    // ft_readを呼び出し
    ssize_t result = ft_read(fd, buf, count);
    int result_errno = errno;
    
    // ファイルポインタをリセット
    if (fd != STDIN_FILENO && result > 0) {
        lseek(fd, -result, SEEK_CUR);
    }
    
    bool data_match = true;
    if (result > 0 && expected > 0) {
        data_match = (memcmp(buf, expected_buf, result) == 0);
    }
    
    if (result == expected && result_errno == expected_errno && data_match) {
        std::cout << "✓ " << test_name << ": PASSED (read: " << result << " bytes)" << std::endl;
        return true;
    } else {
        std::cerr << "✗ " << test_name << ": FAILED (expected: " << expected 
                  << ", got: " << result << ", errno: " << result_errno 
                  << " vs " << expected_errno << ", data match: " << data_match << ")" << std::endl;
        return false;
    }
}

static bool test_error(const char *test_name, int fd, void *buf, size_t count, int &failed) {
    errno = 0;
    ssize_t expected = read(fd, buf, count);
    int expected_errno = errno;
    
    errno = 0;
    ssize_t result = ft_read(fd, buf, count);
    int result_errno = errno;
    
    if (result == expected && result_errno == expected_errno) {
        std::cout << "✓ " << test_name << ": PASSED (errno: " << result_errno << ")" << std::endl;
        return true;
    } else {
        std::cerr << "✗ " << test_name << ": FAILED (expected: " << expected 
                  << ", got: " << result << ", errno: " << result_errno 
                  << " vs " << expected_errno << ")" << std::endl;
        failed++;
        return false;
    }
}

int test_ft_read() {
    std::cout << "Testing ft_read..." << std::endl;
    int failed = 0;
    
    // テスト用の一時ファイルを作成
    const char *tmpfile = "/tmp/ft_read_test.txt";
    const char *test_data = "Hello, World!\nThis is a test file.\n12345";
    size_t test_data_len = strlen(test_data);
    
    // テストファイルにデータを書き込む
    int write_fd = open(tmpfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (write_fd < 0) {
        std::cerr << "Failed to create test file" << std::endl;
        return 1;
    }
    write(write_fd, test_data, test_data_len);
    close(write_fd);
    
    // 読み取り用ファイルを開く
    int fd = open(tmpfile, O_RDONLY);
    if (fd < 0) {
        std::cerr << "Failed to open test file" << std::endl;
        unlink(tmpfile);
        return 1;
    }
    
    char buf[1024] = {0};
    
    // 正常な読み込みテスト
    if (!test_read("Read simple string", fd, buf, 13)) failed++;
    if (!test_read("Read with newline", fd, buf, 20)) failed++;
    if (!test_read("Read single character", fd, buf, 1)) failed++;
    if (!test_read("Read entire file", fd, buf, 1024)) failed++;
    
    // ファイルを閉じて再度開く（位置をリセット）
    close(fd);
    fd = open(tmpfile, O_RDONLY);
    
    // 部分的な読み込みテスト
    if (!test_read("Read partial data", fd, buf, 5)) failed++;
    if (!test_read("Read more data", fd, buf, 10)) failed++;
    
    // 空の読み込み（EOF）
    lseek(fd, 0, SEEK_END);
    if (!test_read("Read at EOF", fd, buf, 10)) failed++;
    
    close(fd);
    unlink(tmpfile);
    
    // エラーケース1: 無効なファイルディスクリプタ（負の値）
    test_error("Error: Invalid file descriptor (negative)", -1, buf, 10, failed);
    
    // エラーケース2: 非常に大きなファイルディスクリプタ番号
    test_error("Error: Invalid file descriptor (large number)", 99999, buf, 10, failed);
    
    // エラーケース3: 閉じられたファイルディスクリプタ
    int closed_fd = open("/dev/null", O_RDONLY);
    if (closed_fd >= 0) {
        close(closed_fd);
        test_error("Error: Closed file descriptor", closed_fd, buf, 10, failed);
    }
    
    // エラーケース4: 書き込み専用ファイルからの読み込み
    const char *writeonly_file = "/tmp/ft_read_writeonly.txt";
    int writeonly_fd = open(writeonly_file, O_CREAT | O_WRONLY | O_TRUNC, 0222);
    if (writeonly_fd >= 0) {
        test_error("Error: Read from write-only file", writeonly_fd, buf, 10, failed);
        close(writeonly_fd);
        unlink(writeonly_file);
    }
    
    return failed;
}
