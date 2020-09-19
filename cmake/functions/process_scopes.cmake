function(process_scopes default_scope returned_list_name)
    set(scope_keywords INTERFACE PRIVATE PUBLIC)

    if(NOT ${default_scope} IN_LIST scope_keywords)
        message(FATAL_ERROR "'${default_scope}' is not a valid scope keyword and can't be used as default scope.")
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

    set(${returned_list_name} ${PUBLIC_ITEMS} ${PRIVATE_ITEMS} ${INTERFACE_ITEMS} PARENT_SCOPE)
endfunction()



