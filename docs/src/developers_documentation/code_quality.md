# Code Quality


## External tools

### Clang Static Analyzer

To run this tool create a new build directory and run:

~~~ shell
scan-build-10 --status-bugs cmake -DCMAKE_BUILD_TYPE=DEBUG $PATH_TO_SOURCE
scan-build-10 --status-bugs make -j2
~~~

Note that you might need to use a different version number of the tool.

IMPORTANT: Always clear the directory (`rm -rf *`) and run both commands.
It seems like the tool doesn't find any bugs after the first run for some reason.


### Cppcheck

The root directory contains a cppcheck project file named `.cppcheck`.
If you have cppcheck installed, run the following command to perform the checks on your local machine:

~~~ shell
cppcheck --project=.cppcheck --dump
~~~

Note that the config file enables addons that require Python 3.
If you want to run cppcheck without the addons, use:

~~~ shell
cppcheck -Isrc --enable=all --suppress=missingIncludeSystem tests src
~~~

This should basically give you the same test results just without the ones added by the addons.

You can also add a C++ standard with the option `--std=`.
For example, use `--std=c++17` to enable C++ 17 support
