#include "kslicer/utils/recursive_statement_iterator.hpp"
#include "kslicer/utils/assert.hpp"
#include <clang/AST/Stmt.h>

namespace kslicer {

    std::vector<const clang::Stmt*> get_statement_children(const clang::Stmt* stmt) {
        std::vector<const clang::Stmt*> out;
        for (auto i : stmt->children()) {
            out.push_back(i);
        }
        return out;
    }

    RecursiveStatementIterator::RecursiveStatementIterator(const clang::Stmt* root) {
        if (root) {
            stack_.push_back(root);
        }
    }

    const clang::Stmt* RecursiveStatementIterator::operator*() const {
        KSLICER_ASSERT(!stack_.empty());
        return stack_.back();
    }

    RecursiveStatementIterator& RecursiveStatementIterator::operator++() {

        const clang::Stmt* curr = stack_.back();
        stack_.pop_back();

        auto children = get_statement_children(curr);

        for (auto i = children.rbegin(); i != children.rend(); ++i) {
            if (*i) {
                stack_.push_back(*i);
            }
        }

        return *this;
    }

} // namespace kslicer
