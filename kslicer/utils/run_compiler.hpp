#pragma once
#include <filesystem>
#include <functional>
#include <vector>

namespace clang {

    class CompilerInstance;

}

namespace kslicer {

    struct CompilerArgs {
        std::filesystem::path source;
        std::vector<std::filesystem::path> include_directories;
    };

    using RunCompilerHandler = std::function<void(clang::CompilerInstance&)>;
    int run_compiler(const CompilerArgs& compiler_args, RunCompilerHandler handler);

} // namespace kslicer
