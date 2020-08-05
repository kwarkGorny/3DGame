cmake_minimum_required(VERSION 3.10)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Building in-tree is not allowed.")
    return()
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

project(
    "3DGame"
    DESCRIPTION "simple 3D shoot up game"
    LANGUAGES CXX C
)

include(Settings)
include(Utils)
include(Warnings)

add_library(project_warnings INTERFACE)
set_project_warnings(project_warnings)

add_source_folder(src/core)

list(APPEND libs PUBLIC
    project_warnings
)

list(APPEND includes PUBLIC
    src/
)

list(APPEND dlls PUBLIC
)

add_executable (${PROJECT_NAME} ${sources})

target_include_directories(${PROJECT_NAME} ${includes})

if(${dlls})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${libs})

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Copying required dlls.."
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${dlls}
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/data)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Copying required data assets.."
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/data
    )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "${PROJECT_NAME}")