"""Checks if the version number is increased correctly."""

import os
import subprocess
import sys
from typing import List

module_name = str(sys.argv[1])
source_branch = str(sys.argv[2])
target_branch = str(sys.argv[3])


def get_version_number_item(version_header_content: str, module: str, item: str) -> int:
    """Get a specific item (major, minor, etc.) of the version number.

    Parameters
    ----------
    version_header_content : str
        Content of the version header
    module : str
        Name of the Mjolnir module
    item :
        Name of the desired item (major, minor, patch or tweak)

    Returns
    -------
    int :
        Requested version number item

    """
    version_string = f"#define MJOLNIR_{module}_VERSION_{item}"
    pos_start = version_header_content.find(version_string) + len(version_string)
    pos_end = version_header_content.find("\n", pos_start)
    return int(version_header_content[pos_start:pos_end])


def get_version_number(module: str) -> List[int]:
    """Get the version number of a Mjolnir module.

    Parameters
    ----------
    module : str
        Name of the module

    Returns
    -------
    List :
        A list containing the 4 version number items: major, minor, patch, tweak

    """
    file_path = f"src/mjolnir/{module}/version.h"
    if not os.path.exists(file_path):
        return None

    with open(file_path, "r") as file:
        data = file.read()

        module_upper = module.upper()
        major = get_version_number_item(data, module_upper, "MAJOR")
        minor = get_version_number_item(data, module_upper, "MINOR")
        patch = get_version_number_item(data, module_upper, "PATCH")
        tweak = get_version_number_item(data, module_upper, "TWEAK")

        return [major, minor, patch, tweak]


FNULL = open(os.devnull, "w")
source_version = get_version_number(module_name)
subprocess.run(f"git checkout {target_branch}", stdout=FNULL, shell=True)
target_version = get_version_number(module_name)
subprocess.run(f"git checkout {source_branch}", stdout=FNULL, shell=True)

print(f"Source branch: {source_branch}")
print(f"Target branch: {target_branch}")
if target_version is None:
    if source_version is None:
        raise Exception("Both branches have no version header. Please add one.")
    for i in [0, 1, 2, 3]:
        if source_version[i] != 0:
            raise ValueError(
                "The target branch has no version header. The first version number"
                + " must be 0.0.0.0"
            )
    sys.exit()

print(
    "Source version number is: "
    f"{source_version[0]}."
    f"{source_version[1]}."
    f"{source_version[2]}."
    f"{source_version[3]}"
)
print(
    "Target version number is: "
    f"{target_version[0]}."
    f"{target_version[1]}."
    f"{target_version[2]}."
    f"{target_version[3]}"
)


def check_version_number_item(
    source_version: List[int],
    target_version: List[int],
    item_name: str,
    item_index: int,
):
    """Check an item of the version number that changed.

    It should only increase by 1 and all lower items must be set to 0.

    Parameters
    ----------
    source_version : List[int]
        Version of the current branch
    target_version : List[int]
        Version of the target branch
    item_name : str
        Name of the item (major, minor, patch or tweak)
    item_index : int
        Index of the item inside the version list

    """
    if source_version[item_index] != target_version[item_index] + 1:
        raise ValueError(
            f"{item_name} version number shouldn't be increased by more than 1 and "
            + "must be larger than the previous one if it changes."
        )
    for i in [j + item_index + 1 for j in range(3 - item_index)]:
        print(i)
        if source_version[i] != 0:
            raise ValueError(
                f"{item_name} version number increased. All lower version numbers "
                + "must be reset to 0"
            )


if source_version[0] != target_version[0]:
    check_version_number_item(source_version, target_version, "Major", 0)
elif source_version[1] != target_version[1]:
    check_version_number_item(source_version, target_version, "Minor", 1)
elif source_version[2] != target_version[2]:
    check_version_number_item(source_version, target_version, "Patch", 2)
else:
    check_version_number_item(source_version, target_version, "Tweak", 3)
