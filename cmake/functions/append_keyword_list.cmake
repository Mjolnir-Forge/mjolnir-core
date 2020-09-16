#[==[
Add values after a specific keyword in a list.

If the passed keyword isn't found, it is added together with the new values at the end of the list.

PARAMETERS:
-----------

    keyword_list:
        A list containing the keyword

    keyword:
        The keyword after which the values should be added

    ARGN:
        Arbitrary number of values that should be added after the keyword

RETURNS
-------

    keyword_list:
        The updated list

#]==]
function(append_keyword_list keyword_list keyword)
    list(FIND keyword_list ${keyword} idx_find)
    if(idx_find GREATER -1)
      MATH(EXPR index_next "${idx_find} + 1")
      list(INSERT keyword_list ${index_next} ${ARGN})
    else()
        set(keyword_list ${keyword_list} ${keyword} ${ARGN})
    endif()
    set(keyword_list ${keyword_list} PARENT_SCOPE)
endfunction()
