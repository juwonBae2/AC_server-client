#pragma once

#include <string>

class ExecutionResult
{
public:
    ExecutionResult() = default;
    ExecutionResult(bool successFlag, const std::string message) : successFlag_{successFlag}, message_{message} {}

    std::string message() const { return message_; }
    bool isSuccessful() const { return successFlag_; }

    static ExecutionResult failure(const std::string &message_);
    static ExecutionResult success(const std::string &message_);

private:
    bool successFlag_ = false;
    std::string message_ = "";
};