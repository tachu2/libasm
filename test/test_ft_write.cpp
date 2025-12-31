#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <cerrno>

extern "C" {
    ssize_t ft_write(int fd, const void *buf, size_t count);
}

static bool test_write(const char *test_name, int fd, const void *buf, size_t count) {
    // errnoをリセット
    errno = 0;
    
    // 標準ライブラリのwriteを呼び出し
    ssize_t expected = write(fd, buf, count);
    int expected_errno = errno;
    
    // ファイルポインタをリセット（stdoutの場合は無視）
    if (fd != STDOUT_FILENO && fd != STDERR_FILENO) {
        lseek(fd, -expected, SEEK_CUR);
    }
    
    // errnoをリセット
    errno = 0;
    
    // ft_writeを呼び出し
    ssize_t result = ft_write(fd, buf, count);
    int result_errno = errno;
    
    // ファイルポインタをリセット（stdoutの場合は無視）
    if (fd != STDOUT_FILENO && fd != STDERR_FILENO) {
        lseek(fd, -result, SEEK_CUR);
    }
    
    if (result == expected && result_errno == expected_errno) {
        std::cout << "✓ " << test_name << ": PASSED (expected: " << expected 
                  << ", got: " << result << ")" << std::endl;
        return true;
    } else {
        std::cerr << "✗ " << test_name << ": FAILED (expected: " << expected 
                  << ", got: " << result << ", errno: " << result_errno 
                  << " vs " << expected_errno << ")" << std::endl;
        return false;
    }
}

static bool test_error(const char *test_name, int fd, const void *buf, size_t count, int &failed) {
    errno = 0;
    ssize_t expected = write(fd, buf, count);
    int expected_errno = errno;
    
    errno = 0;
    ssize_t result = ft_write(fd, buf, count);
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

int test_ft_write() {
    std::cout << "Testing ft_write..." << std::endl;
    int failed = 0;
    
    // 一時ファイルを作成
    const char *tmpfile = "/tmp/ft_write_test.txt";
    int fd = open(tmpfile, O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd < 0) {
        std::cerr << "Failed to create temporary file" << std::endl;
        return 1;
    }
    
    // 正常な書き込みテスト
    if (!test_write("Write simple string", fd, "hello", 5)) failed++;
    if (!test_write("Write empty buffer", fd, "", 0)) failed++;
    if (!test_write("Write with newline", fd, "hello\n", 6)) failed++;
    if (!test_write("Write long string", fd, "This is a longer string for testing", 35)) failed++;
    if (!test_write("Write single character", fd, "a", 1)) failed++;
    
    // 部分的な書き込みテスト
    const char *long_str = "Hello, World! 12345";
    if (!test_write("Write partial string", fd, long_str, 5)) failed++;
    
    // ファイルを閉じる
    close(fd);
    unlink(tmpfile);
    
    // エラーケース1: 無効なファイルディスクリプタ（負の値）
    test_error("Error: Invalid file descriptor (negative)", -1, "test", 4, failed);
    
    // エラーケース2: 非常に大きなファイルディスクリプタ番号
    test_error("Error: Invalid file descriptor (large number)", 99999, "test", 4, failed);
    
    // エラーケース3: 閉じられたファイルディスクリプタ
    int closed_fd = open("/dev/null", O_WRONLY);
    if (closed_fd >= 0) {
        close(closed_fd);
        test_error("Error: Closed file descriptor", closed_fd, "test", 4, failed);
    }
    
    // エラーケース4: 読み取り専用ファイルへの書き込み
    const char *readonly_file = "/tmp/ft_write_readonly.txt";
    int readonly_fd = open(readonly_file, O_CREAT | O_RDONLY | O_TRUNC, 0444);
    if (readonly_fd >= 0) {
        test_error("Error: Write to read-only file", readonly_fd, "test", 4, failed);
        close(readonly_fd);
        unlink(readonly_file);
    }
    
    // エラーケース5: 存在しないディレクトリ内のファイル
    test_error("Error: Write to file in non-existent directory", -1, "test", 4, failed);
    
    return failed;
}
