import subprocess
import sys

result = subprocess.check_output(["git", "diff", "--name-only", "HEAD"]).decode("utf-8")

if result:
    modiefied_files = result.splitlines()
    err_msg = "The following files are not formatted correctly:\n"
    for filename in modiefied_files:
        err_msg+= f"- {filename}\n"
    sys.exit(err_msg)
