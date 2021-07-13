from sys import exit
from pathlib import Path
from subprocess import DEVNULL, run

ignore = ["build"]
patterns = ["*.cmake", "*CMakeLists.txt"]

root_dir = Path(__file__).parent.parent.parent
config_file = f"{root_dir.absolute().as_posix()}/.cmake-format.yaml"

files = []
for pattern in patterns:
    pattern_files = [
        path.absolute().as_posix()
        for path in root_dir.rglob(pattern)
        if not any(word in str(path) for word in ignore)
    ]
    files += pattern_files

err_msg = ""
for file in files:
    r = run(["cmake-format", file, "--check", "-c", config_file], stderr=DEVNULL)
    if r.returncode != 0:
        err_msg += f"The file '{file}' needs reformatting.\n"

if err_msg:
    exit(err_msg)
