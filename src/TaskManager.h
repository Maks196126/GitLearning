#pragma once

#include "Task.h"

#include <cstddef>
#include <string_view>
#include <vector>

namespace git_learning {

class TaskManager {
public:
    // IDs stay stable after removal and are never reused within a session.
    int add(std::string_view title);
    bool set_completed(int id, bool completed = true);
    bool rename(int id, std::string_view title);
    bool remove(int id);
    [[nodiscard]] const std::vector<Task>& tasks() const noexcept;
    [[nodiscard]] std::size_t completed_count() const noexcept;

private:
    std::vector<Task> tasks_;
    int next_id_ = 1;
};

} // namespace git_learning
