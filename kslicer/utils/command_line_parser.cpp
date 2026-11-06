#include "kslicer/utils/command_line_parser.hpp"
#include "kslicer/utils/diagnostics.hpp"

namespace kslicer {

    bool CommandLineParser::parse(const std::vector<std::filesystem::path>& args) const {

        std::vector<bool> set_args(options_.size());

        bool error = false;
        for (size_t i = 1; i < args.size(); ++i) {
            bool is_known_option = false;
            for (size_t j = 0; j < options_.size(); ++j) {
                if (args[i] == options_[j].name) {
                    is_known_option = true;
                    if (options_[j].store) {
                        if (i + 1 >= args.size()) {
                            diagnostic(Error("missing value after %0")) << args[i].u8string();
                            return false;
                        }
                        set_args[j] = true;
                        options_[j].store(options_[j].destination, args[i + 1]);
                        ++i;
                    } else {
                        *static_cast<bool*>(options_[j].destination) = true;
                    }
                    break;
                }
            }
            if (!is_known_option) {
                error = true;
                diagnostic(Error("unrecognized command line option '%0'")) << args[i].u8string();
            }
        }

        for (size_t i = 0; i < options_.size(); ++i) {
            if (options_[i].is_required && !set_args[i]) {
                diagnostic(Error("option '%0' is missing")) << options_[i].name;
                error = true;
            }
        }

        return !error;
    }

} // namespace kslicer
