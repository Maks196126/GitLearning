#include "TaskManager.h"
#include "TestSupport.h"

using git_learning::TaskManager;

int main() {
    TestSuite suite;
    suite.run("empty manager", [] {
        const TaskManager manager;
        check(manager.tasks().empty(), "Manager must start empty");
        check(manager.completed_count() == 0, "No completed tasks");
    });
    suite.run("add trims titles and allocates stable IDs", [] {
        TaskManager manager;
        check(manager.add("  Learn Git\t") == 1, "First ID");
        check(manager.add("Build C++") == 2, "Second ID");
        check(manager.tasks()[0].title == "Learn Git", "Trimmed title");
        check(!manager.tasks()[0].completed, "New task is open");
    });
    suite.run("blank titles do not consume IDs", [] {
        TaskManager manager;
        expect_invalid([&] { manager.add(""); });
        expect_invalid([&] { manager.add(" \t\r\n\f\v"); });
        check(manager.tasks().empty(), "Invalid add must not mutate");
        check(manager.add("Valid") == 1, "ID unchanged after errors");
    });
    suite.run("complete and reopen are idempotent", [] {
        TaskManager manager;
        const int id = manager.add("Task");
        check(manager.set_completed(id), "Complete existing task");
        check(manager.set_completed(id), "Complete again");
        check(manager.completed_count() == 1, "Count only once");
        check(manager.set_completed(id, false), "Reopen existing task");
        check(manager.completed_count() == 0, "Reopened count");
    });
    suite.run("missing IDs do not mutate tasks", [] {
        TaskManager manager;
        manager.add("Keep me");
        check(!manager.set_completed(99), "Missing completion");
        check(!manager.remove(0), "Invalid removal");
        check(!manager.rename(-1, "Other"), "Invalid rename");
        check(manager.tasks()[0].title == "Keep me", "Title unchanged");
        check(manager.completed_count() == 0, "Status unchanged");
    });
    suite.run("rename preserves ID and completion", [] {
        TaskManager manager;
        const int id = manager.add("Before");
        manager.set_completed(id);
        check(manager.rename(id, "  After  "), "Rename existing task");
        check(manager.tasks()[0].title == "After", "Rename is trimmed");
        check(manager.tasks()[0].id == id && manager.tasks()[0].completed,
              "Rename preserves other fields");
    });
    suite.run("invalid rename preserves original title", [] {
        TaskManager manager;
        const int id = manager.add("Original");
        expect_invalid([&] { manager.rename(id, " \t"); });
        check(manager.tasks()[0].title == "Original", "Title unchanged");
    });
    suite.run("remove keeps remaining IDs and order", [] {
        TaskManager manager;
        manager.add("First");
        const int middle = manager.add("Middle");
        const int last = manager.add("Last");
        manager.set_completed(middle);
        check(manager.remove(middle), "Remove middle task");
        check(!manager.remove(middle), "Cannot remove twice");
        check(manager.tasks().size() == 2, "Remaining count");
        check(manager.tasks()[1].id == last, "Remaining ID is stable");
        check(manager.completed_count() == 0, "Removed task excluded from count");
        check(manager.add("Next") == 4, "Removed ID is not reused");
    });
    suite.run("duplicate titles are independent tasks", [] {
        TaskManager manager;
        manager.add("Same");
        const int second = manager.add("Same");
        manager.set_completed(second);
        check(!manager.tasks()[0].completed, "First task stays open");
        check(manager.tasks()[1].completed, "Second task completed");
    });
    suite.run("titles preserve punctuation and UTF-8 bytes", [] {
        TaskManager manager;
        const std::string title = "Read C++: caf\xC3\xA9 / GitHub!";
        manager.add(title);
        check(manager.tasks()[0].title == title, "Title bytes preserved");
    });
    suite.run("task formatting", [] {
        check(git_learning::format_task({7, "Learn Git", false}) == "[ ] 7 Learn Git",
              "Open formatting");
        check(git_learning::format_task({7, "Learn Git", true}) == "[x] 7 Learn Git",
              "Done formatting");
    });
    return suite.result();
}
