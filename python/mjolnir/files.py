"""Contains functions that are related to files."""

from pathlib import Path
from typing import List

from .definitions import PATH_MJOLNIR_ROOT_DIR


def get_file_paths(
    search_dir: Path, file_patterns: List[str], ignore_patterns: List[str] = None
) -> List[str]:
    """Return a list of file paths that match a certain file pattern.

    Parameters
    ----------
    search_dir :
        The directory that should be searched recursively
    file_patterns :
        A list of file patterns that should be considered
    ignore_patterns :
        If a filepath contains one of the strings in this list, it is rejected

    Returns
    -------
    List[str] :
        List of file paths that match the passed file pattern.

    """
    if ignore_patterns is None:
        ignore_patterns = []

    files = []
    for pattern in file_patterns:
        pattern_files = [
            path.absolute().as_posix()
            for path in search_dir.rglob(pattern)
            if not any(word in str(path) for word in ignore_patterns)
        ]
        files += pattern_files
    return files


def get_cmake_files() -> List[str]:
    """Get all list of all cmake files.

    Returns
    -------
    List[str] :
        List of all cmake files

    """
    return get_file_paths(
        PATH_MJOLNIR_ROOT_DIR,
        file_patterns=["*.cmake", "*CMakeLists.txt"],
        ignore_patterns=["build"],
    )
