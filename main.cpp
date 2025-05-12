#include "Token.h"
#include "RegretDB.h"
#include "fmt/ranges.h"
#include "operators/ComparisonOperators.h"
#include "operators/Operand.h"


int main() {
//
    RegretDB engine = RegretDB();
    std::string sql1 = "CREATE TABLE users (id NUMBER, name TEXT)";
    engine.executeOrder66(sql1);

    std::string sql2 = "INSERT INTO users (name, id) VALUES ('Ash', 1)";
    engine.executeOrder66(sql2);

    std::string sql3 = "SELECT * FROM users where (ala > 1 or ala < 1) and (kot = 'ma')";
    engine.executeOrder66(sql3);

}