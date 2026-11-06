#pragma once

namespace clang {
    class CompilerInstance;
}

namespace kslicer {
    clang::CompilerInstance& get_compiler_instance();
    void set_compiler_instance(clang::CompilerInstance& compiler_instance);
} // namespace kslicer
