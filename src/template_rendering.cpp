#include "template_rendering.h"
#include "diagnostics.h"
#include "io.h"
#include <inja/inja.hpp>

namespace kslicer {

    bool render_template(const nlohmann::json& data, std::filesystem::path template_path,
                         std::filesystem::path output_path) {
        if (auto scope = read_entire_text_file(template_path)) {
            auto inja_error = make_error("inja: %0");
            try {
                inja::Environment env;
                env.set_trim_blocks(true);
                env.set_lstrip_blocks(true);
                inja::Template t = env.parse_template(template_path);
                std::string result = env.render(t, data);
                return write_entire_text_file(output_path, result);
            } catch (const inja::FileError& e) {
                // We already successfully opened file ourselves in read_entire_text_file().
                // So, if inja has a problem with that, we just reoutput its error.
                diagnostic(inja_error) << e.message;
                return false;
            } catch (const inja::InjaError& e) {
                diagnostic(inja_error, scope, e.location.line, e.location.column) << e.message;
                return false;
            }
        } else {
            return false;
        }
    }

} // namespace kslicer
