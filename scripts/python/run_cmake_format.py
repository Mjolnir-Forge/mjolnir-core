from pathlib import Path

ignore = ["build"]
patterns = ["*.cmake", "*CMakeLists.txt"]


files = []
for pattern in patterns:
    pattern_files = [
        path
        for path in Path("../..").rglob(pattern)
        if not any(word in str(path) for word in ignore)
    ]
    files += pattern_files

for path in files:
    print(path)
