"""Contains some global definitions."""
from pathlib import Path

PATH_MJOLNIR_ROOT_DIR = Path(__file__).parents[2]
MJOLNIR_ROOT_DIR = PATH_MJOLNIR_ROOT_DIR.absolute().as_posix()

CMAKE_CONFIG_FILE_PATH = f"{MJOLNIR_ROOT_DIR}/.cmake-format.yaml"
