# Guard gegen mehrfache Includes
if(PROVIDE_LIBRARYS_INCLUDED)

    return()
endif()
set(PROVIDE_LIBRARYS_INCLUDED TRUE)

function(provideLib targetDir repoUrl includeDirs cppDirs linkLibs extraOptions gitTag)
    
    if(NOT EXISTS ${targetDir})

        # GitHub Projekt clonen und submodules rekursiv updaten
        find_package(Git REQUIRED)
        message(STATUS "Cloning ${repoUrl} into ${targetDir}")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} clone --recursive --progress ${repoUrl} ${targetDir}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE git_result
        )
        if(NOT git_result EQUAL "0")
            message(FATAL_ERROR "Failed to clone ${repoUrl}")
        endif()

        execute_process(

            COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${targetDir}
            RESULT_VARIABLE git_result
        )

        #
        if(NOT gitTag STREQUAL "")

            execute_process(
                COMMAND ${GIT_EXECUTABLE} checkout ${gitTag}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${targetDir}
                RESULT_VARIABLE git_result
            )
            if(NOT git_result EQUAL "0")
                message(FATAL_ERROR "Failed to checkout tag ${gitTag}")
            endif()
            
            message(STATUS "Checked out tag ${gitTag}")
        endif()
    else()

        message(STATUS "Repository ${targetDir} exists")
    endif()

    #
    foreach(dir ${includeDirs})
        list(APPEND INCLUDES ${dir})
    endforeach()
    set(INCLUDES ${INCLUDES} PARENT_SCOPE)
    
    #
    set(LOCAL_CPPOBJS ${CPPOBJS})
    foreach(pattern ${cppDirs})

        if(pattern MATCHES "\\.(c|cpp|cc)$")

            list(APPEND LOCAL_CPPOBJS "${pattern}")

        else()

            # Ordner checken
            file(GLOB_RECURSE found_cpp_files "${pattern}/*.cpp")
            file(GLOB_RECURSE found_c_files "${pattern}/*.c")
            set(found_files ${found_cpp_files} ${found_c_files})
            
            if(found_files)

                message(STATUS "Pattern '${pattern}' resolved to ${found_files}")
                list(APPEND LOCAL_CPPOBJS ${found_files})
            endif()
        endif()
    endforeach()
    set(CPPOBJS ${LOCAL_CPPOBJS} PARENT_SCOPE)
    
    # Links
    foreach(lib ${linkLibs})
        list(APPEND LINKLIBS ${lib})
    endforeach()
    set(LINKLIBS ${LINKLIBS} PARENT_SCOPE)

    #
    foreach(option ${extraOptions})
    
        string(REPLACE "=" ";" option_pair ${option})
        list(GET option_pair 0 option_name)
        list(GET option_pair 1 option_value)
        set(${option_name} ${option_value} CACHE BOOL "" FORCE)
    endforeach()
    
    add_subdirectory(${targetDir})

endfunction()

function(provideHeaderOnlyLib targetDir repoUrl includeDirs extraOptions gitTag)

    provideLib("${targetDir}" "${repoUrl}" "${includeDirs}" "" "" "${extraOptions}" "${gitTag}")
endfunction()

function(includePrebuildLib label includes location)

    #
    add_library(${label} STATIC IMPORTED)

    #
    set_target_properties(${label} PROPERTIES
        IMPORTED_LOCATION "${location}"
        INTERFACE_INCLUDE_DIRECTORIES "${includes}"
    )

    list(APPEND LINKLIBS ${label})
    set(LINKLIBS ${LINKLIBS} PARENT_SCOPE)

endfunction()

function(getLib targetDir repoUrl gitTag)
    
    if(NOT EXISTS ${targetDir})

        # GitHub Projekt clonen und submodules rekursiv updaten
        find_package(Git REQUIRED)
        message(STATUS "Cloning ${repoUrl} into ${targetDir}")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} clone --recursive --progress ${repoUrl} ${targetDir}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE git_result
        )
        if(NOT git_result EQUAL "0")
            message(FATAL_ERROR "Failed to clone ${repoUrl}")
        endif()

        execute_process(

            COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${targetDir}
            RESULT_VARIABLE git_result
        )

        #
        if(NOT gitTag STREQUAL "")

            execute_process(
                COMMAND ${GIT_EXECUTABLE} checkout ${gitTag}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${targetDir}
                RESULT_VARIABLE git_result
            )
            if(NOT git_result EQUAL "0")
                message(FATAL_ERROR "Failed to checkout tag ${gitTag}")
            endif()
            
            message(STATUS "Checked out tag ${gitTag}")
        endif()
    else()

        message(STATUS "Repository ${targetDir} exists")
    endif()

endfunction()

function(getModule targetDir repoUrl gitTag)
    
    if(NOT EXISTS ${targetDir})

        # GitHub Projekt clonen und submodules rekursiv updaten
        find_package(Git REQUIRED)
        message(STATUS "Cloning ${repoUrl} into ${targetDir}")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} clone --progress ${repoUrl} ${targetDir}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE git_result
        )
        if(NOT git_result EQUAL "0")
            message(FATAL_ERROR "Failed to clone ${repoUrl}")
        endif()

        execute_process(

            COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${targetDir}
            RESULT_VARIABLE git_result
        )

        #
        if(NOT gitTag STREQUAL "")

            execute_process(
                COMMAND ${GIT_EXECUTABLE} checkout ${gitTag}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${targetDir}
                RESULT_VARIABLE git_result
            )
            if(NOT git_result EQUAL "0")
                message(FATAL_ERROR "Failed to checkout tag ${gitTag}")
            endif()
            
            message(STATUS "Checked out tag ${gitTag}")
        endif()
    else()

        message(STATUS "Repository ${targetDir} exists")
    endif()

endfunction()