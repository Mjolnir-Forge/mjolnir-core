# Changelog

## Version 0.0.1 (pre-release)

### Added

- `LinearMemory` in `core/memory/linear_memory.h` - Memory system for fast
  allocation and deallocation
  [[PR #51](https://github.com/Mjolnir-Forge/mjolnir-core/pull/51)]
- `MemorySystemAllocator`  in `core/memory/memory_system_allocator.h`- STL
  compatible allocator that can be used with different memory systems
  [[PR #51](https://github.com/Mjolnir-Forge/mjolnir-core/pull/51)]
- `MemorySystemDeleter` in `core/memory/memory_system_deleter.h` - STL
  compatible deleter that can be used with different memory systems
  [[PR #51](https://github.com/Mjolnir-Forge/mjolnir-core/pull/51)]
- `NewDeleteCounter` in `testing/new_delete_counter.h` - Utility class for
  testing and debugging that counts `new` and `delete` calls
  [[PR #51](https://github.com/Mjolnir-Forge/mjolnir-core/pull/51)]
- `core/memory/definitions.h` - contains memory related definitions and
  concepts [[PR #51](https://github.com/Mjolnir-Forge/mjolnir-core/pull/51)]
- `core/memory/utility.h` - contains utility functions for the memory
  submodule [[PR #51](https://github.com/Mjolnir-Forge/mjolnir-core/pull/51)]
- `core/math/math.h` - contains some basic math functions
  [[PR #11](https://github.com/Mjolnir-Forge/mjolnir-caaore/pull/11)]
- `core/utility/bit_operations.h` - contains functions to read and
  manipulate specific bits
  [[PR #11](https://github.com/Mjolnir-Forge/mjolnir-caaore/pull/11)]
- `core/utility/is_close.h` - contains functions to deal with floating point
  comparison within a certain tolerance
  [[PR #11](https://github.com/Mjolnir-Forge/mjolnir-caaore/pull/11)]
- `core/utility/parameter_pack.h` - contains utility functions for parameter
  packs [[PR #11](https://github.com/Mjolnir-Forge/mjolnir-caaore/pull/11)]
- `core/utility/pointer_operations.h` - contains utility functions for pointers
  [[PR #11](https://github.com/Mjolnir-Forge/mjolnir-caaore/pull/11)]
- `core/utility/type.h` - contains functions for type related operations
  [[PR #11](https://github.com/Mjolnir-Forge/mjolnir-caaore/pull/11)]
- `core/x86/comparison.h` - contains functions for SSE/AVX register comparison
  [[PR #11](https://github.com/Mjolnir-Forge/mjolnir-caaore/pull/11)]
- `core/x86/definitions.h` - contains SSE/AVX related definitions and concepts
  [[PR #11](https://github.com/Mjolnir-Forge/mjolnir-caaore/pull/11)]
- `core/x86/direct_access.h` - contains function to access specific SSE/AVX
  register elements by index
  [[PR #11](https://github.com/Mjolnir-Forge/mjolnir-caaore/pull/11)]
- `core/x86/element_summation.h` - contains functions that calculate the sum of
  specific or all SSE/AVX register elements
  [[PR #11](https://github.com/Mjolnir-Forge/mjolnir-caaore/pull/11)]
- `core/x86/intrinsics.h` - contains template versions of the SSE/AVX
  intrinsics with the register type as template parameter
  [[PR #11](https://github.com/Mjolnir-Forge/mjolnir-caaore/pull/11)]
- `core/x86/permutation.h` - contains functions to permute the elements
  of SSE/AVX registers
  [[PR #11](https://github.com/Mjolnir-Forge/mjolnir-caaore/pull/11)]
- `core/x86/sign_manipulation.h` - contains functions to manipulate the
  sign of the individual SSE/AVX register elements
  [[PR #11](https://github.com/Mjolnir-Forge/mjolnir-caaore/pull/11)]
- `core/x86/x86.h` - includes the correct, operating system dependent x86
  intrinsics header
  [[PR #11](https://github.com/Mjolnir-Forge/mjolnir-caaore/pull/11)]
- `core/definitions.h` - contains core definitions and concepts
  [[PR #11](https://github.com/Mjolnir-Forge/mjolnir-caaore/pull/11)]
- `core/exception.h` - contains exception classes and macros
  [[PR #11](https://github.com/Mjolnir-Forge/mjolnir-caaore/pull/11)]
- `core/fundamental_types.h` - contains type definitions for fundamental
  datatypes like integers and floats
  [[PR #11](https://github.com/Mjolnir-Forge/mjolnir-caaore/pull/11)]
- `core/version.h` - contains version information of this library
  [[PR #11](https://github.com/Mjolnir-Forge/mjolnir-caaore/pull/11)]
