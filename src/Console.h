#pragma once

#include <iosfwd>

namespace git_learning {

void print_help(std::ostream& output);
int run_console(std::istream& input, std::ostream& output, std::ostream& errors);

} // namespace git_learning
