// PlanNodes.cpp
#include "PlanNodeBase.h"
#include "SelectPlans.h"
#include "../DataManager.h"
#include "fmt/base.h"
#include "fmt/ranges.h"
#include <algorithm>
#include <utility>
#include <iomanip>


////---------------- Table Scan ----------------
TableScan::TableScan(std::string table) : table(std::move(table)) {}

void TableScan::execute() {
    resultData = DataManager::getInstance().getTablesData(table);
}

std::string TableScan::toString(int level) const {
    return "TableScan('" + table + "')";
}

TypeHints::TableData TableScan::getResult() const {
    return resultData;
}

////---------------- Cross Join ----------------
CrossJoin::CrossJoin(std::shared_ptr<PlanNodeBase> left, std::shared_ptr<PlanNodeBase> right)
        : left(std::move(left)), right(std::move(right)) {}

void CrossJoin::execute() {
    left->execute();
    right->execute();
    TypeHints::TableData leftData = left->getResult();
    TypeHints::TableData rightData = right->getResult();

    for (const auto &lrow: leftData) {
        for (const auto &rrow: rightData) {
            TypeHints::Row merged = lrow;
            merged.insert(rrow.begin(), rrow.end());
            resultData.push_back(merged);
        }
    }
}

std::string CrossJoin::toString(int level) const {
    return "CrossJoinPlan(\n" + std::string(level, ' ') + "left=" + left->toString(level + 1) + ",\n" +
           std::string(level, ' ') + "right=" + right->toString(level + 1) + "\n" + std::string(level - 1, ' ') + ")";
}

TypeHints::TableData CrossJoin::getResult() const {
    return resultData;
}

////---------------- Filter ----------------

Filter::Filter(std::shared_ptr<PlanNodeBase> source, Operand& condition)
        : source(std::move(source)), condition(std::move(condition)) {}

void Filter::execute() {
    source->execute();
    auto data = source->getResult();

    for (const auto& row : data) {
        auto result = condition.evaluate(row);
        if (result.value_or(false)) {
//            fmt::println("Filter TRUEEEE");
            resultData.push_back(row);
        }
    }
}

TypeHints::TableData Filter::getResult() const {
    return resultData;
}

std::string Filter::toString(int level) const {
    return "FilterPlan(\n" + std::string(level, ' ') + "condition=" + condition.toString() + ",\n" +
           std::string(level, ' ') + "source=" + source->toString(level + 1) + "\n" + std::string(level - 1, ' ') + ")";
}

////---------------- Project ----------------

Project::Project(std::shared_ptr<PlanNodeBase> source, const std::vector<std::string>& columns)
        : source(std::move(source)), columns(columns) {}

void Project::execute() {

    source->execute();
    auto data = source->getResult();

    for (const auto& row : data) {
        TypeHints::Row newRow;
        for (const auto& col : columns) {
            newRow[col] = row.at(col);
        }
        resultData.push_back(newRow);
    }

}

TypeHints::TableData Project::getResult() const {
    return resultData;
}

std::string Project::toString(int level) const {

    std::string columnsRepr = "[";
    for (auto i = 0; i < columns.size(); ++i) {
        columnsRepr += columns[i];
        if (i < columns.size() - 1) {
            columnsRepr += ", ";
        }
    }

    return "SelectPlan(\n" + std::string(level, ' ') + "projection=" + columnsRepr + "],\n" +
           std::string(level, ' ') + "source=" + source->toString(level + 1) + "\n" + std::string(level - 1, ' ') + ")";
}


//////---------------- Sort ----------------

Sort::Sort(std::shared_ptr<PlanNodeBase> source, const std::vector<std::pair<std::string, bool>>& orderBy)
        : source(std::move(source)), orderBy(orderBy) {}

void Sort::execute() {
    source->execute();  // Execute the source PlanNode (get the result data)
    auto data = source->getResult();  // Get the result data
    //todo
//    // Perform sorting based on the orderBy vector
//    for (const auto& [column, isDescending] : orderBy) {
//        bool reverse = isDescending;  // Sort in reverse if true (for DESC)
//
//        // Sort the data by the column, based on the direction (ascending or descending)
//        std::sort(data.begin(), data.end(), [&](const TypeHints::Row& a, const TypeHints::Row& b) {
//            const auto& valA = a.at(column);
//            const auto& valB = b.at(column);
//
//            // Using std::visit to handle all possible types in the variant
//            return reverse ? std::visit([&](auto&& aVal, auto&& bVal) {
//                return aVal > bVal;  // Descending order comparison
//            }, valA, valB)
//                           : std::visit([&](auto&& aVal, auto&& bVal) {
//                        return aVal < bVal;  // Ascending order comparison
//                    }, valA, valB);
//        });
//    }

    resultData = std::move(data);
}

TypeHints::TableData Sort::getResult() const {
    return resultData;
}

std::string Sort::toString(int level) const {
    return "SortPlan(\n" + std::string(level, ' ') + "keys=" + std::to_string(orderBy.size()) + ",\n" +
           std::string(level, ' ') + "source=" + source->toString(level + 1) + "\n" + std::string(level - 1, ' ') + ")";
}

////---------------- Visualize ----------------

Visualize::Visualize(std::shared_ptr<PlanNodeBase> source) : source(std::move(source)) {}

void Visualize::execute() {
    source->execute();
    data = source->getResult();
    if (!data.empty()) {
        headers.clear();
        for (const auto &[key, _]: data[0]) {
            headers.push_back(key);
        }
    }
    visualizeTable();

}


void Visualize::visualizeTable() {
    if (data.empty()) {
        fmt::println("No data to display.");
        return;
    }

    std::vector<size_t> colWidths(headers.size(), 0);
    for (auto i = 0; i < headers.size(); ++i) {
        colWidths[i] = headers[i].length();
    }

    for (const auto &row: data) {
        for (size_t i = 0; i < headers.size(); ++i) {
            const auto &val = row.at(headers[i]);
            std::string str = Literal(val).toString();
            colWidths[i] = std::max(colWidths[i], str.length());
        }
    }

    auto printDivider = [&]() {
        std::cout << "+";
        for (size_t width: colWidths) {
            std::cout << std::string(width + 2, '-') << "+";
        }
        std::cout << "\n";
    };

    auto printRow = [&](const std::vector<std::string> &rowData) {
        std::cout << "| ";
        for (size_t i = 0; i < rowData.size(); ++i) {
            std::cout << std::left << std::setw(colWidths[i]) << rowData[i] << " | ";
        }
        std::cout << "\n";
    };

    std::cout << "\nResult: " << std::endl;
    printDivider();
    printRow(headers);
    printDivider();

    for (const auto &row: data) {
        std::vector<std::string> rowStrs;
        rowStrs.reserve(headers.size());
        for (const auto &h: headers) {
            rowStrs.push_back(Literal(row.at(h)).toString());
        }
        printRow(rowStrs);
    }

    printDivider();
}


TypeHints::TableData Visualize::getResult() const {
    throw IntegrityError("Do not call getResult() on VisualizePlan");
}

std::string Visualize::toString(int level) const {
    return "Visualize(\n" + std::string(level, ' ') + "source=" + source->toString(level + 1) + "\n" + std::string(level - 1, ' ') + ")";
}


