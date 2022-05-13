"""Checks if the version number is increased correctly."""

import os
import sys
from typing import List, Union

import git

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


def get_version_number(module: str) -> Union[List[int], None]:
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


def get_version_string(version: Union[None, List[int]]) -> str:
    """Get the version number as string.

    Parameters
    ----------
    version :
        A list containing major, minor, patch and tweak version numbers

    Returns
    -------
    str :
        The version number as string

    """
    if version is not None:
        return f"{version[0]}.{version[1]}.{version[2]}.{version[3]}"
    return "None"


repo = git.Repo(".")
active_branch = repo.active_branch

repo.git.checkout(source_branch)
source_version = get_version_number(module_name)
repo.git.checkout(target_branch)
target_version = get_version_number(module_name)
repo.git.checkout(active_branch)

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

if source_version is not None:
    source_version_str = (
        f"{source_version[0]}.{source_version[1]}.{source_version[2]}."
        "{source_version[3]}"
    )

print(f"Source version number is: {get_version_string(source_version)}")
print(f"Target version number is: {get_version_string(target_version)}")


if source_version is not None and target_version is not None:
    if source_version[0] != target_version[0]:
        check_version_number_item(source_version, target_version, "Major", 0)
    elif source_version[1] != target_version[1]:
        check_version_number_item(source_version, target_version, "Minor", 1)
    elif source_version[2] != target_version[2]:
        check_version_number_item(source_version, target_version, "Patch", 2)
    else:
        check_version_number_item(source_version, target_version, "Tweak", 3)
else:
    raise ValueError("Missing version number for comparison.")
