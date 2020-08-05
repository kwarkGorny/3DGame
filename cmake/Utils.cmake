macro (create_shared_library name)
    add_library(${name} SHARED IMPORTED)
    set_target_properties(
        ${name} 
        PROPERTIES IMPORTED_LOCATION
        ${DISTRIBUTION_DIR}/${ANDROID_BUILD_DIR}/obj/local/${ANDROID_ABI}/lib${name}.so
    )
endmacro()

macro (add_sources)
    file (RELATIVE_PATH _relPath "${PROJECT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
    foreach (_src ${ARGN})
        if (_relPath)
            list (APPEND sources "${_relPath}/${_src}")
        else()
            list (APPEND sources "${_src}")
        endif()
    endforeach()
    if (_relPath)
        set (sources ${sources} PARENT_SCOPE)
    endif()
endmacro()

macro (add_source_folder name)
    add_subdirectory(${name})
    string(REPLACE "/" "\\" filtername ${name})
    source_group(${filtername} FILES ${files})
endmacro()