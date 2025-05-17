#include <iostream>
#include <algorithm>
#include "TypeHints.h"
#include "fmt/base.h"
#include "DataManager.h"

std::pair<std::string, std::string> splitColumn(const std::string &column) {
    auto dot_pos = column.find('.');
    if (dot_pos != std::string::npos) {
        return {column.substr(0, dot_pos), column.substr(dot_pos + 1)};
    } else {
        return {"", column};
    }
}

void printRow(const TypeHints::Row &row) {
    std::cout << "{ ";
    for (const auto &[key, value]: row) {
        std::cout << key << ": " << Literal(value).toString() << ", ";
    }
    std::cout << "}" << std::endl;
}

void printTable(TypeHints::TableData data) {

    for (auto rowIdx = 0; rowIdx < data.size(); ++rowIdx) {
        const auto &row = data[rowIdx];
        fmt::print("Row {}:\n", rowIdx);

        for (const auto &[key, literal]: row) {

            fmt::print("  {} = {}\n", key, literal.toString());
        }
    }
}

std::string typeToString(Literal::Type type) {
    switch (type) {
        case Literal::Type::NULL_VALUE:
            return "NULL";
        case Literal::Type::TEXT:
            return "TEXT";
        case Literal::Type::NUMBER:
            return "NUMBER";
        case Literal::Type::BOOLEAN:
            return "BOOLEAN";
        default:
            return "UNKNOWN";
    }
}

void visualizeTableStructure(const std::string &table_name) {
    DataManager &dataManager = DataManager::getInstance();
    if (!dataManager.doesTableExist(table_name)) {
        fmt::println("Table '{}' does not exist.", table_name);
        return;
    }

    const auto &columnTypesMap = dataManager.getColumnTypesForTable(table_name);

    fmt::println("Table '{}':", table_name);
    fmt::println("Columns and their types:");

    for (const auto &[columnName, columnType]: columnTypesMap) {
        std::string typeString = typeToString(columnType);
        fmt::println("  Column: '{}' | Type: '{}'", columnName, typeString);
    }
}

///https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void trim(std::string &s) {
    rtrim(s);
    ltrim(s);
}

std::string trim_copy(std::string s) {
    trim(s);
    return s;
}