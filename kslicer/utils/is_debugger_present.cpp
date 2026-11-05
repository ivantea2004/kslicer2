#include "kslicer/utils/is_debugger_present.hpp"
#if defined(_WIN32)
#include <Windows.h>
#elif defined(__linux__)
#include <cerrno>
#include <cstdio>
#include <cstring>
#else
#endif

namespace kslicer {

    bool is_debugger_present() {
#if defined(_WIN32)
        return IsDebuggerPresent();
#elif defined(__linux__) // https://stackoverflow.com/a/24969863

        char buffer[4096] = {};

        FILE* status_file = fopen("/proc/self/status", "r");
        if (!status_file) {
            return false;
        }
        errno = 0;
        (void)fread(buffer, 1, sizeof(buffer) - 1, status_file);
        fclose(status_file);
        if (errno) {
            return false;
        }

        const char key[] = "TracerPid:";
        const char* value = std::strstr(buffer, key);
        if (!value) {
            return false;
        }
        value += std::strlen(key);

        while (*value == ' ' || *value == '\t')
            ++value;

        if (*value) {
            if ('0' <= *value && *value <= '9') {
                return *value != '0';
            } else {
                return false;
            }
        } else {
            return false;
        }
#else
        return false;
#endif
    }

} // namespace kslicer
