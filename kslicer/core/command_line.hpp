#pragma once
#include <filesystem>
#include <optional>
#include <vector>

namespace kslicer {

    struct CommandLine {
        bool from_args(const std::vector<std::filesystem::path>& args);
        void dump() const;

        std::filesystem::path kslicer_directory;    // -selfdir
        std::filesystem::path config_path;          // -config
        bool verbose = false;                       // --verbose
        std::optional<std::string> shader_compiler; // -shaderCC
    };

} // namespace kslicer
