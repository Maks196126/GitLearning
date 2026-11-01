# GitLearning

A small C++20 console task manager for practicing Git and GitHub in Qt Creator.
It uses the C++ standard library only: Qt Creator is the IDE; Qt libraries are not required.

## Start in Qt Creator

1. Open **File > Open File or Project**, then select this folder's `CMakeLists.txt`.
2. Select a desktop kit with a C++20 compiler, CMake, and a build tool. On this
   computer, Qt's MinGW 13.1 compiler is installed under `C:/Qt/Tools/mingw1310_64`.
3. Configure a Debug build in a separate build folder and build the project.
4. Select `git_learning` as the run target. Enable **Run in terminal** in the
   project's run settings if the output pane does not accept keyboard input.
5. Run it and type the commands below. To run the tests, use CTest from a terminal
   with `--test-dir` pointing to the build folder shown by Qt Creator.

Qt Creator's [opening projects guide](https://doc.qt.io/qtcreator/creator-project-opening.html)
documents opening `CMakeLists.txt`. If no suitable kit is offered, configure the
compiler, CMake, and Ninja in Qt Creator's kit settings first.

## Build and test from a terminal

Requirements: CMake 3.21+, a C++20 compiler (for example GCC 13 or a recent MSVC),
and a compatible build tool. No downloaded test framework is needed.
Run the commands from this project folder.

### Windows PowerShell with the installed Qt MinGW tools

```powershell
$env:PATH = "C:/Qt/Tools/mingw1310_64/bin;C:/Qt/Tools/Ninja;" + $env:PATH
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
ctest --test-dir build --output-on-failure
.\build\git_learning.exe
```

The PATH change lasts only for this terminal. Adjust the compiler folder on
another computer. Keep the MinGW `bin` folder on PATH when running its executables.

### Windows with Visual Studio

```powershell
cmake -S . -B build-msvc -G "Visual Studio 18 2026" -A x64
cmake --build build-msvc --config Debug --parallel
ctest --test-dir build-msvc -C Debug --output-on-failure
.\build-msvc\Debug\git_learning.exe
```

Use `Visual Studio 17 2022` if that is the installed version instead. A build
folder belongs to one generator/compiler: use a new folder when switching kits.

### Linux or macOS

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
ctest --test-dir build --output-on-failure
./build/git_learning
```

## Try the application

```text
add Learn Git basics
add Build the project in Qt Creator
list
done 1
list done
rename 2 Run the tests in Qt Creator
reopen 1
stats
remove 2
help
quit
```

Task IDs are positive integers. Titles may contain spaces and need no quotes.
`list`, `list all`, `list open`, and `list done` filter the output. IDs stay
stable when other tasks are removed. Empty titles, invalid IDs, unknown tasks,
and extra arguments produce an error and let you try again.

**Tasks exist only for the current session.** Quitting or reaching end of input
discards them. File persistence is a suggested later exercise.
`git_learning --help` shows usage without starting a session. Unsupported startup
options exit with code 2; an input failure exits with code 1. Correctable command
errors within an interactive session do not make the final exit code nonzero.

## Project layout

```text
GitLearning/
  CMakeLists.txt              Library, application and CTest targets
  .gitignore                 Builds, IDE settings and local scratch data
  .gitattributes             Consistent source line endings
  src/
    Task.h / Task.cpp        Task data and formatting
    TaskManager.h / .cpp     Add, update, remove and count tasks
    Console.h / .cpp         Command parsing and interactive loop
    main.cpp                Executable entry point
  tests/
    TestSupport.h           Small checks that also work in Release builds
    test_task_manager.cpp   11 task behavior cases
    test_console.cpp        11 console behavior cases
  exercises/
    git_exercises.md        Progressive hands-on lessons
    github_setup.md         Connect your account and publish
    conflict.txt            A predictable merge-conflict exercise
    notes.md                Your practice notes
```

CTest runs two test executables (22 cases total) and two executable argument
checks. It covers IDs, invalid input, completion, renaming, deletion, filtering,
statistics, input failures, and command recovery. Set `-DBUILD_TESTING=OFF` if
you want an application-only build.

## Learn Git and GitHub

Begin with [the exercises](exercises/git_exercises.md), then follow
[GitHub setup](exercises/github_setup.md) for account `Maks196126`.
The intended repository is a new private `GitLearning` repository.

The prepared working folder may already contain a local Git repository and an
initial commit; check `git status` and `git log --oneline` first. A downloaded
source ZIP contains no Git history, so start with exercise 1.

Good practice: change one thing, build and test it, inspect `git diff`, then
commit with a message explaining the change. Keep generated build files out of Git.

## Next C++ features to practice on branches

- `feature/search`: search titles without changing existing list behavior.
- `feature/priority`: add priorities, formatting, and tests.
- `feature/persistence`: explicitly save/load tasks, preserve IDs, and handle
  missing or malformed files without overwriting good data.

This is a learning starter, not a durable task database. No license has been
chosen; add one deliberately before distributing the project as open source.
