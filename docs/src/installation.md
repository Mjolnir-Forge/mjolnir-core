# Installation {#Installation}

## Build from scratch

To build this library from scratch, you first have to clone the repository from GitHub.
For the core module, you can use:

~~~
git clone https://github.com/Mjolnir-Forge/mjolnir-core.git
~~~

Now run CMake:

~~~
cmake -S . -B <BUILD_DIR>
~~~

This will create a new directory `<BUILD_DIR>` and create all build files inside of it.
It might occur that CMake fails due to an incompatible compiler version.
In this case, update your compiler to the required version.
You can specify a specific C++ compiler version by adding 
`-DCMAKE_CXX_COMPILER=<YOUR_COMPILER>` to the command above.
Alternatively, you can also add `-DIGNORE_CXX_COMPILER_COMPATIBILITY=True` to ignore the
result of the compatibility check.
However, this is strongly discouraged and will most likely cause the build to fail.

If CMake succeeded, start the build process with:

~~~
cmake --build <BUILD_DIR>
~~~

Adding `-j N` will accelerate the build process by using `N` processor cores.

## Additional CMake parameters

The following list collects additional parameters that can be passed to CMake via the
command line in the form `-D<PARAMETER_NAME>=`:

**`IGNORE_CXX_COMPILER_COMPATIBILITY`:**

If the parameters content is evaluates to `True`, errors due to incompatible compiler 
versions will be downgraded to warnings


