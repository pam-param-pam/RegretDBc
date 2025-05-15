#pragma once

#include <string>
#include <memory>
#include "Parser.h"
#include "ExecutionPlanner.h"

class RegretDB {
public:
    RegretDB();

    void executeOrder66(const std::string& sql_stmt);  // May the 4th be with you

private:
    Parser parser;
    ExecutionPlanner planner;

    void dumpToSQL(const std::string &filename);
};
