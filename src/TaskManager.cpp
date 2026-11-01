#include "TaskManager.h"

#include <algorithm>
#include <limits>
#include <stdexcept>
#include <utility>

namespace git_learning {

int TaskManager::add(std::string_view title) {
    auto cleaned = trim_title(title);
    if (cleaned.empty()) {
        throw std::invalid_argument("Task title cannot be empty.");
    }
    if (next_id_ == std::numeric_limits<int>::max()) {
        throw std::overflow_error("No more task IDs are available.");
    }
    const int id = next_id_;
    tasks_.push_back(Task{id, std::move(cleaned), false});
    ++next_id_;
    return id;
}

bool TaskManager::set_completed(int id, bool completed) {
    const auto it = std::ranges::find(tasks_, id, &Task::id);
    if (it == tasks_.end()) {
        return false;
    }
    it->completed = completed;
    return true;
}

bool TaskManager::rename(int id, std::string_view title) {
    auto cleaned = trim_title(title);
    if (cleaned.empty()) {
        throw std::invalid_argument("Task title cannot be empty.");
    }
    const auto it = std::ranges::find(tasks_, id, &Task::id);
    if (it == tasks_.end()) {
        return false;
    }
    it->title = std::move(cleaned);
    return true;
}

bool TaskManager::remove(int id) {
    return std::erase_if(tasks_, [id](const Task& task) { return task.id == id; }) != 0;
}

const std::vector<Task>& TaskManager::tasks() const noexcept {
    return tasks_;
}

std::size_t TaskManager::completed_count() const noexcept {
    return static_cast<std::size_t>(std::ranges::count(tasks_, true, &Task::completed));
}

} // namespace git_learning
