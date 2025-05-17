#include <fstream>
#include "../DataManager.h"
#include "DumpPlan.h"
#include "../utils.h"


DumpPlan::DumpPlan(std::string filePath)
        : filePath(std::move(filePath)) {}

void DumpPlan::execute() {
    auto &dm = DataManager::getInstance();
    std::ofstream out(filePath);
    if (!out.is_open()) {
        throw IntegrityError("Failed to open file for writing: " + filePath);
    }

    for (const auto &tableName: dm.getAllTables()) {
        const auto &columnTypes = dm.getColumnTypesForTable(tableName);
        const auto &columnOrder = dm.getColumnsForTable(tableName);

        const auto &rows = dm.getTablesData(tableName);

        // --- CREATE TABLE ---
        out << "CREATE TABLE " << tableName << " (";
        //iterate in reverse order to preserve column order
        for (int i = columnOrder.size() - 1; i >= 0; --i) {
            const auto &col = columnOrder[i];
            const auto &colType = columnTypes.at(col);
            auto [tbName, colName] = splitColumn(col);

            out << colName << " " << Literal::typeToString(colType);
            if (i != 0)
                out << ", ";
        }
        out << ");\n";

        // --- INSERT INTO ---
        for (const auto &row: rows) {
            out << "INSERT INTO " << tableName << " (";
            for (auto i = 0; i < columnOrder.size(); ++i) {
                auto [tbName, colName] = splitColumn(columnOrder[i]);
                out << colName;
                if (i != columnOrder.size() - 1)
                    out << ", ";
            }

            out << ") VALUES (";

            for (auto i = 0; i < columnOrder.size(); ++i) {
                const auto &col = columnOrder[i];
                const auto &literal = row.at(col);
                out << literal.toString();
                if (i != columnOrder.size() - 1)
                    out << ", ";
            }

            out << ");\n";
        }

        out << "\n";
    }

    out.close();
}

TypeHints::TableData DumpPlan::getResult() const {
    throw IntegrityError("Do not call getResult() on DumpPlan");
}

std::string DumpPlan::toString(int level) const {
    std::string result = "DumpPlan(\n";
    result += std::string(level + 1, ' ') + "filePath='" + filePath + "',\n";
    result += "]\n" + std::string(level, ' ') + ")";
    return result;
}
