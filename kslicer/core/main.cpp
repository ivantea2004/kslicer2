#include "kslicer/core/kslicer_main.hpp"
#include "kslicer/utils/diagnostics.hpp"
#include "kslicer/utils/is_debugger_present.hpp"
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/PrettyStackTrace.h>
#include <llvm/Support/Signals.h>

#ifdef _WIN32
#include <Windows.h>
#include <llvm/Support/WindowsError.h>
#endif

int main(int argc, char** argv) {

    if (!kslicer::is_debugger_present()) {
#ifdef _WIN32
        {
            ULONG stack_overflow_recovery_stack_size = 64 * 1024;
            (void)SetThreadStackGuarantee(&stack_overflow_recovery_stack_size);
        }
#endif

        llvm::setBugReportMsg("Kernel Slicer has crashed\n");
        llvm::sys::PrintStackTraceOnErrorSignal(argv[0]);
    }
    llvm::PrettyStackTraceProgram program(argc, argv);

#ifdef _WIN32
    std::unique_ptr<LPWSTR[], void (*)(LPWSTR*)> wide_argv(::CommandLineToArgvW(::GetCommandLineW(), &argc),
                                                           [](LPWSTR* s) { LocalFree(s); });
    if (!argv) {
        std::error_code e = llvm::mapWindowsError(::GetLastError());
        kslicer::diagnostic(kslicer::Error("%0")) << e.message();
        return 1;
    }
    std::vector<std::filesystem::path> args(argc);
    for (int i = 0; i < argc; ++i) {
        args[i] = wide_argv[i];
    }
#else
    std::vector<std::filesystem::path> args(argc);
    for (int i = 0; i < argc; ++i) {
        args[i] = argv[i];
    }
#endif

    return kslicer::kslicer_main(args);
}