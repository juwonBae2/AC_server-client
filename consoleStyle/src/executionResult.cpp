#include "executionResult.hpp"

ExecutionResult ExecutionResult::failure(const std::string &msg)
{
    return ExecutionResult(false, msg);
}

ExecutionResult ExecutionResult::sucess(const std::string &msg)
{
    return ExecutionResult(true, msg);
}