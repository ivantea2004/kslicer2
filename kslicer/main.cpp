#include "kslicer/codegen_host.hpp"
#include "kslicer/command_line.hpp"
#include "kslicer/config.hpp"
#include "kslicer/utils.hpp"
#include "kslicer/utils/compiler_instance.hpp"
#include "kslicer/utils/diagnostics.hpp"
#include "kslicer/utils/is_debugger_present.hpp"
#include "kslicer/utils/run_compiler.hpp"
#include "kslicer/utils/standalone_diagnostics_engine.hpp"
#include <llvm/Support/PrettyStackTrace.h>

namespace kslicer {
    using namespace clang;

    void handle_translation_unit(std::filesystem::path kslicer_directory, const Config& config) {
        auto decl = find_global_declaration(config.class_name);
        ClassInfo class_info(dyn_cast<CXXRecordDecl>(decl));
        diagnostic(Remark("found class"), decl->getLocation());
        class_info.dump();
        codegen(kslicer_directory, config, class_info);
    }

    int main(int argc, char** argv) {

        llvm::PrettyStackTraceProgram program(argc, argv);

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
                            [&](CompilerInstance& compiler_instance) {
                                set_compiler_instance(compiler_instance);
                                handle_translation_unit(command_line.kslicer_directory, config);
                            }) ||
               get_standalone_diagnostics_engine().hasErrorOccurred();
    }
} // namespace kslicer

#ifdef _WIN32
#include <Windows.h>
#endif
#include <llvm/Support/Signals.h>

int main(int argc, char** argv) {
    if (kslicer::is_debugger_present()) {
        return kslicer::main(argc, argv);
    } else {
#ifdef _WIN32
        {
            ULONG stack_overflow_recovery_stack_size = 64 * 1024;
            (void)SetThreadStackGuarantee(&stack_overflow_recovery_stack_size);
        }
#endif

        llvm::setBugReportMsg("Kernel Slicer has crashed\n");
        llvm::sys::PrintStackTraceOnErrorSignal(argv[0]);
        return kslicer::main(argc, argv);
    }
}