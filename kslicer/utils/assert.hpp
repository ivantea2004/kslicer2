#pragma once
#include <cassert>
#include <cstdlib>

namespace kslicer {

    void report_failed_assertion(const char* what);

}

#ifdef NDEBUG
#define KSLICER_ASSERT(x) (!(x) ? (::kslicer::report_failed_assertion(#x), std::abort()) : (void)0)
#else
#define KSLICER_ASSERT(x) assert(x)
#endif
