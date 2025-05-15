#include "RegretDB.h"

RegretDB::RegretDB()
        : parser(), planner() {}

void RegretDB::executeOrder66(const std::string& sql_stmt) {
    std::shared_ptr<ASTNode> statement;
    std::shared_ptr<PlanNodeBase> plan;

    statement = parser.parse(sql_stmt);
//    fmt::println(statement->repr());
    statement->setSqlText(sql_stmt);
//    fmt::println("setSqlText");
    statement->verify();
//    fmt::println("verified");
    plan = planner.plan(statement);
//    fmt::println("ExecutionPlanner: " + plan->toString(1));

//    fmt::println("44444444444444");
    plan->execute();
//    fmt::println("5555555555555");
}
void RegretDB::dumpToSQL(const std::string& filename) {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Failed to open file for writing: " << filename << "\n";
        return;
    }

    for (const auto& [tableName, table] : tables) {
        // Write CREATE TABLE
        out << "CREATE TABLE " << tableName << " (";
        for (size_t i = 0; i < table.columns.size(); ++i) {
            const auto& col = table.columns[i];
            out << col << " TEXT";  // You can store column types too if available
            if (i < table.columns.size() - 1) out << ", ";
        }
        out << ");\n";

        // Write INSERT INTO
        for (const auto& row : table.rows) {
            out << "INSERT INTO " << tableName << " (";
            for (size_t i = 0; i < table.columns.size(); ++i) {
                out << table.columns[i];
                if (i < table.columns.size() - 1) out << ", ";
            }
            out << ") VALUES (";

            for (size_t i = 0; i < table.columns.size(); ++i) {
                const std::string& col = table.columns[i];
                const auto& val = row.at(col);

                if (std::holds_alternative<std::string>(val)) {
                    out << "'" << escapeString(std::get<std::string>(val)) << "'";
                } else if (std::holds_alternative<double>(val)) {
                    out << std::get<double>(val);
                } else if (std::holds_alternative<bool>(val)) {
                    out << (std::get<bool>(val) ? "TRUE" : "FALSE");
                } else {
                    out << "NULL";
                }

                if (i < table.columns.size() - 1) out << ", ";
            }

            out << ");\n";
        }
    }

    out.close();
    std::cout << "Database dumped to " << filename << "\n";
}