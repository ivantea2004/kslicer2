#include "kslicer/core/slicing.hpp"
#include "kslicer/class_info.hpp"
#include "kslicer/codegen_host.hpp"
#include "kslicer/utils.hpp"

namespace kslicer {

    void slice(const std::filesystem::path& kslicer_directory, const Config& config) {
        auto decl = find_global_declaration(config.class_name);
        ClassInfo class_info(dyn_cast<CXXRecordDecl>(decl));
        class_info.dump();
        codegen(kslicer_directory, config, class_info);
    }

} // namespace kslicer
