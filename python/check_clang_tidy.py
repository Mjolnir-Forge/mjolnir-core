"""Checks all cmake files if they are correctly formatted."""

from subprocess import DEVNULL, run
from sys import exit

from mjolnir.definitions import CMAKE_CONFIG_FILE_PATH
from mjolnir.files import get_cpp_files

version = 12
err_msg = (
    "The following files need reformatting:\n"
    "--------------------------------------\n\n"
)
threshold = len(err_msg)

for file in get_cpp_files():
    print(file)
    r = run(
        [
            f"clang-tidy-{version}",
            file,
            "-header-filter=.*",
            "--extra-arg=-std=gnu++20",
            "--extra-arg=-pthread",
            "--",
            "-I",
            "src",
            "-isystem",
            "build_tidy/_deps/googletest-src/googletest/include",
        ],
        stderr=DEVNULL,
    )
    if r.returncode != 0:
        err_msg += f"{file}\n"

if len(err_msg) > threshold:
    exit(err_msg)
