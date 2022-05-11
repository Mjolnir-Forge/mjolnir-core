"""Checks all cmake files if they are correctly formatted."""

import sys
from subprocess import DEVNULL, run

from mjolnir.definitions import CMAKE_CONFIG_FILE_PATH
from mjolnir.files import get_cmake_files

err_msg = (
    "The following files need reformatting:\n"
    "--------------------------------------\n\n"
)
threshold = len(err_msg)

for file in get_cmake_files():
    r = run(
        ["cmake-format", file, "--check", "-c", CMAKE_CONFIG_FILE_PATH],
        stderr=DEVNULL,
    )
    if r.returncode != 0:
        err_msg += f"{file}\n"

if len(err_msg) > threshold:
    sys.exit(err_msg)
