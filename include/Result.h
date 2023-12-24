#ifndef RESULT_H
#define RESULT_H

// 定义一个名为Result的枚举类，用于表示操作结果
enum class Result {
    Success,        // 操作成功
    Failure,        // 操作失败
    FileNotFound,   // 文件未找到
    AccessDenied,   // 访问被拒绝
    IOError,        // 输入/输出错误
    OutOfMemory,    // 内存不足
    InvalidData,    // 无效数据
    NotImplemented, // 未实现的功能
    UnknownError    // 未知错误
};

#endif // RESULT_H
