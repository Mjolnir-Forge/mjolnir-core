# Installation

## Build from scratch

To build this library from scratch, you first have to clone the repository from
GitHub. For the core module, you can use:

~~~ shell
git clone https://github.com/Mjolnir-Forge/mjolnir-core.git
~~~

Now run CMake:

~~~ shell
cmake -S . -B <BUILD_DIR>
~~~

This will create a new directory `<BUILD_DIR>` and create all build files inside
of it. It might occur that CMake fails due to an incompatible compiler version.
In this case, update your compiler to the required version. You can specify a
specific C++ compiler version by adding `-DCMAKE_CXX_COMPILER=<YOUR_COMPILER>`
to the command above. Alternatively, you can also add
`-DIGNORE_CXX_COMPILER_COMPATIBILITY=True` to ignore the result of the
compatibility check. However, this is strongly discouraged and will most likely
cause the build to fail.

If CMake succeeded, start the build process with:

~~~ shell
cmake --build <BUILD_DIR>
~~~

Adding `-j N` will accelerate the build process by using `N` processor cores.

## Build options

The following CMake options can be used to customize the build:

- `MJOLNIR_CORE_ENABLE_TESTS`: Enables the tests of the core module

- `MJOLNIR_CORE_ENABLE_COMPILER_EXTENSIONS`: Enables compiler extensions for the
  core module

- `MJOLNIR_CORE_ENABLE_LTO`: Enables link time optimizations for the core module

The following preprocessor defines also affect the build:

- `STATIC_ANALYSIS`: If defined, code sections that have no effect on the
  quality of the static analysis but cause a huge overhead are removed or
  modified. An example would be the `TYPED_TEST_SERIES` macro defined in
  `mjolnir/testing/typed_test_series.h`. Have a look at its doxygen
  documentation for more information.

## Additional CMake parameters

The following list collects additional parameters that can be passed to CMake
via the command line in the form `-D<PARAMETER_NAME>=`:

- `IGNORE_CXX_COMPILER_COMPATIBILITY`: If the parameters content is evaluates to
  `True`, errors due to incompatible compiler versions will be downgraded to
  warnings

- `MJOLNIR_CORE_ADDITIONAL_COMPILE_FEATURES`: Lets you specify additional CMake
  compile features that should be used during the build process of the core
  library

- `MJOLNIR_CORE_ADDITIONAL_COMPILE_OPTIONS`: Lets you specify additional
  compiler options that should be used during the build process of the core
  library

- `MJOLNIR_CORE_ADDITIONAL_LINK_OPTIONS`: Lets you specify additional linker
  options that should be used during the build process of the core library

- `MJOLNIR_ROOT_DIR`: Specifies the root directory of the full library

- `MJOLNIR_CORE_ROOT_DIR`: Specifies the root directory of the core library
