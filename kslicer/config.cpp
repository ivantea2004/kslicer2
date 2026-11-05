#include "config.h"
#include "io.h"
#define JSON_DIAGNOSTIC_POSITIONS 1
#include <nlohmann/json.hpp>
namespace kslicer {

    bool Config::from_file(std::filesystem::path config_path) {

        if (auto scope = read_entire_text_file(config_path)) {
            try {
                nlohmann::json config = nlohmann::json::parse(scope.data(), nullptr, true, /* ignore_comments */ true);

                auto validate_known_keys = [&](const nlohmann::json& j, std::initializer_list<const char*> known_keys) {
                    for (auto i = j.begin(); i != j.end(); ++i) {
                        bool ok = false;
                        for (auto key : known_keys) {
                            if (i.key() == key) {
                                ok = true;
                                break;
                            }
                        }
                        if (!ok) {
                            auto value = j.at(i.key());
                            diagnostic(Warning("unknown key '%0'"), scope, value.start_pos()) << i.key();
                        }
                    }
                };

                validate_known_keys(
                    config, {"mainClass", "baseDirectory", "source", "includeProcess", "includeIgnore", "options"});

                validate_known_keys(config.at("options"), {"-shaderCC", "-shaderFolderPrefix", "-suffix"});

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

                shaders_directory =
                    (base_directory / (shaders_directory_prefix + "shaders" + suffix)).lexically_normal();

                return true;
            } catch (const nlohmann::json::exception& e) {

                std::string m = e.what();
                const size_t pos1 = m.find("] (bytes ");
                const size_t pos2 = m.find(") ");
                if (pos1 == std::string::npos || pos2 == std::string::npos) {
                    // Somehow exception does not have expected pattern
                    diagnostic(Error("%0"), scope, 0) << m;
                    return false;
                }
                const size_t location_pos = pos1 + 9;
                const size_t message_pos = pos2 + 2;
                size_t location = 0;

                for (size_t i = location_pos; i < m.length() && std::isdigit(m[i]); ++i) {
                    location *= 10;
                    location += m[i] - '0';
                }

                diagnostic(Error("%0"), scope, location) << m.substr(message_pos);
                return false;
            }
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
