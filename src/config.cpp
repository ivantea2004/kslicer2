#include "config.h"
#include "io.h"
#include <nlohmann/json.hpp>
namespace kslicer {

    bool Config::from_file(std::filesystem::path config_path) {

        if (auto scope = read_entire_text_file(config_path)) {

            nlohmann::json config = nlohmann::json::parse(scope.data(), nullptr, true, /* ignore_comments */ true);
            class_name = config.at("mainClass");
            suffix = config.at("options").at("-suffix");
            shaders_directory_prefix = config.at("options").at("-shaderFolderPrefix");
            shader_compiler = config.at("options").at("-shaderCC");

            std::filesystem::path config_directory = config_path.parent_path();
            std::filesystem::path base_directory = config_directory / config.at("baseDirectory");

            std::vector<std::filesystem::path> source = config.at("source");
            std::vector<std::filesystem::path> include_process = config.at("includeProcess");
            std::vector<std::filesystem::path> include_ignore = config.at("includeIgnore");

            for (auto i : source) {
                sources.push_back(base_directory / i);
            }
            for (auto i : include_process) {
                include_directories.push_back(base_directory / i);
            }
            for (auto i : include_ignore) {
                include_directories.push_back(base_directory / i);
            }

            shaders_directory = (base_directory / (shaders_directory_prefix + "shaders" + suffix)).lexically_normal();

            return true;
        } else {
            return false;
        }
    }

    void Config::dump() const {

        llvm::outs() << "class_name: " << class_name << "\n";
        llvm::outs() << "suffix: " << suffix << "\n";
        llvm::outs() << "shaders_directory_prefix: " << shaders_directory_prefix << "\n";
        llvm::outs() << "shader_compiler: " << shader_compiler << "\n";
        for (size_t i = 0; i < sources.size(); ++i) {
            llvm::outs() << "sources[" << i << "]: " << sources[i] << "\n";
        }
        for (size_t i = 0; i < include_directories.size(); ++i) {
            llvm::outs() << "include_directories[" << i << "]: " << include_directories[i] << "\n";
        }
        llvm::outs() << "shaders_directory: " << shaders_directory << "\n";
    }

} // namespace kslicer
