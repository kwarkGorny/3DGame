cmake_minimum_required(VERSION 3.10)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Building in-tree is not allowed.")
    return()
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

set(EXTERNAL_LOCATION ${CMAKE_SOURCE_DIR}/external)
set(SDL_LOCATION ${EXTERNAL_LOCATION})

project(
    "3DGame"
    DESCRIPTION "simple 3D shoot up game"
    LANGUAGES CXX C
)

include(Settings)
include(Utils)
include(Warnings)

find_package(SDL2 REQUIRED)

add_library(project_warnings INTERFACE)
set_project_warnings(project_warnings)

set(sources_folder "src")
get_subdirectories(source_folders "${CMAKE_SOURCE_DIR}/${sources_folder}")
foreach(folder ${source_folders})
    add_source_folder("${sources_folder}/${folder}")
endforeach()

list(APPEND libs PUBLIC
    project_warnings
    SDL2::Main
)

list(APPEND includes PUBLIC
    "${sources_folder}/"
)

list(APPEND dlls
    ${SDL2_DLLS}
)

add_executable (${PROJECT_NAME} ${sources})

target_include_directories(${PROJECT_NAME} ${includes})

target_link_libraries(${PROJECT_NAME} PUBLIC ${libs})

if(${dlls})
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