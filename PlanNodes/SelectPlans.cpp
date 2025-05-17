#include "PlanNodeBase.h"
#include "SelectPlans.h"
#include "../DataManager.h"
#include "fmt/base.h"
#include <iomanip>


///---------------- Table Scan ----------------
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

///---------------- Cross Join ----------------
CrossJoin::CrossJoin(std::unique_ptr<PlanNodeBase> left, std::unique_ptr<PlanNodeBase> right)
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

///---------------- Filter ----------------

Filter::Filter(std::unique_ptr<PlanNodeBase> source, Condition condition)
        : source(std::move(source)), condition(std::move(condition)) {}

void Filter::execute() {
    source->execute();
    auto data = source->getResult();

    for (const auto &row: data) {
        auto result = condition.evaluate(row);
        if (result.value_or(false)) {
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

///---------------- Project ----------------

Project::Project(std::unique_ptr<PlanNodeBase> source, const std::vector<std::string> &columns)
        : source(std::move(source)), columns(columns) {}

void Project::execute() {

    source->execute();
    auto data = source->getResult();

    for (const auto &row: data) {
        TypeHints::Row newRow;
        for (const auto &col: columns) {
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


///---------------- Sort ----------------

Sort::Sort(std::unique_ptr<PlanNodeBase> source, const std::vector<std::pair<std::string, bool>> &orderBy)
        : source(std::move(source)), orderBy(orderBy) {}

void Sort::execute() {
    source->execute();
    auto data = source->getResult();

    // Yea I don't remember how this works, but it works so don't touch it
    std::sort(data.begin(), data.end(), [&](const TypeHints::Row &a, const TypeHints::Row &b) {
        for (const auto &[column, isDescending]: orderBy) {
            const Literal &valA = a.at(column);
            const Literal &valB = b.at(column);

            if (valA == valB) {
                continue;
            }

            bool less = valA < valB;
            return isDescending == !less;
        }
        return false;
    });

    resultData = std::move(data);
}

TypeHints::TableData Sort::getResult() const {
    return resultData;
}

std::string Sort::toString(int level) const {
    return "SortPlan(\n" + std::string(level, ' ') + "keys=" + std::to_string(orderBy.size()) + ",\n" +
           std::string(level, ' ') + "source=" + source->toString(level + 1) + "\n" + std::string(level - 1, ' ') + ")";
}

///---------------- Visualize ----------------

Visualize::Visualize(std::unique_ptr<PlanNodeBase> source) : source(std::move(source)) {}

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
    for (size_t i = 0; i < headers.size(); ++i) {
        colWidths[i] = headers[i].length();
    }

    for (const auto &row: data) {
        for (size_t i = 0; i < headers.size(); ++i) {
            const auto &val = row.at(headers[i]);
            std::string str = Literal(val).toString();
            colWidths[i] = std::max(colWidths[i], str.length());
        }
    }

    std::ostringstream buffer;

    auto printDivider = [&buffer, &colWidths]() {
        buffer << "+";
        for (size_t width: colWidths) {
            buffer << std::string(width + 2, '-') << "+";
        }
        buffer << "\n";
    };

    auto printRow = [&buffer, &colWidths](const std::vector<std::string> &rowData) {
        buffer << "| ";
        for (size_t i = 0; i < rowData.size(); ++i) {
            buffer << std::left << std::setw(colWidths[i]) << rowData[i] << " | ";
        }
        buffer << "\n";
    };

    printDivider();
    printRow(headers);
    printDivider();

    for (const auto &row: data) {
        std::vector<std::string> rowStrings;
        rowStrings.reserve(headers.size());
        for (const auto &h: headers) {
            rowStrings.push_back(Literal(row.at(h)).toString());
        }
        printRow(rowStrings);
    }

    printDivider();

    buffer << "\n" << data.size() << " row" << (data.size() != 1 ? "s" : "") << " returned.\n";

    // Flush buffered output once
    std::cout << buffer.str();
}


TypeHints::TableData Visualize::getResult() const {
    throw IntegrityError("Do not call getResult() on VisualizePlan");
}

std::string Visualize::toString(int level) const {
    return "Visualize(\n" + std::string(level, ' ') + "source=" + source->toString(level + 1) + "\n" + std::string(level - 1, ' ') + ")";
}


