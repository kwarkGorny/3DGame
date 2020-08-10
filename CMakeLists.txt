cmake_minimum_required(VERSION 3.10)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Building in-tree is not allowed.")
    return()
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(LIB_PATH_SUFFIX x64)
else()
  set(LIB_PATH_SUFFIX x86)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

set(EXTERNAL_LOCATION ${CMAKE_SOURCE_DIR}/external)

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

find_package(OpenGL REQUIRED)

list(APPEND CMAKE_PREFIX_PATH "${EXTERNAL_LOCATION}/glew/")
find_package(GLEW REQUIRED)

list(APPEND CMAKE_PREFIX_PATH "${EXTERNAL_LOCATION}/SDL2/")
find_package(SDL2 REQUIRED)

set(sources_folder "src")
get_subdirectories(source_folders "${CMAKE_SOURCE_DIR}/${sources_folder}")
foreach(folder ${source_folders})
    add_source_folder("${sources_folder}/${folder}")
endforeach()

list(APPEND libs PUBLIC
    project_warnings
    GLEW::GLEW
    SDL2::Main
    opengl32
)

add_subdirectory(${EXTERNAL_LOCATION}/lua)

list(APPEND includes PUBLIC
    "${sources_folder}/"
    ${EXTERNAL_LOCATION}/glm/
    ${EXTERNAL_LOCATION}/stb/src/
    ${EXTERNAL_LOCATION}/tinyobjloader/src/
    ${EXTERNAL_LOCATION}/spdlog/include/
    ${EXTERNAL_LOCATION}/entt/src/
    ${EXTERNAL_LOCATION}/sol/include/
    ${EXTERNAL_LOCATION}/lua/src/
    ${GLEW_INCLUDE_DIRS}
)

list(APPEND STB_DEFINITIONS 
    -DSTB_IMAGE_IMPLEMENTATION
    -DSTBI_ONLY_PNG
)

list(APPEND WIN_DEFINITIONS 
    -D_CRT_SECURE_NO_WARNINGS
    -DNOMINMAX
)

list(APPEND GLM_DEFINITIONS 
    -DGLM_ENABLE_EXPERIMENTAL
)

list(APPEND SOL_DEFINITIONS 
    -DSOL_NO_EXCEPTIONS
    -DSOL_NO_RTTI
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEVELOPMENT")

add_definitions(${GLEW_DEFINITIONS} ${STB_DEFINITIONS} ${WIN_DEFINITIONS} ${GLM_DEFINITIONS} ${SOL_DEFINITIONS})

add_executable (${PROJECT_NAME} ${sources})

target_include_directories(${PROJECT_NAME} ${includes})

target_link_libraries(${PROJECT_NAME} PUBLIC ${libs})

if(EXISTS ${CMAKE_SOURCE_DIR}/data)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Copying required data assets.."
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/data
    )
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Copying required dlls.."
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${EXTERNAL_LOCATION}/SDL2/lib/${LIB_PATH_SUFFIX}/SDL2.dll"
    "${EXTERNAL_LOCATION}/glew/bin/Release/x64/glew32.dll"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "${PROJECT_NAME}")