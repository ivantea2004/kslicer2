#pragma once
#include <clang/Basic/Diagnostic.h>

namespace clang {
    class SourceLocation;
}

namespace kslicer {
    using DiagnosticsEngine = clang::DiagnosticsEngine;
    using DiagnosticBuilder = clang::DiagnosticBuilder;
    using CustomDiagDesc = clang::DiagnosticIDs::CustomDiagDesc;
    class FileScope;

    CustomDiagDesc make_error(const char* format);

    DiagnosticBuilder diagnostic(const CustomDiagDesc& desc);
    DiagnosticBuilder diagnostic(const CustomDiagDesc& desc, const FileScope& scope, size_t offset);
    DiagnosticBuilder diagnostic(const CustomDiagDesc& desc, const FileScope& scope, size_t line, size_t column);
    DiagnosticBuilder diagnostic(const CustomDiagDesc& desc, const clang::SourceLocation& location);

} // namespace kslicer
