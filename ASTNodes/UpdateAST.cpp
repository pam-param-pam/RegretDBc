#include "UpdateAST.h"
#include <sstream>

UpdateAST::UpdateAST(Identifier table, const std::vector<std::pair<Identifier, Literal>>& assignments, std::string  where_expr)
        : table(std::move(table)), assignments(assignments), where_expr(std::move(where_expr)) {}

void UpdateAST::performChecks() {
}

std::string UpdateAST::repr() const {
    std::ostringstream ss;
    ss << "UpdateStmt(table=" << table.value << ", assignments=[";

    for (size_t i = 0; i < assignments.size(); ++i) {
        ss << "(" << assignments[i].first.value << ", " << assignments[i].second.toString() << ")";
        if (i < assignments.size() - 1) ss << ", ";
    }

    ss << "], where=" << where_expr << ")";
    return ss.str();
}
