#include <fstream>
#include "RegretDB.h"

RegretDB::RegretDB()
        : parser(), planner() {}

RegretDB& RegretDB::getInstance() {
    static RegretDB instance;
    return instance;
}
void RegretDB::executeOrder66(const std::string& sqlString) {
    std::shared_ptr<ASTNode> statement;
    std::shared_ptr<PlanNodeBase> plan;

    statement = parser.parse(sqlString);
    statement->setSqlText(sqlString);
    statement->verify();
    plan = planner.plan(statement);
    plan->execute();
}