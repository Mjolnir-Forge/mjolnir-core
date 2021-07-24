"""Checks all cmake files if they are correctly formatted."""

from argparse import ArgumentParser
from subprocess import DEVNULL, run
from sys import exit

from mjolnir.files import get_cpp_files

parser = ArgumentParser()
parser.add_argument("--version", help="Clang-tidy version", type=int, default=12)
args = parser.parse_args()

cmd = f"clang-tidy-{args.version}"
print(f"Running {cmd}\n")

error = False

for file in get_cpp_files():
    print(file)
    r = run(
        [
            f"clang-tidy-{args.version}",
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
    )
    if r.returncode != 0:
        error = True

if error:
    exit(1)
