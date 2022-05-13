"""Checks if all cpp files are formatted correctly."""

import sys

import git

repo = git.Repo(".")
result = repo.git.diff("HEAD", name_only=True)

if result:
    modified_files = result.splitlines()
    err_msg = "FAIL - The following files are not formatted correctly:\n"
    for filename in modified_files:
        err_msg += f"- {filename}\n"
    sys.exit(err_msg)
print("SUCCESS - all files are formatted correctly")
