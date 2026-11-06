#include "kslicer/core/command_line.hpp"
#include "kslicer/utils/command_line_parser.hpp"
#include "kslicer/utils/io.hpp"

namespace kslicer {

    bool CommandLine::from_args(const std::vector<std::filesystem::path>& args) {
        CommandLineParser parser;
        parser.add_option("-selfdir", kslicer_directory);
        parser.add_option("-config", config_path);
        parser.add_flag("--verbose", verbose);
        parser.add_option("-shaderCC", shader_compiler);
        return parser.parse(args);
    }

    void CommandLine::dump() const {
        llvm::outs() << "kslicer_directory: " << kslicer_directory << "\n";
        llvm::outs() << "config_path: " << config_path << "\n";
        llvm::outs() << "verbose: " << verbose << "\n";
        llvm::outs() << "shader_compiler: " << shader_compiler << "\n";
    }

} // namespace kslicer
