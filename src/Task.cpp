#include "Task.h"

namespace git_learning {

std::string trim_title(std::string_view title) {
    constexpr std::string_view whitespace = " \t\r\n\f\v";
    const auto first = title.find_first_not_of(whitespace);
    if (first == std::string_view::npos) {
        return {};
    }
    const auto last = title.find_last_not_of(whitespace);
    return std::string{title.substr(first, last - first + 1)};
}

std::string format_task(const Task& task) {
    return std::string{task.completed ? "[x] " : "[ ] "}
        + std::to_string(task.id) + " " + task.title;
}

} // namespace git_learning
