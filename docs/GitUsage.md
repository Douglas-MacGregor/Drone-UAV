# Git Usage Guidelines

## Branching Strategy

To maintain a clean and organized codebase, we follow the Git Flow branching strategy:

- **Main Branch:** The `main` branch contains the stable release code. Only thoroughly tested and reviewed code should be merged here.
- **Development Branch:** The `develop` branch is used for integrating features and bug fixes. This branch should always be in a deployable state.
- **Feature Branches:** Create a new branch for each feature or bug fix from the `develop` branch. Name the branches descriptively, e.g., `feature/imu-integration` or `bugfix/lora-connection`.

## Commit Message Conventions

To ensure clarity and consistency in our commit history, please follow these conventions:

- Use the present tense ("Add feature" not "Added feature").
- Start with a capital letter.
- Keep the subject line under 50 characters.
- Provide a detailed description in the body if necessary, wrapped at 72 characters.

## Pull Requests

- Before merging a feature branch into `develop`, create a pull request (PR).
- Ensure that the PR is reviewed by at least one other team member.
- Squash commits when merging to keep the commit history clean.
- Create new tests for any new functionality and ensure existing tests pass.

## Using git commands

Here are some common git commands to help you manage your branches and commits:

- Create a new branch: `git checkout -b feature/your-feature-name`
- Switch to an existing branch: `git checkout branch-name`
- Stage changes: `git add .` or `git add path/to/file`
- Commit changes: `git commit -m "Your commit message"`
- Push changes to remote: `git push origin branch-name`
- Create a pull request on GitHub after pushing your branch.
- Use `git branch` to list branches and `git status` to check the status of your working directory.
