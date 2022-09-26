"""Checks if all cpp files pass the clang-tidy tests."""

import sys
import time
from argparse import ArgumentParser
from subprocess import run  # nosec

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
    "--benchmark_dir",
    help="Directory containing the google benchmark header",
    type=str,
    default=None,
)
parser.add_argument(
    "--build_dir",
    help="The build directory (Can be used to automatically find dependencies fetched "
    "by CMake)",
    type=str,
    default="build",
)
parser.add_argument(
    "--filter",
    help="All files not containing the passed string in their file path are excluded",
    type=str,
    default=None,
)
args = parser.parse_args()

if args.gtest_dir is None:
    args.gtest_dir = f"{args.build_dir}/_deps/googletest-src/googletest/include"

if args.benchmark_dir is None:
    args.benchmark_dir = f"{args.build_dir}/_deps/benchmark-src/include"


cmd = f"clang-tidy-{args.version}"
print(f"Running {cmd}\n")
sys.stdout.flush()

error = False

files = get_cpp_files()
if args.filter is not None:
    files = [file for file in files if args.filter in file]


for file in files:
    print(f"Checking file: {file}")
    start_time = time.time()
    r = run(
        [
            f"clang-tidy-{args.version}",
            file,
            "-header-filter=.*",
            "--extra-arg=-std=gnu++20",
            "--extra-arg=-pthread",
            "--extra-arg=-DSTATIC_ANALYSIS",
            "--extra-arg=-DDISABLE_NEW_DELETE_COUNTER",
            "--",
            "-I",
            "src",
            "-isystem",
            args.gtest_dir,
            args.benchmark_dir,
        ],
    )
    print(f"Elapsed time: {round(time.time()-start_time, 2)}s")
    print("\n")
    sys.stdout.flush()
    if r.returncode != 0:
        error = True

if error:
    sys.exit(1)
