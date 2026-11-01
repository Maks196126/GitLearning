# Progressive Git and GitHub exercises

Work from the **GitLearning project folder**, not a parent folder. Commands are
compatible with PowerShell and common Unix shells unless marked otherwise.
Each exercise gives a checkpoint. Finish exercises in order with a clean
`git status`, except where a lesson deliberately creates uncommitted changes.
Use Qt Creator to edit files and the terminal to learn exactly what Git does.

## 1. Repository, identity, and first commit

```sh
git status
git log --oneline -5
```

If this prepared folder already has a repository and initial commit, inspect
them and skip initialization. A source ZIP has no `.git` folder: initialize it:

```sh
git init -b main
git config --get user.name
git config --get user.email
```

If either identity value is missing or wrong, set it for this repository only
(replace the example values with your chosen commit identity):

```sh
git config user.name "Your Name"
git config user.email "your-chosen-commit-email@example.com"
```

You can use the no-reply email shown in GitHub's email settings if you prefer.
Then stage and review the initial files:

```sh
git add .
git diff --cached --stat
git diff --cached
git commit -m "Create C++20 task manager learning project"
git status
```

Checkpoint: `main` has an initial commit and a clean working tree. Understand
the three places: working files, staging area (index), and committed history.

## 2. A focused edit and commit

Add a sentence to `exercises/notes.md` about what the staging area means.

```sh
git status --short
git diff
git add exercises/notes.md
git diff --cached
git commit -m "Document what the staging area does"
git log --oneline --decorate -5
git show --stat HEAD
```

Checkpoint: the new commit contains only your note. `git diff` shows unstaged
changes; `git diff --cached` shows what the next commit will contain.

## 3. Ignore generated and local files

Build into `build` using the README. Create `scratch/practice.txt` with any
temporary text (make the `scratch` folder first).

```sh
git status --short
git check-ignore -v build/CMakeCache.txt scratch/practice.txt
```

Checkpoint: build and scratch files are ignored and absent from normal status.
`.gitignore` does not untrack files that were already committed. If you ever
accidentally track a generated file, `git rm --cached path/to/file` stops tracking
it while leaving the working copy; inspect the staged change before committing.

## 4. Branch, edit, test, and merge

```sh
git switch -c feature/welcome-message
```

Change the welcome line in `src/Console.cpp` to your own wording. Keep its
meaning clear. Build and run CTest with the commands in the README.

```sh
git diff
git add src/Console.cpp
git commit -m "Personalize the task manager welcome message"
git switch main
git merge --no-ff feature/welcome-message -m "Merge welcome message exercise"
git log --oneline --graph --all --decorate
git branch -d feature/welcome-message
```

Checkpoint: main includes the welcome change and a merge commit. `--no-ff`
makes the branch integration visible even when a fast-forward was possible.

## 5. Resolve a predictable merge conflict

Start from clean main. Both branches will replace the same single line.

```sh
git switch -c exercise/goal-a
```

Replace `exercises/conflict.txt` with:

```text
Learning goal: practice branches every day.
```

```sh
git add exercises/conflict.txt
git commit -m "Focus learning goal on branches"
git switch main
git switch -c exercise/goal-b
```

Replace the same file with:

```text
Learning goal: practice testing every day.
```

```sh
git add exercises/conflict.txt
git commit -m "Focus learning goal on testing"
git switch main
git merge --ff-only exercise/goal-a
git merge exercise/goal-b
git status
```

The last merge is expected to report a conflict. Open the file, inspect the
`<<<<<<<`, `=======`, and `>>>>>>>` markers, and replace the entire contents with:

```text
Learning goal: practice branches and testing every day.
```

```sh
git add exercises/conflict.txt
git diff --cached
git commit -m "Combine branch and testing learning goals"
git branch -d exercise/goal-a exercise/goal-b
git log --oneline --graph -8
```

Checkpoint: no conflict markers remain and status is clean. To abandon an
unfinished merge before committing, use `git merge --abort`.

## 6. Stash work while switching context

Make an unfinished edit to the tracked `exercises/notes.md` file.

```sh
git stash push -m "Unfinished learning note"
git status
git stash list
git stash show -p
git stash pop
git diff
```

Checkpoint: the edit disappears while stashed and returns after pop. Complete
the note, stage it, and commit it before continuing. Ordinary stash does not
include untracked files; `git stash push -u` includes those when needed.

## 7. Undo at three different stages

For each substep, use only the disposable note you just created.

**Unstage without losing your edit:** edit `exercises/notes.md`, then:

```sh
git add exercises/notes.md
git restore --staged exercises/notes.md
git diff
```

**Discard that uncommitted edit:** after checking the diff contains nothing
you want to keep:

```sh
git restore exercises/notes.md
git status
```

**Undo a commit while preserving history:** add a new disposable note and commit it:

```sh
git add exercises/notes.md
git commit -m "Add disposable undo practice note"
git revert --no-edit HEAD
git log --oneline -3
```

Checkpoint: revert adds a new commit reversing the prior one. Prefer revert
for shared history. Restore discards uncommitted contents of the named file.

## 8. Practice reset on an unpublished branch

```sh
git switch -c exercise/local-reset
```

Add a temporary line to `exercises/notes.md`, then:

```sh
git add exercises/notes.md
git commit -m "Temporary local reset exercise"
git reset --soft HEAD~1
git status
git diff --cached
git commit -m "Record reset practice observation"
git switch main
git merge --ff-only exercise/local-reset
git branch -d exercise/local-reset
```

Checkpoint: soft reset moved the branch tip back one commit but kept the edit
staged. Use this only before publishing that commit. Do not use `reset --hard`
or force-push for these exercises. `git reflog` can help locate a previous commit
after an accidental branch movement; it is not a backup for uncommitted work.

## 9. Connect your GitHub account and push

Follow [GitHub setup](github_setup.md) to create the empty public repository,
authenticate through the browser, add `origin`, and push main.

```sh
git remote -v
git branch -vv
git status -sb
```

Checkpoint: main tracks `origin/main` and the web repository shows your files.
`origin` is a local nickname for the GitHub repository; `origin/main` is your
last fetched view of its main branch.

## 10. Clone, fetch, and pull with a second working copy

In a second terminal, choose a separate practice parent folder, then:

```sh
git clone https://github.com/Maks196126/GitLearning.git GitLearning-second
cd GitLearning-second
```

Add a note to `exercises/notes.md` in this second copy and commit/push it:

```sh
git add exercises/notes.md
git commit -m "Add a note from a second clone"
git push
```

Back in your original GitLearning folder:

```sh
git fetch origin
git log --oneline main..origin/main
git diff main origin/main
git pull --ff-only
```

Checkpoint: fetch updates remote-tracking information without editing working
files; pull fast-forwards your local main to the remote commit. If `--ff-only`
refuses, inspect the history: local and remote branches have diverged. Do not
force the command or discard local commits to make the error disappear.

## 11. A GitHub pull request

From the original copy, with main current and clean:

```sh
git switch -c docs/first-pull-request
```

Add a useful usage tip to README.md, then:

```sh
git diff
git add README.md
git commit -m "Add a task manager usage tip"
git push -u origin docs/first-pull-request
```

On GitHub, open a pull request with base `main` and compare
`docs/first-pull-request`. Describe what changed and how you checked it.
Review the Files changed tab, then merge the PR using **Create a merge commit**
for this exercise. You can review and merge your own PR even though you cannot
approve your own PR as a separate reviewer.

```sh
git switch main
git pull --ff-only
git branch -d docs/first-pull-request
git fetch --prune
```

Checkpoint: the merged tip is present locally. If you delete the branch on
GitHub, `fetch --prune` removes its stale remote-tracking reference locally.

## 12. Tag a tested milestone

Build and test main again, then:

```sh
git tag -a v1.0.0 -m "First tested GitLearning milestone"
git show v1.0.0
git push origin v1.0.0
```

Checkpoint: GitHub lists the tag. Ordinary `git push` does not automatically
push all tags. Choose a new version rather than replacing a published tag.

## Next round: use a feature branch for real C++ work

Implement title search, priorities, or save/load. First specify behavior in a
test, make it pass, run the whole suite, review the diff, and open a PR. Keep
changes small enough that you can explain every line in your review.
