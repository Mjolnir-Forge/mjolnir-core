"""Runs valgrind on all relevant tests."""

import sys
import time
from argparse import ArgumentParser
from pathlib import Path
from subprocess import run  # nosec

from mjolnir.files import get_file_paths

parser = ArgumentParser()
parser.add_argument(
    "--build_dir",
    help="The build directory (contains the build test files)",
    type=str,
    default="build",
)
args = parser.parse_args()
path = f"{args.build_dir}/tests/core/memory"
files = get_file_paths(Path(path), ["*test_*"], ["CMake"])

error = False

for file in files:
    print(f"Checking file: {file}")
    start_time = time.time()
    r = run(
        [
            "valgrind",
            "--tool=memcheck",
            "--leak-check=full",
            # "--error-exitcode=1",
            "--show-leak-kinds=all",
            "--track-origins=yes",
            "--show-reachable=yes",
            file,
        ],
    )
    print(f"Elapsed time: {round(time.time()-start_time, 2)}s")
    print("\n")
    sys.stdout.flush()
    if r.returncode != 0:
        error = True
        print(f"THE BAD RETURN CODE IS {r.returncode}")

if error:
    sys.exit(1)
