#include "RegretDB.h"

RegretDB::RegretDB()
        : parser(), planner() {}

void RegretDB::executeOrder66(const std::string& sql_stmt) {
    std::shared_ptr<ASTNode> statement;
    std::shared_ptr<PlanNodeBase> plan;

    statement = parser.parse(sql_stmt);
    fmt::println(statement->repr());
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
