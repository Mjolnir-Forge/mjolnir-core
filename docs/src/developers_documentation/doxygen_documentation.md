# Doxygen documentation {#DoxygenDocumentation}

## Building the documentation

### Local build

To build this documentation locally, you need Doxygen installed.
In its current configuration, the `graphviz` package is also required to build
without warnings or errors.
If you are using a Linux system, just install both with:

~~~
sudo apt-get install doxygen graphviz
~~~

Alternatively, if you have Conda available, you can install the dependencies
using Conda.
A dedicated environment file is located at `docs/environment.yml`.
Now navigate to the root directory and run:

~~~
doxygen .doxyfile
~~~

If you have issues building the documentation, check if your local Doxygen
version is identical to the version used in `environment.yml`.
This environment is used to check that the documentation can be build without
any warnings or errors.
So unless the corresponding GitHub action is also failing, you should be able to
build the documentation with an identical setup.
In case you are using a different Doxygen version and only some warnings are
causing the build to fail (they are treated as errors), you can also set the
option `WARN_AS_ERROR` in the configuration file `.doxyfile` to `NO`.

### ReadTheDocs

The documentation published on `readthedocs.org` is build using the Conda
environment defined in `docs/environment.yml`.
To learn how the setup works, have a look into
[this Stack Overflow question](https://stackoverflow.com/q/36064976/6700329) and
its answers.

