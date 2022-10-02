#[==[
Include all cmake functions from the "cmake/functions" folder.
#]==]

include(functions/add_to_list_after_keyword)
include(functions/process_scopes)
include(functions/target_apply_setup)

include(functions/add_generic_executable)
include(functions/add_generic_benchmark)
include(functions/add_generic_test)

include(functions/add_mjolnir_test)
include(functions/add_mjolnir_benchmark)
include(functions/extract_mjolnir_version_number)

include(functions/add_mjolnir_core_test)
include(functions/add_mjolnir_core_benchmark)
