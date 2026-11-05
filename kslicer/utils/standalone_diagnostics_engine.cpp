#include "kslicer/utils/standalone_diagnostics_engine.hpp"
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/LangOptions.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>

namespace kslicer {
    using namespace clang;

    DiagnosticsEngine& get_standalone_diagnostics_engine() {

        static DiagnosticOptions diagnostic_options;
        diagnostic_options.ShowColors = true;
        static llvm::IntrusiveRefCntPtr<DiagnosticIDs> diagnostic_ids = llvm::makeIntrusiveRefCnt<DiagnosticIDs>();
        static TextDiagnosticPrinter diagnostic_printer(llvm::errs(), diagnostic_options);
        static DiagnosticsEngine diagnostics_engine(diagnostic_ids, diagnostic_options, &diagnostic_printer, false);
        static FileSystemOptions filesystem_options;
        static FileManager file_manager(filesystem_options);
        static SourceManager source_manager(diagnostics_engine, file_manager);
        diagnostics_engine.setSourceManager(&source_manager);
        return diagnostics_engine;
    }

} // namespace kslicer
