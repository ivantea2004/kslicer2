#pragma once

namespace clang {
    class DiagnosticsEngine;
}

namespace kslicer {

    clang::DiagnosticsEngine& get_standalone_diagnostics_engine();

}
