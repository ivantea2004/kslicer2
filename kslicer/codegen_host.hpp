#pragma once
#include "kslicer/class_info.hpp"
#include "kslicer/core/config.hpp"

namespace kslicer {

    bool codegen(std::filesystem::path kslicer_directory, const Config& config, const ClassInfo& info);

}
