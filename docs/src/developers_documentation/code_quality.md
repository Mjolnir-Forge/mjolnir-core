# Code Quality

## Introduction

This section lists all external tools that are used to maintain a good code quality
standard. Every time a new commit is pushed to the repository, they run inside a GitHub
action and are required to pass in order to merge a pull request into the master branch.
Most of them can be run locally. The following sections describe the tools and how to
run them. In case the descriptions do not work, have a look into the GitHub workflow
files (`.github/workflows`) and check which commands they use. If something differs,
consider opening an issue to request an update of this documentation.

## C++ tools

### Clang Static Analyzer

To run this tool, create a new build directory and run:

~~~ shell
scan-build-10 --status-bugs cmake -DCMAKE_BUILD_TYPE=DEBUG $PATH_TO_SOURCE
scan-build-10 --status-bugs make -j2
~~~

Note that you might need to use a different version number of the tool.

IMPORTANT: Always clear the directory (`rm -rf *`) and run both commands. It seems like
the tool doesn't find any bugs after the first run for some reason.

### Cppcheck

The root directory contains a cppcheck project file named `.cppcheck`. If you have
cppcheck installed, run the following command to perform the checks on your local
machine:

~~~ shell
cppcheck --project=.cppcheck --dump
~~~

Note that the config file enables addons that require Python 3. If you want to run
cppcheck without the addons, use:

~~~ shell
cppcheck -Isrc --enable=all --suppress=missingIncludeSystem tests src
~~~

This should basically give you the same test results just without the ones added by the
addons.

You can also add a C++ standard with the option `--std=`. For example, use `--std=c++17`
to enable C++ 17 support

## CMake tools

### cmake-format

`cmake-format` is a Python based tool that checks the formatting of CMake files. To
install it locally, use Pythons package manager `pip`:

~~~ shell
pip install cmakelang
~~~

You can run it from the libraries root directory with:

~~~ shell
cmake-format <FILE> --check -c .cmake-format.yaml
~~~

The `--check` option lets the program fail in case of a formatting error.
`-c .cmake-format.yaml` specifies the configuration file that should be used. So in case
you want to run the program inside any other directory than the rood directory, you have
to adjust the path. To let `cmake-format` correct any formatting errors, simply replace
`--check` with `-i`:

~~~ shell
cmake-format <FILE> -i -c .cmake-format.yaml 
~~~

Unfortunately, there is no build in mechanism to check multiple files at once. However,
there are two python scripts that perform this task. To check all files run the
following command (requires Python3) in the root directory:

~~~ shell
python python/check_cmake_format.py
~~~

To reformat all files run:

~~~ shell
python python/format_cmake_files.py 
~~~

The official documentation can be
found [here](https://cmake-format.readthedocs.io/en/latest/)

### cmake-lint

`cmake-lint` comes together with `cmake-format` when installing `cmakelang`. Its usage
is quite similar to `cmake-format` and both share the same configuration file:

~~~ shell
cmake-lint <FILE> -c .cmake-format.yaml
~~~

It also has the limitation that it runs only with a single file. The Python script that
runs `cmake-lint` for all files can be executed with:

~~~ shell
python python/check_cmake_lint.py
~~~

The official documentation can be
found [here](https://cmake-format.readthedocs.io/en/latest/)
