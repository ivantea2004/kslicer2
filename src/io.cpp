#include "io.h"
#include "diagnostics.h"
#include "standalone_diagnostics_engine.h"
#include <clang/Basic/LangOptions.h>
#include <clang/Basic/SourceManager.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/WithColor.h>
namespace kslicer {

    llvm::raw_ostream& operator<<(llvm::raw_ostream& s, const std::filesystem::path& p) { return s << p.u8string(); }

    FileScope::FileScope(const clang::FileID& id, std::string data) : id_(id.getHashValue()), data_(std::move(data)) {
        get_standalone_diagnostics_engine().getClient()->BeginSourceFile({});
    }

    FileScope::~FileScope() { get_standalone_diagnostics_engine().getClient()->EndSourceFile(); }

    void FileScope::get_id(clang::FileID& id) const {
        static_assert(sizeof(clang::FileID) == sizeof(id_));
        std::memcpy(&id, &id_, sizeof(id_));
    }

    const std::string& FileScope::data() const { return data_; }

    FileScope read_entire_text_file(const std::filesystem::path& file_path) {
        if (auto buffer = llvm::MemoryBuffer::getFile(file_path.u8string(), true)) {
            auto& source_manager = get_standalone_diagnostics_engine().getSourceManager();
            clang::FileID id = source_manager.createFileID(std::move(*buffer));
            bool invalid = true;
            std::string data = source_manager.getBufferData(id, &invalid).str();
            if (invalid) {
                return {{}, ""};
            } else {
                return {id, std::move(data)};
            }
        } else {
            std::error_code e = buffer.getError();
            diagnostic(make_error("%0: %1")) << e.message() << file_path.u8string();
            return {{}, ""};
        }
    }

    bool write_entire_text_file(const std::filesystem::path& file_path, const std::string& data) {

        std::error_code e;
        llvm::raw_fd_ostream file(file_path.u8string(), e, llvm::sys::fs::OF_TextWithCRLF);
        if (e) {
            diagnostic(make_error("%0: %1")) << e.message() << file_path.u8string();
            return false;
        }
        file << data;
        file.close();
        return true;
    }

} // namespace kslicer
