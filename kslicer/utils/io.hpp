#pragma once
#include <filesystem>
#include <llvm/Support/raw_ostream.h>

namespace clang {
    class FileID;
}
namespace kslicer {

    llvm::raw_ostream& operator<<(llvm::raw_ostream& s, const std::filesystem::path& p);

    class [[nodiscard]] FileScope {
    public:
        FileScope(const clang::FileID& id, std::string data);
        FileScope(const FileScope&) = delete;
        FileScope(FileScope&&) = delete;
        FileScope& operator=(const FileScope&) = delete;
        FileScope& operator=(FileScope&&) = delete;
        ~FileScope();

        operator bool() const { return id_ != 0; }

        void get_id(clang::FileID& id) const;
        const std::string& data() const;

    private:
        uint32_t id_;
        std::string data_;
    };

    FileScope read_entire_text_file(const std::filesystem::path& file_path);
    [[nodiscard]] bool write_entire_text_file(const std::filesystem::path& file_path, const std::string& data);

} // namespace kslicer
