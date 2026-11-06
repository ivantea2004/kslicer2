#include "kslicer/utils/run_compiler.hpp"
#include <clang/AST/ASTConsumer.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Tooling/CompilationDatabase.h>
#include <clang/Tooling/Tooling.h>

namespace kslicer {

    struct RunCompilerErrorException {};

    class ASTConsumer : public clang::ASTConsumer {
    public:
        ASTConsumer(RunCompilerHandler handler, clang::CompilerInstance& compiler_instance)
            : handler_(handler), compiler_instance_(compiler_instance) {}
        void HandleTranslationUnit(clang::ASTContext&) override {
            if (compiler_instance_.getDiagnostics().hasErrorOccurred()) {
                throw RunCompilerErrorException{};
            }
            handler_(compiler_instance_);
            if (compiler_instance_.getDiagnostics().hasErrorOccurred()) {
                throw RunCompilerErrorException{};
            }
        }

    private:
        RunCompilerHandler handler_;
        clang::CompilerInstance& compiler_instance_;
    };

    class ASTFrontedAction : public clang::ASTFrontendAction {
    public:
        ASTFrontedAction(RunCompilerHandler handler) : handler_(handler) {}
        std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance& compiler_instance,
                                                              llvm::StringRef) override {
            return std::make_unique<ASTConsumer>(handler_, compiler_instance);
        }

    private:
        RunCompilerHandler handler_;
    };

    class FrontendActionFactory : public clang::tooling::FrontendActionFactory {
    public:
        FrontendActionFactory(RunCompilerHandler handler) : handler_(handler) {}
        std::unique_ptr<clang::FrontendAction> create() override {
            return std::make_unique<ASTFrontedAction>(handler_);
        }

    private:
        RunCompilerHandler handler_;
    };

    int run_compiler(const CompilerArgs& compiler_args, RunCompilerHandler handler) {
        std::vector<std::string> args;
        args.push_back("-std=c++17");

        for (auto i : compiler_args.include_directories) {
            args.push_back("-I");
            args.push_back(i.u8string());
        }

        clang::tooling::FixedCompilationDatabase db(".", args);
        clang::tooling::ClangTool tool(db, {compiler_args.source.u8string()});
        FrontendActionFactory action_factory(handler);
        try {
            return tool.run(&action_factory);
        } catch (const RunCompilerErrorException&) {
            return 1;
        }
    }

} // namespace kslicer
