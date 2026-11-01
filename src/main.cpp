#include "Console.h"

#include <exception>
#include <iostream>
#include <string_view>

int main(int argc, char* argv[]) {
    if (argc == 2 && (std::string_view{argv[1]} == "--help"
                      || std::string_view{argv[1]} == "-h")) {
        git_learning::print_help(std::cout);
        return 0;
    }
    if (argc != 1) {
        std::cerr << "Usage: git_learning [--help]\n";
        return 2;
    }
    try {
        return git_learning::run_console(std::cin, std::cout, std::cerr);
    } catch (const std::exception& error) {
        std::cerr << "Fatal error: " << error.what() << '\n';
        return 1;
    }
}
