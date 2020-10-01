import subprocess
import sys

result = subprocess.check_output(["git", "diff", "--name-only", "HEAD"]).decode("utf-8")

if result:
    modified_files = result.splitlines()
    err_msg = "FAIL - The following files are not formatted correctly:\n"
    for filename in modified_files:
        err_msg+= f"- {filename}\n"
    sys.exit(err_msg)
print("SUCCESS - all files are formatted correctly")
