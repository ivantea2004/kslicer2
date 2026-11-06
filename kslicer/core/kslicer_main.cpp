#include "kslicer/core/kslicer_main.hpp"
#include "kslicer/core/command_line.hpp"
#include "kslicer/core/config.hpp"
#include "kslicer/core/slicing.hpp"
#include "kslicer/utils/compiler_instance.hpp"
#include "kslicer/utils/run_compiler.hpp"
#include "kslicer/utils/standalone_diagnostics_engine.hpp"
#include <clang/Basic/Diagnostic.h>

namespace kslicer {

    int kslicer_main(const std::vector<std::filesystem::path>& args) {

        CommandLine command_line{};
        if (!command_line.from_args(args)) {
            return 1;
        }
        command_line.dump();

        Config config;
        if (!config.from_file(command_line.config_path)) {
            return 1;
        }
        config.shader_compiler = command_line.shader_compiler.value_or(config.shader_compiler);
        config.dump();

        CompilerArgs compiler_args;
        compiler_args.source = config.sources[0];
        compiler_args.include_directories = config.include_directories;
        compiler_args.include_directories.push_back(command_line.kslicer_directory / "TINYSTL");

        return run_compiler(compiler_args,
                            [&](clang::CompilerInstance& compiler_instance) {
                                set_compiler_instance(compiler_instance);
                                slice(command_line.kslicer_directory, config);
                            }) ||
               get_standalone_diagnostics_engine().hasErrorOccurred();
    }

} // namespace kslicer
