#include "execution_result.hpp"

ExecutionResult ExecutionResult::failure(const std::string &msg)
{
    return ExecutionResult(false, msg);
}

ExecutionResult ExecutionResult::success(const std::string &msg)
{
    return ExecutionResult(true, msg);
}