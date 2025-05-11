#include "CreateAST.h"
#include "fmt/ranges.h"


CreateAST::CreateAST(std::string table_name, const std::vector<Identifier> &columns, std::vector<Literal::Type> &columnTypes)
        : name(std::move(table_name)), columns(columns), columnTypes(columnTypes){}

void CreateAST::performChecks() {
    if (columns.size() != columnTypes.size()) {
        throw PreProcessorError("Mismatch between number of columns and column types.");
    }
    if (DataManager::getInstance().doesTableExist(name)) {
        throw PreProcessorError("Table '" + name + "' already exists");
    }

    for (const auto& column : columns) {
        qualifiedColumns.push_back(name + "." + column.value);
    }
//    fmt::println("Columns: {}\n", fmt::join(columnTypes, " "));
//    fmt::println("Columns: {}\n", fmt::join(qualifiedColumns, " "));

}

std::string CreateAST::repr() const {
    std::ostringstream ss;
    ss << "CreateStmt(table=" << name << ", columns=[";
    for (size_t i = 0; i < columns.size(); ++i) {
        ss << "(" << columns[i].value << ", " << Literal::typeToString(columnTypes[i]) << ")";
        if (i < columns.size() - 1) ss << ", ";
    }
    ss << "])";
    return ss.str();
}


