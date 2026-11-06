#include "kslicer/utils/diagnostics.hpp"
#include "kslicer/utils/compiler_instance.hpp"
#include "kslicer/utils/io.hpp"
#include "kslicer/utils/standalone_diagnostics_engine.hpp"
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>

namespace kslicer {
    using namespace clang;

    Error::operator kslicer::CustomDiagDesc() const {
        return CustomDiagDesc(clang::diag::Severity::Error, format_, DiagnosticIDs::Class::CLASS_ERROR, true, true);
    }

    Warning::operator kslicer::CustomDiagDesc() const {
        return CustomDiagDesc(clang::diag::Severity::Warning, format_, DiagnosticIDs::Class::CLASS_WARNING, true, true);
    }

    Remark::operator kslicer::CustomDiagDesc() const {
        return CustomDiagDesc(clang::diag::Severity::Remark, format_, DiagnosticIDs::Class::CLASS_REMARK, true, true);
    }

    DiagnosticBuilder diagnostic(const DiagnosticIDs::CustomDiagDesc& desc) {
        auto& e = get_standalone_diagnostics_engine();
        if (auto p = static_cast<TextDiagnosticPrinter*>(e.getClient())) {
            p->setPrefix("kslicer");
        }
        return e.Report(e.getDiagnosticIDs()->getCustomDiagID(desc));
    }

    DiagnosticBuilder diagnostic(const DiagnosticIDs::CustomDiagDesc& desc, const FileScope& scope, size_t offset) {
        auto& e = get_standalone_diagnostics_engine();
        if (auto p = static_cast<TextDiagnosticPrinter*>(e.getClient())) {
            p->setPrefix("");
        }
        auto& s = e.getSourceManager();
        FileID id{};
        scope.get_id(id);
        auto start_loc = s.getLocForStartOfFile(id);
        return e.Report(start_loc.getLocWithOffset(offset), e.getDiagnosticIDs()->getCustomDiagID(desc));
    }

    DiagnosticBuilder diagnostic(const DiagnosticIDs::CustomDiagDesc& desc, const FileScope& scope, size_t line,
                                 size_t column) {
        auto& e = get_standalone_diagnostics_engine();
        if (auto p = static_cast<TextDiagnosticPrinter*>(e.getClient())) {
            p->setPrefix("");
        }
        auto& s = e.getSourceManager();
        FileID id{};
        scope.get_id(id);
        auto loc = s.translateLineCol(id, line, column);
        return e.Report(loc, e.getDiagnosticIDs()->getCustomDiagID(desc));
    }

    DiagnosticBuilder diagnostic(const DiagnosticIDs::CustomDiagDesc& desc, const SourceLocation& loc) {
        auto& e = get_compiler_instance().getDiagnostics();
        return e.Report(loc, e.getDiagnosticIDs()->getCustomDiagID(desc));
    }

} // namespace kslicer
