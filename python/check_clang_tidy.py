"""Checks all cmake files if they are correctly formatted."""

from argparse import ArgumentParser
from subprocess import DEVNULL, run
from sys import exit, stdout

from mjolnir.files import get_cpp_files

parser = ArgumentParser()
parser.add_argument("--version", help="Clang-tidy version", type=int, default=12)
parser.add_argument(
    "--gtest_dir",
    help="Directory containing the gtest header",
    type=str,
    default=None,
)
parser.add_argument(
    "--build_dir",
    help="The build directory (Can be used to automatically find dependencies fetched"
    " py CMake)",
    type=str,
    default="build",
)
args = parser.parse_args()

if args.gtest_dir is None:
    args.gtest_dir = f"{args.build_dir}/_deps/googletest-src/googletest/include"


cmd = f"clang-tidy-{args.version}"
print(f"Running {cmd}\n")
stdout.flush()

error = False

for file in get_cpp_files():
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
            args.gtest_dir,
        ],
    )
    print("\n")
    stdout.flush()
    if r.returncode != 0:
        error = True

if error:
    exit(1)
