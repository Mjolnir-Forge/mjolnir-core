"""Formats all CMake files."""

from subprocess import run  # nosec

from mjolnir.definitions import CMAKE_CONFIG_FILE_PATH
from mjolnir.files import get_cmake_files

for file in get_cmake_files():
    run(["cmake-format", file, "-i", "-c", CMAKE_CONFIG_FILE_PATH])  # nosec
