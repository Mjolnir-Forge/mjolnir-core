"""Checks all CMake files using cmake-lint."""

from subprocess import DEVNULL, run
from sys import exit

from mjolnir.definitions import CMAKE_CONFIG_FILE_PATH
from mjolnir.files import get_cmake_files

err_msg = "The following files contain cmake-lint issues:\n"
threshold = len(err_msg)

for file in get_cmake_files():
    r = run(["cmake-lint", file, "-c", CMAKE_CONFIG_FILE_PATH], stdout=DEVNULL)
    if r.returncode != 0:
        err_msg += f"{file}\n"

if len(err_msg) > threshold:
    exit(err_msg)
