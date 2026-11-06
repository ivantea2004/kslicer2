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

    struct Error {
        template <size_t N>
        explicit Error(const char (&format)[N]) : format_(format) {}

        operator CustomDiagDesc() const;

    private:
        const char* format_;
    };

    struct FatalError {
        template <size_t N>
        explicit FatalError(const char (&format)[N]) : format_(format) {}

        operator CustomDiagDesc() const;

    private:
        const char* format_;
    };

    struct Warning {
        template <size_t N>
        explicit Warning(const char (&format)[N]) : format_(format) {}

        operator CustomDiagDesc() const;

    private:
        const char* format_;
    };

    struct Remark {
        template <size_t N>
        explicit Remark(const char (&format)[N]) : format_(format) {}

        operator CustomDiagDesc() const;

    private:
        const char* format_;
    };

    DiagnosticBuilder diagnostic(const CustomDiagDesc& desc);
    DiagnosticBuilder diagnostic(const CustomDiagDesc& desc, const FileScope& scope, size_t offset);
    DiagnosticBuilder diagnostic(const CustomDiagDesc& desc, const FileScope& scope, size_t line, size_t column);
    DiagnosticBuilder diagnostic(const CustomDiagDesc& desc, const clang::SourceLocation& location);

} // namespace kslicer
