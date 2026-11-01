#pragma once

#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

// Unlike assert(), these checks still run in Release builds.
inline void check(bool condition, std::string_view message) {
    if (!condition) {
        throw std::runtime_error(std::string{message});
    }
}

template <typename Function>
void expect_invalid(Function action) {
    bool thrown = false;
    try {
        action();
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    check(thrown, "Expected std::invalid_argument");
}

class TestSuite {
public:
    template <typename Function>
    void run(std::string_view name, Function test) {
        ++total_;
        try {
            test();
            std::cout << "PASS: " << name << '\n';
        } catch (const std::exception& error) {
            ++failures_;
            std::cerr << "FAIL: " << name << ": " << error.what() << '\n';
        }
    }

    int result() const {
        std::cout << total_ - failures_ << '/' << total_ << " cases passed\n";
        return failures_ == 0 ? 0 : 1;
    }

private:
    int total_ = 0;
    int failures_ = 0;
};
