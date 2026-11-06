#include "kslicer/core/kslicer_main.hpp"
#include "kslicer/core/command_line.hpp"
#include "kslicer/core/config.hpp"
#include "kslicer/core/slicing.hpp"
#include "kslicer/utils/compiler_instance.hpp"
#include "kslicer/utils/run_compiler.hpp"
#include "kslicer/utils/standalone_diagnostics_engine.hpp"
#include <clang/Basic/Diagnostic.h>

namespace kslicer {

    int kslicer_main(int argc, char** argv) {

        CommandLine command_line{};
        if (!command_line.from_args(argc, argv)) {
            return 1;
        }
        command_line.dump();

        Config config;
        if (!config.from_file(command_line.config_path)) {
            return 1;
        }
        config.shader_compiler = command_line.shader_compiler.value_or(config.shader_compiler);
        config.dump();

        CompilerArgs args;
        args.source = config.sources[0];
        args.include_directories = config.include_directories;
        args.include_directories.push_back(command_line.kslicer_directory / "TINYSTL");

        return run_compiler(args,
                            [&](clang::CompilerInstance& compiler_instance) {
                                set_compiler_instance(compiler_instance);
                                slice(command_line.kslicer_directory, config);
                            }) ||
               get_standalone_diagnostics_engine().hasErrorOccurred();
    }

} // namespace kslicer
