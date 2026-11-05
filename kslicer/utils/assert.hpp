#pragma once
#include <cassert>

namespace kslicer {

    [[noreturn]] void failed_assertion_handler(const char* what);

}

#ifdef NDEBUG
#define KSLICER_ASSERT(x) (!(x) ? ::kslicer::failed_assertion_handler(#x) : (void)0)
#else
#define KSLICER_ASSERT(x) assert(x)
#endif
