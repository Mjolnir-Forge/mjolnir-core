# Code Quality {#CodeQuality}

## Introduction

This section lists all external tools that are used to maintain a good code
quality standard. Every time a new commit is pushed to the repository, they run
inside a GitHub action and are required to pass in order to merge a pull request
into the master branch. Most of them can be run locally. The following sections
describe the tools and how to run them. In case the descriptions do not work,
have a look into the GitHub workflow files (`.github/workflows`) and check which
commands they use. If something differs, consider opening an issue to request an
update of this documentation.

## C++ tools

### Clang-Format

Clang-format automatically refactors your code to match a specified format.
It is possible to run it from the command line, for example with:

~~~
clang-format -i <FILE>
~~~

However, most IDEs support clang-format so that your code gets formatted
everytime the file is saved.
If you use QtCreator, enable the beautifier plugin in the C++ section.
This generates a new submenu in the Options where you can select and configure
clang-format.
The configuration file `.clang-format` is located in the root directory.

Every time you push to the GitHub repository, all files are checked.
In case that a file is not formatted correctly, the corresponding action fails
and pull requests can't be merged into the master branch until all issues are
fixed.

### Clang-Tidy

Clang-tidy checks that the code follows several guidelines. It also checks the
coding style, which is defined in the file `.clang-tidy`. It only runs on `.cpp`
files and can check just a single file per execution. You can run it like this:

~~~ shell
clang-tidy-<VERSION> <FILE>
~~~

However, this will most likely produce unwanted errors because of missing
includes or ignore the code of the included headers.
This can be solved by additional parameters that are explained in the
[official documentation](https://clang.llvm.org/extra/clang-tidy/) (check for
newer versions).
There is a special python script to run clang-tidy on all project files with the
correct setup.
It can be executed with:

~~~ shell
python python/check_clang_tidy.py
~~~

You can specify which clang-tidy version should by adding `--version <VERSION>`.
To list all other available options, use:

~~~ shell
python python/check_clang_tidy.py -h
~~~

### Clang Sanitizer

Clang comes with different sanitizer tools that can be activated with additional
compiler and linker commands.
When the compiled code is executed it generates extra warnings and errors if
problems are detected.
Have a look into the file
`.github/workflows/compiletime_and_runtime_analysis.yml` to get an overview
which tools are currently used to maintain a good code quality.
The basic usage is mostly the same.
Run CMake as follows:

~~~
cmake -DCMAKE_BUILD_TYPE=DEBUG \
      -DCMAKE_C_COMPILER=clang \
      -DCMAKE_CXX_COMPILER=clang++ \
      -DMJOLNIR_CORE_ADDITIONAL_COMPILE_OPTIONS="-fsanitize=<TOOL>" \
      -DMJOLNIR_CORE_ADDITIONAL_LINK_OPTIONS="-fsanitize=<TOOL>" \
      -S . -B <BUILD_DIR>
~~~

The CMake variables `MJOLNIR_CORE_ADDITIONAL_COMPILE_OPTIONS` and
`MJOLNIR_CORE_ADDITIONAL_LINK_OPTIONS` are used to build all library files with
the specified options.
Now run the following command to build:

~~~
cmake --build <BUILD_DIR> -j <NUM_THREADS>
~~~

Finally, run the compiled code to see if there are any issues.
In the GitHub actions, all tests are build and run with different tools:

~~~
 ctest --output-on-failure
~~~

You can always have a look into `compiletime_and_runtime_analysis.yml` if you
are not sure how to run a specific tool that runs on the GitHub repository.

### Clang Static Analyzer

To run this tool, create a new build directory and run:

~~~ shell
scan-build-10 --status-bugs cmake -DCMAKE_BUILD_TYPE=DEBUG $PATH_TO_SOURCE
scan-build-10 --status-bugs make -j2
~~~

Note that you might need to use a different version number of the tool.

IMPORTANT: Always clear the directory (`rm -rf *`) and run both commands. It
seems like the tool doesn't find any bugs after the first run for some reason.

### CppCheck

The root directory contains a cppcheck project file named `.cppcheck`. If you
have cppcheck installed, run the following command to perform the checks on your
local machine:

~~~ shell
cppcheck --project=.cppcheck --dump
~~~

Note that the config file enables addons that require Python 3. If you want to
run cppcheck without the addons, use:

~~~ shell
cppcheck -Isrc --enable=all --suppress=missingIncludeSystem tests src
~~~

This should basically give you the same test results just without the ones added
by the addons.

You can also add a C++ standard with the option `--std=`. For example, use
`--std=c++17` to enable C++ 17 support

## CMake tools

### cmake-format

`cmake-format` is a Python based tool that checks the formatting of CMake files.
To install it locally, use Pythons package manager `pip`:

~~~ shell
pip install cmakelang
~~~

You can run it from the libraries root directory with:

~~~ shell
cmake-format <FILE> --check -c .cmake-format.yaml
~~~

The `--check` option lets the program fail in case of a formatting error.
`-c .cmake-format.yaml` specifies the configuration file that should be used. So
in case you want to run the program inside any other directory than the rood
directory, you have to adjust the path. To let `cmake-format` correct any
formatting errors, simply replace `--check` with `-i`:

~~~ shell
cmake-format <FILE> -i -c .cmake-format.yaml
~~~

Unfortunately, there is no build in mechanism to check multiple files at once.
However, there are two python scripts that perform this task. To check all files
run the following command (requires Python3) in the root directory:

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

`cmake-lint` comes together with `cmake-format` when installing `cmakelang`. Its
usage is quite similar to `cmake-format` and both share the same configuration
file:

~~~ shell
cmake-lint <FILE> -c .cmake-format.yaml
~~~

It also has the limitation that it runs only with a single file. The Python
script that runs `cmake-lint` for all files can be executed with:

~~~ shell
python python/check_cmake_lint.py
~~~

The official documentation can be
found [here](https://cmake-format.readthedocs.io/en/latest/)
