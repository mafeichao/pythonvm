cmake_minimum_required(VERSION 3.16)

project(vmtest)

set(SRCFILES
    alltest.py
    attr.py
    church.py
    closure.py
    dict_view.py
    emro.py
    fib.py
    func.py
    gc.py
    make_func.py
    mro.py
    oom.py
    op.py
    st_attr.py
    subscr.py
    test_all.py
    test_builtin.py
    test_class.py
    test_closure.py
    test_default.py
    test_dict.py
    test_for.py
    test_func.py
    test_if.py
    test_len.py
    test_list.py
    test_param.py
    test_ret.py
    test_sort.py
    test_string.py
    test_type.py
    test_var.py
    test_while.py
    timpt.py
    tsum.py
    vec.py
    )

set(TESTFILES
    alltest.pyc
    attr.pyc
    church.pyc
    closure.pyc
    dict_view.pyc
    emro.pyc
    fib.pyc
    func.pyc
    gc.pyc
    make_func.pyc
    mro.pyc
    oom.pyc
    op.pyc
    st_attr.pyc
    subscr.pyc
    test_builtin.pyc
    test_class.pyc
    test_closure.pyc
    test_default.pyc
    test_dict.pyc
    test_for.pyc
    test_func.pyc
    test_if.pyc
    test_len.pyc
    test_list.pyc
    test_param.pyc
    test_ret.pyc
    test_sort.pyc
    test_string.pyc
    test_type.pyc
    test_var.pyc
    test_while.pyc
    timpt.pyc
    tsum.pyc
    vec.pyc
)

add_custom_command(OUTPUT ${TESTFILES}
    POST_BUILD
    DEPENDS ${SRCFILES}
    COMMAND echo "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/test_all.py
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(Test ALL
    DEPENDS ${TESTFILES}
)

