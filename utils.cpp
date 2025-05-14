#include <iostream>
#include "TypeHints.h"
#include "fmt/base.h"
#include "DataManager.h"

void printRow(const TypeHints::Row& row) {
    std::cout << "{ ";
    for (const auto& [key, value] : row) {
        std::cout << key << ": " << Literal(value).toString() << ", ";
    }
    std::cout << "}" << std::endl;
}

void printTable(TypeHints::TableData data) {
    fmt::println("Printing project data ({} rows):", data.size());

    for (size_t rowIdx = 0; rowIdx < data.size(); ++rowIdx) {
        const auto& row = data[rowIdx];
        fmt::print("Row {}:\n", rowIdx);

        for (const auto& [key, val] : row) {
            Literal::Type type;

            if (std::holds_alternative<std::monostate>(val)) {
                type = Literal::Type::NULL_VALUE;
            } else if (std::holds_alternative<std::string>(val)) {
                type = Literal::Type::TEXT;
            } else if (std::holds_alternative<int>(val)) {
                type = Literal::Type::INTEGER;
            } else if (std::holds_alternative<bool>(val)) {
                type = Literal::Type::BOOLEAN;
            } else {
                throw std::runtime_error("Unsupported variant type");
            }

            Literal lit(type, val);
            fmt::print("  {} = {}\n", key, lit.toString());
        }
    }
}

std::string typeToString(Literal::Type type)  {
    switch (type) {
        case Literal::Type::NULL_VALUE: return "NULL";
        case Literal::Type::TEXT: return "TEXT";
        case Literal::Type::INTEGER: return "INTEGER";
        case Literal::Type::BOOLEAN: return "BOOLEAN";
        default: return "UNKNOWN";
    }
}

void visualizeTableStructure(const std::string& table_name)  {
    DataManager& dataManager = DataManager::getInstance();
    if (!dataManager.doesTableExist(table_name)) {
        fmt::println("Table '{}' does not exist.", table_name);
        return;
    }

    // Get the column types for the table
    const auto& columnTypesMap = dataManager.getColumnTypesForTable(table_name);

    // Print the table name
    fmt::println("Table '{}':", table_name);
    fmt::println("Columns and their types:");

    // Iterate through the columns and print their names and types
    for (const auto& [columnName, columnType] : columnTypesMap) {
        // Convert the column type to a human-readable string
        std::string typeString = typeToString(columnType);

        // Print the column name and type
        fmt::println("  Column: '{}' | Type: '{}'", columnName, typeString);
    }
}


