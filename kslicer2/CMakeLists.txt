
add_executable(kslicer2
    main.cpp
    io.cpp
    utils.cpp
    errors.cpp
    codegen_host.cpp
    class_info.cpp
    codegen.cpp
    compiler.cpp
    command_line.cpp
    command_line_parser.cpp
    config.cpp
    kslicer_cpp_attributes.cpp
    template_rendering.cpp
)

# Imported target LLVMDebugInfoPDB wants to link with a library which probably wouldn't be present.
# Clearing INTERFACE_LINK_LIBRARIES does not break linking, so that library is not need anyway.
set_target_properties(LLVMDebugInfoPDB PROPERTIES INTERFACE_LINK_LIBRARIES "")

if(WIN32 AND(NOT MINGW))
    # When linking with clang static libraries on Windows this macro should be defined for macro CLANG_ABI to be defined correctly.
    # Not doing so can cause linking warnings (and possible silent errors)
    # Normally, this macro is added during CMake configuration of clang/llvm project and then propagated through imported targets in ClangTargets.cmake,
    # but if clang/llvm is built on Windows not by msvc but by clang then this macro is not defined.
    # It is a bug and it is already fixed in llvm 23.0.
    target_compile_definitions(kslicer2 PRIVATE CLANG_BUILD_STATIC)
endif()

if(MSVC)
    target_compile_options(kslicer2 PRIVATE /FI ${CMAKE_CURRENT_SOURCE_DIR}/do_not_use_printf_and_cout_use_kslicer_outs_intead.h)
else()
    target_compile_options(kslicer2 PRIVATE -include ${CMAKE_CURRENT_SOURCE_DIR}/do_not_use_printf_and_cout_use_kslicer_outs_intead.h)
endif()

target_include_directories(kslicer2 SYSTEM PRIVATE ${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})
target_link_directories(kslicer2 PRIVATE ${LLVM_LIBRARY_DIRS})
target_link_libraries(kslicer2 PRIVATE clangTooling inja)

if(MSVC)
    target_compile_options(kslicer2 PRIVATE /W4)

    # Disabling this warnings because even when included as SYSTEM (/external) it still produces warnings inside llvm headers.
    # Warnings C47xx are not affected by SYSTEM (/external).
    # https://learn.microsoft.com/en-us/cpp/build/reference/external-external-headers-diagnostics?view=msvc-170#limitations
    target_compile_options(kslicer2 PRIVATE /wd4702)
else()
    target_compile_options(kslicer2 PRIVATE -Wall -Wextra)
endif()
