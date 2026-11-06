#include "kslicer/core/kslicer_main.hpp"
#include "kslicer/utils/is_debugger_present.hpp"
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/PrettyStackTrace.h>

#ifdef _WIN32
#include <Windows.h>
#endif
#include <llvm/Support/Signals.h>

int main(int argc, char** argv) {
    if (kslicer::is_debugger_present()) {
        return kslicer::kslicer_main(argc, argv);
    } else {
#ifdef _WIN32
        {
            ULONG stack_overflow_recovery_stack_size = 64 * 1024;
            (void)SetThreadStackGuarantee(&stack_overflow_recovery_stack_size);
        }
#endif

        llvm::setBugReportMsg("Kernel Slicer has crashed\n");
        llvm::sys::PrintStackTraceOnErrorSignal(argv[0]);
        llvm::PrettyStackTraceProgram program(argc, argv);
        return kslicer::kslicer_main(argc, argv);
    }
}