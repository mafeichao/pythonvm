cmake_minimum_required(VERSION 3.16)

project(vmtest)

set(SRCFILES
    alltest.py
    cf/*.py
    object/*.py
    func/*.py
    gc/*.py
    module/*.py
)

set(TESTFILES
    alltest.pyc
)

add_custom_command(OUTPUT ${TESTFILES}
    POST_BUILD
    #DEPENDS ${SRCFILES}
    COMMAND echo "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/main.py
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(Test ALL
    DEPENDS ${TESTFILES}
)

