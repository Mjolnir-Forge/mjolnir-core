"""Checks all cmake files if they are correctly formatted."""

from subprocess import DEVNULL, run
from sys import exit

from python.mjolnir.definitions import CMAKE_CONFIG_FILE_PATH
from python.mjolnir.files import get_cmake_files

err_msg = ""
for file in get_cmake_files():
    r = run(
        ["cmake-format", file, "--check", "-c", CMAKE_CONFIG_FILE_PATH],
        stderr=DEVNULL,
    )
    if r.returncode != 0:
        err_msg += f"The file '{file}' needs reformatting.\n"

if err_msg:
    exit(err_msg)
