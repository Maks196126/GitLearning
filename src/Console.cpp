#include "Console.h"
#include "TaskManager.h"

#include <charconv>
#include <exception>
#include <istream>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <system_error>

namespace git_learning {
namespace {

int read_id(std::istringstream& arguments) {
    std::string token;
    arguments >> token;
    int id = 0;
    const auto [end, error] = std::from_chars(token.data(), token.data() + token.size(), id);
    if (error != std::errc{} || end != token.data() + token.size() || id <= 0) {
        throw std::invalid_argument("Expected a positive integer task ID.");
    }
    return id;
}

void require_end(std::istringstream& arguments) {
    std::string extra;
    if (arguments >> extra) {
        throw std::invalid_argument("Unexpected extra arguments.");
    }
}

void require_found(bool found) {
    if (!found) {
        throw std::invalid_argument("Task not found.");
    }
}

} // namespace

void print_help(std::ostream& output) {
    output << "Commands:\n"
           << "  add <title>          Add a task (no quotes needed)\n"
           << "  list [all|open|done] List tasks\n"
           << "  done <id>            Mark a task complete\n"
           << "  reopen <id>          Mark a task open\n"
           << "  rename <id> <title>  Change a task title\n"
           << "  remove <id>          Remove a task\n"
           << "  stats                Show task counts\n"
           << "  help                 Show commands\n"
           << "  quit                 Exit (tasks are kept in memory only)\n";
}

int run_console(std::istream& input, std::ostream& output, std::ostream& errors) {
    TaskManager manager;
    output << "GitLearning - C++20 task manager\n"
           << "Tasks last for this session only. Type help for commands.\n";
    std::string line;
    while (output << "> " << std::flush, std::getline(input, line)) {
        std::istringstream arguments{line};
        std::string command;
        if (!(arguments >> command)) {
            continue;
        }
        try {
            if (command == "add") {
                std::string title;
                std::getline(arguments, title);
                const int id = manager.add(title);
                output << "Added task " << id << ".\n";
            } else if (command == "list") {
                std::string filter = "all";
                std::string requested;
                if (arguments >> requested) {
                    filter = requested;
                }
                require_end(arguments);
                if (filter != "all" && filter != "open" && filter != "done") {
                    throw std::invalid_argument("Use list all, list open, or list done.");
                }
                bool any = false;
                for (const auto& task : manager.tasks()) {
                    if ((filter == "open" && task.completed)
                        || (filter == "done" && !task.completed)) {
                        continue;
                    }
                    output << format_task(task) << '\n';
                    any = true;
                }
                if (!any) {
                    output << "No tasks match.\n";
                }
            } else if (command == "done" || command == "reopen" || command == "remove") {
                const int id = read_id(arguments);
                require_end(arguments);
                if (command == "remove") {
                    require_found(manager.remove(id));
                    output << "Removed task " << id << ".\n";
                } else {
                    require_found(manager.set_completed(id, command == "done"));
                    output << "Updated task " << id << ".\n";
                }
            } else if (command == "rename") {
                const int id = read_id(arguments);
                std::string title;
                std::getline(arguments, title);
                require_found(manager.rename(id, title));
                output << "Renamed task " << id << ".\n";
            } else if (command == "stats") {
                require_end(arguments);
                output << "Total: " << manager.tasks().size()
                       << ", open: " << manager.tasks().size() - manager.completed_count()
                       << ", done: " << manager.completed_count() << '\n';
            } else if (command == "help") {
                require_end(arguments);
                print_help(output);
            } else if (command == "quit" || command == "exit") {
                require_end(arguments);
                output << "Goodbye!\n";
                return 0;
            } else {
                throw std::invalid_argument("Unknown command. Type help for commands.");
            }
        } catch (const std::invalid_argument& error) {
            errors << "Error: " << error.what() << '\n';
        }
    }
    if (input.bad() || (input.fail() && !input.eof())) {
        errors << "Error: Could not read input.\n";
        return 1;
    }
    output << "\nGoodbye!\n";
    return 0;
}

} // namespace git_learning
