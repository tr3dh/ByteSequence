# Modul das aggressive Optimierungen für den Release bietet

# Guard gegen mehrfach Includes
if(AGGRESSIVE_OPTIMIZATIONS_INCLUDED)
    return()
endif()
set(AGGRESSIVE_OPTIMIZATIONS_INCLUDED TRUE)

#
function(apply_aggressive_optimizations_to_interface TARGET_NAME)

    message(STATUS "Applying aggressive optimizations to target: ${TARGET_NAME}")
    
    if(MSVC)
        target_compile_options(${TARGET_NAME} INTERFACE
            /O2                 # Maximum Speed
            /Ob3                # Aggressive Inlining
            /Oi                 # Intrinsic Functions
            /Ot                 # Favor Fast Code
            /GL                 # Whole Program Optimization
            /fp:fast            # Fast Floating Point
            /arch:AVX2          # AVX2 Instructions
            /GS-                # No Buffer Security Check
            /Gy                 # Function-Level Linking
            /Gw                 # Optimize Global Data
            /favor:AMD64        # x64 Optimization
        )
        
        target_link_options(${TARGET_NAME} INTERFACE
            /LTCG               # Link Time Code Generation
            /OPT:REF            # Remove Unreferenced
            /OPT:ICF            # Identical COMDAT Folding
            /INCREMENTAL:NO     # No Incremental Linking
        )
        
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        target_compile_options(${TARGET_NAME} INTERFACE
            -O3
            -march=native
            -mtune=native
            -ffast-math
            -funroll-loops
            -fomit-frame-pointer
            -finline-functions
            -ftree-vectorize
            -ftree-loop-vectorize
            -fvect-cost-model=unlimited
            -fpredictive-commoning
            -fgcse-after-reload
            -ftree-partial-pre
            -funswitch-loops
            -fpeel-loops
            -fipa-cp-clone
            -fno-signed-zeros
            -fno-trapping-math
            -fassociative-math
            -freciprocal-math
            -fno-math-errno
            -ffinite-math-only
        )
        
        if(NOT CMAKE_BUILD_TYPE MATCHES Debug)
            target_compile_options(${TARGET_NAME} INTERFACE -flto=auto)
            target_link_options(${TARGET_NAME} INTERFACE -flto=auto -fuse-linker-plugin)
        endif()
        
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(${TARGET_NAME} INTERFACE
            -O3
            -march=native
            -mtune=native
            -ffast-math
            -funroll-loops
            -fomit-frame-pointer
            -finline-functions
            -fvectorize
            -fslp-vectorize
            -fno-signed-zeros
            -fno-trapping-math
            -fassociative-math
            -freciprocal-math
            -fno-math-errno
            -ffinite-math-only
        )
        
        if(NOT CMAKE_BUILD_TYPE MATCHES Debug)
            target_compile_options(${TARGET_NAME} INTERFACE -flto=thin)
            target_link_options(${TARGET_NAME} INTERFACE -flto=thin)
        endif()
    endif()
    
    message(STATUS "  ✓ Aggressive optimizations applied to ${TARGET_NAME}")
endfunction()