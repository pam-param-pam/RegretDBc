#include "AlterAST.h"
#include "fmt/ranges.h"


//AlterAST::AlterAST(Identifier table, std::string action, Identifier column, std::string dataTypeOrName)
//        : name(std::move(table_name)), columns(columns), columnTypes(columnTypes){}
//
//void AlterAST::performChecks() {
//    if (columns.size() != columnTypes.size()) {
//        throw PreProcessorError("Mismatch between number of columns and column types.");
//    }
//    if (DataManager::getInstance().doesTableExist(name)) {
//        throw PreProcessorError("Table '" + name + "' already exists");
//    }
//
//    for (const auto& column : columns) {
//        qualifiedColumns.push_back(name + "." + column.value);
//    }
////    fmt::println("Columns: {}\n", fmt::join(columnTypes, " "));
////    fmt::println("Columns: {}\n", fmt::join(qualifiedColumns, " "));
//
//}
//
//std::string AlterAST::repr() const {
//
//}




