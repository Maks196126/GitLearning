# Connect GitLearning to GitHub

Account: `Maks196126` ([profile](https://github.com/Maks196126)).
Target: a **new private** repository named `GitLearning`.
The URL below is the intended destination, not proof that the repository already exists.

## 1. Check the local project

Open a terminal inside the GitLearning folder:

```sh
git status
git log --oneline -5
git remote -v
```

If it is not a repository yet, complete exercise 1 in `git_exercises.md` first.
If `origin` already exists, inspect it before making changes. Do not replace a
remote pointing at another project. Git's commit name/email and the GitHub
account used for authentication are separate settings.

## 2. Create the empty repository

Sign in as `Maks196126`, then open [Create a new repository](https://github.com/new).
Choose owner `Maks196126`, name `GitLearning`, and **Private**. Leave README,
`.gitignore`, and license initialization unchecked: these files already exist
locally. Create the repository. If GitHub says the name is already taken, stop
and inspect that repository rather than overwriting or force-pushing anything.

## 3. Connect and push

If no `origin` is configured:

```sh
git remote add origin https://github.com/Maks196126/GitLearning.git
git remote -v
git push -u origin main
```

Git Credential Manager is installed on this computer. If a sign-in prompt
appears, use its browser flow and authorize the intended account. Complete
two-factor authentication in the browser. Never put a password or token in
source files, commands saved in your history, or chat.

If the push is rejected because the remote contains commits, do not force-push.
Fetch and inspect the histories first. The simplest beginner route is to use a
new empty repository, or clone the existing one separately and copy the source
files into it after reviewing its contents.

## Alternative: GitHub CLI

If `gh` is installed, its [browser login](https://cli.github.com/manual/gh_auth_login)
can authenticate and its repository command can create and push in one step:

```sh
gh auth login --hostname github.com --git-protocol https --web
gh auth status
gh api user --jq .login
gh auth setup-git
```

Verify the returned login is `Maks196126`. Then, only if the target repository
does not already exist and `origin` is absent:

```sh
gh repo create Maks196126/GitLearning --private --source=. --remote=origin --push
```

## 4. Verify the connection

```sh
git fetch origin
git status -sb
git branch -vv
git ls-remote origin refs/heads/main
git rev-parse HEAD
```

The remote `main` hash and local HEAD should match immediately after the push.
Open `https://github.com/Maks196126/GitLearning` while signed in and confirm the
Private label, source files, README, and commit history. Continue with the pull
request exercise. A private repository is visible only to authorized accounts.
