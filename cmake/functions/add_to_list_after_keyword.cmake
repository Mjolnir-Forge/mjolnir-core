#[==[
Add values after a specific keyword in a list and return the result.

If the passed keyword isn't found, it is added together with the new values at the end
of the list.

PARAMETERS:
-----------

    original_list:
        A list containing the keyword

    keyword:
        The keyword after which the values should be added

    returned_list_name:
        Name of the returned list

    ARGN:
        Arbitrary number of values that should be added after the keyword

RETURNS
-------

    ${returned_list_name}:
        The updated list

#]==]
function(add_to_list_after_keyword original_list returned_list_name keyword)

    list(FIND original_list ${keyword} idx_find)

    if(idx_find GREATER -1)
        math(EXPR index_next "${idx_find} + 1")
        list(INSERT original_list ${index_next} ${ARGN})
    else()
        set(original_list ${original_list} ${keyword} ${ARGN})
    endif()

    set(${returned_list_name}
        ${original_list}
        PARENT_SCOPE)
endfunction()
