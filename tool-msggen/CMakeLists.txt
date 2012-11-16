cmake_minimum_required(VERSION 2.8.5)
project(tool-msggen)

bison_target(tool-msggen-parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c COMPILE_FLAGS "-d -y")
flex_target(tool-msggen-lexer lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.c COMPILE_FLAGS \"--header-file=${CMAKE_CURRENT_BINARY_DIR}/lexer.h\")
add_flex_bison_dependency(tool-msggen-lexer tool-msggen-parser)
if(NOT MSVC)
    set_source_files_properties(${FLEX_tool-msggen-lexer_OUTPUTS} ${BISON_tool-msggen-parser_OUTPUTS} PROPERTIES COMPILE_FLAGS -Wno-implicit-function-declaration)
endif()

add_executable(tool-msggen
    ${FLEX_tool-msggen-lexer_OUTPUTS}
    ${BISON_tool-msggen-parser_OUTPUTS}
    parser.y
    lexer.l
)
include_directories(
    ${FLEX_UNISTD_INCLUDE}
    ${simclist_SOURCE_DIR}
    ${libdcpu_SOURCE_DIR}
    ${bstring_SOURCE_DIR}
    ${argtable2_SOURCE_DIR}
)
target_link_libraries(tool-msggen
    simclist
    bstring
    argtable2
)

function(generate_messages out_header out_source)
    set(files "")
    foreach(i ${ARGN})
        list(APPEND files ${CMAKE_CURRENT_SOURCE_DIR}/${i})
    endforeach()
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${out_header} ${CMAKE_CURRENT_BINARY_DIR}/${out_source}
        COMMAND tool-msggen ${CMAKE_CURRENT_BINARY_DIR}/${out_header} ${CMAKE_CURRENT_BINARY_DIR}/${out_source} ${files}
        DEPENDS tool-msggen ${files}
        COMMENT "Generating messages..."
        )
endfunction()
