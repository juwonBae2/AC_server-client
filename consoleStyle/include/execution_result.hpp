#pragma once

#include <string>

class ExecutionResult
{
public:
    ExecutionResult(bool success, const std::string &message) : success_(success), message_(message) {}

    bool success() const { return success_; }
    const std::string &message() const { return message_; }

    static ExecutionResult failure(const std::string &message);
    static ExecutionResult success(const std::string &message);

private:
    bool success_;
    std::string message_;
};