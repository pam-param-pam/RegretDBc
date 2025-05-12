//// PlanNodes.cpp
//#include "PlanNodeBase.h"
//#include "SelectPlans.h"
//#include "../DataManager.h"
//#include <iostream>
//#include <iomanip>
//#include <algorithm>
//
//TableScan::TableScan(const std::string& table) : table(table) {}
//
//TypeHints::TableData TableScan::execute() {
//    return DataManager::getInstance().getTableData(table);
//}
//
//std::string TableScan::toString(int level) const {
//    return "TableScan('" + table + "')";
//}
//
//Filter::Filter(std::shared_ptr<PlanNode> source, std::shared_ptr<Condition> condition)
//        : source(std::move(source)), condition(std::move(condition)) {}
//
//TypeHints::TableData Filter::execute() {
//    TypeHints::TableData filtered;
//    for (const auto& row : source->execute()) {
//        if (condition->execute(row)) {
//            filtered.push_back(row);
//        }
//    }
//    return filtered;
//}
//
//std::string Filter::toString(int level) const {
//    return "FilterPlan(\n" + std::string(level, ' ') + "condition=" + condition->toString() + ",\n" +
//           std::string(level, ' ') + "source=" + source->toString(level + 1) + "\n" + std::string(level - 1, ' ') + ")";
//}
//
//Visualize::Visualize(std::shared_ptr<PlanNode> source) : source(std::move(source)) {}
//
//TypeHints::TableData Visualize::execute() {
//    data = source->execute();
//    if (!data.empty()) {
//        headers.clear();
//        for (const auto& [key, _] : data[0]) {
//            headers.push_back(key);
//        }
//    }
//    visualizeTable();
//    return data;
//}
//
//std::string Visualize::toString(int level) const {
//    return "Visualize(\n" + std::string(level, ' ') + "source=" + source->toString(level + 1) + "\n" + std::string(level - 1, ' ') + ")";
//}
//
//void Visualize::visualizeTable() {
//    if (data.empty()) {
//        std::cout << "\nNo data to display." << std::endl;
//        return;
//    }
//
//    std::vector<size_t> colWidths(headers.size(), 0);
//    for (size_t i = 0; i < headers.size(); ++i) {
//        colWidths[i] = headers[i].length();
//    }
//
//    for (const auto& row : data) {
//        for (size_t i = 0; i < headers.size(); ++i) {
//            const auto& val = row.at(headers[i]);
//            std::string str = std::any_cast<std::string>(val);
//            colWidths[i] = std::max(colWidths[i], str.length());
//        }
//    }
//
//    auto printDivider = [&]() {
//        std::cout << "+";
//        for (size_t width : colWidths) {
//            std::cout << std::string(width + 2, '-') << "+";
//        }
//        std::cout << "\n";
//    };
//
//    auto printRow = [&](const std::vector<std::string>& rowData) {
//        std::cout << "| ";
//        for (size_t i = 0; i < rowData.size(); ++i) {
//            std::cout << std::left << std::setw(colWidths[i]) << rowData[i] << " | ";
//        }
//        std::cout << "\n";
//    };
//
//    std::cout << "\nResult: " << std::endl;
//    printDivider();
//    printRow(headers);
//    printDivider();
//    for (const auto& row : data) {
//        std::vector<std::string> rowStrs;
//        for (const auto& h : headers) {
//            rowStrs.push_back(std::any_cast<std::string>(row.at(h)));
//        }
//        printRow(rowStrs);
//    }
//    printDivider();
//}
//
//Project::Project(std::shared_ptr<PlanNode> source, const std::vector<std::string>& columns)
//        : source(std::move(source)), columns(columns) {}
//
//TypeHints::TableData Project::execute() {
//    TypeHints::TableData result;
//    for (const auto& row : source->execute()) {
//        TypeHints::Row newRow;
//        for (const auto& col : columns) {
//            newRow[col] = row.at(col);
//        }
//        result.push_back(newRow);
//    }
//    return result;
//}
//
//std::string Project::toString(int level) const {
//    return "SelectPlan(\n" + std::string(level, ' ') + "projection=" + std::to_string(columns.size()) + ",\n" +
//           std::string(level, ' ') + "source=" + source->toString(level + 1) + "\n" + std::string(level - 1, ' ') + ")";
//}
//
//Sort::Sort(std::shared_ptr<PlanNode> source, const std::vector<std::pair<std::string, std::string>>& orderBy)
//        : source(std::move(source)), orderBy(orderBy) {}
//
//TypeHints::TableData Sort::execute() {
//    TypeHints::TableData rows = source->execute();
//    for (auto it = orderBy.rbegin(); it != orderBy.rend(); ++it) {
//        const auto& [column, direction] = *it;
//        bool reverse = direction == "DESC";
//        std::sort(rows.begin(), rows.end(), [&](const TypeHints::Row & a, const TypeHints::Row& b) {
//            auto va = std::any_cast<std::string>(a.at(column));
//            auto vb = std::any_cast<std::string>(b.at(column));
//            return reverse ? va > vb : va < vb;
//        });
//    }
//    return rows;
//}
//
//std::string Sort::toString(int level) const {
//    return "SortPlan(\n" + std::string(level, ' ') + "keys=" + std::to_string(orderBy.size()) + ",\n" +
//           std::string(level, ' ') + "source=" + source->toString(level + 1) + "\n" + std::string(level - 1, ' ') + ")";
//}
//
//CrossJoin::CrossJoin(std::shared_ptr<PlanNode> left, std::shared_ptr<PlanNode> right)
//        : left(std::move(left)), right(std::move(right)) {}
//
//TypeHints::TableData CrossJoin::execute() {
//    TypeHints::TableData leftData = left->execute();
//    TypeHints::TableData rightData = right->execute();
//    TypeHints::TableData result;
//
//    for (const auto& lrow : leftData) {
//        for (const auto& rrow : rightData) {
//            TypeHints::Row merged = lrow;
//            merged.insert(rrow.begin(), rrow.end());
//            result.push_back(merged);
//        }
//    }
//    return result;
//}
//
//std::string CrossJoin::toString(int level) const {
//    return "CrossJoinPlan(\n" + std::string(level, ' ') + "left=" + left->toString(level + 1) + ",\n" +
//           std::string(level, ' ') + "right=" + right->toString(level + 1) + "\n" + std::string(level - 1, ' ') + ")";
//}
