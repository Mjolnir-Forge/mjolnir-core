#[[
Process scope keywords (PRIVATE, PUBLIC, INTERFACE) in a list.

Duplicate keywords will be merged and members without scope are added to the
specified default scope.

PARAMETERS:
-----------

    default_scope:
        The default scope for all list elements that are not assigned to any
        scope

    returned_list_name:
        Name of the returned list

    ARGN:
        All list items that should be processed

RETURNS
-------

    ${returned_list_name}:
        The updated list

]]
# silence [C0112]
function(process_scopes default_scope returned_list_name)
    set(scope_keywords INTERFACE PRIVATE PUBLIC)

    if(NOT ${default_scope} IN_LIST scope_keywords)
        message(
            FATAL_ERROR
                "'${default_scope}' is not a valid default scope key word."
        )
    endif()

    set(current_scope ${default_scope})
    string(TOLOWER "${current_scope}_items" list_name)

    foreach(item ${ARGN})
        # Handle scope keywords
        if(${item} IN_LIST scope_keywords)
            set(current_scope ${item})
            string(TOLOWER "${current_scope}_items" list_name)
            continue()
        endif()


        # Add item to correct list
        if(NOT DEFINED ${list_name})
            # cmake-lint: disable=C0103
            set(${list_name} ${current_scope} ${item})
        else()
            # cmake-lint: disable=C0103
            set(${list_name} ${${list_name}} ${item})
        endif()

    endforeach()

    set(${returned_list_name}
        ${public_items} ${private_items} ${interface_items}
        PARENT_SCOPE)
endfunction()
