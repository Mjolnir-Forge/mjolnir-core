"""Checks all CMake files using cmake-lint."""

import sys
from subprocess import DEVNULL, run  # nosec

from mjolnir.definitions import CMAKE_CONFIG_FILE_PATH
from mjolnir.files import get_cmake_files

err_msg = (
    "The following files contain cmake-lint issues:\n"
    "----------------------------------------------\n\n"
)
threshold = len(err_msg)

for file in get_cmake_files():
    r = run(["cmake-lint", file, "-c", CMAKE_CONFIG_FILE_PATH], stdout=DEVNULL)  # nosec
    if r.returncode != 0:
        err_msg += f"{file}\n"

if len(err_msg) > threshold:
    sys.exit(err_msg)
