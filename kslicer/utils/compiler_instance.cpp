#include "kslicer/utils/compiler_instance.hpp"
#include "kslicer/utils/assert.hpp"

namespace kslicer {
    static clang::CompilerInstance* compiler_instance_;

    clang::CompilerInstance& get_compiler_instance() { return *compiler_instance_; }
    void set_compiler_instance(clang::CompilerInstance& compiler_instance) {
        KSLICER_ASSERT(compiler_instance_ == nullptr);
        compiler_instance_ = &compiler_instance;
    }

} // namespace kslicer
