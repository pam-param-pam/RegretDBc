#pragma once

#include <string>
#include <memory>
#include "Parser.h"
#include "ExecutionPlanner.h"

class RegretDB {
public:

    RegretDB(const RegretDB&) = delete;
    RegretDB& operator=(const RegretDB&) = delete;
    static RegretDB &getInstance();

    void executeOrder66(const std::string& sqlString);  // May the 4th be with you

    RegretDB();

private:

    Parser parser;
    ExecutionPlanner planner;

};
