#include <iostream>
#include "spdlog/fmt/fmt.h"
#include "../consoleStlye/include/consoleStlye.h"
// #include "consoleStlye.h"

int main()
{
    fmt::print("This is the client dir.\n");

    std::string text = "Font test";

    std::string boldText = console_stlye::makeBold(text);
    std::string underlinedText = console_stlye::makeUnderlined(text);
    std::string italicText = console_stlye::makeItalic(text);
    std::string inverseText = console_stlye::makeInverse(text);

    std::cout << "Bold: " << boldText << std::endl;
    std::cout << "Underlined: " << underlinedText << std::endl;
    std::cout << "Italic: " << italicText << std::endl;
    std::cout << "Inverse: " << inverseText << std::endl;

    return 0;
}