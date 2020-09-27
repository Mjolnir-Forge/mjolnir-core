import sys
import os
import subprocess

module_name = str(sys.argv[1])
source_branch = str(sys.argv[2])
target_branch = str(sys.argv[3])


def get_version_number_item(data, module, item):
    version_string = f"#define MJOLNIR_{module}_VERSION_{item}"
    pos_start = data.find(version_string) + len(version_string)
    pos_end = data.find("\n", pos_start)
    return int(data[pos_start:pos_end])

def get_version_number(module):
    with open(f"mjolnir/{module}/version.h", "r") as file:
        data = file.read()

        module_upper = module.upper()
        major = get_version_number_item(data, module_upper, "MAJOR")
        minor = get_version_number_item(data, module_upper, "MINOR")
        patch = get_version_number_item(data, module_upper, "PATCH")
        tweak = get_version_number_item(data, module_upper, "TWEAK")

        return [major, minor, patch, tweak]


FNULL = open(os.devnull, "w")
source_version = get_version_number(module_name)
subprocess.run(f"git checkout {target_branch}",stdout=FNULL,  shell=True)
target_version = get_version_number(module_name)
subprocess.run(f"git checkout {source_branch}", stdout=FNULL, shell=True)

print(f"Source branch: {source_branch}")
print("Source version number is: "
      f"{source_version[0]}."
      f"{source_version[1]}."
      f"{source_version[2]}."
      f"{source_version[3]}"
      )
print(f"Target branch: {target_branch}")
print("Target version number is: "
     f"{target_version[0]}."
     f"{target_version[1]}."
     f"{target_version[2]}."
     f"{target_version[3]}"
      )

def check_version_number_item(source_version, target_version, item_name, item_index):
    if source_version[item_index] != target_version[item_index] + 1:
        raise ValueError(f"{item_name} version number shouldn't be increased by more than 1 and must be larger than the previous one if it changes.")
    for i in [j+item_index+1 for j in range(3-item_index)]:
        print(i)
        if source_version[i] != 0:
            raise ValueError(f"{item_name} version number increased. All lower version numbers must be reset to 0")

if source_version[0] != target_version[0]:
    check_version_number_item(source_version, target_version, "Major", 0)
elif source_version[1] != target_version[1]:
    check_version_number_item(source_version, target_version, "Minor", 1)
elif source_version[2] != target_version[2]:
    check_version_number_item(source_version, target_version, "Patch", 2)
else:
    check_version_number_item(source_version, target_version, "Tweak", 3)
