#pragma once
#include <clang/AST/StmtIterator.h>
#include <vector>

namespace kslicer {

    std::vector<const clang::Stmt*> get_statement_children(const clang::Stmt* stmt);

    class RecursiveStatementIterator {
    public:
        RecursiveStatementIterator() = default;
        RecursiveStatementIterator(const clang::Stmt* root);

        const clang::Stmt* operator*() const;
        const clang::Stmt* operator->() const { return operator*(); }
        RecursiveStatementIterator& operator++();
        RecursiveStatementIterator operator++(int) {
            RecursiveStatementIterator tmp(*this);
            operator++();
            return tmp;
        }
        bool operator==(const RecursiveStatementIterator& other) const { return stack_ == other.stack_; }
        bool operator!=(const RecursiveStatementIterator& other) const { return !operator==(other); }

        RecursiveStatementIterator begin() const { return *this; }
        RecursiveStatementIterator end() const { return {}; }

    private:
        std::vector<const clang::Stmt*> stack_;
    };

} // namespace kslicer
