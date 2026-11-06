#include "assert.hpp"
#include "kslicer/utils/diagnostics.hpp"

namespace kslicer {

    void report_failed_assertion(const char* what) { diagnostic(FatalError("assertion failed: %0")) << what; }

} // namespace kslicer
