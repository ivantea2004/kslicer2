#include "kslicer/utils/command_line_parser.hpp"
#include "kslicer/utils/diagnostics.hpp"

namespace kslicer {

    bool CommandLineParser::parse(const std::vector<std::filesystem::path>& args) const {

        std::vector<bool> set_args(args_.size());

        bool error = false;
        for (size_t i = 1; i < args.size(); ++i) {
            bool is_known_option = false;
            for (size_t j = 0; j < args_.size(); ++j) {
                if (args[i] == args_[j].name) {
                    is_known_option = true;
                    if (args_[j].store) {
                        if (i + 1 >= args.size()) {
                            diagnostic(Error("missing value after %0")) << args[i].u8string();
                            return false;
                        }
                        set_args[j] = true;
                        args_[j].store(args_[j].destination, args[i + 1]);
                        ++i;
                    } else {
                        *static_cast<bool*>(args_[j].destination) = true;
                    }
                    break;
                }
            }
            if (!is_known_option) {
                error = true;
                diagnostic(Error("unrecognized command line option '%0'")) << args[i].u8string();
            }
        }

        for (size_t i = 0; i < args_.size(); ++i) {
            if (!args_[i].is_optional && !set_args[i]) {
                diagnostic(Error("option '%0' is missing")) << args_[i].name;
                error = true;
            }
        }

        return !error;
    }

} // namespace kslicer
