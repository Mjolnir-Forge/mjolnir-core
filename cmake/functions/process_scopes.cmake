#[==[
Process scope keywords (PRIVATE, PUBLIC, INTERFACE) in a list.

Duplicate keywords will be merged and members without scope are added to the specified
default scope.

PARAMETERS:
-----------

    default_scope:
        The default scope for all list elements that are not assigned to any scope

    returned_list_name:
        Name of the returned list

    ARGN:
        All list items that should be processed

RETURNS
-------

    ${returned_list_name}:
        The updated list

#]==]
function(process_scopes default_scope returned_list_name)
    set(scope_keywords INTERFACE PRIVATE PUBLIC)

    if(NOT ${default_scope} IN_LIST scope_keywords)
        message(
            FATAL_ERROR
                "'${default_scope}' is not a valid scope keyword and can't be used as default scope."
        )
    endif()

    set(current_scope ${default_scope})

    foreach(item ${ARGN})
        # Handle scope keywords
        if(${item} IN_LIST scope_keywords)
            set(current_scope ${item})
            continue()
        endif()

        # Add item to correct list
        if(NOT DEFINED "${current_scope}_ITEMS")
            set("${current_scope}_ITEMS" ${current_scope} ${item})
        else()
            set("${current_scope}_ITEMS" ${${current_scope}_ITEMS} ${item})
        endif()

    endforeach()

    set(${returned_list_name}
        ${PUBLIC_ITEMS} ${PRIVATE_ITEMS} ${INTERFACE_ITEMS}
        PARENT_SCOPE)
endfunction()
