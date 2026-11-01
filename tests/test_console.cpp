#include "Console.h"
#include "TestSupport.h"

#include <sstream>
#include <string>

struct Result {
    int status;
    std::string output;
    std::string errors;
};

Result run(std::string_view commands) {
    std::istringstream input{std::string{commands}};
    std::ostringstream output;
    std::ostringstream errors;
    const int status = git_learning::run_console(input, output, errors);
    return {status, output.str(), errors.str()};
}

bool contains(std::string_view text, std::string_view part) {
    return text.find(part) != std::string_view::npos;
}

int main() {
    TestSuite suite;
    suite.run("full task lifecycle", [] {
        const auto result = run("add Learn Git\ndone 1\nlist done\nreopen 1\n"
                                "rename 1 Practice branches\nlist open\nstats\nremove 1\nlist\nquit\n");
        check(result.status == 0 && result.errors.empty(), "Successful session");
        check(contains(result.output, "[x] 1 Learn Git"), "Done listing");
        check(contains(result.output, "[ ] 1 Practice branches"), "Renamed open listing");
        check(contains(result.output, "Total: 1, open: 1, done: 0"), "Statistics");
        check(contains(result.output, "No tasks match."), "Empty after removal");
    });
    suite.run("filters exclude other statuses", [] {
        const auto open = run("add Alpha\nadd Beta\ndone 2\nlist open\nquit\n");
        check(contains(open.output, "[ ] 1 Alpha"), "Open task present");
        check(!contains(open.output, "[x] 2 Beta"), "Done task excluded");
        const auto done = run("add Alpha\nadd Beta\ndone 2\nlist done\nquit\n");
        check(!contains(done.output, "[ ] 1 Alpha"), "Open task excluded");
        check(contains(done.output, "[x] 2 Beta"), "Done task present");
    });
    suite.run("IDs reject signs, suffixes, zero, overflow and missing input", [] {
        for (const auto* token : {"", "0", "-1", "+1", "1x", "1.0", "99999999999999999999"}) {
            const auto result = run(std::string{"add Keep\ndone "} + token + "\nlist\nquit\n");
            check(contains(result.errors, "positive integer task ID"), "Invalid ID reported");
            check(contains(result.output, "[ ] 1 Keep"), "Invalid ID must not mutate");
        }
    });
    suite.run("extra arguments are rejected before mutation", [] {
        for (const auto* command : {"done 1 extra", "remove 1 extra", "reopen 1 extra"}) {
            const auto result = run(std::string{"add Keep\n"} + command + "\nlist\nquit\n");
            check(contains(result.errors, "Unexpected extra arguments"), "Extra argument reported");
            check(contains(result.output, "[ ] 1 Keep"), "Original task unchanged");
        }
    });
    suite.run("blank titles are rejected and console recovers", [] {
        const auto result = run("add   \nadd Original\nrename 1   \nlist\nquit\n");
        check(contains(result.errors, "Task title cannot be empty"), "Blank title reported");
        check(contains(result.output, "[ ] 1 Original"), "Recovery and original title");
    });
    suite.run("missing tasks and unknown commands", [] {
        const auto result = run("remove 42\nnonsense\nhelp\nquit\n");
        check(contains(result.errors, "Task not found"), "Missing task reported");
        check(contains(result.errors, "Unknown command"), "Unknown command reported");
        check(contains(result.output, "Commands:"), "Help after errors");
    });
    suite.run("invalid filter", [] {
        const auto result = run("list pending\nquit\n");
        check(contains(result.errors, "Use list all"), "Invalid filter reported");
    });
    suite.run("CRLF, blank lines, tabs and EOF", [] {
        const auto result = run("\r\n\tadd\tLearn Git\r\nlist\r\n");
        check(result.status == 0 && result.errors.empty(), "Clean EOF");
        check(contains(result.output, "[ ] 1 Learn Git\n"), "CRLF trimmed");
        check(contains(result.output, "Goodbye!"), "EOF exits");
    });
    suite.run("quit stops processing subsequent lines", [] {
        const auto result = run("quit\nadd Ignored\n");
        check(!contains(result.output, "Added task"), "Quit stops input");
    });
    suite.run("empty input and exit alias", [] {
        check(run("").status == 0, "Empty stream exits");
        check(run("exit\n").status == 0, "Exit alias");
    });
    suite.run("input failure reports error", [] {
        std::istringstream input;
        input.setstate(std::ios::badbit);
        std::ostringstream output;
        std::ostringstream errors;
        check(git_learning::run_console(input, output, errors) == 1, "Read failure exit code");
        check(contains(errors.str(), "Could not read input"), "Read error reported");
    });
    return suite.result();
}
