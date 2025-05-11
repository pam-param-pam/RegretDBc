#include "Token.h"
#include "RegretDB.h"
#include "fmt/ranges.h"


int main() {

    RegretDB engine = RegretDB();
    std::string sql = "CREATE TABLE users (id NUMBER, name TEXT)";
    engine.executeOrder66(sql);

//    auto columns = DataManager::getInstance().getColumnsForTable("users");
//    fmt::print("{}\n", fmt::join(columns, " "));

    std::string sql1 = "INSERT INTO users (name, id) VALUES ('Ash', 1)";
    engine.executeOrder66(sql1);

//    auto rows = DataManager::getInstance().getTablesData("users");
//    // Iterate over each row
//    for (const auto& row : rows) {
//        // Iterate over each key-value pair in the row
//        for (const auto& [column, value] : row) {
//            // Print the column name and its value
//            std::visit([&](const auto& v) {
//                fmt::print("{}: {}\n", column, v);
//            }, value);
//        }
//    }
}