#include "assert.hpp"
#include <llvm/Support/raw_ostream.h>

namespace kslicer {

    void failed_assertion_handler(const char* what) {
        llvm::errs() << "assertion failed: " << what << "\n";
        std::abort();
    }

} // namespace kslicer
