#pragma once

#include <string>
#include <string_view>

namespace git_learning {

struct Task {
    int id;
    std::string title;
    bool completed = false;
};

std::string trim_title(std::string_view title);
std::string format_task(const Task& task);

} // namespace git_learning
